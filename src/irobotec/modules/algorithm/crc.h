/*
  Copyright (c) 2024 XDU-IRobot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file  irobotec/modules/algorithm/crc.h
 * @brief crc8/crc16
 */

#ifndef IROBOTEC_MODULES_ALGORITHM_CRC_H
#define IROBOTEC_MODULES_ALGORITHM_CRC_H

#include <string>

#include "irobotec/core/typedefs.h"

namespace irobot_ec::modules::algorithm {

constexpr u8 CRC8_INIT = 0xff;
constexpr u16 CRC16_INIT = 0xffff;

u8 Crc8(unsigned char *input, unsigned int len, unsigned char uc_crc8);

u8 Crc8(std::string_view input, unsigned char uc_crc8);

u8 Crc8(const std::string &input, unsigned char uc_crc8);

void AppendCrc8(unsigned char *input, unsigned int len);

u16 Crc16(u8 *input, uint32_t len, u16 w_crc);

u16 Crc16(std::string_view input, u16 w_crc);

u16 Crc16(const std::string &input, u16 w_crc);

void AppendCrc16(u8 *input, uint32_t len);

}  // namespace irobot_ec::modules::algorithm

#endif  // IROBOTEC_MODULES_ALGORITHM_CRC_H