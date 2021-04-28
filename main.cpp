#include <iostream>
#include "Table.h"

int main(int argc, char *argv[]) {
    try {
        Table tab(argv[1]);
        tab.print_done_table();
    }
    catch (runtime_error except) {
        std::cout << except.what();
        return -1;
    }
    return 0;
}
