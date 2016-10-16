/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016, Christopher Friedt <chrisfriedt@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LIBMCP9808_H_
#define LIBMCP9808_H_

#include <sys/cdefs.h>

#include <stdint.h>
#include <stdbool.h>

__BEGIN_DECLS

#define MCP9808_MANUFACTURER_ID_MSB 0x00
#define MCP9808_MANUFACTURER_ID_LSB 0x54

#define MCP9808_DEVICE_ID       0x04
#define MCP9808_DEVICE_REVISION 0x00

#define I2C_ADDR_MCP9808_MIN 0x18
#define I2C_ADDR_MCP9808_MAX 0x1f

#include "../libmcp9808/mcp9808-regs.h"

typedef enum {
	MCP9808_ALERT_OUTPUT_MODE_COMPARATOR,
	MCP9808_ALERT_OUTPUT_MODE_INTERRUPT,
} mcp9808_alert_output_mode_t;

typedef bool mcp9808_alert_output_polarity_t;
typedef enum {
	MCP9808_ALERT_OUTPUT_SELECT_TUPPER_TLOWER_TCRIT,
	MCP9808_ALERT_OUTPUT_SELECT_TA_GT_TCRIT,
} mcp9808_alert_output_select_t;
typedef bool mcp9808_alert_output_control_t;
typedef bool mcp9808_alert_output_status_t;
typedef bool mcp9808_interrupt_clear_t;
typedef bool mcp9808_window_locked_t;
typedef bool mcp9808_critical_locked_t;
typedef bool mcp9808_shutdown_mode_t;
typedef enum {
	MCP9808_THYST_0P0_DEGREES_C,
	MCP9808_THYST_1P5_DEGREES_C,
	MCP9808_THYST_3P0_DEGREES_C,
	MCP9808_THYST_6P0_DEGREES_C,
} mcp9808_hystoresis_t;
typedef enum {
	MCP9808_RESOLUTION_0P5000_DEGREES_C,
	MCP9808_RESOLUTION_0P2500_DEGREES_C,
	MCP9808_RESOLUTION_0P1250_DEGREES_C,
	MCP9808_RESOLUTION_0P0625_DEGREES_C,
} mcp9808_resolution_val_t;

typedef struct __attribute__(( packed )) {
	union {
		struct __attribute__(( packed )) {
			mcp9808_alert_output_mode_t      alert_mode        : 1;
			mcp9808_alert_output_polarity_t  alert_active_high : 1;
			mcp9808_alert_output_select_t    alert_select      : 1;
			mcp9808_alert_output_control_t   alert_enabled     : 1;
			mcp9808_alert_output_status_t    alert_asserted    : 1;
			mcp9808_interrupt_clear_t        interrupt_clear   : 1;
			mcp9808_window_locked_t          window_locked     : 1;
			mcp9808_critical_locked_t        critical_locked   : 1;
			mcp9808_shutdown_mode_t          low_power_enable  : 1;
			mcp9808_hystoresis_t t_hyst            : 2;
		} vector;
		uint16_t scalar : 11;
	} value;
} mcp9808_config_t;

typedef struct __attribute__(( packed )) {
	union {
		struct __attribute__(( packed )) {
			uint8_t fractional  : 4;
			uint8_t integral    : 8;
			bool    negative    : 1;
			bool    ta_lt_tlower : 1;
			bool    ta_gt_tupper : 1;
			bool    ta_ge_tcrit  : 1;
		} vector;
		uint16_t scalar : 16;
	} value;
} mcp9808_temperature_t;

typedef struct __attribute__(( packed )) {
	union {
		struct __attribute__(( packed )) {
			mcp9808_resolution_val_t resolution : 2;
		} vector;
		uint8_t scalar : 2;
	} value;
} mcp9808_resolution_t;

typedef struct __attribute__(( packed )) {
	union {
		struct __attribute__(( packed )) {
			uint16_t manufacturer_id       : 16;
		} vector;
		uint16_t scalar : 16;
	} value;
} mcp9808_manufacturer_id_t;

