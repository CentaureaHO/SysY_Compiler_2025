#include <str/format_str.h>
#include <iostream>
using namespace std;

string getFirstPrefix(string str, bool isLast)
{
    size_t pos = str.rfind('|');
    if (pos != string::npos && pos + 3 < str.size())
    {
        str[pos]     = isLast ? '`' : '|';
        str[pos + 1] = '-';
        str[pos + 2] = '-';
        str[pos + 3] = ' ';
    }
    return str;
}

string removeLastPrefix(string str)
{
    size_t pos = str.rfind('|');
    if (pos != string::npos && pos + 3 < str.size())
    {
        str[pos]     = ' ';
        str[pos + 1] = ' ';
        str[pos + 2] = ' ';
        str[pos + 3] = ' ';
    }
    if (pos + 4 < str.size()) str.erase(pos + 4);
    return str;
}