# CarLaneDetecct

## 项目概述

本项目是一个基于 OpenCV 的图像处理程序，用于从视频流中检测道路并计算道路曲率，进而根据曲率计算左右轮的速度，适用于自动驾驶车辆的路径规划。

## 环境依赖

- OpenCV 4.x
- CMake
- build-essential

## 编译指南

1. 克隆项目到本地
   ```
   git clone [项目仓库地址]
   ```
2. 进入项目目录
   ```
   cd [项目目录]
   ```
3. 创建构建目录并进入
   ```
   mkdir build
   cd build
   ```
4. 使用 CMake 生成 Makefile
   ```
   cmake ..
   ```
5. 使用 make 命令编译项目
   ```
   make
   ```

## 运行指南

1. 确保摄像头设备 `/dev/video0` 可用。
2. 运行编译后的程序
   ```
   ./[编译后的程序名]
   ```
3. 观察程序窗口，查看图像处理结果和速度计算。

## 代码说明

- `polyfit` 函数：用于多项式拟合，输入为点集和拟合次数。
- `imageCallback` 函数：图像回调函数，处理图像并计算曲率。
- `calculateMotorSpeeds` 函数：根据曲率计算左右轮速度。
- `main` 函数：程序入口，初始化摄像头并循环调用 `imageCallback`。

## 配置参数

- `halfFarW` 和 `halfCloseW`：定义远近视角的宽度。
- `H1` 和 `H2`：定义图像中道路的上下边界。
- `minBarDistance` 和 `minBarS`：定义检测道路的最小距离和大小。
- `L`：定义车轮间距。

## 注意事项

- 确保摄像头权限正确设置，避免访问失败。
- 根据实际摄像头调整 `/dev/video0` 设备路径。
- 根据实际车辆参数调整车轮间距 `L`。

## 贡献与反馈

欢迎提交 Pull Request 或创建 Issue 来贡献代码或反馈问题。

## 许可

本项目采用 [MIT 许可证](LICENSE)。
