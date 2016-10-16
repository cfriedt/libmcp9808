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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include "libmcp9808/libmcp9808.h"
#include <libi2c/libi2c.h>

#define MCP9808_MAX_BUF_LEN 2

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) ( sizeof( x ) / sizeof( (x)[ 0 ] ) )
#endif // ARRAY_SIZE

#ifndef min
#define min( a, b ) ( (a) < (b) ? (a) : (b) )
#endif // min

int mcp9808_i2c_read( unsigned bus, uint16_t addr, void *buf, unsigned buf_len, uint8_t pointer ) {

	int r;

	struct i2c_msg msg[ 2 ];

	memset( msg, 0, sizeof( msg ) );

	msg[ 0 ].addr = addr;
	msg[ 0 ].buf = & pointer;
	msg[ 0 ].len = 1;

	msg[ 1 ].addr = addr;
	msg[ 1 ].flags = I2C_M_RD;
	msg[ 1 ].buf = buf;
	msg[ 1 ].len = buf_len;

	r = i2c_transfer( bus, msg, ARRAY_SIZE( msg ) );
	if ( ARRAY_SIZE( msg ) == r ) {
		r = EXIT_SUCCESS;
		if ( sizeof( uint16_t ) == buf_len ) {
			*( (uint16_t *)buf ) = ntohs( *( (uint16_t *)buf ) );
		}
	}

	return r;
}

int mcp9808_i2c_write( unsigned bus, uint16_t addr, void *buf, unsigned buf_len, uint8_t pointer ) {

	int r;

	uint8_t wbuf[ 1 + MCP9808_MAX_BUF_LEN ];
	struct i2c_msg msg;
	unsigned m_len;

	m_len = min( MCP9808_MAX_BUF_LEN, buf_len );
	memset( & msg, 0, sizeof( msg ) );

	wbuf[ 0 ] = pointer;
	memcpy( & wbuf[ 1 ], buf, m_len );
	if ( sizeof( uint16_t ) + 1 == buf_len ) {
		*( (uint16_t *)& wbuf[ 1 ] ) = htons( *( (uint16_t *)& wbuf[ 1 ] ) );
	}

	r = i2c_write( bus, addr, wbuf, 1 + m_len );
	if ( 1 + m_len == r ) {
		r = EXIT_SUCCESS;
	}

	return r;
}
