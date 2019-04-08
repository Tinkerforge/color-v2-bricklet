/* color-v2-bricklet
 * Copyright (C) 2019 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define COLOR_V2_THRESHOLD_OPTION_OFF 'x'
#define COLOR_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define COLOR_V2_THRESHOLD_OPTION_INSIDE 'i'
#define COLOR_V2_THRESHOLD_OPTION_SMALLER '<'
#define COLOR_V2_THRESHOLD_OPTION_GREATER '>'

#define COLOR_V2_GAIN_1X 0
#define COLOR_V2_GAIN_4X 1
#define COLOR_V2_GAIN_16X 2
#define COLOR_V2_GAIN_60X 3

#define COLOR_V2_INTEGRATION_TIME_2MS 0
#define COLOR_V2_INTEGRATION_TIME_24MS 1
#define COLOR_V2_INTEGRATION_TIME_101MS 2
#define COLOR_V2_INTEGRATION_TIME_154MS 3
#define COLOR_V2_INTEGRATION_TIME_700MS 4

#define COLOR_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define COLOR_V2_BOOTLOADER_MODE_FIRMWARE 1
#define COLOR_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define COLOR_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define COLOR_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define COLOR_V2_BOOTLOADER_STATUS_OK 0
#define COLOR_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define COLOR_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define COLOR_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define COLOR_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define COLOR_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define COLOR_V2_STATUS_LED_CONFIG_OFF 0
#define COLOR_V2_STATUS_LED_CONFIG_ON 1
#define COLOR_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define COLOR_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_COLOR 1
#define FID_SET_COLOR_CALLBACK_CONFIGURATION 2
#define FID_GET_COLOR_CALLBACK_CONFIGURATION 3
#define FID_GET_ILLUMINANCE 5
#define FID_SET_ILLUMINANCE_CALLBACK_CONFIGURATION 6
#define FID_GET_ILLUMINANCE_CALLBACK_CONFIGURATION 7
#define FID_GET_COLOR_TEMPERATURE 9
#define FID_SET_COLOR_TEMPERATURE_CALLBACK_CONFIGURATION 10
#define FID_GET_COLOR_TEMPERATURE_CALLBACK_CONFIGURATION 11
#define FID_SET_LIGHT 13
#define FID_GET_LIGHT 14
#define FID_SET_CONFIG 15
#define FID_GET_CONFIG 16

#define FID_CALLBACK_COLOR 4
#define FID_CALLBACK_ILLUMINANCE 8
#define FID_CALLBACK_COLOR_TEMPERATURE 12

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetColor;

typedef struct {
	TFPMessageHeader header;
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t c;
} __attribute__((__packed__)) GetColor_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetColorCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetColorCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetColorCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t c;
} __attribute__((__packed__)) Color_Callback;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) SetLight;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetLight;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) GetLight_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t gain;
	uint8_t integration_time;
} __attribute__((__packed__)) SetConfig;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t gain;
	uint8_t integration_time;
} __attribute__((__packed__)) GetConfig_Response;


// Function prototypes
BootloaderHandleMessageResponse get_color(const GetColor *data, GetColor_Response *response);
BootloaderHandleMessageResponse set_color_callback_configuration(const SetColorCallbackConfiguration *data);
BootloaderHandleMessageResponse get_color_callback_configuration(const GetColorCallbackConfiguration *data, GetColorCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_light(const SetLight *data);
BootloaderHandleMessageResponse get_light(const GetLight *data, GetLight_Response *response);
BootloaderHandleMessageResponse set_config(const SetConfig *data);
BootloaderHandleMessageResponse get_config(const GetConfig *data, GetConfig_Response *response);

// Callbacks
bool handle_color_callback(void);
bool handle_illuminance_callback(void);
bool handle_color_temperature_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 3
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_color_callback, \
	handle_illuminance_callback, \
	handle_color_temperature_callback, \


#endif
