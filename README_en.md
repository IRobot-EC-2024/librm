# irobotEC [[中文]](README.md)

[![build](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/ci_build.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/ci_build.yml)
[![clang-format Check](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/style_check.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/style_check.yml)
[![docs](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/doxygen-gh-pages.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/lunarifish/irobotec/badge)](https://www.codefactor.io/repository/github/lunarifish/irobotec)

Cross-platform embedded software development framework for Robomaster robotics competition, providing unified peripheral
APIs, device drivers, and algorithm libraries for multiple hardware platforms.

- [x] STM32F/H
- [x] Linux
- [x] Raspberry Pi/Orange Pi
- [x] Jetson

Minimum required C++ standard is C++17.

## API Documentation

You may build the API docs using Doxygen, or view it [here](https://irobot-ec-2024.github.io/irobotEC/).

```shell
doxygen ./Doxyfile
```

Compiled HTML docs will be placed in the `docs/` folder.

## Integrate to Your Project

1. Clone this repo. **Please NOTE that** in order to download third-party libraries as well, you may use `--recursive` parameter when cloning:

    ```shell
    git clone --recursive https://github.com/IRobot-EC-2024/irobotEC.git
    ```

2. Add the whole repo as a subdirectory in your CMakeLists.txt, and link the `irobotEC` static library to the target that needs to use this library:

    ```cmake
    add_subdirectory(<仓库路径>)
    target_link_libraries(<目标> PUBLIC irobotEC)
    ```

3. There will be some extra steps if you are compiling for STM32:

    1. Add the macro definition of the corresponding chip model:

        ```cmake
        add_definitions(-DSTM32F407xx)
        # add_definitions(-DSTM32H723xx)
        ```

    2. Enable Register Callback for UART, CANFD, and CAN in CubeMX, as shown in the following picture:

       ![](https://img.picui.cn/free/2024/06/25/6679bb2a8c77b.png)

4. Start working on your project! Please refer to the examples in the [examples](examples/) folder for usage.

## Project Structure

- `cmake/`: CMake util scripts

- `examples/`

- `libs/`: third-party libraries

- `src/irobotec/`

    - `core/`: Runtime code for the framework itself

    - `device/`：Device drivers
        - `actuator/`：Motor, servo, etc.
        - `remote/`
        - `sensor/`
        - `supercap/`
        - `referee/`

    - `hal/`：Hardware abstraction layer
        - `stm32/`
        - `linux/`
        - `raspi/`
        - `jetson/`

    - `modules/`：Software modules
        - `algorithm/`：Common algorithms
