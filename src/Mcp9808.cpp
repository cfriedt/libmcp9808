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

#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <math.h>

#include "libmcp9808/Mcp9808.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

Mcp9808::Mcp9808()
:
	bus( -1 ),
	addr( -1 )
{
}
Mcp9808::Mcp9808( unsigned bus, uint16_t addr )
:
	bus( bus ),
	addr( addr )
{
}
Mcp9808::~Mcp9808()
{
}

mcp9808_config_t Mcp9808::config() {
	mcp9808_config_t r;
	int rr;
	rr = mcp9808_config_get( bus, addr, & r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
	return r;
}
void Mcp9808::config( mcp9808_config_t config ) {
	int rr;
	rr = mcp9808_config_set( bus, addr, & config );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
}

mcp9808_temperature_t Mcp9808::temp( int reg ) {
	mcp9808_temperature_t r;
	int rr;
	rr = mcp9808_temp_get( bus, addr, & r, reg  );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
	return r;
}

void Mcp9808::temp( mcp9808_temperature_t t, int reg ) {
	int rr;
	rr = mcp9808_temp_set( bus, addr, & t, reg  );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
}

mcp9808_manufacturer_id_t Mcp9808::mid() {
	mcp9808_manufacturer_id_t r;
	int rr;
	rr = mcp9808_mid_get( bus, addr, & r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
	return r;
}

mcp9808_device_id_t Mcp9808::did() {
	mcp9808_device_id_t r;
	int rr;
	rr = mcp9808_did_get( bus, addr, & r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
	return r;
}

mcp9808_resolution_t Mcp9808::resolution() {
	mcp9808_resolution_t r;
	int rr;
	rr = mcp9808_resolution_get( bus, addr, & r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
	return r;
}

void Mcp9808::resolution( mcp9808_resolution_t res ) {
	int rr;
	rr = mcp9808_resolution_set( bus, addr, & res );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}
}

mcp9808_temperature_t Mcp9808::degreesCelciusToTemperature( float x ) {
	mcp9808_temperature_t r;

	float integral;
	float fractional;

	fractional = ::modff( ::fabs( x ), & integral );

	r.value.scalar = 0;
	r.value.vector.negative = x < 0 ? true : false;
	r.value.vector.integral = (unsigned) integral;
	r.value.vector.fractional = (unsigned) ( fractional * (float)(1 << 4) );

	return r;
}

float Mcp9808::temperatureToDegreesCelcius( mcp9808_temperature_t x ) {
	float y;
	float integral;
	float fractional;
	float sign;
	integral = x.value.vector.integral;
	fractional = (float)x.value.vector.fractional / (float)(1 << 4);
	sign = x.value.vector.negative ? -1.0f : 1.0f;
	y = sign * (integral + fractional);
	return y;
}

std::ostream & operator<<( std::ostream & os, const mcp9808_config_t & cfg ) {
	os
		<< "mcp9808_config_t:"
		<< std::endl << "\t" << "alert_mode: " << cfg.value.vector.alert_mode
		<< std::endl << "\t" << "alert_active_high: " << cfg.value.vector.alert_active_high
		<< std::endl << "\t" << "alert_select: " << cfg.value.vector.alert_select
		<< std::endl << "\t" << "alert_enabled: " << cfg.value.vector.alert_enabled
		<< std::endl << "\t" << "alert_asserted: " << cfg.value.vector.alert_asserted
		<< std::endl << "\t" << "interrupt_clear: " << cfg.value.vector.interrupt_clear
		<< std::endl << "\t" << "window_locked: " << cfg.value.vector.window_locked
		<< std::endl << "\t" << "critical_locked: " << cfg.value.vector.critical_locked
		<< std::endl << "\t" << "low_power_enable: " << cfg.value.vector.low_power_enable
		<< std::endl << "\t" << "t_hys: " << cfg.value.vector.t_hyst
	;
	return os;
}
