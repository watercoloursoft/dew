#pragma once

#ifndef __cplusplus
#    include <assert.h>     // static_assert
#    ifndef static_assert
#        if SX_COMPILER_MSVC && (SX_COMPILER_MSVC < 1928)
#            define static_assert(_e, _msg)
#        else
#            define static_assert(_e, _msg) _Static_assert(_e, _msg)
#        endif
#    endif
#endif
