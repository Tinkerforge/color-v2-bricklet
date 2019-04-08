/* color-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "tcs3472.h"
#include "configs/config_tcs3472.h"
#include "xmc_gpio.h"

#define CALLBACK_VALUE_TYPE CALLBACK_VALUE_TYPE_UINT16
#include "bricklib2/utility/callback_value.h"
CallbackValue_uint16_t callback_value_color_temperature;

#undef CALLBACK_VALUE_TYPE
#define CALLBACK_VALUE_TYPE CALLBACK_VALUE_TYPE_UINT32
#include "bricklib2/utility/callback_value.h"
CallbackValue_uint32_t callback_value_illuminance;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_COLOR: return get_color(message, response);
		case FID_SET_COLOR_CALLBACK_CONFIGURATION: return set_color_callback_configuration(message);
		case FID_GET_COLOR_CALLBACK_CONFIGURATION: return get_color_callback_configuration(message, response);
		case FID_GET_ILLUMINANCE: return get_callback_value_uint32_t(message, response, &callback_value_illuminance);
		case FID_SET_ILLUMINANCE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint32_t(message, &callback_value_illuminance);
		case FID_GET_ILLUMINANCE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint32_t(message, response, &callback_value_illuminance);
		case FID_GET_COLOR_TEMPERATURE: return get_callback_value_uint16_t(message, response, &callback_value_color_temperature);
		case FID_SET_COLOR_TEMPERATURE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint16_t(message, &callback_value_color_temperature);
		case FID_GET_COLOR_TEMPERATURE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint16_t(message, response, &callback_value_color_temperature);
		case FID_SET_LIGHT: return set_light(message);
		case FID_GET_LIGHT: return get_light(message, response);
		case FID_SET_CONFIG: return set_config(message);
		case FID_GET_CONFIG: return get_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse get_color(const GetColor *data, GetColor_Response *response) {
	response->header.length = sizeof(GetColor_Response);
	response->r             = tcs3472.rgbc.r;
	response->g             = tcs3472.rgbc.g;
	response->b             = tcs3472.rgbc.b;
	response->c             = tcs3472.rgbc.c;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_color_callback_configuration(const SetColorCallbackConfiguration *data) {
	tcs3472.color_callback_period              = data->period;
	tcs3472.color_callback_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_color_callback_configuration(const GetColorCallbackConfiguration *data, GetColorCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetColorCallbackConfiguration_Response);
	response->period              = tcs3472.color_callback_period;
	response->value_has_to_change = tcs3472.color_callback_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_light(const SetLight *data) {
	if(data->enable) {
        XMC_GPIO_SetOutputHigh(TCS3472_LED_PIN);
	} else {
        XMC_GPIO_SetOutputLow(TCS3472_LED_PIN);
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_light(const GetLight *data, GetLight_Response *response) {
	response->header.length = sizeof(GetLight_Response);
	response->enable        = XMC_GPIO_GetInput(TCS3472_LED_PIN);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_config(const SetConfig *data) {
	if((data->gain > COLOR_V2_GAIN_60X) || (data->integration_time > COLOR_V2_INTEGRATION_TIME_700MS)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	tcs3472.config_integration_time = data->integration_time;
	tcs3472.config_gain             = data->gain;
	tcs3472.config_new              = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_config(const GetConfig *data, GetConfig_Response *response) {
	response->header.length    = sizeof(GetConfig_Response);
	response->integration_time = tcs3472.config_integration_time;
	response->gain             = tcs3472.config_gain;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}


bool handle_color_callback(void) {
	static bool is_buffered = false;
	static Color_Callback cb;
	static TCS3472RGBC last_rgbc = {0, 0, 0, 0};
	static uint32_t last_time = 0;

	if(!is_buffered) {
		if(tcs3472.color_callback_period == 0 || !system_timer_is_time_elapsed_ms(last_time, tcs3472.color_callback_period)) {
			return false;
		}

		if(tcs3472.color_callback_value_has_to_change && (last_rgbc.r == tcs3472.rgbc.r)  && (last_rgbc.g == tcs3472.rgbc.g) && (last_rgbc.b == tcs3472.rgbc.b) && (last_rgbc.c == tcs3472.rgbc.c)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Color_Callback), FID_CALLBACK_COLOR);
		cb.r = tcs3472.rgbc.r;
		cb.g = tcs3472.rgbc.g;
		cb.b = tcs3472.rgbc.b;
		cb.c = tcs3472.rgbc.c;
		
		last_rgbc.r = cb.r;
		last_rgbc.g = cb.g;
		last_rgbc.b = cb.b;
		last_rgbc.c = cb.c;

		last_time   = system_timer_get_ms();
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Color_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_illuminance_callback(void) {
	return handle_callback_value_callback_uint32_t(&callback_value_illuminance, FID_CALLBACK_ILLUMINANCE);
}

bool handle_color_temperature_callback(void) {
	return handle_callback_value_callback_uint16_t(&callback_value_color_temperature, FID_CALLBACK_COLOR_TEMPERATURE);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_uint16_t(&callback_value_color_temperature, tcs3472_get_color_temperature);
	callback_value_init_uint32_t(&callback_value_illuminance, tcs3472_get_illuminance);

	communication_callback_init();
}
