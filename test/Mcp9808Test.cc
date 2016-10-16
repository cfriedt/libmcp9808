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

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/utsname.h>

#include <libgpio/Gpio.h>
#include <libi2c/I2c.h>
#include "libmcp9808/Mcp9808.h"

#include <gtest/gtest.h>

using namespace ::std;
using namespace ::com::github::cfriedt;

#define OSPASS_() \
	if ( ospass ) { \
		std::cout << "skipping test because this platform is not 'Linux'" << std::endl; \
		return; \
	} \

class Mcp9808Test : public testing::Test
{

public:

	static const float TUPPER_F;
	static const float TLOWER_F;
	static const float TCRIT_F;

	Mcp9808 mcp9808;
	Gpio gpio;
	bool ospass;

	Mcp9808Test();
	~Mcp9808Test();

	void SetUp();
	void TearDown();

	void reset();
};
const float Mcp9808Test::TUPPER_F = +25.0f;
const float Mcp9808Test::TLOWER_F = -5.0f;
const float Mcp9808Test::TCRIT_F  = 100.0f;

Mcp9808Test::Mcp9808Test()
: ospass( false )
{
}

Mcp9808Test::~Mcp9808Test() {
}

void Mcp9808Test::SetUp() {
	struct utsname utsname;
	int rr;

	rr = uname( & utsname );
	ASSERT_EQ( EXIT_SUCCESS, rr );
	ospass = 0 != strcmp( "Linux", utsname.sysname );
	if ( ! ospass ) {
		mcp9808 = Mcp9808( 0, I2C_ADDR_MCP9808_MIN );
		gpio = Gpio( 496, GPIO_EDGE_FALLING );
		reset();
		mcp9808_config_t cfg = mcp9808.config();

		ASSERT_EQ( MCP9808_CONFIG_DEFAULT, cfg.value.scalar );

		cfg.value.vector.alert_mode = MCP9808_ALERT_OUTPUT_MODE_INTERRUPT;
		cfg.value.vector.alert_enabled = true;

		mcp9808.config( cfg );
	}
}

void Mcp9808Test::TearDown() {
	OSPASS_();

	reset();
}

void Mcp9808Test::reset() {
	mcp9808_config_t config; config.value.scalar = MCP9808_CONFIG_DEFAULT;
	mcp9808_temperature_t tupper; tupper.value.scalar = MCP9808_TUPPER_DEFAULT;
	mcp9808_temperature_t tlower; tlower.value.scalar = MCP9808_TLOWER_DEFAULT;
	mcp9808_temperature_t tcrit; tcrit.value.scalar = MCP9808_TCRIT_DEFAULT;
	mcp9808_resolution_t res; res.value.scalar = MCP9808_RES_DEFAULT;

	mcp9808.config( config );
	mcp9808.tupper( tupper );
	mcp9808.tlower( tlower );
	mcp9808.tcrit( tcrit );
	mcp9808.resolution( res );
}

TEST_F( Mcp9808Test, CorrectEndianness ) {
	OSPASS_();

	uint16_t expected_uint16_t;
	uint16_t actual_uint16_t;

	mcp9808_temperature_t expected_temp;
	mcp9808_temperature_t actual_temp;

	expected_uint16_t = 0;
	actual_uint16_t = MCP9808_TUPPER_WRITE_MASK & MCP9808_TUPPER_READ_MASK;
	EXPECT_NE( expected_uint16_t, actual_uint16_t );

	expected_temp.value.scalar = MCP9808_TUPPER_WRITE_MASK & MCP9808_TUPPER_READ_MASK;
	EXPECT_EQ( actual_uint16_t, expected_temp.value.scalar );

	mcp9808.tupper( expected_temp );
	actual_temp = mcp9808.tupper();
	EXPECT_EQ( expected_temp.value.scalar, actual_temp.value.scalar );
}

TEST_F( Mcp9808Test, CorrectValueAfterSetup ) {
	OSPASS_();

	mcp9808_config_t expected_cfg;
	mcp9808_config_t actual_cfg;

	expected_cfg.value.scalar = 0;
	expected_cfg.value.vector.alert_enabled = true;
	expected_cfg.value.vector.alert_mode = MCP9808_ALERT_OUTPUT_MODE_INTERRUPT;
	actual_cfg = mcp9808.config();

//	std::cout << "expected config" << std::endl;
//	std::cout << expected_cfg << std::endl;
//	std::cout << "actual config" << std::endl;
//	std::cout << actual_cfg << std::endl;

	EXPECT_EQ( expected_cfg.value.scalar, actual_cfg.value.scalar );
}

TEST_F( Mcp9808Test, TestConfigRead ) {
	OSPASS_();

	mcp9808.config();
}

TEST_F( Mcp9808Test, TestConfigWrite ) {
	OSPASS_();

	mcp9808_config_t config;
	config.value.vector.alert_active_high = false;
	config.value.vector.alert_enabled = true;
	mcp9808.config( config );
}

TEST_F( Mcp9808Test, TestTupperRead ) {
	OSPASS_();

	mcp9808.tupper();
}
TEST_F( Mcp9808Test, TestTupperWrite ) {
	OSPASS_();

	mcp9808_temperature_t tupper;
	tupper.value.vector.negative = true;
	mcp9808.tupper( tupper );
}

