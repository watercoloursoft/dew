#include "dew/string_util.h"
#include "dew/collections/array.h"
#include "dew/memory/allocator.h"

#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_STATIC
DEW_PRAGMA_DIAGNOSTIC_PUSH()
DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-function")
DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-parameter")
DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wsign-compare")
DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wstrict-aliasing")
#include "dew/internal/stb_sprintf.h"
DEW_PRAGMA_DIAGNOSTIC_POP();

// Mattias' strpool implementation
#define STRPOOL_IMPLEMENTATION
#define STRPOOL_MEMSET(ptr, val, cnt) (dew_memset(ptr, val, cnt))
#define STRPOOL_MEMCPY(dst, src, cnt) (dew_memcpy(dst, src, cnt))
#define STRPOOL_MEMCMP(pr1, pr2, cnt) (dew_memcmp(pr1, pr2, cnt))
//#define STRPOOL_STRNICMP(s1, s2, len)   ( dew_strncmpnocase(s1, s2, len) )
#define STRPOOL_MALLOC(ctx, size) (dew_malloc((const dew_allocator *)ctx, size))
#define STRPOOL_FREE(ctx, ptr) (dew_free((const dew_allocator *)ctx, ptr))
DEW_PRAGMA_DIAGNOSTIC_PUSH()
DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-parameter")
DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wsign-compare")
#include "dew/internal/strpool.h"
DEW_PRAGMA_DIAGNOSTIC_POP()

DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC(
    "-Wincompatible-pointer-types-discards-qualifiers")

typedef struct dew__printf_ctx_s {
  const dew_allocator *alloc;
  char *buff;
  i32 len;
  char tmp[STB_SPRINTF_MIN];
} dew__printf_ctx;

i32 dew_snprintf(char *str, i32 size, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  i32 r = dew_vsnprintf(str, size, fmt, args);
  va_end(args);
  return r;
}

i32 dew_vsnprintf(char *str, i32 size, const char *fmt, va_list args) {
  return stbsp_vsnprintf(str, size, fmt, args);
}

char *dew_snprintf_alloc(const dew_allocator *alloc, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *str = dew_vsnprintf_alloc(alloc, fmt, args);
  va_end(args);
  return str;
}

static char *dew__vsnprintf_callback(const char *buff, void *user, i32 len) {
  dew_unused(buff);
  dew__printf_ctx *ctx = (dew__printf_ctx *)user;
  i32 len_ = len + 1; // Reserve one character for null-termination
  dew_array_add(ctx->alloc, ctx->buff, len_);
  dew_memcpy(ctx->buff + ctx->len, ctx->tmp, len);
  ctx->len += len;
  return ctx->tmp;
}

char *dew_vsnprintf_alloc(const dew_allocator *alloc, const char *fmt,
                          va_list args) {
  dew__printf_ctx ctx;
  ctx.alloc = alloc;
  ctx.buff = NULL;
  ctx.len = 0;
  stbsp_vsprintfcb(dew__vsnprintf_callback, &ctx, ctx.tmp, fmt, args);
  ctx.buff[ctx.len] = '\0';
  return ctx.buff;
}

char *dew_strcpy(char *DEW_RESTRICT dst, i32 dst_sz,
                 const char *DEW_RESTRICT src) {
  dew_assert(dst);
  dew_assert(src);

  const i32 len = dew_strlen(src);
  const int32_t max = dst_sz - 1;
  const int32_t num = (len < max ? len : max);
  if (num > 0) {
    dew_memcpy((madd)dst, (const madd)src, num);
  }
  dst[num] = '\0';

  return &dst[num];
}

