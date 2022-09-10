#pragma once

#include <bits/stdint-uintn.h>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <random>
#include <array>
#include <functional>

extern "C" {
#include <Xoodoo-SnP.h>
// void Xoodoo_Permute_12rounds( uint32_t * state);
}


namespace XoodooPRNG{

constexpr size_t state_bits = 3*4*4*8;


// Returns values of type "result_type" (must be a built-in unsigned integer type).
// C++11 URBG interface:
template <typename result_type, size_t rate_byte = 40>
struct alignas(64) XoodooPRNG{
  static constexpr size_t capacity_byte =  48 - rate_byte;
  static_assert(std::is_unsigned<result_type>::value,
                "Must be parameterized by a built-in unsigned integer");
  static_assert((rate_byte%(std::numeric_limits<result_type>::digits/8))==0,"rate_byte must be a multiple of sizeof(result_type)");
  static constexpr size_t buffend = (8*rate_byte)/std::numeric_limits<result_type>::digits; /**< End position of outer part of state*/
  using state_array = std::array<result_type,state_bits/std::numeric_limits<result_type>::digits>;
  alignas(64) state_array state; /**< Current PRNG state */
  size_t next = 0; /**< counter for output which indicates next position of output in the state*/

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  explicit XoodooPRNG(result_type seed_value = 0) { state = {}; state[0] = seed_value; Xoodoo_Permute_12rounds(reinterpret_cast<uint32_t*>(state.data()));}

  explicit XoodooPRNG(std::random_device& seed_gen){
  // https://cpprefjp.github.io/reference/random/seed_seq.html
  std::array<
    std::random_device::result_type,
    state_bits/std::numeric_limits<std::random_device::result_type>::digits
  > seed_data;

  std::generate(seed_data.begin(), seed_data.end(), std::ref(seed_gen));

  state = {};

  if constexpr(std::numeric_limits<result_type>::digits > std::numeric_limits<std::random_device::result_type>::digits){
    for(int i = 0; i < state.size();i++) for(int j = 0; j < std::numeric_limits<result_type>::digits/std::numeric_limits<std::random_device::result_type>::digits; j++) state[i] ^= static_cast<result_type>(seed_data[i]) << (j*std::numeric_limits<std::random_device::result_type>::digits);
  }else{
    constexpr size_t ratio = std::numeric_limits<std::random_device::result_type>::digits/std::numeric_limits<result_type>::digits;
    for(int i = 0; i < state.size()/ratio;i++) for(int j = 0; j < ratio; j++) state[i] = seed_data[i*ratio+j] >> (j*std::numeric_limits<result_type>::digits);
  }
  Xoodoo_Permute_12rounds(reinterpret_cast<uint32_t*>(state.data()));
  }

    // Returns random bits from the buffer in units of T.
  result_type operator()() {
    // Refill the buffer if needed (unlikely).
    if (next == buffend) {
      const alignas(64) state_array prev_state = state;
      Xoodoo_Permute_12rounds(reinterpret_cast<uint32_t*>(state.data()));
      for(int i = buffend; i < state.size(); i++) state[i] ^= prev_state[i];
      next = 0;
    }

    const result_type ret = state[next];
    next++;
    return ret;
  }
};
}