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

#include <gtest/gtest.h>

#include <libgpio/Gpio.h>
#include <libi2c/I2c.h>
#include "libmcp9808/Mcp9808.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

class Mcp9808StaticTest : public testing::Test
{

public:

	Mcp9808 mcp9808;

	Mcp9808StaticTest();
	~Mcp9808StaticTest();

	void SetUp();
	void TearDown();
};

Mcp9808StaticTest::Mcp9808StaticTest()
{
}

Mcp9808StaticTest::~Mcp9808StaticTest() {
}

void Mcp9808StaticTest::SetUp() {
}

void Mcp9808StaticTest::TearDown() {
}

TEST_F( Mcp9808StaticTest, TestBitFieldZero ) {

	mcp9808_config_t config;
	unsigned expected_unsigned;
	unsigned actual_unsigned;

	config.value.scalar = 0;

	expected_unsigned = 0;

	actual_unsigned = config.value.vector.alert_mode;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.alert_active_high;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.alert_select;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.alert_enabled;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.alert_asserted;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.interrupt_clear;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.window_locked;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.critical_locked;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.low_power_enable;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	actual_unsigned = config.value.vector.t_hyst;
	EXPECT_EQ( expected_unsigned, actual_unsigned );
}

TEST_F( Mcp9808StaticTest, TestBitFieldOneOhOne ) {

	mcp9808_config_t config;
	unsigned expected_unsigned;
	unsigned actual_unsigned;

	config.value.scalar = 0x101;

	expected_unsigned = 1;
	actual_unsigned = config.value.vector.alert_mode;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.alert_active_high;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.alert_select;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.alert_enabled;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.alert_asserted;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.interrupt_clear;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.window_locked;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.critical_locked;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 1;
	actual_unsigned = config.value.vector.low_power_enable;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.t_hyst;
	EXPECT_EQ( expected_unsigned, actual_unsigned );
}

TEST_F( Mcp9808StaticTest, TestBitFieldOhOneNine ) {

	mcp9808_config_t config;
	unsigned expected_unsigned;
	unsigned actual_unsigned;

	config.value.scalar = 0x019;

	expected_unsigned = 1;
	actual_unsigned = config.value.vector.alert_mode;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.alert_active_high;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.alert_select;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 1;
	actual_unsigned = config.value.vector.alert_enabled;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 1;
	actual_unsigned = config.value.vector.alert_asserted;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.interrupt_clear;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.window_locked;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.critical_locked;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.low_power_enable;
	EXPECT_EQ( expected_unsigned, actual_unsigned );

	expected_unsigned = 0;
	actual_unsigned = config.value.vector.t_hyst;
	EXPECT_EQ( expected_unsigned, actual_unsigned );
}

TEST_F( Mcp9808StaticTest, TestDegreesCelciusToTemperature ) {
	float deg_c;
	mcp9808_temperature_t expected_t;
	mcp9808_temperature_t actual_t;

	deg_c = 0.0f;
	expected_t.value.scalar = 0;
	expected_t.value.vector.negative = 0;
	expected_t.value.vector.integral = 0;
	expected_t.value.vector.fractional = 0;

	actual_t = Mcp9808::degreesCelciusToTemperature( deg_c );
	EXPECT_EQ( expected_t.value.scalar, actual_t.value.scalar );

	deg_c = -25.0f;
	expected_t.value.scalar = 0;
	expected_t.value.vector.negative = 1;
	expected_t.value.vector.integral = 25;
	expected_t.value.vector.fractional = 0;

	actual_t = Mcp9808::degreesCelciusToTemperature( deg_c );
	EXPECT_EQ( expected_t.value.scalar, actual_t.value.scalar );

	deg_c = -25.25f;
	expected_t.value.scalar = 0;
	expected_t.value.vector.negative = 1;
	expected_t.value.vector.integral = 25;
	expected_t.value.vector.fractional = 0b0100;

	actual_t = Mcp9808::degreesCelciusToTemperature( deg_c );
	EXPECT_EQ( expected_t.value.scalar, actual_t.value.scalar );

	deg_c = 25.25f;
	expected_t.value.scalar = 0;
	expected_t.value.vector.negative = 0;
	expected_t.value.vector.integral = 25;
	expected_t.value.vector.fractional = 0b0100;

	actual_t = Mcp9808::degreesCelciusToTemperature( deg_c );
	EXPECT_EQ( expected_t.value.scalar, actual_t.value.scalar );

	deg_c = 25.00f;
	expected_t.value.scalar = 0;
	expected_t.value.vector.negative = 0;
	expected_t.value.vector.integral = 25;
	expected_t.value.vector.fractional = 0;

	actual_t = Mcp9808::degreesCelciusToTemperature( deg_c );
	EXPECT_EQ( expected_t.value.scalar, actual_t.value.scalar );
}

TEST_F( Mcp9808StaticTest, TestTemperatureToDegreesCelcius ) {
	mcp9808_temperature_t t;
	float expected_float;
	float actual_float;

	expected_float = 0.0f;
	t.value.scalar = 0;
	t.value.vector.negative = 0;
	t.value.vector.integral = 0;
	t.value.vector.fractional = 0;

	actual_float = Mcp9808::temperatureToDegreesCelcius( t );
	EXPECT_EQ( expected_float, actual_float );

	expected_float = -25.0f;
	t.value.scalar = 0;
	t.value.vector.negative = 1;
	t.value.vector.integral = 25;
	t.value.vector.fractional = 0;

	actual_float = Mcp9808::temperatureToDegreesCelcius( t );
	EXPECT_EQ( expected_float, actual_float );

	expected_float = -25.25f;
	t.value.scalar = 0;
	t.value.vector.negative = 1;
	t.value.vector.integral = 25;
	t.value.vector.fractional = 0b0100;

	actual_float = Mcp9808::temperatureToDegreesCelcius( t );
	EXPECT_EQ( expected_float, actual_float );

	expected_float = 25.25f;
	t.value.scalar = 0;
	t.value.vector.negative = 0;
	t.value.vector.integral = 25;
	t.value.vector.fractional = 0b0100;

	actual_float = Mcp9808::temperatureToDegreesCelcius( t );
	EXPECT_EQ( expected_float, actual_float );

	expected_float = 25.0f;
	t.value.scalar = 0;
	t.value.vector.negative = 0;
	t.value.vector.integral = 25;
	t.value.vector.fractional = 0;

	actual_float = Mcp9808::temperatureToDegreesCelcius( t );
	EXPECT_EQ( expected_float, actual_float );
}
