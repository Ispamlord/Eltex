#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>


typedef double (*operation_func)(int, ...);

typedef struct {
  const char* name;
  operation_func func;
}func;

double sum(int num_arg, ...) {
  va_list ap;
  double result=0.0;
  va_start(ap, num_arg);
  for (int i = 0;i < num_arg;i++) {
    result += va_arg(ap, double);
  }
  va_end(ap);
  return result;
}
double multiply(int num_arg, ...) {
  va_list ap;
  double result = 0.0;
  va_start(ap, num_arg);
  result += va_arg(ap, double);
  for (int i = 1;i < num_arg;i++) {
    result *= va_arg(ap, double);
  }
  va_end(ap);
  return result;
}
double subtraction(int num_arg, ...) {
  va_list ap;
  double result = 0.0;
  va_start(ap, num_arg);
  result = va_arg(ap, double);
  for (int i = 1;i < num_arg;i++) {
    result -= va_arg(ap, double);
  }
  va_end(ap);
  return result;

}
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
int main() {
  double result = 0.0;
  func operation[] = { {"сумма", sum},{"вычитание",subtraction},{"умножение",multiply},{"деление",division}};
    while (1) {
    printf("выберите функцию:\n1-Суммирование\n2-Вычитание\n3-Умножение\n4-Деление(1 это делимое остальное делитель)\n");
    int num_func =0;
    scanf("%d", &num_func);
    num_func--;
    printf("Введите количество аргументов\n");
    int num_arg = 0;
    scanf("%d", &num_arg);
    if (num_arg < 2) {
      continue;
    }
    printf("col-vo arg %d\n",num_arg);
    double* numbers = (double*)malloc(num_arg * sizeof(double));
    for (int i = 0; i < num_arg;i++) {
        printf("введите число %d\n",i);
        scanf("%lf", &numbers[i]);
        if(num_func==3&numbers[i]==0)
          printf("нельзя делить на 0");
          free(numbers);
    }
    switch (num_func)
    {
    case 0:
      result = operation[0].func(num_arg, numbers[0], numbers[1], numbers[2],numbers[3]);
      break;
    case 1:
      result = operation[1].func(num_arg, numbers[0], numbers[1], numbers[2], numbers[3]);
      break;
    case 2:
      result = operation[2].func(num_arg, numbers[0], numbers[1], numbers[2], numbers[3]);
      break;
    default:
      result = operation[3].func(num_arg, numbers[0], numbers[1], numbers[2], numbers[3]);
      break;
    }
    printf("результат: %lf\n", result);
    free(numbers);
  }


  return 0;
}