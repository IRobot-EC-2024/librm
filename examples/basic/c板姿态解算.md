# c板姿态解算

## 简介

本例程是一个FreeRTOS任务，演示如何使用本库中的BMI088、IST8310封装和Mahony姿态解算算法实现姿态解算。

## 步骤

1. 引入相关头文件
    - `device/sensor/bmi088/bmi088.h`：BMI088传感器封装
    - `device/sensor/ist8310/ist8310.h`：IST8310传感器封装
    - `modules/algorithm/mahony_ahrs.h`：Mahony姿态解算算法
2. 调用传感器的`Update`函数更新传感器数据
3. 调用Mahony姿态解算器的`Update`函数更新姿态数据
4. 获取姿态数据
5. 重复2~4

## 代码

```c++
#include "cmsis_os.h"
#include "i2c.h"
#include "spi.h"

#include "device/sensor/bmi088/bmi088.h"
#include "device/sensor/ist8310/ist8310.h"
#include "modules/algorithm/mahony_ahrs.h"

using irobot_ec::device::sensor::BMI088;
using irobot_ec::device::sensor::IST8310;
using irobot_ec::modules::algorithm::MahonyAhrs;

// 注意！！：不要在全局作用域中创建传感器对象，因为这样会导致对象的构造函数在main函数之前调用，而HAL库此时还未初始化
// 如果需要全局访问，可以将对象指针声明为全局变量，然后在任务函数中new一个对象绑定到指针上
// 像这样：
// BMI088 *bmi088;
// void task() {
//   bmi088 = new BMI088(...);
// }

extern "C" {

void ExampleAttitudeTask(const void *pv_arg) {
  // 对于BMI088，这个传感器由两个SPI设备组成，一个是加速度计，一个是陀螺仪
  // 使用时分别传入SPI总线指针、加速度计的CS引脚和陀螺仪的CS引脚
  BMI088 bmi088(hspi1, CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, CS1_GYRO_GPIO_Port,
                CS1_GYRO_Pin);
  // IST8310是一个I2C设备，传入I2C总线指针和RESET引脚
  IST8310 ist8310(hi2c3, GPIOG, GPIO_PIN_6);
  // 创建Mahony姿态解算器对象，传入采样率
  MahonyAhrs mahony(1000.0f);

  for (;;) {
    osDelay(1);

    // 更新传感器数据
    bmi088.Update();
    ist8310.Update();

    // 更新姿态解算器
    mahony.Update(bmi088.gyro_x(), bmi088.gyro_y(), bmi088.gyro_z(),
                  bmi088.accel_x(), bmi088.accel_y(), bmi088.accel_z(),
                  ist8310.mag_x(), ist8310.mag_y(), ist8310.mag_z());
    // 更新姿态解算器(禁用磁力计)
    mahony.UpdateImu(bmi088.gyro_x(), bmi088.gyro_y(), bmi088.gyro_z(),
                     bmi088.accel_x(), bmi088.accel_y(), bmi088.accel_z());

    // 获取姿态数据(欧拉角，弧度)
    mahony.euler_pitch();
    mahony.euler_roll();
    mahony.euler_yaw();
    // 或者获取四元数
    mahony.quat_w();
    mahony.quat_x();
    mahony.quat_y();
    mahony.quat_z();
  }
}
}
```