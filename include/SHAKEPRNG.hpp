#pragma once

extern "C" {
    #include<KeccakHash.h>
    #include<TurboSHAKE.h>
}

namespace SHAKEPRNG{
// Returns values of type "result_type" (must be a built-in unsigned integer type).
// C++11 URBG interface:
template <typename result_type>
struct alignas(64) SHAKE128PRNG{
  Keccak_HashInstance hashInstance;

  static_assert(std::is_unsigned<result_type>::value,
                "Must be parameterized by a built-in unsigned integer");

  result_type res;

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  explicit SHAKE128PRNG(result_type seed_value = 0) { Keccak_HashInitialize_SHAKE128(&hashInstance);  Keccak_HashUpdate(&hashInstance,(BitSequence*)&seed_value,std::numeric_limits<result_type>::digits); Keccak_HashFinal(&hashInstance,(BitSequence*)&res);}

  explicit SHAKE128PRNG(std::random_device& seed_gen){
    Keccak_HashInitialize_SHAKE128(&hashInstance);

  // https://cpprefjp.github.io/reference/random/seed_seq.html
  std::array<uint8_t, 32> seed_data;

  std::generate(seed_data.begin(), seed_data.end(), std::ref(seed_gen));

  Keccak_HashUpdate(&hashInstance,seed_data.data(),8*32);

  Keccak_HashFinal(&hashInstance,&res);
  }

    // Returns random bits from the buffer in units of T.
  result_type operator()() {
    Keccak_HashSqueeze(&hashInstance, (BitSequence*)&res, std::numeric_limits<result_type>::digits);
    return res;
  }
};

template <typename result_type>
struct alignas(64) TurboSHAKE128PRNG{
  TurboSHAKE_Instance hashInstance;

  static_assert(std::is_unsigned<result_type>::value,
                "Must be parameterized by a built-in unsigned integer");

  result_type res;

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  explicit TurboSHAKE128PRNG(result_type seed_value = 0) { 
    TurboSHAKE128_Initialize(&hashInstance); 
    TurboSHAKE_Absorb(&hashInstance,(BitSequence*)&seed_value,std::numeric_limits<result_type>::digits/8); 
    TurboSHAKE_AbsorbDomainSeparationByte(&hashInstance,0x1F);
    }

  explicit TurboSHAKE128PRNG(std::random_device& seed_gen){
    TurboSHAKE128_Initialize(&hashInstance);

  // https://cpprefjp.github.io/reference/random/seed_seq.html
  std::array<uint8_t, 32> seed_data;

  std::generate(seed_data.begin(), seed_data.end(), std::ref(seed_gen));

  TurboSHAKE_Absorb(&hashInstance,seed_data.data(),32);

  TurboSHAKE_AbsorbDomainSeparationByte(&hashInstance,0x1F);
  }

    // Returns random bits from the buffer in units of T.
  result_type operator()() {
    TurboSHAKE_Squeeze(&hashInstance, (BitSequence*)&res, std::numeric_limits<result_type>::digits/8);
    return res;
  }
};
}