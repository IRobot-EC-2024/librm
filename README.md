# irobotEC [[en]](README_en.md)

[![build](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/ci_build.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/ci_build.yml)
[![clang-format Check](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/style_check.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/style_check.yml)
[![docs](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/doxygen-gh-pages.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/lunarifish/irobotec/badge)](https://www.codefactor.io/repository/github/lunarifish/irobotec)

跨平台Robomaster嵌入式软件开发框架，为多种硬件平台提供统一的外设API、设备驱动和算法库。

- [x] STM32F/H
- [x] Linux
- [x] Raspberry Pi/Orange Pi
- [x] Jetson

编译所需最低C++标准为C++17。

## API文档

API文档可以使用Doxygen构建，也可以在[这里](https://irobot-ec-2024.github.io/irobotEC/)查阅。

```shell
doxygen ./Doxyfile
```

正确构建后，文档会被放在`docs/`文件夹下。

## 使用方法

1. 下载仓库。注意为了连同第三方库一起下载，clone时需要使用`--recursive`参数：

    ```shell
    git clone --recursive https://github.com/IRobot-EC-2024/irobotEC.git
    ```

2. 在CMakeLists.txt里把整个仓库添加为子目录，并把`irobotEC`静态库链接到需要使用本库的目标上；

    ```cmake
    add_subdirectory(<仓库路径>)
    target_link_libraries(<目标> PUBLIC irobotEC)
    ```

3. 如果是为STM32平台编译，则需要一些额外步骤

    1. 在CMakeLists.txt里添加对应芯片型号的宏定义；

        ```cmake
        add_definitions(-DSTM32F407xx)
        # add_definitions(-DSTM32H723xx)
        ```

    2. 启用UART、CANFD、CAN的Register Callback功能，在CubeMX中的配置项位置如下图：

       ![](https://img.picui.cn/free/2024/06/25/6679bb2a8c77b.png)

4. 开始使用。使用方法请参考[examples](examples/)文件夹下的例程。

## 项目结构

- `cmake/`：CMake脚本

- `examples/`：例程

- `libs/`：第三方库

- `src/irobotec/`

    - `core/`：框架自身运行依赖的代码

    - `device/`：设备驱动
        - `actuator/`：作动器，如电机和舵机等
        - `remote/`：遥控器/接收机
        - `sensor/`：传感器
        - `supercap/`：超级电容
        - `referee/`：裁判系统

    - `hal/`：硬件抽象层，框架跨平台的核心
        - `stm32/`：STM32 HAL
        - `linux/`：Linux HAL
        - `raspi/`：树莓派/香橙派 HAL
        - `jetson/`：Jetson HAL

    - `modules/`：软件模块
        - `algorithm/`：算法库
