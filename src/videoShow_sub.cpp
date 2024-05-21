#include <opencv2/opencv.hpp>

#include "math.h"

using namespace cv;
using namespace std;

// #define halfFarW 50
// #define halfCloseW 290
#define halfFarW 70
#define halfCloseW 400
// #define H1 460
// #define H2 650
#define H1 150
#define H2 500

#define minBarDistance 400
#define minBarS 100
double calculated_curvature;
double theata = 0;							// 用于累加局部曲率
int theata_count = 0;						// 用于累加曲率数.后面求平均
Mat polyfit(vector<Point> &in_point, int n) //
{
	int size = in_point.size();
	// 所求未知数个数
	int x_num = n + 1;
	// 构造矩阵U和Y
	Mat mat_u(size, x_num, CV_64F);
	Mat mat_y(size, 1, CV_64F);

	for (int i = 0; i < mat_u.rows; ++i)
		for (int j = 0; j < mat_u.cols; ++j)
		{
			mat_u.at<double>(i, j) = pow(in_point[i].x, j);
		}

	for (int i = 0; i < mat_y.rows; ++i)
	{
		mat_y.at<double>(i, 0) = in_point[i].y;
	}

	// 矩阵运算，获得系数矩阵K
	Mat mat_k(x_num, 1, CV_64F);
	mat_k = (mat_u.t() * mat_u).inv() * mat_u.t() * mat_y;
	// cout << mat_k << endl;
	return mat_k;
}
void imageCallback(Mat src)
{

	Mat dst2(1080, 840, CV_8UC3); // 用來繪制識別出的正交的路徑
	dst2.setTo(0);
	Mat mask(src.rows, src.cols, CV_8UC3); // 用來繪制將正交路徑變回原圖的遮罩
	mask.setTo(0);

	Point2f src_vertices[4]; // 从原图中选择出需要识别的透视区域，用于变换
	src_vertices[0] = Point(src.size().width / 2 - halfFarW, H1);
	src_vertices[1] = Point(src.size().width / 2 + halfFarW, H1);
	src_vertices[2] = Point(src.size().width / 2 - halfCloseW, H2);
	src_vertices[3] = Point(src.size().width / 2 + halfCloseW, H2);

	Point2f dst_vertices[4]; // 设置透视区域变换后的区域
	dst_vertices[0] = Point(200, 0) + Point(20, 20);
	dst_vertices[1] = Point(640, 0) + Point(20, 20);
	dst_vertices[2] = Point(200, 1080);
	dst_vertices[3] = Point(640, 1080);

	Mat M = getPerspectiveTransform(src_vertices, dst_vertices); // 透视变换矩阵
	Mat dst(1080, 840, CV_8UC3);								 // 变换后的区域

	Mat binery, lab, gray, grayToBinary, hls;

	warpPerspective(src, dst, M, dst.size(), INTER_LINEAR, BORDER_CONSTANT); // 变换
	cvtColor(dst, gray, COLOR_BGR2GRAY);									 // 获取灰度图
	cvtColor(dst, lab, cv::COLOR_BGR2Lab);									 // 获取lab图
	cvtColor(dst, hls, cv::COLOR_BGR2HLS);
	// dst.copyTo(lab);
	// GaussianBlur(lab,lab,Size(5,5),0);

	std::vector<Mat> split_vec; // hsv的分离通道
	split(hls, split_vec);		// 分离以便查看特征
	GaussianBlur(dst, dst, Size(5, 5), 20, 20);
	// threshold(gray, grayToBinary,230, 255, THRESH_BINARY);
	// threshold(gray, grayToBinary,200, 255, THRESH_BINARY);
	// inRange(dst, Scalar(232, 232, 232), Scalar(255, 255, 255), binery);
	inRange(hls, Scalar(0, 232, 150), Scalar(255, 255, 255), binery);
	// inRange(lab, Scalar(0, 43, 145), Scalar(255, 255, 255), binery);//二值化到gary里
	// inRange(lab, Scalar(0, 145, 145), Scalar(120, 255, 255), binery);
	// bitwise_or(binery,grayToBinary,binery);//src1,src2,dst或运算
	//  Mat element2;// = getStructuringElement(MORPH_RECT, Size(15, 10));//用于腐蚀和膨胀
	//  Mat element = getStructuringElement(MORPH_RECT, Size(45, 45)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的

	// element2 = getStructuringElement(MORPH_RECT, Size(4, 4));
	// erode(binery, binery, element2);
	// //腐蚀操作
	// dilate(binery, binery, element);
	// medianBlur(binery, binery, 5);

	// static Point *pointArr=new Point[binery.rows/50];//建立点的数组
	// static Point *pointArr1=new Point[binery.rows/50];

	// double theata = 0;	  // 用于累加局部曲率
	// int theata_count = 0; // 用于累加曲率数.后面求平均

	std::vector<Point> lastAllowedPoints;
	bool lastIfOneOnRight = false;
	std::vector<std::vector<Point>> vec_p;

	std::vector<Vec4i> vec_4f;
	findContours(binery, vec_p, vec_4f, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	std::vector<Point> keyPoints; // 储存经过过滤的 线段的关键顶点,用于拟合

	for (int i = 0; i < vec_p.size(); i++)
	{
		if (fabs(arcLength(vec_p.at(i), true)) > 216)
		{ // 周长限制
			Point max = vec_p.at(i).at(0);
			Point min = vec_p.at(i).at(0);
			for (int j = 1; j < vec_p.at(i).size(); j++)
			{ // 寻找端点
				if (vec_p.at(i).at(j).y > max.y)
				{
					max = vec_p.at(i).at(j);
				}
				if (vec_p.at(i).at(j).y < min.y)
				{
					min = vec_p.at(i).at(j);
				}
			}
			if ((max.x - min.x) * (max.x - min.x) + (max.y - min.y) * (max.y - min.y) > 8000)
			{ // 长度限制
				circle(dst2, min, 7, Scalar(255, 0, 255));
				circle(dst2, max, 7, Scalar(255, 255, 0));

				// keyPoints.push_back(max);
				// keyPoints.push_back(min);
				keyPoints.push_back(Point(max.y, max.x));
				keyPoints.push_back(Point((min.y + max.y) / 2, (min.x + max.x) / 2));
				keyPoints.push_back(Point(min.y, min.x));
			}
			// drawContours(dst2, vec_p,i, Scalar(255, 255, 100),23);
		}
	}
	// for(int i=0;i<keyPoints.size();i++){
	// 	char index[10]="";
	// 	sprintf(index,"%d",i);
	// 	putText(dst2,index,keyPoints.at(i), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0,255,255));
	// }
	if (keyPoints.size() > 0)
	{
		int n = 2; // 拟合到2次函数

		Mat mat_k = polyfit(keyPoints, n);
		double lasty = 0;  // 记录上一个y值用于微分
		double lastdy = 0; // 记录上一个dy值用于二次微分
		// 画出拟合曲线
		for (int i = keyPoints.back().x; i < keyPoints.front().x; ++i)
		{

			Point2d ipt;
			ipt.y = i;
			ipt.x = 0;
			for (int j = 0; j < n + 1; ++j)
			{
				ipt.x += mat_k.at<double>(j, 0) * pow(i, j);
			}
			if (i != keyPoints.front().x)
			{ // weifen,用于曲率计算
				double dy = ipt.x - lasty;
				if (i != keyPoints.front().x + 1)
				{
					double ddy = dy - lastdy;
					theata += ddy;
					theata_count++;
				}
				lastdy = dy;
			}
			lasty = ipt.x;
			circle(dst2, ipt, 7, Scalar(255, 255, 0));
			// circle(lab, pointArr[i], 3, cv::Scalar(0, 255, 0),3);
		}
	}
	//
	// for(int i=0;i<binery.rows/50;i++){
	// 	Mat ROIimage=binery(Rect(0,50*i,binery.cols-1,50));
	// 	//注意此处的ROI仍在image上
	// 	//imshow("ROI image ",ROIimage);

	// 	std::vector<std::vector<Point> > vec_p;

	// 	std::vector<Vec4i> vec_4f;

	// 	//drawContours(lab, vec_p, -1, Scalar(0,0,255), 3,Point(0, i*50));
	// 	//cout << vec_p.size() << endl;
	// }

	if (theata_count > 0)
	{ // 计算曲率平均值并绘制到图上
		char index[10] = "";

		sprintf(index, "%f", theata / theata_count);
		putText(src, index, Point(100, 100), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 255));
	}

	Mat M2 = getPerspectiveTransform(dst_vertices, src_vertices); // 用于变换回原图

	warpPerspective(dst2, mask, M2, mask.size(), INTER_LINEAR, BORDER_CONSTANT); // 变换

	Mat binary2;
	cvtColor(mask, binary2, COLOR_BGR2GRAY); // 转黑白
	// imshow("binary2", binary2);
	threshold(binary2, binary2, 50, 255, THRESH_BINARY); // 二值化
	mask.copyTo(src, binary2);							 // 剔除遮罩

	// imshow("dst", dst2);
	imshow("src", src);
	// imshow("lab", lab);
	// imshow("binery", binery);
	// imshow("gray", gray);
	// imshow("split_vec0", split_vec[0]);
	// imshow("split_vec1", split_vec[1]);
	// imshow("split_vec2", split_vec[2]);
	cv::waitKey(1);
}
// 引用计算曲率的头文件，确保其中包含了对 calculated_curvature 的声明
extern double calculated_curvature;