TEST_F( Mcp9808Test, TestTlowerRead ) {
	OSPASS_();

	mcp9808.tlower();
}
TEST_F( Mcp9808Test, TestTlowerWrite ) {
	OSPASS_();

	mcp9808_temperature_t tlower;
	tlower.value.vector.negative = true;
	mcp9808.tlower( tlower );
}

TEST_F( Mcp9808Test, TestTcritRead ) {
	OSPASS_();

	mcp9808.tcrit();
}
TEST_F( Mcp9808Test, TestTcritWrite ) {
	OSPASS_();

	mcp9808_temperature_t tcrit;
	tcrit.value.vector.negative = true;
	mcp9808.tcrit( tcrit );
}

TEST_F( Mcp9808Test, TestTambientRead ) {
	OSPASS_();

	mcp9808.tambient();
}

TEST_F( Mcp9808Test, TestMidRead ) {
	OSPASS_();

	mcp9808_manufacturer_id_t expected_manufacturer_id_t;
	mcp9808_manufacturer_id_t actual_manufacturer_id_t;

	expected_manufacturer_id_t.value.scalar = MCP9808_MID_DEFAULT;

	actual_manufacturer_id_t = mcp9808.mid();

	EXPECT_EQ( expected_manufacturer_id_t.value.scalar, actual_manufacturer_id_t.value.scalar );
}

TEST_F( Mcp9808Test, TestDidRead ) {
	OSPASS_();

	mcp9808_device_id_t expected_device_id_t;
	mcp9808_device_id_t actual_device_id_t;

	expected_device_id_t.value.scalar = MCP9808_DID_DEFAULT;

	actual_device_id_t = mcp9808.did();

	EXPECT_EQ( expected_device_id_t.value.scalar, actual_device_id_t.value.scalar );
}

TEST_F( Mcp9808Test, TestResolutionRead ) {
	OSPASS_();

	mcp9808.resolution();
}
TEST_F( Mcp9808Test, TestResolutionWrite ) {
	OSPASS_();

	mcp9808_resolution_t res;
	res.value.vector.resolution = MCP9808_RESOLUTION_0P5000_DEGREES_C;
	mcp9808.resolution( res );
}

TEST_F( Mcp9808Test, TestWaitForInterrupt ) {
	OSPASS_();

	gpio.wait( 3000 );
}

TEST_F( Mcp9808Test, TestInterruptSet ) {
	OSPASS_();

	mcp9808_config_t config;
	mcp9808_temperature_t tambient;
	mcp9808_temperature_t tupper;
	mcp9808_temperature_t tlower;
	mcp9808_temperature_t tcrit;

	bool expected_gpio_value;
	bool actual_gpio_value;

	float expected_float;
	float actual_float;

	bool expected_bool;
	bool actual_bool;

	mcp9808.tlower( Mcp9808::degreesCelciusToTemperature( TLOWER_F ) );
	mcp9808.tupper( Mcp9808::degreesCelciusToTemperature( TUPPER_F ) );
	mcp9808.tcrit( Mcp9808::degreesCelciusToTemperature( TCRIT_F ) );


//	struct timeval then;
//	struct timeval now;
//	gettimeofday( & then, NULL );
	gpio.wait( 3000 );
//	gettimeofday( & now, NULL );

//	timersub( & now, & then, & now );
//	std::cout << "waited approximately " << now.tv_sec << "." << ( now.tv_usec / 1000 ) << " s" << std::endl;

	config = mcp9808.config();
//	std::cout << "config: " << config << std::endl;
//	std::cout << "config: " << std::hex << config.value.scalar << std::endl;

	expected_bool = true;
	actual_bool = config.value.vector.alert_asserted;
	EXPECT_EQ( expected_bool, actual_bool );

	// should be active low until cleared
	expected_gpio_value = GPIO_VALUE_LOW;
	actual_gpio_value = gpio.value();
	EXPECT_EQ( expected_gpio_value, actual_gpio_value );

	tambient = mcp9808.tambient();
	tcrit = mcp9808.tcrit();
	tupper = mcp9808.tupper();
	tlower = mcp9808.tlower();

	sleep( 1 );
	// should _still_ be low for an unhandled interrupt!

	expected_gpio_value = GPIO_VALUE_LOW;
	actual_gpio_value = gpio.value();
	EXPECT_EQ( expected_gpio_value, actual_gpio_value );

	expected_float = TCRIT_F;
	actual_float = Mcp9808::temperatureToDegreesCelcius( tambient );
	if ( tambient.value.vector.ta_ge_tcrit ) {
		EXPECT_GE( actual_float, expected_float );
	} else {
		EXPECT_LT( actual_float, expected_float );
	}

	expected_float = TUPPER_F;
	if ( tambient.value.vector.ta_gt_tupper ) {
		EXPECT_GT( actual_float, expected_float );
	} else {
		EXPECT_LE( actual_float, expected_float );
	}

	expected_float = TLOWER_F;
	if ( tambient.value.vector.ta_lt_tlower ) {
		EXPECT_LT( actual_float, expected_float );
	} else {
		EXPECT_GE( actual_float, expected_float );
	}

	config = mcp9808.config();
	config.value.vector.interrupt_clear = true;
	mcp9808.config( config );

	expected_gpio_value = GPIO_VALUE_HIGH;
	actual_gpio_value = gpio.value();
	EXPECT_EQ( expected_gpio_value, actual_gpio_value );
}
