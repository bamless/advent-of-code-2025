#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define EXTLIB_IMPL
#include "../common/extlib.h"
#include "../common/timer.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    size_t start, end;
} Range;

typedef struct {
    Range* items;
    size_t size, capacity;
    Allocator* allocator;
} Ranges;

typedef struct {
    size_t* items;
    size_t size, capacity;
    Allocator* allocator;
} Ingridients;

static bool in_range(Range r, size_t i) {
    return r.start <= i && i <= r.end;
}

static int range_cmp(const void* a, const void* b) {
    const Range *r1 = a, *r2 = b;
    if(r1->start > r2->start) return 1;
    if(r1->start < r2->start) return -1;
    return 0;
}

static Ranges merge_ranges(const Ranges* in) {
    Ranges merged = {0};
    if(!in->size) return merged;

    array_reserve(&merged, in->size);
    array_push(&merged, in->items[0]);

    for(size_t i = 1; i < in->size; i++) {
        Range* prev_merged = &merged.items[merged.size - 1];
        Range cur = in->items[i];
        if(prev_merged->end >= cur.start) {
            prev_merged->end = max(prev_merged->end, cur.end);
        } else {
            array_push(&merged, cur);
        }
    }

    return merged;
}

static size_t parse_int(StringSlice ss) {
    size_t num = 0;
    size_t i = 0;
    while(i < ss.size && isdigit(ss.data[i])) {
        num = num * 10 + (ss.data[i] - '0');
        i++;
    }
    return num;
}

static Ranges parse_ranges(StringSlice* ss) {
    Ranges ranges = {0};
    while(ss->size) {
        StringSlice line = ss_split_once(ss, '\n');
        if(ss_eq(line, SS(""))) break;
        StringSlice start = ss_split_once(&line, '-');
        StringSlice end = ss_split_once(&line, '-');
        array_push(&ranges, ((Range){.start = parse_int(start), .end = parse_int(end)}));
    }
    return ranges;
}

static Ingridients parse_ingridients(StringSlice* ss) {
    Ingridients ingridients = {0};
    while(ss->size) {
        StringSlice line = ss_split_once(ss, '\n');
        array_push(&ingridients, parse_int(line));
    }
    return ingridients;
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }
    const char* input_file = argv[1];

    StringBuffer input = {0};
    if(!read_file(input_file, &input)) return false;

    TIMED("Part 1") {
        StringSlice ss = sb_to_ss(input);
        Ranges ranges = parse_ranges(&ss);
        Ingridients ingridients = parse_ingridients(&ss);
        qsort(ranges.items, ranges.size, sizeof(*ranges.items), &range_cmp);
        Ranges sorted_and_merged_ranges = merge_ranges(&ranges);

        // Brute force 'cause I can't be fucked to do binary search rn
        size_t fresh = 0;
        array_foreach(const size_t, i, &ingridients) {
            array_foreach(const Range, r, &sorted_and_merged_ranges) {
                if(in_range(*r, *i)) {
                    fresh++;
                    goto continue_outer;
                }
            }
        continue_outer:;
        }
        printf("Part 1: %zu\n", fresh);
    }

    TIMED("Part 2") {
        StringSlice ss = sb_to_ss(input);
        Ranges ranges = parse_ranges(&ss);
        qsort(ranges.items, ranges.size, sizeof(*ranges.items), &range_cmp);
        Ranges sorted_and_merged_ranges = merge_ranges(&ranges);

        size_t all_fresh = 0;
        array_foreach(const Range, r, &sorted_and_merged_ranges) {
            all_fresh += r->end - r->start + 1;
        }
        printf("Part 2: %zu\n", all_fresh);
    }
}
