#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define EXTLIB_IMPL
#include "../common/extlib.h"
#include "../common/timer.h"

typedef struct {
    int64_t* items;
    size_t size, capacity;
    Allocator* allocator;
} Accumulators;

typedef struct {
    char* items;
    size_t size, capacity;
    Allocator* allocator;
} Operations;

static size_t parse_int(StringSlice ss) {
    size_t num = 0;
    size_t i = 0;
    while(i < ss.size && isdigit(ss.data[i])) {
        num = num * 10 + (ss.data[i] - '0');
        i++;
    }
    return num;
}

void part1(const StringBuffer* input) {
    TIMED("Part 1") {
        StringSlice ss = sb_to_ss(*input);

        StringSlice ops_line = ss_trim(ss_rsplit_once(&ss, '\n'));
        Operations operations = {0};
        while(ops_line.size) {
            StringSlice op = ss_split_once_ws(&ops_line);
            ASSERT(op.size == 1, "Operator must be length 1");
            array_push(&operations, op.data[0]);
        }

        Accumulators accs = {0};
        array_reserve_exact(&accs, operations.size);
        memset(accs.items, 0, operations.size);
        accs.size = operations.size;

        size_t idx = 0;
        StringSlice first_line = ss_trim(ss_split_once(&ss, '\n'));
        while(first_line.size) {
            size_t n = parse_int(ss_split_once_ws(&first_line));
            accs.items[idx++] = n;
        }

        while(ss.size) {
            StringSlice line = ss_trim(ss_split_once(&ss, '\n'));
            size_t idx = 0;
            while(line.size) {
                size_t n = parse_int(ss_split_once_ws(&line));

                switch(operations.items[idx]) {
                case '+':
                    accs.items[idx] += n;
                    break;
                case '-':
                    accs.items[idx] -= n;
                    break;
                case '*':
                    accs.items[idx] *= n;
                    break;
                }

                idx++;
            }
        }

        int64_t res = 0;
        array_foreach(const int64_t, n, &accs) {
            res += *n;
        }

        printf("Part 1: %" PRId64 "\n", res);
    }
}

void part2(const StringBuffer* input) {
    TIMED("Part 2") {
        StringSlice ss = sb_to_ss(*input);

        StringSlice ops_line = ss_trim(ss_rsplit_once(&ss, '\n'));
        Operations operations = {0};
        while(ops_line.size) {
            StringSlice op = ss_split_once_ws(&ops_line);
            ASSERT(op.size == 1, "Operator must be length 1");
            array_push(&operations, op.data[0]);
        }

    }
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }
    const char* input_file = argv[1];

    StringBuffer input = {0};
    if(!read_file(input_file, &input)) return 1;
    if(input.items[input.size - 1] == '\n') input.size--;

    part1(&input);
    part2(&input);
}
