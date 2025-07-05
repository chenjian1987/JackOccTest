#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

JACKC_COMMON_INTERFACE_EXPORT std::string base64_encode(unsigned char const*, unsigned int len);
JACKC_COMMON_INTERFACE_EXPORT std::string base64_decode(std::string const& s);