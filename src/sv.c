#include "pch.h"
#include "sv.h"
#include <string.h>

String_View
sv_make (const char *data, size_t count)
{
  return (String_View) { .data = data, .count = count };
}

bool
sv_equals (const String_View a, const String_View b)
{
  if (a.count != b.count)
    return false;

  return memcmp (a.data, b.data, a.count) == 0;
}
