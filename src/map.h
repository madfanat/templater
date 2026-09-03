#pragma once

#include <cstddef>
#include <cstring>

struct Map {
  static constexpr std::size_t kMaxPairCount = 1024;
  static constexpr std::size_t kMaxKeySize = 100;
  static constexpr std::size_t kMaxValueSize = 100;

  struct Entry {
    char key[kMaxKeySize + 1];
    char value[kMaxValueSize + 1];
    bool used = false;
  };

  Entry entries[kMaxPairCount];

  bool Set(const char* key, const char* value) {
    if (key == nullptr || value == nullptr) {
      return false;
    }

    const std::size_t key_size = std::strlen(key);
    const std::size_t value_size = std::strlen(value);

    if (key_size > kMaxKeySize || value_size > kMaxValueSize) {
      return false;
    }

    const std::size_t hash = Hash(key);

    for (std::size_t offset = 0; offset < kMaxPairCount; ++offset) {
      const std::size_t index = (hash + offset) % kMaxPairCount;
      Entry& entry = entries[index];

      if (!entry.used || std::strcmp(entry.key, key) == 0) {
        std::memcpy(entry.key, key, key_size + 1);
        std::memcpy(entry.value, value, value_size + 1);
        entry.used = true;
        return true;
      }
    }

    return false;
  }

  const char* Get(const char* key) const {
    if (key == nullptr || std::strlen(key) > kMaxKeySize) {
      return nullptr;
    }

    const std::size_t hash = Hash(key);

    for (std::size_t offset = 0; offset < kMaxPairCount; ++offset) {
      const std::size_t index = (hash + offset) % kMaxPairCount;
      const Entry& entry = entries[index];

      if (!entry.used) {
        return nullptr;
      }

      if (std::strcmp(entry.key, key) == 0) {
        return entry.value;
      }
    }

    return nullptr;
  }

 private:
  static std::size_t Hash(const char* key) {
    std::size_t hash = 0;

    while (*key != '\0') {
      hash = hash * 31 + static_cast<unsigned char>(*key);
      ++key;
    }

    return hash % kMaxPairCount;
  }
};
