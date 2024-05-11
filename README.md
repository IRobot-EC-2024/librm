# RM EC C++ Library [WIP]

[![build](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/ci_build.yml/badge.svg)](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/ci_build.yml)
[![clang-format Check](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/style_check.yml/badge.svg)](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/style_check.yml)
[![docs](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/IRobot-EC-2024/ec-cpp-library/actions/workflows/doxygen-gh-pages.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/lunarifish/ec-cpp-library/badge)](https://www.codefactor.io/repository/github/lunarifish/ec-cpp-library)

使用C++编写的电控库，统一所有车的底层代码和少量通用业务代码。开发在STM32F407和STM32H723平台上进行，其他芯片未经验证。

项目基于C++17/C11标准，遵守[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)[[中文](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents.html)]
。建议使用clang-format自动格式化代码。style check action失败可参考输出结果加以修改。

**NOTE: 由于使用了GCC/Clang编译器的一些特性和较新版本的C/C++标准，所以不保证在Keil的ARMCC编译器上能够正常编译。**

## 文档

文档可以自行使用Doxygen构建，也可以在[这里](https://irobot-ec-2024.github.io/ec-cpp-library/)查看。

```shell
doxygen ./Doxyfile
```

正确构建后，文档会被放在`docs/`文件夹下。

## 项目结构/开发进度

未打钩的复选框表示还在开发中。

- `examples/`：例程

- `libs/`：第三方库

- `src/`

    - `device/`：设备驱动和封装
        - `motor/`：电机
            - `dji_motor`：大疆电机
            - [ ] `unitree_motor`：宇树电机
        - `remote/`：遥控器/接收机
            - `dr16`：DT7遥控器/DR16接收机
        - `sensor/`：传感器
            - [ ] `icm42688p/`：[ICM42688P IMU](https://product.tdk.com.cn/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000347-icm-42688-p-v1.6.pdf)
            - `bmi088/`：[BMI088 IMU](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi088-ds001.pdf)
            - `ist8310/`：[IST8310磁力计](https://tw.isentek.com/userfiles/files/IST8310Datasheet_3DMagneticSensors.pdf)
        - `supercap/`：超级电容
        - [ ] `referee/`：裁判系统

    - [ ] `hal/`：基于STM32 HAL库封装的C++类库

    - `modules/`：软件模块
        - `algorithm/`：常用算法

## 开发环境

- [OpenOCD](https://github.com/openocd-org/openocd/releases/) `0.12.0`
- STM32CubeMX `6.10.0`
    - STM32Cube MCU Package for STM32F4 series `1.28.0`
    - STM32Cube MCU Package for STM32H7 series `1.11.1`
- [CMake](https://cmake.org/download/) `3.28.3`
- [GNU Arm Embedded Toolchain, AArch32 bare-metal target (arm-none-eabi)](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) `13.2.Rel1`

## 使用方法

1. Clone仓库（包括子模块），在CMakeLists.txt里添加为子目录并且把`irobotEC`静态库链接到主目标：

    ```shell
    git clone --recursive https://github.com/IRobot-EC-2024/ec-cpp-library.git
   ```

    ```cmake
    add_subdirectory(<仓库路径>)
    target_link_libraries(<目标> PUBLIC irobotEC)
    ```

2. 在CMakeLists.txt里添加对应芯片型号的预处理宏定义（注意检查是否已经自动生成过）

    ```cmake
    add_definitions(-DSTM32F407xx)
    # add_definitions(-DSTM32H723xx)
    ```

   芯片支持情况：
    - [x] STM32F407xx
    - [x] STM32H723xx

3. 启用UART、CAN、I2C、SPI的Register Callback功能，在CubeMX中的配置项位置如下图：

   ![](https://img2.imgtp.com/2024/04/10/AUnAPRby.png)

4. 在代码里包含头文件（未来计划只用包含一个头文件，目前还没做）

    ```cpp
    #include "device/motor/dji_motor.hpp"
    // #include ...
    ```
