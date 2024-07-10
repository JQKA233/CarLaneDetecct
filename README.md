# CarLaneDetecct

## 项目概述

本项目是一个基于 OpenCV 的图像处理程序，用于从视频流中检测道路并计算道路曲率，进而根据曲率计算左右轮的速度，适用于自动驾驶车辆的路径规划。

## 环境依赖

- Linux 操作系统（推荐使用树莓派）
- OpenCV 4.x
- CMake
- build-essential
- libopencv-dev

### 安装依赖

在树莓派上安装必要的依赖项，可以通过以下命令完成：

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libopencv-dev
```

确保你的树莓派已经安装了 OpenCV 4.x 库。如果尚未安装，可以使用以下命令安装：

```bash
sudo apt-get install libopencv-core-dev libopencv-highgui-dev libopencv-imgproc-dev libopencv-contrib-dev
```

### 编译和运行

编译和运行的步骤与之前提供的指南相同，但请确保在树莓派上执行这些操作。

## 注意事项

- 树莓派的摄像头模块需要额外的配置。如果你使用的是树莓派的官方摄像头，确保启用了摄像头模块。可以通过运行 `sudo raspi-config` 来启用。
- 根据你的树莓派型号和摄像头类型，`/dev/video0` 可能需要更改为其他设备文件，例如 `/dev/video1`。
- 考虑到树莓派的计算能力，建议优化算法以减少处理时间。

### 性能优化提示

- 考虑降低图像分辨率以加快处理速度。
- 使用多线程或 GPU 加速（如果可用）来提高性能。

### 兼容性

- 本项目在树莓派上经过测试，但可能需要根据具体硬件配置进行调整。

### 社区支持

- 如果在树莓派上运行项目遇到问题，可以访问树莓派社区论坛寻求帮助。

---

请注意，上述指南假设你已经熟悉 Linux 命令行操作和基本的树莓派设置。如果你对这些操作不太熟悉，可能需要查阅更多关于树莓派的入门指南。


## 编译指南

1. 克隆项目到本地
   ```
   git clone https://github.com/JQKA233/CarLaneDetecct.git
   ```
2. 进入项目目录
   ```
   cd CarLaneDetecct
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
   ./car_road_line_detect
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
