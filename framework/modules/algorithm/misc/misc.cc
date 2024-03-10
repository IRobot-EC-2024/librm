
#include "misc.hpp"


using namespace modules::algorithm::misc;


/***
 * @brief 角度转弧度
 * @param deg   角度值
 * @return      弧度值
 */
fp32 degToRad(fp32 deg) {
    return deg * M_PI / 180;
}


/***
 * @brief 四元数转欧拉角
 * @param q         四元数
 * @param euler     欧拉角
 */
void quatToEuler(const fp32 q[4], fp32 euler[3]) {
    euler[0] = atan2f(2 * (q[0] * q[1] + q[2] * q[3]), 1 - 2 * (q[1] * q[1] + q[2] * q[2]));
    euler[1] = asinf(2 * (q[0] * q[2] - q[3] * q[1]));
    euler[2] = atan2f(2 * (q[0] * q[3] + q[1] * q[2]), 1 - 2 * (q[2] * q[2] + q[3] * q[3]));
}

/* EOF */
