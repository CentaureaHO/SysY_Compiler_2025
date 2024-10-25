#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <cstring>
using namespace std;

long long convertToInt(const char* str, const char end, bool& isLongLong)
{
    int         base       = 10;
    long long   result     = 0;
    int         isNegative = 0;
    const char* ptr        = str;
    static int  zeroOffset = '0';
    static int  aOffset    = 'a' - 10;
    static int  AOffset    = 'A' - 10;
    int*        offset     = NULL;
    isLongLong             = false;  // 初始化为false，表示默认是int类型

    if (*ptr == '-')
    {
        isNegative = 1;
        ++ptr;
    }
    else if (*ptr == '+') { ++ptr; }

    if (ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X'))
    {
        base = 16;
        ptr += 2;
    }
    else if (ptr[0] == '0')
    {
        base = 8;
        ptr += 1;
    }

    while (*ptr != end)
    {
        int value = 0;

        if (base == 16)
        {
            if (*ptr >= '0' && *ptr <= '9')
                offset = &zeroOffset;
            else if (*ptr >= 'a' && *ptr <= 'f')
                offset = &aOffset;
            else if (*ptr >= 'A' && *ptr <= 'F')
                offset = &AOffset;
        }
        else { offset = &zeroOffset; }

        value  = *ptr - *offset;
        result = result * base + value;
        ++ptr;
    }

    if (isNegative) result = -result;

    // 如果结果超出了int范围，但在long long范围内，返回long long类型
    if (result > numeric_limits<int>::max() || result < numeric_limits<int>::min())
    {
        isLongLong = true;  // 标记返回的值超出int范围
        if (result > numeric_limits<long long>::max() || result < numeric_limits<long long>::min())
        {
            throw std::out_of_range(str + string(" overflow or underflow for long long"));
        }
    }

    return result;
}

float convertToFloatDec(const char* str)
{
    const char* head = str;
    if (str == NULL) { return 0.0f; }

    int sign = 1;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+') { str++; }

    double integerPart = 0.0;
    while (isdigit(*str))
    {
        integerPart = integerPart * 10 + (*str - '0');
        str++;
    }

    double fractionPart = 0.0;
    if (*str == '.')
    {
        str++;
        double divisor = 10.0;
        while (isdigit(*str))
        {
            fractionPart += (*str - '0') / divisor;
            divisor *= 10.0;
            str++;
        }
    }

    double value = integerPart + fractionPart;

    if (*str == 'e' || *str == 'E')
    {
        str++;
        int expSign = 1;
        if (*str == '-')
        {
            expSign = -1;
            str++;
        }
        else if (*str == '+') { str++; }

        int exponent = 0;
        while (isdigit(*str))
        {
            exponent = exponent * 10 + (*str - '0');
            str++;
        }

        value *= pow(10, expSign * exponent);
    }

    value = sign * value;

    if (value > numeric_limits<float>::max() || value < -numeric_limits<float>::max())
    {
        throw out_of_range(
            head + ((value > numeric_limits<float>::max()) ? string(" overflow") : string(" underflow")));
    }

    return static_cast<float>(value);
}

float convertToFloatHex(const char* str)
{
    const char* head = str;
    if (str == NULL) { return 0.0f; }

    int sign = 1;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+') { str++; }

    if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X')) { str += 2; }

    unsigned long long integerPart = 0;
    while (isxdigit(*str))
    {
        int digit;
        if (isdigit(*str)) { digit = (*str - '0'); }
        else if (*str >= 'a' && *str <= 'f') { digit = (*str - 'a' + 10); }
        else if (*str >= 'A' && *str <= 'F') { digit = (*str - 'A' + 10); }
        integerPart = integerPart * 16 + digit;
        str++;
    }

    double fractionPart = 0.0;
    if (*str == '.')
    {
        str++;
        unsigned long long numerator   = 0;
        unsigned long long denominator = 1;
        while (isxdigit(*str))
        {
            int digit;
            if (isdigit(*str)) { digit = (*str - '0'); }
            else if (*str >= 'a' && *str <= 'f') { digit = (*str - 'a' + 10); }
            else if (*str >= 'A' && *str <= 'F') { digit = (*str - 'A' + 10); }
            numerator = numerator * 16 + digit;
            denominator *= 16;
            str++;
        }
        fractionPart = (double)numerator / (double)denominator;
    }

    double value = (double)integerPart + fractionPart;

    if (*str == 'p' || *str == 'P')
    {
        str++;
        int expSign = 1;
        if (*str == '-')
        {
            expSign = -1;
            str++;
        }
        else if (*str == '+') { str++; }

        int exponent = 0;
        while (isdigit(*str))
        {
            exponent = exponent * 10 + (*str - '0');
            str++;
        }

        value *= pow(2, expSign * exponent);
    }

    value = sign * value;

    if (value > numeric_limits<float>::max() || value < -numeric_limits<float>::max())
    {
        throw out_of_range(
            head + ((value > numeric_limits<float>::max()) ? string(" overflow") : string(" underflow")));
    }

    return static_cast<float>(value);
}