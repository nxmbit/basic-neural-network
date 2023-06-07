#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Python.h>
#include "include/ui.h"

int main(int argc, char *argv[]) {
    Py_Initialize();
    srand(time(NULL));
    main_menu();
    Py_Finalize();
    return 0;
}