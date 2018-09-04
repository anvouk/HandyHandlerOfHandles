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

#include "hhoh.h"

#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>

/*------------------------------------------------------------------------------
	Open Functions
------------------------------------------------------------------------------*/

// _O_CREAT, _O_TEXT, ...
BOOL
hhoh_open_cfd(HHANDLER* hd, const TCHAR* filename, int mode)
{
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/sopen-s-wsopen-s
	errno_t err = _tsopen_s(
		&hd->fd,
		filename,
		mode,
		_SH_DENYNO,
		_S_IREAD | _S_IWRITE
	);
	if (err != 0) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->current = HHOH_HANDLE_C_FD;
	return TRUE;
}

// CREATE_NEW, OPEN_EXISTING, ...
BOOL
hhoh_open_win32(HHANDLER* hd, const TCHAR* filename, DWORD mode, BOOL shared)
{
	// https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-createfilea
	HANDLE hfile = CreateFile(
		filename,
		GENERIC_READ | GENERIC_WRITE,
		shared ? FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE : 0,
		NULL,
		mode,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hfile == INVALID_HANDLE_VALUE) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->handle = hfile;
	hd->current = HHOH_HANDLE_WIN32;
	return TRUE;
}

// "r", "w", ...
BOOL
hhoh_open_file(HHANDLER* hd, const TCHAR* filename, const TCHAR* mode)
{
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(TCHAR%2F_tfopen)%3Bk(_tfopen)%3Bk(DevLang-C%2B%2B)%3Bk(TargetOS-Windows)%26rd%3Dtrue%26f%3D255%26MSPPError%3D-2147217396
	FILE* file = _tfopen(filename, mode);
	if (!file) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->file = file;
	hd->current = HHOH_HANDLE_FILE;
	return TRUE;
}

/*------------------------------------------------------------------------------
	Close Functions
------------------------------------------------------------------------------*/

BOOL
hhoh_close_cfd(HHANDLER* hd)
{
	if (hd->current != HHOH_HANDLE_C_FD)
		return FALSE;
	hd->current = HHOH_HANDLE_INVALID;
	return _close(hd->fd) == 0;
}

BOOL
hhoh_close_win32(HHANDLER* hd)
{
	if (hd->current != HHOH_HANDLE_WIN32)
		return FALSE;
	hd->current = HHOH_HANDLE_INVALID;
	return CloseHandle(hd->handle);
}

BOOL
hhoh_close_file(HHANDLER* hd)
{
	if (hd->current != HHOH_HANDLE_FILE)
		return FALSE;
	hd->current = HHOH_HANDLE_INVALID;
	return fclose(hd->file) == 0;
}

BOOL
hhoh_close_auto(HHANDLER* hd)
{
	switch (hd->current) {
		case HHOH_HANDLE_C_FD:
			return hhoh_close_cfd(hd);
		case HHOH_HANDLE_WIN32:
			return hhoh_close_win32(hd);
		case HHOH_HANDLE_FILE:
			return hhoh_close_file(hd);
		default:
			return FALSE;
	}
}

/*------------------------------------------------------------------------------
	Conversions
------------------------------------------------------------------------------*/

BOOL
hhoh_file_to_cfd(HHANDLER* hd)
{
	hd->fd = _fileno(hd->file);
	if (hd->fd == -1) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->current = HHOH_HANDLE_C_FD;
	return TRUE;
}

// "r", "w", ...
BOOL
hhoh_cfd_to_file(HHANDLER* hd, const TCHAR* mode)
{
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fdopen-wfdopen?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(TCHAR%2F_tfdopen)%3Bk(_tfdopen)%3Bk(DevLang-C%2B%2B)%3Bk(TargetOS-Windows)%26rd%3Dtrue%26f%3D255%26MSPPError%3D-2147217396
	hd->file = _tfdopen(hd->fd, mode);
	if (!hd->file) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->current = HHOH_HANDLE_FILE;
	return TRUE;
}

// _O_RDONLY, _O_TEXT, ...
BOOL
hhoh_win32_to_cfd(HHANDLER* hd, int flags)
{
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/open-osfhandle?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(CORECRT_IO%2F_open_osfhandle)%3Bk(_open_osfhandle)%3Bk(DevLang-C%2B%2B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	hd->fd = _open_osfhandle((intptr_t)hd->handle, flags);
	if (hd->fd == -1) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->current = HHOH_HANDLE_C_FD;
	return TRUE;
}

BOOL
hhoh_cfd_to_win32(HHANDLER* hd)
{
	hd->handle = (HANDLE)_get_osfhandle(hd->fd);
	if (hd->handle == INVALID_HANDLE_VALUE) {
		hd->current = HHOH_HANDLE_INVALID;
		return FALSE;
	}
	hd->current = HHOH_HANDLE_WIN32;
	return TRUE;
}
