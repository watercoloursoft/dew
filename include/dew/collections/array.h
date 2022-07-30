#pragma once

#include "../memory/allocator.h"
#include "../util.h"

#define dew_array(T) T *

#define dew_array_free(_allocator, a)                                          \
  ((a) ? dew_free(_allocator, dew__sbraw(a)), 0 : 0)
#define dew_array_push(_allocator, a, v)                                       \
  (dew__sbmaybegrow(_allocator, a, 1), (a)[dew__sbn(a)++] = (v))
#define dew_array_count(a) ((a) ? dew__sbn(a) : 0)
#define dew_array_add(_allocator, a, n)                                        \
  (dew__sbmaybegrow(_allocator, a, n), dew__sbn(a) += (n),                     \
   &(a)[dew__sbn(a) - (n)])
#define dew_array_last(a) ((a)[dew__sbn(a) - 1])
#define dew_array_pop(a, idx)                                                  \
  do {                                                                         \
    (a)[idx] = dew_array_last(a);                                              \
    --dew__sbn(a);                                                             \
  } while (0)
#define dew_array_pop_last(a)                                                  \
  do {                                                                         \
    --dew__sbn(a);                                                             \
  } while (0)
#define dew_array_pop_lastn(a, n)                                              \
  do {                                                                         \
    dew__sbn(a) -= (n);                                                        \
  } while (0)
#define dew_array_clear(a) ((a) ? (dew__sbn(a) = 0) : 0)
#define dew_array_reserve(_allocator, a, n)                                    \
  (dew_array_add(_allocator, a, n), dew_array_clear(a))
#define dew_array_push_byindex(_allocator, a, v, _index)                       \
  do {                                                                         \
    if ((_index) >= dew_array_count(a))                                        \
      dew_array_push(_allocator, a, v);                                        \
    else                                                                       \
      (a)[(_index)] = (v);                                                     \
  } while (0)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal
#define dew__sbraw(a) ((int *)(a)-2)
#define dew__sbm(a) dew__sbraw(a)[0]
#define dew__sbn(a) dew__sbraw(a)[1]

#define dew__sbneedgrow(a, n) ((a) == 0 || dew__sbn(a) + (n) >= dew__sbm(a))
#define dew__sbmaybegrow(_allocator, a, n)                                     \
  (dew__sbneedgrow(a, (n)) ? dew__sbgrow(_allocator, a, n) : 0)
#define dew__sbgrow(_allocator, a, n)                                          \
  (*((void **)&(a)) = dew__sbgrowf((a), (n), sizeof(*(a)), (_allocator),       \
                                   __FILE__, __FUNCTION__, __LINE__))

DEW_INLINE void *dew__sbgrowf(void *arr, int increment, int itemsize,
                              const dew_allocator *alloc, const char *file,
                              const char *func, int line) {
  int new_count = arr ? (dew__sbm(arr) << 1) : 0;
  new_count = new_count > DEW_CONFIG_ARRAY_INIT_SIZE
                  ? new_count
                  : DEW_CONFIG_ARRAY_INIT_SIZE;
  int min_needed = dew_array_count(arr) + increment;
  int m = new_count > min_needed ? new_count : min_needed;
  int *p = (int *)dew__realloc(alloc, arr ? dew__sbraw(arr) : 0,
                               (size_t)itemsize * (size_t)m + sizeof(int) * 2,
                               0, file, func, line);

  if (p) {
    p[0] = m;
    if (!arr)
      p[1] = 0;
    return p + 2;
  } else {
    dew_out_of_memory();
    return 0x0; // NULL
  }
}

#ifdef __cplusplus
namespace dew {
template <typename T> class array {
  T *m_array;
  const dew_allocator *m_alloc;

public:
  array() {
    m_array = nullptr;
    m_alloc = nullptr;
  }

  array(const dew_allocator *alloc) {
    m_array = nullptr;
    m_alloc = alloc;
  }
  array(const dew_allocator &alloc) {
    m_array = nullptr;
    m_alloc = &alloc;
  }
  explicit array(const allocator alloc)
      : m_alloc(alloc.get_raw()), m_array(nullptr) {}

  ~array() {
    if (m_alloc) {
      dew_array_free(m_alloc, m_array);
      m_alloc = nullptr;
    }
    m_array = nullptr;
  }

  void init(const allocator al, usize init_count = 0) {
    dew_assert(al);
    m_alloc = al.get_raw();
    if (init_count > 0) {
      dew_array_reserve(al, m_array, init_count);
    }
  }

  void release() {
    dew_assert(m_alloc);
    dew_array_free(m_alloc, m_array);
    m_array = nullptr;
    m_alloc = nullptr;
  }

  void push(const T &_value) {
    dew_assert(m_alloc);
    dew_array_push(m_alloc, m_array, _value);
  }

  void pop(usize _index) {
    dew_assert(m_alloc);
    dew_assert(_index < dew_array_count(m_array));
    dew_array_pop(m_array, _index);
  }

  void pop_last() {
    dew_assert(m_alloc);
    dew_assert(dew_array_count(m_array));
    dew_array_pop_last(m_array);
  }

  void clear() {
    dew_assert(m_alloc);
    dew_array_clear(m_array);
  }

  usize count() const {
    dew_assert(m_alloc);
    return dew_array_count(m_array);
  }

  T *expand(usize count) {
    dew_assert(m_alloc);
    return dew_array_add(m_alloc, m_array, count);
  }

  void reserve(usize count) {
    dew_assert(m_alloc);
    dew_array_reserve(m_alloc, m_array, count);
  }

  T &operator[](usize _index) {
    dew_assert(m_array);
    return this->m_array[_index];
  }

  const T &operator[](usize index) const {
    dew_assert(m_array);
    return this->m_array[index];
  }

  T *begin() { return &m_array[0]; }

  T *end() { return &m_array[count()]; }
};
} // namespace dew

#endif // __cplusplus
