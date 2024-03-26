//=====================================================================================================
// MahonyAHRS.h
//=====================================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H
#define EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile float twoKp;           // 2 * proportional gain (Kp)
extern volatile float twoKi;           // 2 * integral gain (Ki)
extern volatile float q0, q1, q2, q3;  // quaternion of sensor frame relative to auxiliary frame

//---------------------------------------------------------------------------------------------------
// Function declarations

void MahonyAHRSupdate(float q[4], float gx, float gy, float gz, float ax, float ay, float az, float mx, float my,
                      float mz);

void MahonyAHRSupdateIMU(float q[4], float gx, float gy, float gz, float ax, float ay, float az);

#endif  // EC_LIB_MODULES_ALGORITHM_MAHONY_AHRS_H
//=====================================================================================================
// End of file
//=====================================================================================================
