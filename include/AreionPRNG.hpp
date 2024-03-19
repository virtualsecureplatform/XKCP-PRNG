#pragma once

extern "C" {
#include <areion.h>
}

namespace AreionPRNG{

// Returns values of type "result_type" (must be a built-in unsigned integer type).
// C++11 URBG interface:
template <typename result_type>
struct alignas(64) Areion512PRNG{
  static constexpr size_t state_bits = 512;
  static constexpr size_t state_byte = state_bits/8;
  static constexpr size_t capacity_byte =  16;
  static constexpr size_t rate_byte = state_byte - capacity_byte;
  static_assert(std::is_unsigned<result_type>::value,
                "Must be parameterized by a built-in unsigned integer");
  static constexpr size_t buffend = (8*rate_byte)/std::numeric_limits<result_type>::digits; /**< End position of outer part of state*/
  using state_array = std::array<result_type,state_bits/std::numeric_limits<result_type>::digits>;
  alignas(64) state_array state; /**< Current PRNG state */
  size_t next = buffend; /**< counter for output which indicates next position of output in the state*/

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  explicit Areion512PRNG(result_type seed_value = 0) { state = {}; state[0] = seed_value;}

  explicit Areion512PRNG(std::random_device& seed_gen){
  // https://cpprefjp.github.io/reference/random/seed_seq.html

  std::generate(state, state, std::ref(seed_gen));
  }

    // Returns random bits from the buffer in units of T.
  result_type operator()() {
    // Refill the buffer if needed (unlikely).
    if (next == buffend) {
      const alignas(64) state_array prev_state = state;
      permute_areion_512u8(reinterpret_cast<uint8_t*>(state.data()),reinterpret_cast<const uint8_t*>(prev_state.data()));
      for(int i = buffend; i < state.size(); i++) state[i] ^= prev_state[i];
      next = 0;
    }

    const result_type ret = state[next];
    next++;
    return ret;
  }
};

// Returns values of type "result_type" (must be a built-in unsigned integer type).
// C++11 URBG interface:
template <typename result_type>
struct alignas(64) Areion256PRNG{
  static constexpr size_t state_bits = 256;
  static constexpr size_t state_byte = state_bits/8;
  static constexpr size_t capacity_byte =  16;
  static constexpr size_t rate_byte = state_byte - capacity_byte;
  static_assert(std::is_unsigned<result_type>::value,
                "Must be parameterized by a built-in unsigned integer");
  static constexpr size_t buffend = (8*rate_byte)/std::numeric_limits<result_type>::digits; /**< End position of outer part of state*/
  using state_array = std::array<result_type,state_bits/std::numeric_limits<result_type>::digits>;
  alignas(64) state_array state; /**< Current PRNG state */
  size_t next = buffend; /**< counter for output which indicates next position of output in the state*/

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  explicit Areion256PRNG(result_type seed_value = 0) { state = {}; state[0] = seed_value;}

  explicit Areion256PRNG(std::random_device& seed_gen){
  // https://cpprefjp.github.io/reference/random/seed_seq.html

  std::generate(state, state, std::ref(seed_gen));
  }

    // Returns random bits from the buffer in units of T.
  result_type operator()() {
    // Refill the buffer if needed (unlikely).
    if (next == buffend) {
      const alignas(64) state_array prev_state = state;
      permute_areion_256u8(reinterpret_cast<uint8_t*>(state.data()),reinterpret_cast<const uint8_t*>(prev_state.data()));
      for(int i = buffend; i < state.size(); i++) state[i] ^= prev_state[i];
      next = 0;
    }

    const result_type ret = state[next];
    next++;
    return ret;
  }
};
}