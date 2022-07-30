#pragma once
#include "../macros.h"
#include "../primitives.h"

typedef struct dew_slice {
  madd ptr;
  usize size;
} dew_slice;

DEW_INLINE const dew_slice dew_slice_make(madd ptr, usize size) {
  dew_slice s = {ptr, size};
  return s;
}

DEW_INLINE const dew_slice dew_slice_make_null() {
  dew_slice s = {0, 0};
  return s;
}

DEW_INLINE const bool dew_slice_has(const dew_slice s) {
  return s.ptr != nullptr;
}

#if defined(__cplusplus)
namespace dew {
struct slice {
  slice(const dew_slice s) : m_proxy_slice(s) {}
  slice(madd ptr, usize size = 0) : m_proxy_slice(dew_slice_make(ptr, size)) {}
  slice() : m_proxy_slice(dew_slice_make_null()) {}

  madd ptr() const { return m_proxy_slice.ptr; }
  usize size() const { return m_proxy_slice.size; }
  bool has() const { return dew_slice_has(m_proxy_slice); }

  operator dew_slice() { return m_proxy_slice; }

private:
  dew_slice m_proxy_slice;
};
} // namespace dew

#endif
