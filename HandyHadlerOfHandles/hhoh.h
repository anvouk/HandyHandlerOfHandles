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

/*
 * Big thanks to: https://www.codeproject.com/articles/1044/a-handy-guide-to-handling-handles
 */

#include "config.h"

#include <windows.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
#if 0 /* auto-indent fix */
}
#endif

#define HHOH_HANDLE_INVALID	0
#define HHOH_HANDLE_C_FD	1
#define HHOH_HANDLE_WIN32	2
#define HHOH_HANDLE_FILE	3

typedef struct HHANDLER {
	union {
		int fd;
		HANDLE handle;
		FILE* file;
	};
	int current;
} HHANDLER;

/*------------------------------------------------------------------------------
	Open - Close
------------------------------------------------------------------------------*/

HHOHAPI BOOL HHOHCALL hhoh_open_cfd(HHANDLER* hd, const TCHAR* filename, int mode);
HHOHAPI BOOL HHOHCALL hhoh_open_win32(HHANDLER* hd, const TCHAR* filename, DWORD mode, BOOL shared);
HHOHAPI BOOL HHOHCALL hhoh_open_file(HHANDLER* hd, const TCHAR* filename, const TCHAR* mode);

HHOHAPI BOOL HHOHCALL hhoh_close_cfd(HHANDLER* hd);
HHOHAPI BOOL HHOHCALL hhoh_close_win32(HHANDLER* hd);
HHOHAPI BOOL HHOHCALL hhoh_close_file(HHANDLER* hd);

HHOHAPI BOOL HHOHCALL hhoh_close_auto(HHANDLER* hd);

/*------------------------------------------------------------------------------
	Conversions
------------------------------------------------------------------------------*/

HHOHAPI BOOL HHOHCALL hhoh_file_to_cfd(HHANDLER* hd);
HHOHAPI BOOL HHOHCALL hhoh_cfd_to_file(HHANDLER* hd, const TCHAR* mode);

HHOHAPI BOOL HHOHCALL hhoh_win32_to_cfd(HHANDLER* hd, int flags);
HHOHAPI BOOL HHOHCALL hhoh_cfd_to_win32(HHANDLER* hd);

#ifdef __cplusplus
}
#endif