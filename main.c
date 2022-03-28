#include "manager.h"

/**
 * parent_main
 */
int main() {
    register_child_signal();
    setup_environment();
    shell();
    return 0;
}