// https://github.com/lattera/glibc/blob/master/string/strlen.c
i32 dew_strlen(const char *str) {
  const char *char_ptr;
  const uintptr_t *longword_ptr;
  uintptr_t longword, himagic, lomagic;

  for (char_ptr = str; ((uintptr_t)char_ptr & (sizeof(longword) - 1)) != 0;
       ++char_ptr) {
    if (*char_ptr == '\0')
      return (i32)(intptr_t)(char_ptr - str);
  }
  longword_ptr = (uintptr_t *)char_ptr;
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
#if DEW_ARCH_64BIT
  /* 64-bit version of the magic.  */
  /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
  himagic = ((himagic << 16) << 16) | himagic;
  lomagic = ((lomagic << 16) << 16) | lomagic;
#endif

  for (;;) {
    longword = *longword_ptr++;

    if (((longword - lomagic) & ~longword & himagic) != 0) {
      const char *cp = (const char *)(longword_ptr - 1);

      if (cp[0] == 0)
        return (i32)(intptr_t)(cp - str);
      if (cp[1] == 0)
        return (i32)(intptr_t)(cp - str + 1);
      if (cp[2] == 0)
        return (i32)(intptr_t)(cp - str + 2);
      if (cp[3] == 0)
        return (i32)(intptr_t)(cp - str + 3);
#if DEW_ARCH_64BIT
      if (cp[4] == 0)
        return (i32)(intptr_t)(cp - str + 4);
      if (cp[5] == 0)
        return (i32)(intptr_t)(cp - str + 5);
      if (cp[6] == 0)
        return (i32)(intptr_t)(cp - str + 6);
      if (cp[7] == 0)
        return (i32)(intptr_t)(cp - str + 7);
#endif
    }
  }

#if !DEW_COMPILER_MSVC
  dew_assertf(0, "Not a null-terminated string");
  return -1;
#endif
}

static inline i32 dew__strnlen(const char *str, i32 _max) {
  const char *char_ptr;
  const uintptr_t *longword_ptr;
  uintptr_t longword, himagic, lomagic;

  for (char_ptr = str; ((uintptr_t)char_ptr & (sizeof(longword) - 1)) != 0;
       ++char_ptr) {
    if (*char_ptr == '\0') {
      i32 _len = (i32)(uintptr_t)(char_ptr - str);
      return (_len > _max) ? _max : _len;
    }
  }

  longword_ptr = (uintptr_t *)char_ptr;
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
#if DEW_ARCH_64BIT
  /* 64-bit version of the magic.  */
  /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
  himagic = ((himagic << 16) << 16) | himagic;
  lomagic = ((lomagic << 16) << 16) | lomagic;
#endif

  for (;;) {
    longword = *longword_ptr++;

    if (((longword - lomagic) & ~longword & himagic) != 0) {
      const char *cp = (const char *)(longword_ptr - 1);
      i32 base_offset = (i32)(intptr_t)(cp - str);
      if (base_offset > _max)
        return _max;

      if (cp[0] == 0)
        return dew_min(_max, base_offset);
      if (cp[1] == 0)
        return dew_min(_max, base_offset + 1);
      if (cp[2] == 0)
        return dew_min(_max, base_offset + 2);
      if (cp[3] == 0)
        return dew_min(_max, base_offset + 3);
#if DEW_ARCH_64BIT
      if (cp[4] == 0)
        return dew_min(_max, base_offset + 4);
      if (cp[5] == 0)
        return dew_min(_max, base_offset + 5);
      if (cp[6] == 0)
        return dew_min(_max, base_offset + 6);
      if (cp[7] == 0)
        return dew_min(_max, base_offset + 7);
#endif
    }
  }

#if !DEW_COMPILER_MSVC
  dew_assertf(0, "Not a null-terminated string");
  return -1;
#endif
}

char *dew_strncpy(char *DEW_RESTRICT dst, i32 dst_sz,
                  const char *DEW_RESTRICT src, i32 _num) {
  dew_assert(dst);
  dew_assert(src);

  const i32 len = dew__strnlen(src, _num);
  const int32_t max = dst_sz - 1;
  const int32_t num = (len < max ? len : max);
  if (num > 0) {
    dew_memcpy((madd)dst, (const madd)src, num);
  }
  dst[num] = '\0';

  return &dst[num];
}

char *dew_strcat(char *DEW_RESTRICT dst, i32 dst_sz,
                 const char *DEW_RESTRICT src) {
  dew_assert(dst);
  dew_assert(src);
  dew_assert(dst_sz > 0);

  i32 len = dew_strlen(dst);
  return dew_strcpy(dst + len, dst_sz - len, src);
}

char *dew_strncat(char *DEW_RESTRICT dst, i32 dst_sz,
                  const char *DEW_RESTRICT src, i32 _num) {
  dew_assert(dst);
  dew_assert(src);
  dew_assert(dst_sz > 0);

  i32 len = dew_strlen(dst);
  return dew_strncpy(dst + len, dst_sz - len, src, _num);
}

bool dew_isspace(char ch) {
  return (u32)(ch - 1) < 32 && ((0x80001F00 >> (u32)(ch - 1)) & 1) == 1;
}

