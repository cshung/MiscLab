#pragma once

#define IfFailRet(x) if ((result = x) != success) { goto error; }

enum result_t
{
    success,
    file_io_error,
};