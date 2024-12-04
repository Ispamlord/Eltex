#include <stdarg.h>

double sum(int num_arg, ...) {
    va_list ap;
    double result = 0.0;
    va_start(ap, num_arg);
    for (int i = 0;i < num_arg;i++) {
        result += va_arg(ap, double);
    }
    va_end(ap);
    return result;
}