// https://github.com/lattera/glibc/blob/master/string/strrchr.c
const char *dew_strrchar(const char *str, char ch) {
  const char *found = NULL, *p;
  ch = (uint8_t)ch;

  if (ch == '\0')
    return dew_strchar(str, '\0');
  while ((p = dew_strchar(str, ch)) != NULL) {
    found = p;
    str = p + 1;
  }
  return (const char *)found;
}

// https://github.com/lattera/glibc/blob/master/string/strchr.c
const char *dew_strchar(const char *str, char ch) {
  const uint8_t *char_ptr;
  uintptr_t *longword_ptr;
  uintptr_t longword, magic_bits, charmask;
  uint8_t c = (uint8_t)ch;

  // Handle the first few characters by reading one character at a time.
  // Do this until CHAR_PTR is aligned on a longword boundary.
  for (char_ptr = (const uint8_t *)str;
       ((uintptr_t)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr) {
    if (*char_ptr == c)
      return (const char *)char_ptr;
    else if (*char_ptr == '\0')
      return NULL;
  }

  longword_ptr = (uintptr_t *)char_ptr;
  magic_bits = (uintptr_t)-1;
  magic_bits = magic_bits / 0xff * 0xfe << 1 >> 1 | 1;
  charmask = c | (c << 8);
  charmask |= charmask << 16;
#if DEW_ARCH_64BIT
  charmask |= (charmask << 16) << 16;
#endif

  for (;;) {
    longword = *longword_ptr++;

    if ((((longword + magic_bits) ^ ~longword) & ~magic_bits) != 0 ||
        ((((longword ^ charmask) + magic_bits) ^ ~(longword ^ charmask)) &
         ~magic_bits) != 0) {
      const uint8_t *cp = (const uint8_t *)(longword_ptr - 1);

      if (*cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
#if DEW_ARCH_64BIT
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
      if (*++cp == c)
        return (const char *)cp;
      else if (*cp == '\0')
        break;
#endif
    }
  }

  return NULL;
}

const char *dew_strstr(const char *DEW_RESTRICT str,
                       const char *DEW_RESTRICT find) {
  dew_assert(str);
  dew_assert(find);

  char ch = find[0];
  const char *_start = dew_strchar(str, ch);
  i32 find_len = dew_strlen(find);
  i32 len = dew_strlen(str);

  while (_start) {
    // We have the first character, check the rest
    len -= (i32)(intptr_t)(_start - str);
    if (len < find_len)
      return NULL;
    str = _start;

    if (dew_memcmp((const madd)_start, (const madd)find, find_len) == 0)
      return str;

    _start = dew_strchar(_start + 1, ch);
  }

  return NULL;
}

// https://github.com/clibs/wildcardcmp
bool dew_strstr_wildcard(const char *str, const char *pattern) {
  dew_assert(str);
  dew_assert(pattern);

  const char *w = NULL; // last `*`
  const char *s = NULL; // last checked char

  // loop 1 char at a time
  while (1) {
    if (!*str) {
      if (!*pattern)
        return true;
      dew_assert(s);
      if (!*s)
        return false;
      str = s++;
      pattern = w;
      continue;
    } else {
      dew_assert(pattern);
      if (*pattern != *str) {
        if ('*' == *pattern) {
          w = ++pattern;
          s = str;
          // "*" -> "foobar"
          if (*pattern)
            continue;
          return true;
        } else if (w) {
          str++;
          // "*ooba*" -> "foobar"
          continue;
        }
        return false;
      }
    }

    str++;
    pattern++;
  }

  return true;
}

bool dew_strequal(const char *DEW_RESTRICT a, const char *DEW_RESTRICT b) {
  i32 alen = dew_strlen(a);
  i32 blen = dew_strlen(b);
  if (alen != blen)
    return false;

  for (i32 i = 0; i < alen; i++) {
    if (a[i] != b[i])
      return false;
  }
  return true;
}

bool dew_strequalnocase(const char *DEW_RESTRICT a,
                        const char *DEW_RESTRICT b) {
  i32 alen = dew_strlen(a);
  i32 blen = dew_strlen(b);
  if (alen != blen)
    return false;

  for (i32 i = 0; i < alen; i++) {
    if (dew_tolowerchar(a[i]) != dew_tolowerchar(b[i]))
      return false;
  }
  return true;
}

bool dew_strnequal(const char *DEW_RESTRICT a, const char *DEW_RESTRICT b,
                   i32 num) {
  i32 _alen = dew_strlen(a);
  i32 _blen = dew_strlen(b);
  i32 alen = dew_min(num, _alen);
  i32 blen = dew_min(num, _blen);
  if (alen != blen)
    return false;

  for (i32 i = 0; i < alen; i++) {
    if (a[i] != b[i])
      return false;
  }
  return true;
}

bool dew_strnequalnocase(const char *DEW_RESTRICT a, const char *DEW_RESTRICT b,
                         i32 num) {
  i32 _alen = dew_strlen(a);
  i32 _blen = dew_strlen(b);
  i32 alen = dew_min(num, _alen);
  i32 blen = dew_min(num, _blen);
  if (alen != blen)
    return false;

  for (i32 i = 0; i < alen; i++) {
    if (dew_tolowerchar(a[i]) != dew_tolowerchar(b[i]))
      return false;
  }
  return true;
}

char dew_tolowerchar(char ch) { return ch + (dew_isupperchar(ch) ? 0x20 : 0); }

char dew_toupperchar(char ch) { return ch - (dew_islowerchar(ch) ? 0x20 : 0); }

bool dew_isrange(char ch, char from, char to) {
  return (uint8_t)(ch - from) <= (uint8_t)(to - from);
}

bool dew_isalphachar(char ch) {
  return dew_isrange(ch, 'a', 'z') || dew_isrange(ch, 'A', 'Z');
}
bool dew_isalpha(const char *str, usize size) {
  dew_assert(str);
  i32 len = 0;
  for (const char *c = str; *c || size == len; c++, len++) {
    if (dew_isalphachar(*c))
      return false;
  }
  return true;
}

bool dew_isupperchar(char ch) { return dew_isrange(ch, 'A', 'Z'); }
bool dew_isupper(const char *str, usize size) {
  dew_assert(str);
  i32 len = 0;
  for (const char *c = str; *c || size == len; c++, len++) {
    if (dew_isupperchar(*c))
      return false;
  }
  return true;
}

bool dew_islowerchar(char ch) { return dew_isrange(ch, 'a', 'z'); }
bool dew_islower(const char *str, usize size) {
  dew_assert(str);
  i32 len = 0;
  for (const char *c = str; *c || size == len; c++, len++) {
    if (dew_isupperchar(*c))
      return false;
  }
  return true;
}

const char *dew_skip_whitespace(const char *str) {
  while (*str) {
    if (dew_isspace(*str))
      ++str;
    else
      break;
  }
  return str;
}

const char *dew_skip_word(const char *str) {
  for (char ch = *str++;
       ch > 0 && (dew_islowerchar(ch) || dew_isupperchar(ch) ||
                  dew_isnumchar(ch) || ch == '_');
       ch = *str++) {
  }
  return str - 1;
}

char *dew_trim_whitespace(char *dest, i32 dest_sz, const char *src) {
  i32 len = dew_min(dew_strlen(src), dest_sz - 1);
  i32 offset = 0;
  for (i32 i = 0; i < len; i++) {
    if (!dew_isspace(src[i]))
      dest[offset++] = src[i];
  }
  dest[offset] = '\0';
  return dest;
}

char *dew_trim(char *dest, i32 dest_sz, const char *src, const char *trim) {
  i32 len = dew_min(dew_strlen(src), dest_sz - 1);
  i32 offset = 0;
  for (i32 i = 0; i < len; i++) {
    const char *t = trim;
    char sch = src[i];
    bool trim_it = false;
    while (*t) {
      if (sch != *t) {
        ++t;
        continue;
      }
      trim_it = true;
      break;
    }

    if (!trim_it)
      dest[offset++] = src[i];
  }
  dest[offset] = '\0';
  return dest;
}

char *dew_trimchar(char *dest, i32 dest_sz, const char *src, char trim_ch) {
  i32 len = dew_min(dew_strlen(src), dest_sz - 1);
  i32 offset = 0;
  for (i32 i = 0; i < len; i++) {
    if (trim_ch != src[i])
      dest[offset++] = src[i];
  }
  dest[offset] = '\0';
  return dest;
}

char *dew_replace(char *dest, i32 dest_sz, const char *src, const char *find,
                  const char *replace) {
  dew_assert(dest != src);

  char f = find[0];
  i32 flen = dew_strlen(find);
  i32 rlen = dew_strlen(replace);
  i32 srclen = dew_strlen(src);
  i32 offset = 0;
  i32 dest_max = dest_sz - 1;
  const char *start = src;

  while (*src && offset < dest_max) {
    // Found first character, check for rest
    if (f != *src) {
      dest[offset++] = *src;
    } else {
      srclen -= (i32)(intptr_t)(src - start);

      if (srclen >= flen &&
          dew_memcmp((const madd)src, (const madd)find, flen) == 0) {
        src += flen;
        i32 l = dew_min(dest_max - offset, rlen);
        dew_memcpy(dest + offset, (const madd)replace, l);
        offset += l;
      } else {
        dest[offset++] = *src;
      }
    }

    ++src;
  }

  dest[offset] = '\0';
  return dest;
}

char *dew_replacechar(char *dest, i32 dest_sz, const char *src, const char find,
                      const char replace) {
  i32 dest_max = dest_sz - 1;
  i32 offset = 0;
  while (*src && offset < dest_max) {
    if (*src != find)
      dest[offset] = *src;
    else
      dest[offset] = replace;
    ++src;
  }
  dest[offset] = '\0';
  return dest;
}

char *dew_EOL_LF(char *dest, i32 dest_sz, const char *src) {
  assert(dest_sz > 0);
  char *end = dest + dest_sz - 1;
  for (char ch = *src++; ch != '\0' && dest < end; ch = *src++) {
    if ('\r' != ch)
      *dest++ = ch;
  }
  *dest = '\0';
  return dest;
}

bool dew_split(char *dest1, i32 dest1_sz, char *dest2, i32 dest2_sz,
               const char *src, char splitch) {
  const char *sptr = dew_strchar(src, splitch);
  if (sptr) {
    dew_strncpy(dest1, dest1_sz, src, (i32)(uintptr_t)(sptr - src));
    dew_strcpy(dest2, dest2_sz, src + 1);
    return true;
  }

  return false;
}

dew_str_block dew_findblock(const char *str, char open, char close) {
  i32 count = 0;
  dew_str_block b = {NULL, NULL};

  for (char ch = *str; ch && count >= 0; ch = *++str) {
    if (!b.start && ch == open) {
      b.start = str + 1;
    } else if (b.start && ch == close) {
      b.end = str - 1;
      return b;
    }
  }

  return b;
}

bool dew_isnumchar(char ch) { return dew_isrange(ch, '0', '9'); }

bool dew_isnum(const char *str) {
  while (*str) {
    if (!dew_isnumchar(*str))
      return false;
    ++str;
  }
  return true;
}

bool dew_ishexchar(char ch) {
  return dew_isrange(dew_tolowerchar(ch), 'a', 'f') || dew_isnumchar(ch);
}

bool dew_ishex(const char *str) {
  while (*str) {
    if (!dew_ishexchar(*str))
      return false;
    ++str;
  }
  return true;
}

i32 dew_hexvalue(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return 0;
}

char *dew_tolower(char *dst, i32 dst_sz, const char *str) {
  i32 offset = 0;
  i32 dst_max = dst_sz - 1;
  while (*str && offset < dst_max) {
    dst[offset++] = dew_tolowerchar(*str);
    ++str;
  }
  dst[offset] = '\0';
  return dst;
}

char *dew_toupper(char *dst, i32 dst_sz, const char *str) {
  i32 offset = 0;
  i32 dst_max = dst_sz - 1;
  while (*str && offset < dst_max) {
    dst[offset++] = dew_toupperchar(*str);
    ++str;
  }
  dst[offset] = '\0';
  return dst;
}

bool dew_tobool(const char *str) {
  char ch = dew_tolowerchar(str[0]);
  return ch == 't' || ch == '1';
}

i32 dew_toint(const char *str, usize len) {
  i32 val = 0;
  bool neg = false;
  u32 index = 0;

  if (str[0] == '-') {
    neg = true;
    str++;
  }

  while (*str && dew_isnumchar(*str) && (index++ < len))
    val = val * 10 + dew_hexvalue(*str++);

  if (neg)
    val = -val;
  return val;
}

u32 dew_touint(const char *str, usize len, i32 base) {
  u32 val = 0;
  u32 index = 0;
  while (*str && dew_isnumchar(*str) && (index++ < len))
    val = val * 10 + dew_hexvalue(*str++);
  return val;
}

f32 dew_tofloat(const char *str, usize len) {
  f32 val = 0;
  bool past_decimal = false;
  bool neg = false;
  f32 scale = 1.0f;
  usize index = 0;

  if (str[0] == '-') {
    neg = true;
    str++;
  }

  while (*str && (dew_isnumchar(*str) || *str == '.') && (index++ < len)) {
    if (past_decimal) {
      scale *= 0.1f;
      val = val + dew_hexvalue(*str++) * scale;
    } else if (*str == '.') {
      past_decimal = true;
      str++;
    } else {
      val = val * 10 + dew_hexvalue(*str++);
    }
  }

  if (neg)
    val = -val;
  return val;
}

f64 dew_todouble(const char *str, usize len) {
  f64 val = 0;
  bool past_decimal = false;
  bool neg = false;
  f64 scale = 1.0f;
  usize index = 0;

  if (str[0] == '-') {
    neg = true;
    str++;
  }

  while (*str && (dew_isnumchar(*str) || *str == '.') && (index++ < len)) {
    if (past_decimal) {
      scale *= 0.1f;
      val = val + dew_hexvalue(*str++) * scale;
    } else if (*str == '.') {
      past_decimal = true;
      str++;
    } else {
      val = val * 10 + dew_hexvalue(*str++);
    }
  }

  if (neg)
    val = -val;
  return val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
dew_strpool *dew_strpool_create(const dew_allocator *alloc,
                                const dew_strpool_config *conf) {
  strpool_config_t sconf;
  sconf.memctx = (void *)alloc;
  if (!conf) {
    sconf.ignore_case = 0;
    sconf.counter_bits = 12;
    sconf.index_bits = 20;
    sconf.entry_capacity = 4096;
    sconf.block_capacity = 32;
    sconf.block_size = 256 * 1024;
    sconf.min_length = 23;
  } else {
    sconf.ignore_case = conf->ignore_case;
    sconf.counter_bits = conf->counter_bits;
    sconf.index_bits = conf->index_bits;
    sconf.entry_capacity = conf->entry_capacity;
    sconf.block_capacity = conf->block_capacity;
    sconf.block_size = conf->block_sz_kb * 1024;
    sconf.min_length = conf->min_str_len;
  }

  strpool_t *sp = (strpool_t *)dew_malloc(alloc, sizeof(strpool_t));
  if (!sp) {
    dew_out_of_memory();
    return NULL;
  }
  strpool_init(sp, &sconf);
  return sp;
}

void dew_strpool_destroy(dew_strpool *sp, const dew_allocator *alloc) {
  dew_assert(sp);
  strpool_term(sp);
  dew_free(alloc, sp);
}

void dew_strpool_defrag(dew_strpool *sp) { strpool_defrag(sp); }

dew_str_t dew_strpool_add(dew_strpool *sp, const char *str, i32 len) {
  STRPOOL_U64 handle = strpool_inject(sp, str, len);
  dew_assertf((handle & 0xffffffff) == handle,
              "u32 overflow, check index_bits and counter_bits in config!");
  return (u32)handle;
}

void dew_strpool_del(dew_strpool *sp, dew_str_t handle) {
  strpool_discard(sp, (STRPOOL_U64)handle);
}

i32 dew_strpool_incref(dew_strpool *sp, dew_str_t handle) {
  return strpool_incref(sp, (STRPOOL_U64)handle);
}

i32 dew_strpool_decref(dew_strpool *sp, dew_str_t handle) {
  return strpool_decref(sp, (STRPOOL_U64)handle);
}

bool dew_strpool_valid(const dew_strpool *sp, dew_str_t handle) {
  return dew_cppbool(strpool_isvalid(sp, (STRPOOL_U64)handle));
}

i32 dew_strpool_ref(dew_strpool *sp, dew_str_t handle) {
  return strpool_getref(sp, (STRPOOL_U64)handle);
}

const char *dew_strpool_cstr(const dew_strpool *sp, dew_str_t handle) {
  return strpool_cstr(sp, (STRPOOL_U64)handle);
}

i32 dew_strpool_len(const dew_strpool *sp, dew_str_t handle) {
  return strpool_length(sp, (STRPOOL_U64)handle);
}

dew_strpool_collate_data dew_strpool_collate(const dew_strpool *sp) {
  dew_strpool_collate_data d;
  d.first = strpool_collate(sp, &d.count);
  return d;
}

void dew_strpool_collate_free(const dew_strpool *sp,
                              dew_strpool_collate_data data) {
  dew_assert(data.first);
  strpool_free_collated(sp, data.first);
}
