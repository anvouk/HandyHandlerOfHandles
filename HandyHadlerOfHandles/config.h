/*
 * The MIT License
 *
 * Copyright 2018 Andrea Vouk.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

/**
 * @file
 *
 * This library uses <a href="https://semver.org/">Semantic Versioning 2.0.0</a>
 */

#define HHOH_STR_(x) #x
#define HHOH_STR HHOH_STR_

#define HHOH_VERSION_MAJOR 0
#define HHOH_VERSION_MINOR 1
#define HHOH_VERSION_PATCH 0
#define HHOH_VERSION_STATE "alpha"

#define _HHOH_VERSION_STR(major, minor, patch) \
	HHOH_STR(major) "." \
	HHOH_STR(minor) "." \
	HHOH_STR(patch) "-" \
	HHOH_VERSION_STATE

#define HHOH_VERSION_STR \
	_HHOH_VERSION_STR(HHOH_VERSION_MAJOR, HHOH_VERSION_MINOR, HHOH_VERSION_PATCH)

#define HHOH_VERSION_MAKE(major, minor, patch) \
	((major) << 16) | ((minor) << 8) | (patch))

/*------------------------------------------------------------------------------
	Linker settings
------------------------------------------------------------------------------*/

#ifndef HHOHAPI
#  define HHOHAPI
#endif

#ifndef HHOHCALL
#  define HHOHCALL
#endif
