#pragma once

#include <type_traits>
#include <new>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <array>

template <typename T, std::size_t Size> class MemoryPool {
  static_assert(std::is_destructible<T>::value,
                "MemoryPool: Type T must have a destructor.");
  static_assert(!std::is_array<T>::value,
                "MemoryPool: Type T cannot be an array type.");

public:
  MemoryPool() : freeHead_(0) {
    for (std::size_t i = 0; i < Size - 1; ++i)
      pool_[i].nextFree = i + 1;

    pool_[Size - 1].nextFree =
        static_cast<std::size_t>(-1); // Use -1 as "End of List"
  }

  template <typename... Args> T *Create(Args &&...args) {
    if (freeHead_ == static_cast<std::size_t>(-1))
      throw std::logic_error("Not enough space in the memory pool");

    std::size_t idx = freeHead_;
    freeHead_ = pool_[idx].nextFree;

    return new (&pool_[idx].data) T(std::forward<Args>(args)...);
  }

  void Destroy(T *ptr) {
    if (!ptr)
      return;

    ptr->~T();

    auto *slotPtr = reinterpret_cast<Slot *>(ptr);
    std::size_t idx = slotPtr - pool_.begin();

    pool_[idx].nextFree = freeHead_;
    freeHead_ = idx;
  }

public:
  struct alignas(T) Slot {
    std::byte data[sizeof(T)];
    std::size_t nextFree;
  };

  std::array<Slot, Size> pool_;
  std::size_t freeHead_;
};