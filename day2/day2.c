#include <math.h>
#include <stddef.h>
#include <stdio.h>

#define EXTLIB_IMPL
#include "../common/extlib.h"

static size_t parse_int(StringSlice ss) {
    size_t num = 0;
    size_t i = 0;
    while(i < ss.size && isdigit(ss.data[i])) {
        num = num * 10 + (ss.data[i] - '0');
        i++;
    }
    return num;
}

static void write_int(size_t n, StringBuffer* out) {
    int len = (int)log10(n) + 1;
    sb_reserve(out, out->size + len + 1);
    int res = snprintf(out->items + out->size, len + 1, "%zu", n);
    ASSERT(len == res, "StringBuffer overflow");
    out->size += len;
}

static bool is_repeated_part1(StringSlice ss) {
    size_t mid = ss.size / 2;
    StringSlice first = ss_from(ss.data, mid);
    StringSlice second = ss_from(ss.data + mid, ss.size - mid);
    return ss_eq(first, second);
}

static bool is_repeated_part2(StringSlice ss) {
    for(size_t seq_size = 1; seq_size <= ss.size / 2; seq_size++) {
        StringSlice seq = ss_from(ss.data, seq_size);
        for(size_t n = seq_size; n < ss.size; n += seq_size) {
            StringSlice cmp = ss_from(ss.data + n, n + seq_size < ss.size ? seq_size : ss.size - n);
            if(!ss_eq(seq, cmp)) {
                goto continue_outer;
            }
        }
        return true;
    continue_outer:;
    }
    return false;
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }
    const char* input_file = argv[1];

    StringBuffer input = {0};
    if(!read_file(input_file, &input)) return 1;

    size_t part1 = 0;
    size_t part2 = 0;

    StringSlice ss = sb_to_ss(input);
    StringBuffer str = {0};
    while(ss.size) {
        StringSlice range = ss_split_once(&ss, ',');
        size_t first = parse_int(ss_split_once(&range, '-'));
        size_t second = parse_int(ss_split_once(&range, '-'));

        for(size_t n = first; n <= second; n++) {
            str.size = 0;
            write_int(n, &str);
            if(is_repeated_part1(sb_to_ss(str))) {
                part1 += n;
            }
            if(is_repeated_part2(sb_to_ss(str))) {
                part2 += n;
            }
        }
    }

    printf("Part 1: %zu\n", part1);
    printf("Part 2: %zu\n", part2);
}
