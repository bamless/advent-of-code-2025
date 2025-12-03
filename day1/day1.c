#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define EXTLIB_IMPL
#include "../common/extlib.h"

static int parse_int(StringSlice ss) {
    int num = 0;
    size_t i = 0;
    while(i < ss.size && isdigit(ss.data[i])) {
        num = num * 10 + (ss.data[i] - '0');
        i++;
    }
    return num;
}

static int mod(int a, int b) {
    int r = a % b;
    return (r < 0) ? r + b : r;
}

int main(int argc, char** argv) {
    if(argc == 1) {
        ext_log(ERROR, "USAGE: %s <input_file>", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];

    StringBuffer input = {0};
    if(!read_file(input_file, &input)) return 1;

    int number_of_zeroes = 0;
    int dial = 50;

    StringSlice ss = sb_to_ss(input);
    while(ss.size) {
        StringSlice line = ss_split_once(&ss, '\n');

        char direction = line.data[0];
        int num = parse_int(ss_from(line.data + 1, line.size - 1));
        int old_dial = dial;

        switch(direction) {
        case 'L':
            dial -= num;
            break;
        case 'R':
            dial += num;
            break;
        default:
            UNREACHABLE();
        }

        int num_cross_zero = 0;
        if(old_dial != 0 && dial == 0) {
            num_cross_zero = 1;
        } else if(dial < 0) {
            num_cross_zero = abs(dial) / 100 + (old_dial != 0 ? 1 : 0);
        } else {
            num_cross_zero = abs(dial) / 100;
        }

        dial = mod(dial, 100);
        number_of_zeroes += num_cross_zero;
    }

    printf("Result: %d\n", number_of_zeroes);
}
