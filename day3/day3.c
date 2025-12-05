#include <stdint.h>
#include <stdio.h>

#define EXTLIB_IMPL
#include "../common/extlib.h"
#include "../common/timer.h"

static int64_t ipow(int64_t base, uint64_t exp) {
    int64_t res = 1;
    for(; exp > 0; exp >>= 1) {
        if(exp & 1) res *= base;
        base *= base;
    }
    return res;
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }
    const char* input_file = argv[1];

    StringBuffer input = {0};
    if(!read_file(input_file, &input)) return 1;

    TIMED("Part 2") {
        size_t res = 0;

        const int max_batteries_in_bank = 12;
        StringSlice ss = sb_to_ss(input);
        while(ss.size) {
            StringSlice line = ss_split_once(&ss, '\n');
            size_t jolt = 0;
            const char* next_start = line.data;
            for(int i = 0; i < max_batteries_in_bank; i++) {
                char max_dig = 0;
                for(const char* it = next_start;
                    it < line.data + line.size - (max_batteries_in_bank - i - 1); it++) {
                    if(*it > max_dig) {
                        max_dig = *it;
                        next_start = it + 1;
                    }
                }
                jolt += (max_dig - '0') * ipow(10, (max_batteries_in_bank - i - 1));
            }
            res += jolt;
        }

        printf("Result: %zu\n", res);
    }
}
