#include<stdarg.h>


double division(int num_arg, ...) {
    va_list ap;
    double result = 0.0;
    va_start(ap, num_arg);
    result = va_arg(ap, double);
    for (int i = 1;i < num_arg;i++) {
        result /= va_arg(ap, double);
    }
    va_end(ap);
    return result;

}