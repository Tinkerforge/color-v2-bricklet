/* color-v2-bricklet
 * Copyright (C) 2018-2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * tcs3472.c: Driver for TCS3472 color sensor
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

#include "tcs3472.h"
#include "configs/config_tcs3472.h"

#include "communication.h"

#include "bricklib2/os/coop_task.h"
#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/logging/logging.h"

TCS3472 tcs3472;
CoopTask tcs3472_task;

inline uint32_t div_round_closest_unsigned(uint32_t n, uint32_t d) {
    return ((n + d/2)/d);
}

inline int32_t div_round_closest(int32_t n, int32_t d) {
    if((n < 0) ^ (d < 0)) {
        return ((n - d/2)/d);
    }

    return ((n + d/2)/d);
}

void tcs3472_calculate_color_temperature_and_illuminance(void) {
	const uint16_t value_r = tcs3472.rgbc.r;
	const uint16_t value_g = tcs3472.rgbc.g;
	const uint16_t value_b = tcs3472.rgbc.b;

    int32_t X = div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_X_G * value_g, TCS3472_COLOR_TEMPERATURE_DIV_X_G) -
                div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_X_R * value_r, TCS3472_COLOR_TEMPERATURE_DIV_X_R) -
                div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_X_B * value_b, TCS3472_COLOR_TEMPERATURE_DIV_X_B);

    int32_t Y = div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_Y_G * value_g, TCS3472_COLOR_TEMPERATURE_DIV_Y_G) -
 	            div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_Y_R * value_r, TCS3472_COLOR_TEMPERATURE_DIV_Y_R) -
                div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_Y_B * value_b, TCS3472_COLOR_TEMPERATURE_DIV_Y_B);

    int32_t Z = div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_Z_G * value_g, TCS3472_COLOR_TEMPERATURE_DIV_Z_G) -
    			div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_Z_R * value_r, TCS3472_COLOR_TEMPERATURE_DIV_Z_R) +
                div_round_closest_unsigned(TCS3472_COLOR_TEMPERATURE_MULT_Z_B * value_b, TCS3472_COLOR_TEMPERATURE_DIV_Z_B);

    tcs3472.illuminance = MAX(0, Y);

    if(X+Y+Z == 0) {
    	return;
    }

    int32_t xc = div_round_closest(X * TCS3472_COLOR_TEMPERATURE_MUL_XC, X + Y + Z);
    int32_t yc = div_round_closest(Y * TCS3472_COLOR_TEMPERATURE_MUL_YC, X + Y + Z);

    if(TCS3472_COLOR_TEMPERATURE_SUB_YC - yc == 0) {
    	return;
    }

    int32_t n_signed = div_round_closest((xc - TCS3472_COLOR_TEMPERATURE_SUB_XC)*TCS3472_COLOR_TEMPERATURE_MUL_N, TCS3472_COLOR_TEMPERATURE_SUB_YC - yc);

    int32_t sign = n_signed < 0 ? -1 : 1;
    uint32_t n = ABS(n_signed);

    uint32_t poly3 = div_round_closest_unsigned(div_round_closest_unsigned(div_round_closest_unsigned(449*n, TCS3472_COLOR_TEMPERATURE_MUL_N)*n, TCS3472_COLOR_TEMPERATURE_MUL_N)*n, TCS3472_COLOR_TEMPERATURE_MUL_N);
    uint32_t poly2 = div_round_closest_unsigned(div_round_closest_unsigned(3525*n, TCS3472_COLOR_TEMPERATURE_MUL_N)*n, TCS3472_COLOR_TEMPERATURE_MUL_N);
    uint32_t poly1 = div_round_closest_unsigned(6823*n, TCS3472_COLOR_TEMPERATURE_MUL_N);

    int32_t result = poly2 + sign*poly3 + sign*poly1 + 5520;
    if(result < 0) {
    	return;
    } else if(result > 0xFFFF) {
    	result = 0xFFFF;
    }

    tcs3472.color_temperature = result;
}

void tcs3472_task_clear_interrupt(void) {
	uint8_t clear = 0b11100110;
	i2c_fifo_coop_write_direct(&tcs3472.i2c_fifo, 1, &clear, true);
}

void tcs3472_task_write(uint8_t reg, uint8_t data) {
	i2c_fifo_coop_write_register(&tcs3472.i2c_fifo, reg | TCS3472_COMMAND_REG_SEL, 1, &data, true);
}

void tcs3472_task_read_colors(void) {
	uint16_t values[4];
	i2c_fifo_coop_read_register(&tcs3472.i2c_fifo, TCS3472_REG_RO_CDATAL | TCS3472_COMMAND_REG_SEL, 8, (uint8_t *)values);

	tcs3472.rgbc.r = values[1];
	tcs3472.rgbc.g = values[2];
	tcs3472.rgbc.b = values[3];
	tcs3472.rgbc.c = values[0];
}

void tcs3472_task_tick(void) {
	coop_task_sleep_ms(10);
	tcs3472_task_write(TCS3472_REG_RW_ENABLE, TCS3472_REG_ENABLE_AIEN | TCS3472_REG_ENABLE_AEN | TCS3472_REG_ENABLE_PON);
	tcs3472_task_write(TCS3472_REG_RW_PERS, 0x00); // Enabling interrupt every RGBC cycle

	while(true) {
		if(tcs3472.config_new) {
			tcs3472.config_new = false;
			tcs3472_task_write(TCS3472_REG_RW_CONTROL, tcs3472.config_gain);

			uint8_t atime;
			switch(tcs3472.config_integration_time) {
				case COLOR_V2_INTEGRATION_TIME_2MS:   atime = TCS3472_REG_ATIME_2MS;   break;
				case COLOR_V2_INTEGRATION_TIME_24MS:  atime = TCS3472_REG_ATIME_24MS;  break;
				case COLOR_V2_INTEGRATION_TIME_101MS: atime = TCS3472_REG_ATIME_101MS; break;
				case COLOR_V2_INTEGRATION_TIME_154MS: atime = TCS3472_REG_ATIME_154MS; break;
				case COLOR_V2_INTEGRATION_TIME_700MS: atime = TCS3472_REG_ATIME_700MS; break;
				default:                              atime = TCS3472_REG_ATIME_154MS; break;
			}
			tcs3472_task_write(TCS3472_REG_RW_ATIME, atime);
		}

		if(!XMC_GPIO_GetInput(TCS3472_INT_PIN)) {
			tcs3472_task_read_colors();
			tcs3472_task_clear_interrupt();

			tcs3472_calculate_color_temperature_and_illuminance();
		}

		coop_task_yield();
	}
}

void tcs3472_init_i2c(void) {
	tcs3472.i2c_fifo.baudrate         = TCS3472_I2C_BAUDRATE;
	tcs3472.i2c_fifo.address          = TCS3472_I2C_ADDRESS;
	tcs3472.i2c_fifo.i2c              = TCS3472_I2C;

	tcs3472.i2c_fifo.scl_port         = TCS3472_SCL_PORT;
	tcs3472.i2c_fifo.scl_pin          = TCS3472_SCL_PIN;
	tcs3472.i2c_fifo.scl_mode         = TCS3472_SCL_PIN_MODE;
	tcs3472.i2c_fifo.scl_input        = TCS3472_SCL_INPUT;
	tcs3472.i2c_fifo.scl_source       = TCS3472_SCL_SOURCE;
	tcs3472.i2c_fifo.scl_fifo_size    = TCS3472_SCL_FIFO_SIZE;
	tcs3472.i2c_fifo.scl_fifo_pointer = TCS3472_SCL_FIFO_POINTER;

	tcs3472.i2c_fifo.sda_port         = TCS3472_SDA_PORT;
	tcs3472.i2c_fifo.sda_pin          = TCS3472_SDA_PIN;
	tcs3472.i2c_fifo.sda_mode         = TCS3472_SDA_PIN_MODE;
	tcs3472.i2c_fifo.sda_input        = TCS3472_SDA_INPUT;
	tcs3472.i2c_fifo.sda_source       = TCS3472_SDA_SOURCE;
	tcs3472.i2c_fifo.sda_fifo_size    = TCS3472_SDA_FIFO_SIZE;
	tcs3472.i2c_fifo.sda_fifo_pointer = TCS3472_SDA_FIFO_POINTER;

	i2c_fifo_init(&tcs3472.i2c_fifo);
}

void tcs3472_init(void) {
	memset(&tcs3472, 0, sizeof(TCS3472));

	tcs3472.config_integration_time = COLOR_V2_INTEGRATION_TIME_154MS;
	tcs3472.config_gain             = COLOR_V2_GAIN_60X;
	tcs3472.config_new              = true;

	tcs3472_init_i2c();

	const XMC_GPIO_CONFIG_t config_low = {
		.mode                = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level        = XMC_GPIO_OUTPUT_LEVEL_LOW,
	};
	XMC_GPIO_Init(TCS3472_LED_PIN, &config_low);

	const XMC_GPIO_CONFIG_t config_input = {
		.mode                = XMC_GPIO_MODE_INPUT_PULL_UP,
		.input_hysteresis    = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};
	XMC_GPIO_Init(TCS3472_INT_PIN, &config_input);

	coop_task_init(&tcs3472_task, tcs3472_task_tick);
}

void tcs3472_tick(void) {
	coop_task_tick(&tcs3472_task);
}

uint16_t tcs3472_get_color_temperature(void) {
	return tcs3472.color_temperature;
}

uint32_t tcs3472_get_illuminance(void) {
	return tcs3472.illuminance;
}