typedef struct __attribute__(( packed )) {
	union {
		struct __attribute__(( packed )) {
			uint8_t device_revision : 8;
			uint8_t device_id       : 8;
		} vector;
		uint16_t scalar : 16;
	} value;
} mcp9808_device_id_t;

int mcp9808_i2c_read( unsigned bus, uint16_t, void *buf, unsigned buf_len, uint8_t pointer );
int mcp9808_i2c_write( unsigned bus, uint16_t, void *buf, unsigned buf_len, uint8_t pointer );

static inline int mcp9808_config_get( unsigned bus, uint16_t addr, mcp9808_config_t *cfg ) {
	return mcp9808_i2c_read( bus, addr, cfg, sizeof( *cfg ), MCP9808_CONFIG );
}
static inline int mcp9808_config_set( unsigned bus, uint16_t addr, mcp9808_config_t *cfg ) {
	return mcp9808_i2c_write( bus, addr, cfg, sizeof( *cfg ), MCP9808_CONFIG );
}
static inline int mcp9808_temp_get( unsigned bus, uint16_t addr, mcp9808_temperature_t *t, uint8_t reg ) {
	return mcp9808_i2c_read( bus, addr, t, sizeof( *t ), reg );
}
static inline int mcp9808_temp_set( unsigned bus, uint16_t addr, mcp9808_temperature_t *t, uint8_t reg ) {
	return mcp9808_i2c_write( bus, addr, t, sizeof( *t ), reg );
}
static inline int mcp9808_tupper_get( unsigned bus, uint16_t addr, bool write, mcp9808_temperature_t *t ) {
	return mcp9808_temp_get( bus, addr, t, MCP9808_TUPPER );
}
static inline int mcp9808_tupper_set( unsigned bus, uint16_t addr, mcp9808_temperature_t *t ) {
	return mcp9808_temp_set( bus, addr, t, MCP9808_TUPPER );
}
static inline int mcp9808_tlower_get( unsigned bus, uint16_t addr, mcp9808_temperature_t *t ) {
	return mcp9808_temp_get( bus, addr, t, MCP9808_TLOWER );
}
static inline int mcp9808_tlower_set( unsigned bus, uint16_t addr, mcp9808_temperature_t *t ) {
	return mcp9808_temp_set( bus, addr, t, MCP9808_TLOWER );
}
static inline int mcp9808_tcrit_get( unsigned bus, uint16_t addr, mcp9808_temperature_t *t ) {
	return mcp9808_temp_get( bus, addr, t, MCP9808_TCRIT );
}
static inline int mcp9808_tcrit_set( unsigned bus, uint16_t addr, mcp9808_temperature_t *t ) {
	return mcp9808_temp_set( bus, addr, t, MCP9808_TCRIT );
}
static inline int mcp9808_tambient_get( unsigned bus, uint16_t addr, mcp9808_temperature_t *t ) {
	return mcp9808_temp_get( bus, addr, t, MCP9808_TA );
}
static inline int mcp9808_mid_get( unsigned bus, uint16_t addr, mcp9808_manufacturer_id_t *mid ) {
	return mcp9808_i2c_read( bus, addr, mid, sizeof( *mid ), MCP9808_MID );
}
static inline int mcp9808_did_get( unsigned bus, uint16_t addr, mcp9808_device_id_t *did ) {
	return mcp9808_i2c_read( bus, addr, did, sizeof( *did ), MCP9808_DID );
}
static inline int mcp9808_resolution_get( unsigned bus, uint16_t addr, mcp9808_resolution_t *res ) {
	return mcp9808_i2c_read( bus, addr, res, sizeof( *res ), MCP9808_RES );
}
static inline int mcp9808_resolution_set( unsigned bus, uint16_t addr, mcp9808_resolution_t *res ) {
	return mcp9808_i2c_write( bus, addr, res, sizeof( *res ), MCP9808_RES );
}

__END_DECLS

#endif // LIBMCP9808_H_
