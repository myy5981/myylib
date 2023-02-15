#ifndef MYY_URL_BASE64_H
#define MYY_URL_BASE64_H

#include <myy/env.h>

__CPP_BEGIN

extern size_t url_base64_encode(const void* src, size_t len, char* dest);
extern size_t url_base64_decode(const char* src, size_t len, void* dest);

__CPP_END

#endif
