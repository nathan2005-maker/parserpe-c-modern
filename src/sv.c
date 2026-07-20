#include "pch.h"
#include "sv.h"
#include <string.h>

String_View
sv_make(const char* data, size_t count)
{
    return (String_View) { .data = data, .count = count };
}

bool
sv_equals(const String_View a, const String_View b)
{
    if (a.count != b.count)
        return false;

    return memcmp(a.data, b.data, a.count) == 0;
}

bool
sv_has_prefix(const String_View sv, const String_View prefix)
{
    if (prefix.count > sv.count)
        return false;

    return memcmp(sv.data, prefix.data, prefix.count) == 0;
}

bool
sv_is_not_dquote(char c)
{
    return c != '"';
}

String_View
sv_chop_while(String_View* sv, String_View_Predicate predicate)
{
    size_t i = 0;
    while (i < sv->count && predicate(sv->data[i]))
        i++;

    String_View result = sv_make(sv->data, i);
    sv->data += i;
    sv->count -= i;
    return result;
}