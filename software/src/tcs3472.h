/* color-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * tcs3472.h: Driver for TCS3472 color sensor
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef TCS3472_H
#define TCS3472_H

#include <stdint.h>

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"

typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t c;
} TCS3472RGBC;

typedef struct {
	I2CFifo i2c_fifo;

	TCS3472RGBC rgbc;

	uint8_t config_integration_time;
	uint8_t config_gain;
	bool    config_new;

	bool color_callback_value_has_to_change;
	uint32_t color_callback_period;

	uint16_t color_temperature;
	uint32_t illuminance;
} TCS3472;

extern TCS3472 tcs3472;

void tcs3472_init(void);
void tcs3472_tick(void);

uint16_t tcs3472_get_color_temperature(void);
uint32_t tcs3472_get_illuminance(void);


#define TCS3472_REG_RW_ENABLE   0x00
#define TCS3472_REG_RW_ATIME    0x01
#define TCS3472_REG_RW_WTIME    0x03
#define TCS3472_REG_RW_AILTL    0x04
#define TCS3472_REG_RW_AILTH    0x05
#define TCS3472_REG_RW_AIHTL    0x06
#define TCS3472_REG_RW_AIHTH    0x07
#define TCS3472_REG_RW_PERS     0x0C
#define TCS3472_REG_RW_CONFIG   0x0D
#define TCS3472_REG_RW_CONTROL  0x0F
#define TCS3472_REG_RO_ID       0x12
#define TCS3472_REG_RO_STATUS   0x13
#define TCS3472_REG_RO_CDATAL   0x14
#define TCS3472_REG_RO_CDATAH   0x15
#define TCS3472_REG_RO_RDATAL   0x16
#define TCS3472_REG_RO_RDATAH   0x17
#define TCS3472_REG_RO_GDATAL   0x18
#define TCS3472_REG_RO_GDATAH   0x19
#define TCS3472_REG_RO_BDATAL   0x1A
#define TCS3472_REG_RO_BDATAH   0x1B

#define TCS3472_REG_ATIME_2MS   0xFF
#define TCS3472_REG_ATIME_24MS  0xF6
#define TCS3472_REG_ATIME_101MS 0xD5
#define TCS3472_REG_ATIME_154MS 0xC0
#define TCS3472_REG_ATIME_700MS 0x00

#define TCS3472_REG_CONTROL_1X_GAIN  0x00
#define TCS3472_REG_CONTROL_4X_GAIN  0x01
#define TCS3472_REG_CONTROL_16X_GAIN 0x02
#define TCS3472_REG_CONTROL_60X_GAIN 0x03

#define TCS3472_REG_ENABLE_AIEN (1 << 4)
#define TCS3472_REG_ENABLE_WEN  (1 << 3)
#define TCS3472_REG_ENABLE_AEN  (1 << 1)
#define TCS3472_REG_ENABLE_PON  (1 << 0)

#define TCS3472_COMMAND_REG_SEL ((1 << 7) | (1 << 5)) //set bit 7 of command register

#define TCS3472_COLOR_TEMPERATURE_MULT_X_R 7141     // -7141 / 50000 = -0.14282     (perfect)
#define TCS3472_COLOR_TEMPERATURE_MULT_X_G 38731    // 38731 / 25000 = 1.54924      (perfect)
#define TCS3472_COLOR_TEMPERATURE_MULT_X_B 9564     // -9564 / 10000 = -0.9564      (should be -0.95641)

#define TCS3472_COLOR_TEMPERATURE_MULT_Y_R 16233    // -16233 / 50000 = -0.32466     (perfect)
#define TCS3472_COLOR_TEMPERATURE_MULT_Y_G 39459    // 39459 / 25000 = 1.57867       (should be 1.57837)
#define TCS3472_COLOR_TEMPERATURE_MULT_Y_B 7319     // -7319 / 10000 = -0.7319       (should be -0.73191)

#define TCS3472_COLOR_TEMPERATURE_MULT_Z_R 34101    // -34101 / 50000 = -0.68202     (perfect)
#define TCS3472_COLOR_TEMPERATURE_MULT_Z_G 9634     // 9634 / 12500 = 0.77072        (should be 0.77073)
#define TCS3472_COLOR_TEMPERATURE_MULT_Z_B 14083    // 14083 / 25000 = 0.56332       (perfect)

#define TCS3472_COLOR_TEMPERATURE_DIV_X_R 50000
#define TCS3472_COLOR_TEMPERATURE_DIV_X_G 25000
#define TCS3472_COLOR_TEMPERATURE_DIV_X_B 10000

#define TCS3472_COLOR_TEMPERATURE_DIV_Y_R 50000
#define TCS3472_COLOR_TEMPERATURE_DIV_Y_G 25000
#define TCS3472_COLOR_TEMPERATURE_DIV_Y_B 10000

#define TCS3472_COLOR_TEMPERATURE_DIV_Z_R 50000
#define TCS3472_COLOR_TEMPERATURE_DIV_Z_G 12500
#define TCS3472_COLOR_TEMPERATURE_DIV_Z_B 25000

#define TCS3472_COLOR_TEMPERATURE_SUB_XC 16600  // 0.3320*50000 (perfect)
#define TCS3472_COLOR_TEMPERATURE_SUB_YC 9290   // 0.1858*50000 (perfect)
#define TCS3472_COLOR_TEMPERATURE_MUL_XC 50000
#define TCS3472_COLOR_TEMPERATURE_MUL_YC 50000

#define TCS3472_COLOR_TEMPERATURE_MUL_N 0xFFFF


#endif