// 定义车轮间距
const double L = 0.5; // 假设车轮间距为0.5米
// 函数，用于根据曲率计算左右轮速度
void calculateMotorSpeeds(double curvature, double &leftSpeed, double &rightSpeed, double baseSpeed)
{
	double centralcurvature;
	// 曲率大于零，右转
	if (curvature > 0.000001)
	{
		double centralcurvature;
		centralcurvature = 1 / ((1 / curvature) + 1.5); // 转化为中心曲率
		double radius = 1.0 / centralcurvature;
		leftSpeed = baseSpeed * (1 + L / (2 * radius));
		rightSpeed = baseSpeed * (1 - L / (2 * radius));
	}
	// 曲率小于零，左转
	if (curvature < -0.000001)
	{
		double centralcurvature;
		centralcurvature = 1 / ((1 / ((-1) * curvature)) + 1.5);
		double radius = 1.0 / centralcurvature;
		leftSpeed = baseSpeed * (1 - L / (2 * radius));
		rightSpeed = baseSpeed * (1 + L / (2 * radius));
	}
	// 曲率为0，直行
	else
	{
		leftSpeed = baseSpeed;
		rightSpeed = baseSpeed;
	}
}
int main(int argc, char *argv[])
{
	VideoCapture video;
	Mat frame;
	video.open("/dev/video0");
	if (!video.isOpened())
	{
		puts("Read Video failed!");
		return -1;
	}
	// 从全局变量中获取曲率
	double curvature;
	while (1)
	{
		video >> frame;
		imageCallback(frame);
		calculated_curvature = theata / theata_count;
		curvature = calculated_curvature;
		// 基础速度设定，例如0.5米/秒
		double baseSpeed = 0.5;
		double leftSpeed, rightSpeed;

		// 根据曲率计算左右轮速度
		calculateMotorSpeeds(curvature, leftSpeed, rightSpeed, baseSpeed);

		// 输出计算结果
		std::cout << "Curvature: " << curvature << std::endl;
		std::cout << "Left Wheel Speed: " << leftSpeed << std::endl;
		std::cout << "Right Wheel Speed: " << rightSpeed << std::endl;
	}

	cv::namedWindow("view");
	cv::startWindowThread();
	cv::destroyWindow("view");

	return 0;
}
