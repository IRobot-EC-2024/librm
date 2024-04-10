/**
 * @file  modules/typedefs.h
 * @brief 工程内全局数据类型定义
 */

#ifndef EC_LIB_MODULES_TYPEDEFS_H
#define EC_LIB_MODULES_TYPEDEFS_H

#ifdef __cplusplus
#include <cstdint>
#include <cctype>
#else
#include <stdint.h>
#include <ctype.h>
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;
typedef float f32;
typedef double f64;

#endif  // EC_LIB_MODULES_TYPEDEFS_H

/* EOF */
