// Adapted from TestU01 manual, Figure 2.2, Figure 2.4
#include <random>
#include <string>
#include <cstdio>
#include <cstdint>
#include <cstddef>

extern "C" {
    #include "TestU01.h"
}

#include <xoodoo-prng.hpp>

XoodooPRNG::XoodooPRNG<uint32_t> rng(42);
uint32_t Rand32(){return rng();}

int main()
{
    // Create TestU01 PRNG object for our generator
    unif01_Gen* gen = unif01_CreateExternGenBits("Xoodoo-PRNG", Rand32);

    // Run the tests.
    // bbattery_SmallCrush(gen);
    bbattery_BigCrush(gen);

    // Clean up.
    unif01_DeleteExternGenBits(gen);

    return 0;
}