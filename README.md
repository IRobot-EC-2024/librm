# RM EC C++ Library [WIP]

[![CI Build](https://github.com/lunarifish/ci-test/actions/workflows/ci_build.yml/badge.svg)](https://github.com/lunarifish/ci-test/actions/workflows/ci_build.yml)

实验性的电控C++库，包含各种设备（电机、传感器等）的抽象封装，以及一些常用工具类。

项目基于C++17标准，遵守[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)[[中文](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents.html)]。

------------------------------------------------------------------------

## 项目结构/开发进度

打钩表示已完成可以使用，空格表示未完成

- `build_template/`：CI构建用的模板工程

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
      - [ ] `fifo/`：FIFO队列
      - [x] `mahony/`：Mahony姿态解算
      - [x] `pid/`：PID控制器
    - [x] `exception/`：异常模块
    - [x] `pubsub/`：发布订阅模块
    - [x] `serial_plotter_socket/`：串口绘图器

  - [ ] `unittest/`：单元测试

## 开发环境

- OpenOCD `0.12.0`
- STM32CubeMX `6.10.0`
  - STM32Cube MCU Package for STM32F4 series `1.24.1`
- CMake `3.28.0-rc2`
- arm-none-eabi-gcc `10.3.1 20210824` `GNU Arm Embedded Toolchain 10.3-2021.10`

## 使用方法

1. 下载本项目，把`framework/`文件夹复制到工程目录，并且添加到编译路径和头文件包含路径中

2. 按需包含头文件
