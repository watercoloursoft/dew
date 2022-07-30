#include "dew/dew.h"
#include "dew/string_util.h"

#if SX_PLATFORM_ANDROID
#include <android/log.h>
#elif SX_PLATFORM_WINDOWS
__declspec(dllimport) void __stdcall OutputDebugStringA(const char *_str);
#else
#include <stdio.h> // fputs, fflush
#endif

static void dew__default_assert_handler(const char *text,
                                        const char *sourcefile, uint32_t line) {
  char output_text[2048];
  if (sourcefile) {
    dew_snprintf(output_text, sizeof(output_text), "%s(%d): ASSERT FAILURE - ",
                 sourcefile, line);
  } else {
    dew_strcpy(output_text, sizeof(output_text), "ASSERT FAILURE - ");
  }
  char *endptr = dew_strcat(output_text, sizeof(output_text), text);
  dew_unused(endptr);

  // print
#if SX_PLATFORM_ANDROID
  __android_log_write(ANDROID_LOG_DEBUG, "", output_text);
#elif SX_PLATFORM_WINDOWS
  dew_strcat(endptr, sizeof(output_text), "\n");
  OutputDebugStringA(output_text);
#else
  fputs(output_text, stderr);
  fflush(stderr);
#endif
}

static dew_assert_cb *g_assert_handler = dew__default_assert_handler;

void dew_set_assert_callback(dew_assert_cb *callback) {
  g_assert_handler = callback ? callback : dew__default_assert_handler;
}

void dew__debug_message(const char *sourcefile, uint32_t line, const char *fmt,
                        ...) {
  char text[2048];

  va_list args;
  va_start(args, fmt);
  dew_vsnprintf(text, sizeof(text), fmt, args);
  va_end(args);

  g_assert_handler(text, sourcefile, line);
}
