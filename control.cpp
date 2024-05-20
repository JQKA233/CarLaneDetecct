#include <iostream>
#include "videoShowSub.cpp"
// 引用计算曲率的头文件，确保其中包含了对 calculated_curvature 的声明
extern double calculated_curvature;

// 定义车轮间距
const double L = 0.5; // 假设车轮间距为0.5米

// 函数，用于根据曲率计算左右轮速度
void calculateMotorSpeeds(double curvature, double &leftSpeed, double &rightSpeed, double baseSpeed)
{
    double centralcurvature;
    // 曲率大于零，左转
    if (curvature > 0.00001)
    {
        double centralcurvature;
        centralcurvature = 1 / ((1 / curvature) + 1.5); // 转化为中心曲率
        double radius = 1.0 / centralcurvature;
        leftSpeed = baseSpeed * (1 - L / (2 * radius));
        rightSpeed = baseSpeed * (1 + L / (2 * radius));
    }
    // 曲率小于零，右转
    if (curvature < -0.00001)
    {
        double centralcurvature;
        centralcurvature = 1 / ((1 / ((-1) * curvature)) + 1.5);
        double radius = 1.0 / centralcurvature;
        leftSpeed = baseSpeed * (1 + L / (2 * radius));
        rightSpeed = baseSpeed * (1 - L / (2 * radius));
    }
    // 曲率为0，直行
    else
    {
        leftSpeed = baseSpeed;
        rightSpeed = baseSpeed;
    }
}

int main()
{
    // 基础速度设定，例如0.5米/秒
    double baseSpeed = 0.5;
    double leftSpeed, rightSpeed;

    // 从全局变量中获取曲率
    double curvature = calculated_curvature;

    // 根据曲率计算左右轮速度
    calculateMotorSpeeds(curvature, leftSpeed, rightSpeed, baseSpeed);

    // 输出计算结果
    std::cout << "Curvature: " << curvature << std::endl;
    std::cout << "Left Wheel Speed: " << leftSpeed << std::endl;
    std::cout << "Right Wheel Speed: " << rightSpeed << std::endl;

    return 0;
}