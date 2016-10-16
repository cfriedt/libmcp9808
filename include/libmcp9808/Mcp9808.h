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

#ifndef com_github_cfriedt_Mcp9808_h_
#define com_github_cfriedt_Mcp9808_h_

#include <stdint.h>
#include "libmcp9808/libmcp9808.h"

#include <iostream>

namespace com {
namespace github {
namespace cfriedt {

class Mcp9808 {
public:
	Mcp9808();
	Mcp9808( unsigned bus, uint16_t addr );
	virtual ~Mcp9808();

	mcp9808_config_t config();
	void config( mcp9808_config_t config );
	void enable() {
		mcp9808_config_t cfg = config();
		cfg.value.vector.alert_enabled = true;
		config( cfg );
	}
	void disable() {
		mcp9808_config_t cfg = config();
		cfg.value.vector.alert_enabled = false;
		config( cfg );
	}

	mcp9808_temperature_t tupper() {
		return temp( MCP9808_TUPPER );
	}
	void tupper( mcp9808_temperature_t tupper ) {
		return temp( tupper, MCP9808_TUPPER );
	}

	mcp9808_temperature_t tlower() {
		return temp( MCP9808_TLOWER );
	}
	void tlower( mcp9808_temperature_t tlower ) {
		return temp( tlower, MCP9808_TLOWER );
	}

	mcp9808_temperature_t tcrit() {
		return temp( MCP9808_TCRIT );
	}
	void tcrit( mcp9808_temperature_t tcrit ) {
		return temp( tcrit, MCP9808_TCRIT );
	}

	mcp9808_temperature_t tambient() {
		return temp( MCP9808_TA );
	}

	mcp9808_manufacturer_id_t mid();
	mcp9808_device_id_t did();

	mcp9808_resolution_t resolution();
	void resolution( mcp9808_resolution_t res );

	static float temperatureToDegreesCelcius( mcp9808_temperature_t x );
	static mcp9808_temperature_t degreesCelciusToTemperature( float x );

protected:
	unsigned bus;
	uint16_t addr;

	mcp9808_temperature_t temp( int reg );
	void temp( mcp9808_temperature_t t, int reg );
};

}
}
} // com.github.cfriedt

std::ostream & operator<<( std::ostream & os, const mcp9808_config_t & cfg );

#endif // com_github_cfriedt_Mcp9808_h_
