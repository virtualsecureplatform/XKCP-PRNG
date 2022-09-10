#include <cstdio>
#include <cstdint>

#include <random>
#include <xoodoo-prng.hpp>

int main()
{
    // std::random_device trng;
   	// XoodooPRNG::XoodooPRNG<uint64_t> rng(trng);
   	XoodooPRNG::XoodooPRNG<uint64_t> rng(42);

    constexpr size_t BUFFER_SIZE = 1024 * 1024 / sizeof(uint64_t);
    static uint64_t buffer[BUFFER_SIZE];

    while (1) {
        for (size_t i = 0; i < BUFFER_SIZE; ++i)
            buffer[i] = rng();
        fwrite((void*) buffer, sizeof(buffer[0]), BUFFER_SIZE, stdout);
    }
}
