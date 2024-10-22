#ifndef __COMMON_STR_CONVERT_H__
#define __COMMON_STR_CONVERT_H__

long long convertToInt(const char* str, const char end, bool& isLongLong);

float convertToFloatDec(const char* str);
float convertToFloatHex(const char* str);

#endif