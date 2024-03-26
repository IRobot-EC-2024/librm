# RM EC C++ Library [WIP]

[![build](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/ci_build.yml/badge.svg)](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/ci_build.yml)
[![clang-format Check](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/style_check.yml/badge.svg)](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/style_check.yml)
[![docs](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/doxygen-gh-pages.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/lunarifish/ec-cpp-library/badge)](https://www.codefactor.io/repository/github/lunarifish/ec-cpp-library)

实验性的电控C++库，包含各种设备（电机、传感器等）的抽象封装，以及一些常用工具类。

项目基于C++17标准，遵守[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)[[中文](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents.html)]。推荐使用clang-format根据项目根目录下的.clang-format文件自动格式化代码。PR和push会自动被CI系统检查是否符合规范，若style check失败可参考action输出结果加以修改。

## 文档

文档可以自行使用Doxygen构建，也可以在[这里](https://irobot-ec-2024.github.io/ec-cpp-library/)查看。

```shell
doxygen ./Doxyfile
```

正确构建后，文档会被放在`docs/`文件夹下。

## 项目结构/开发进度

打钩表示已完成可以使用，空格表示未完成

- `stm32cube_template/`：CI构建用的空白工程

- `examples/`：例程

- `unittest/`：单元测试

- `framework/`

  - [ ] `bsp/`：板级支持包
    - [ ] `dji_devboard_c/`：大疆C板
    - [ ] `dm_h7/`：DM-H7开发板

  - [ ] `components/`：组件，指各种设备的抽象封装
    - [ ] `motor/`：电机
      - [x] `dji/`：大疆电机
      - [ ] `unitree/`：宇树电机
    - [ ] `sensor/`：传感器
      - [ ] `icm42688p`：[ICM42688P IMU](https://product.tdk.com.cn/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000347-icm-42688-p-v1.6.pdf)
      - [ ] `bmi088/`：[BMI088 IMU](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi088-ds001.pdf)
      - [ ] `ist8310/`：[IST8310磁力计](https://tw.isentek.com/userfiles/files/IST8310Datasheet_3DMagneticSensors.pdf)
  
  - [ ] `hal_wrapper/`：HAL库的类封装

  - [ ] `modules/`：模块，指软件功能模块
    - [ ] `algorithm/`：算法
      - [x] `mecanum/`：麦轮运动学解算
      - [x] `mahony/`：Mahony姿态解算
      - [x] `pid/`：PID控制器
    - [x] `exception/`：异常处理
    - [x] `pubsub/`：发布订阅模块
    - [x] `serial_plotter/`：串口绘图器

## 开发环境

### Windows

- [OpenOCD](https://github.com/openocd-org/openocd/releases/) `0.12.0`
- STM32CubeMX `6.10.0`
  - STM32Cube MCU Package for STM32F4 series `1.24.1`
  - STM32Cube MCU Package for STM32H7 series `1.11.1`
- [CMake](https://cmake.org/download/) `3.28.3`
- [GNU Arm Embedded Toolchain, AArch32 bare-metal target (arm-none-eabi)](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) `13.2.Rel1`

### Linux

待补充，但是windows有的基本上linux也有

## 使用方法

1. 下载本项目，把`framework/`文件夹放到某处并添加到编译路径和头文件包含路径中

2. 在CMakeLists.txt或者工程设置里添加对应开发板芯片的预处理宏定义（一些IDE会自动添加）
   - 大疆C板：`#define STM32F407xx`
   - YCMC-H7A开发板：`#define STM32H723xx`

3. 按需包含头文件
