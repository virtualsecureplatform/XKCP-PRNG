#pragma once

extern "C" {
#include <blake3.h>
}

namespace BLAKE3PRNG{
// Returns values of type "result_type" (must be a built-in unsigned integer type).
// C++11 URBG interface:
template <typename result_type>
struct alignas(64) BLAKE3PRNG{
  blake3_hasher hasher;

  static_assert(std::is_unsigned<result_type>::value,
                "Must be parameterized by a built-in unsigned integer");

  static constexpr uint buffer_len = 4096;
  static constexpr uint buffer_result_len = 8*buffer_len/std::numeric_limits<result_type>::digits;
  alignas(64) std::array<result_type,buffer_result_len> buffer;
  std::array<uint8_t,32> seed;
  size_t next = 0; /**< counter for output which indicates next position of output in the buffer*/
  uint32_t counter = 0;

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  explicit BLAKE3PRNG(result_type seed_value = 0) {
    seed = {}; 
    for(int i = 0; i < std::numeric_limits<result_type>::digits/8; i++) seed[i] = ((uint8_t *)&seed_value)[i];
    blake3_hasher_init_keyed(&hasher, seed.data());
    blake3_hasher_update(&hasher, (uint8_t*) &counter, sizeof(uint32_t));
    blake3_hasher_finalize(&hasher, (uint8_t*)buffer.data(), buffer_len);
    }

  explicit BLAKE3PRNG(std::random_device& seed_gen){

  // https://cpprefjp.github.io/reference/random/seed_seq.html
  std::array<uint8_t, 32> seed_data;

  std::generate(seed.begin(), seed.end(), std::ref(seed_gen));

  blake3_hasher_init_keyed(&hasher, seed.data());
  blake3_hasher_update(&hasher, (uint8_t*) &counter, sizeof(uint32_t));
  blake3_hasher_finalize(&hasher, (uint8_t*)buffer.data(), buffer_len);
  }

    // Returns random bits from the buffer in units of T.
  result_type operator()() {
    // Refill the buffer if needed (unlikely).
    if (next == buffer_result_len) {
        counter++;
        blake3_hasher_init_keyed(&hasher, seed.data());
        blake3_hasher_update(&hasher, (uint8_t*) &counter, sizeof(uint32_t));
        blake3_hasher_finalize(&hasher, (uint8_t*)buffer.data(), buffer_len);
        next = 0;
    }
    const result_type ret = buffer[next];
    next++;
    return ret;
  }
};
}