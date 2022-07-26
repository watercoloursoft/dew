#pragma once

#include "dew/base/base.h"

#include <cstdlib>

namespace dew {
template <class T, usize grow_size = 1024> class Pool {
public:
private:
  struct Block {
    Block *next;
  };

  class Buffer {
  public:
    Buffer *next;
    Buffer(Buffer *next) : next(next) {}
    auto get_block(usize index) -> T * {
      return reinterpret_cast<T *>(&m_data[m_block_size * index]);
    }

  private:
    static const usize m_block_size = sizeof(T) > sizeof(Block) ? sizeof(T)
                                                                : sizeof(Block);
    u8 m_data[m_block_size * grow_size];
  };

  Block *m_first_free_block{nullptr};
  Block *m_first_buffer{nullptr};

  usize m_buffered_blocks{grow_size};
};
} // namespace dew
