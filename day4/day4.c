#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define EXTLIB_IMPL
#include "../common/extlib.h"
#include "../common/timer.h"

typedef struct {
    char* items;
    int width;
    int height;
} Board;

static char get_at(const Board* b, int row, int col) {
    if(row < 0 || col < 0) return 0;
    if(row >= b->width || col >= b->height) return 0;
    size_t idx = row * b->width + col;
    return b->items[idx];
}

static void set_at(Board* b, int row, int col, char c) {
    if(row < 0 || col < 0) return;
    if(row >= b->width || col >= b->height) return;
    size_t idx = row * b->width + col;
    b->items[idx] = c;
}

static bool convolve(const Board* board, size_t row, size_t col, uint16_t kernel_sz) {
    int num_rolls = 0;
    int n = kernel_sz - 1;
    for(int i = -n / 2; i <= n / 2; i++) {
        for(int j = -n / 2; j <= n / 2; j++) {
            if(i == 0 && j == 0) continue;
            char elem = get_at(board, row - i, col - j);
            if(elem == '@') {
                num_rolls++;
            }
        }
    }
    return num_rolls < 4;
}

static void part1(const Board* b) {
    TIMED("Part 1") {
        size_t res = 0;
        for(int x = 0; x < b->width; x++) {
            for(int y = 0; y < b->height; y++) {
                if(get_at(b, x, y) == '@' && convolve(b, x, y, 3)) {
                    res++;
                }
            }
        }
        printf("Part 1: %zu\n", res);
    }
}

static void part2(Board* b) {
    TIMED("Part 2") {
        size_t res = 0;
        bool changed;

        do {
            changed = false;
            for(int x = 0; x < b->width; x++) {
                for(int y = 0; y < b->height; y++) {
                    if(get_at(b, x, y) == '@' && convolve(b, x, y, 3)) {
                        changed = true;
                        set_at(b, x, y, 'x');
                        res++;
                    }
                }
            }
        } while(changed);

        printf("Part 2: %zu\n", res);
    }
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }
    const char* input_file = argv[1];

    StringBuffer input = {0};
    FILE* f = fopen(input_file, "rb");
    if(!f) {
        ext_log(ERROR, "%s", strerror(errno));
        return 1;
    }

    int res = 0;
    size_t height = 0, width = 0;
    while((res = read_line(f, &input)) > 0) {
        if(input.items[input.size - 1] == '\n') input.size--;
        if(width == 0) width = input.size;
        height++;
    }

    if(res < 0) {
        ext_log(ERROR, "%s", strerror(errno));
        return 1;
    }

    Board board = {.items = input.items, .width = width, .height = height};
    part1(&board);
    part2(&board);
}
