#pragma once

// from https://github.com/septag/sx/blob/master/include/sx/string.h

#include "dew/macros.h"
#include "dew/memory/allocator.h"
#include "dew/primitives.h"
#include <stdarg.h> // va_list

typedef struct dew_str_block {
  const char *start;
  const char *end;
} dew_str_block;

DEW_API i32 dew_snprintf(char *str, i32 size, const char *fmt, ...);
DEW_API i32 dew_vsnprintf(char *str, i32 size, const char *fmt, va_list args);

DEW_API char *dew_snprintf_alloc(const dew_allocator *alloc, const char *fmt,
                                 ...);
DEW_API char *dew_vsnprintf_alloc(const dew_allocator *alloc, const char *fmt,
                                  va_list args);

// strcpy, strncpy, strcat, strncat returns the pointer to the end of the string
// (str + len)
DEW_API char *dew_strcpy(char *DEW_RESTRICT dst, i32 dst_sz,
                         const char *DEW_RESTRICT src);
DEW_API char *dew_strncpy(char *DEW_RESTRICT dst, i32 dst_sz,
                          const char *DEW_RESTRICT src, i32 _num);
DEW_API char *dew_strcat(char *DEW_RESTRICT dst, i32 dst_sz,
                         const char *DEW_RESTRICT src);
DEW_API char *dew_strncat(char *DEW_RESTRICT dst, i32 dst_sz,
                          const char *DEW_RESTRICT src, i32 _num);

// use this macro to estimate remaining string size when using return pointers
// from strcat/strcpy
#define dew_strsize(_str, _ptr) sizeof(_str) - (i32)(uptr)(_ptr - _str)

DEW_API i32 dew_strlen(const char *str);
DEW_API bool dew_strequal(const char *DEW_RESTRICT a,
                          const char *DEW_RESTRICT b);
DEW_API bool dew_strequalnocase(const char *DEW_RESTRICT a,
                                const char *DEW_RESTRICT b);
DEW_API bool dew_strnequal(const char *DEW_RESTRICT a,
                           const char *DEW_RESTRICT b, i32 num);
DEW_API bool dew_strnequalnocase(const char *DEW_RESTRICT a,
                                 const char *DEW_RESTRICT b, i32 num);

DEW_API const char *dew_strrchar(const char *str, char ch);
DEW_API const char *dew_strchar(const char *str, char ch);
DEW_API const char *dew_strstr(const char *DEW_RESTRICT str,
                               const char *DEW_RESTRICT find);
DEW_API bool dew_strstr_wildcard(const char *str, const char *pattern);

DEW_API const char *dew_skip_whitespace(const char *str);
DEW_API const char *dew_skip_word(const char *str);
DEW_API char *dew_trim_whitespace(char *dest, i32 dest_sz, const char *src);
DEW_API char *dew_trim(char *dest, i32 dest_sz, const char *src,
                       const char *trim);
DEW_API char *dew_trimchar(char *dest, i32 dest_sz, const char *src,
                           char trim_ch);
DEW_API char *dew_replace(char *dest, i32 dest_sz, const char *src,
                          const char *find, const char *replace);
DEW_API char *dew_replacechar(char *dest, i32 dest_sz, const char *src,
                              const char find, const char replace);
DEW_API char *dew_EOL_LF(char *dest, i32 dest_sz, const char *src);
DEW_API bool dew_split(char *dest1, i32 dest1_sz, char *dest2, i32 dest2_sz,
                       const char *src, char splitch);
DEW_API dew_str_block dew_findblock(const char *str, char open, char close);

DEW_API bool dew_isspace(char ch);
DEW_API bool dew_isrange(char ch, char from, char to);
DEW_API bool dew_isupperchar(char ch);
DEW_API bool dew_isupper(const char *str, usize size);
DEW_API bool dew_islowerchar(char ch);
DEW_API bool dew_islower(const char *str, usize size);
DEW_API bool dew_isalphachar(char ch);
DEW_API bool dew_isalpha(const char *str, usize size);
DEW_API bool dew_isnumchar(char ch);
DEW_API bool dew_isnum(const char *str);
DEW_API bool dew_ishexchar(char ch);
DEW_API bool dew_ishex(const char *str);
DEW_API i32 dew_hexvalue(char str);
DEW_API char *dew_tolower(char *dst, i32 dst_sz, const char *str);
DEW_API char *dew_toupper(char *dst, i32 dst_sz, const char *str);
DEW_API char dew_tolowerchar(char ch);
DEW_API char dew_toupperchar(char ch);
DEW_API bool dew_tobool(const char *str);
DEW_API i32 dew_toint(const char *str, usize len);
DEW_API u32 dew_touint(const char *str, usize len, i32 base);
DEW_API float dew_tofloat(const char *str, usize len);
DEW_API double dew_todouble(const char *str, usize len);

// strpool (string interning) implementation
// By Mattias Gustavsson:
// https://github.com/mattiasgustavsson/libs/blob/master/strpool.h
//
typedef struct dew_strpool_config {
  i32 ignore_case;  // =1 case insensitive
  i32 counter_bits; // counter bits in 32bit value for ref count (def: 12bits ->
                    // 4096)
  i32 index_bits; // index bits in 32bit value for array index (def: 20bits -> 1
                  // million)
  i32 entry_capacity; // preallocate number of string entities (def: 4096)
  i32 block_capacity; // number of storage blocks (def: 32), only one is
                      // allocated on init
  i32 block_sz_kb;    // size of each block in kb (def: 256)
  i32 min_str_len; // minimum allocated size for each string, in chars (def: 23)
} dew_strpool_config;

typedef struct dew_strpool_collate_data {
  char *first; // null-terminated strings, after each '\0' comes another str
  i32 count;
} dew_strpool_collate_data;

typedef struct strpool_t dew_strpool;
typedef u32 dew_str_t;

DEW_API dew_strpool *
dew_strpool_create(const dew_allocator *alloc,
                   const dew_strpool_config *conf dew_default(NULL));
DEW_API void dew_strpool_destroy(dew_strpool *sp, const dew_allocator *alloc);

DEW_API void dew_strpool_defrag(dew_strpool *sp);
DEW_API dew_str_t dew_strpool_add(dew_strpool *sp, const char *str, i32 len);
DEW_API void dew_strpool_del(dew_strpool *sp, dew_str_t handle);
DEW_API i32 dew_strpool_incref(dew_strpool *sp, dew_str_t handle);
DEW_API i32 dew_strpool_decref(dew_strpool *sp, dew_str_t handle);
DEW_API bool dew_strpool_valid(const dew_strpool *sp, dew_str_t handle);
DEW_API i32 dew_strpool_ref(dew_strpool *sp, dew_str_t handle);
DEW_API const char *dew_strpool_cstr(const dew_strpool *sp, dew_str_t handle);
DEW_API i32 dew_strpool_len(const dew_strpool *sp, dew_str_t handle);

DEW_API dew_strpool_collate_data dew_strpool_collate(const dew_strpool *sp);
DEW_API void dew_strpool_collate_free(const dew_strpool *sp,
                                      dew_strpool_collate_data data);
