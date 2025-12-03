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
    // I'm lazy
    size_t len = snprintf(NULL, 0, "%zu", n);
    sb_reserve(out, out->size + len + 1);
    snprintf(out->items + out->size, len + 1, "%zu", n);
    out->size += len;
}

static bool is_repeated(StringSlice ss) {
    size_t mid = ss.size / 2;
    StringSlice first = ss_from(ss.data, mid);
    StringSlice second = ss_from(ss.data + mid, ss.size - mid);
    return ss_eq(first, second);
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }
    const char* input_file = argv[1];

    StringBuffer input = {0};
    if(!read_file(input_file, &input)) return 1;

    size_t sum = 0;

    StringSlice ss = sb_to_ss(input);
    StringBuffer str = {0};
    while(ss.size) {
        StringSlice range = ss_split_once(&ss, ',');
        size_t first = parse_int(ss_split_once(&range, '-'));
        size_t second = parse_int(ss_split_once(&range, '-'));

        for(size_t n = first; n <= second; n++) {
            str.size = 0;
            write_int(n, &str);
            if(is_repeated(sb_to_ss(str))) {
                sum += n;
            }
        }
    }

    printf("Result: %zu\n", sum);
}
