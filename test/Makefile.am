# The MIT License (MIT)
# 
# Copyright (c) 2016, Christopher Friedt <chrisfriedt@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

if HAVE_GTEST

noinst_PROGRAMS += \
	test/Mcp9808Test

test_Mcp9808Test_SOURCES = \
	test/Mcp9808Test.cc
test_Mcp9808Test_DEPENDENCIES = \
	src/libmcp9808.la \
	src/libmcp9808++.la
test_Mcp9808Test_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	@LIBI2C_CFLAGS@ \
	@LIBGPIO_CFLAGS@ \
	@GTEST_CPPFLAGS@
test_Mcp9808Test_LDFLAGS = \
	@GTEST_LDFLAGS@
test_Mcp9808Test_LDADD = \
	$(test_Mcp9808Test_DEPENDENCIES) \
	@LIBI2C_LIBS@ \
	@LIBGPIO_LIBS@ \
	@GTEST_LIBS@

TESTS += test/Mcp9808Test

endif