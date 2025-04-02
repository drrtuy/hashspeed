#include <iostream>
#include <bits/stdc++.h>
#include <chrono>

using int128_t = __int128;

#include "hasher.h"
#include "/data/xxHash/xxhash.h"

static constexpr size_t ITERS = 100000000LL;

using namespace std;

int main(int argc, char** argv)
{
    volatile uint32_t seed = 0;
    {
        utils::Hasher_r h;
        auto start = chrono::high_resolution_clock::now(); 
        for (size_t i = 0; i < ITERS; ++i)
        {
            for (size_t it = 0; it < 4; ++it)
            {
                int64_t sum = i + it;
                seed = h((const char*)&sum, sizeof(size_t), seed);
            }
            seed = h.finalize(seed, 32);
        }
        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
        cout << "Existing MurMur3 Loop time is : " << fixed << time_taken * 1e-9 << setprecision(9) << " sec" << endl;
    }

    {
        auto start = chrono::high_resolution_clock::now();
        auto xxh3State = XXH3_createState();
        for (size_t i = 0; i < ITERS; ++i)
        {
            for (size_t it = 0; it < 4; ++it)
            {
                int64_t sum = i + it;
                //seed = XXH3_64bits_withSeed((const void*)&sum, sizeof(size_t), seed);
                XXH3_64bits_update(xxh3State, (const void*)&sum, sizeof(size_t));
            }
            seed = XXH3_64bits_digest(xxh3State);
            XXH3_64bits_reset_withSeed(xxh3State, seed);
        }
        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
        cout << "XXH3_64 Loop time is : " << fixed << time_taken * 1e-9 << setprecision(9) << " sec" << endl;
    }

    {
        auto start = chrono::high_resolution_clock::now(); 
        volatile size_t nr1 = 1;
        volatile size_t nr2 = 4;
        for (size_t i = 0; i < ITERS; ++i)
        {
            for (size_t it = 0; it < 4; ++it)
            {
                int64_t sum = i + it;
                nr1 = utils::my_hash_sort_bin((const unsigned char*)&sum, sizeof(size_t), nr1, nr2);
            }
        }
        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
        cout << "my_hash_sort_bin Loop time is : " << fixed << time_taken * 1e-9 << setprecision(9) << " sec" << endl;
    }


}
