#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
  long double ** a;
} double_array;

typedef struct {
  long double d1;
  long double d2;
  long double d3;
  long double d4;
  int i;
} dataset_t;

// создание двумерного массива double
double_array * arr_d2_create (int x , int y);

// вывод  двумерного массива double
int arr_d2_print (char * name , double_array * arr);

// наполнение массива рандомными значениями
void arr_d2_set_random (double_array * arr);

// создание двумерного массива double с рандомными значениями
double_array * arr_d2_create_random (int x , int y);

// умножение двумерных double массивов
double_array * arr_d2_multiply(double_array * arr , double_array * A , double_array * B);

// умножение двумерных double матриц
double_array * arr_d2_multiply_matrix(double_array * arr , double_array * A , double_array * B);

// умножение двумерного массива на число double
double_array * arr_d2_d_multiply(double_array * arr , double_array * A , long double B);

// вычитание A - B
double_array * arr_d2_decrease(double_array * arr , double_array * A , double_array * B);

// транспонирование двумерного double массива
double_array * arr_d2_transparent(double_array * ret , double_array * arr);

// приравнивание массива
double_array * arr_d2_equating(double_array * arr , double_array * A);

// сложение массива
double_array * arr_d2_increase(double_array * arr , double_array * A);

// освобождение памяти для созданных массивов
int arr_d2_free();

// чтение датасета с данными для обучения из файла
dataset_t ** read_dataset(char * filename);

int dataset_free(dataset_t ** dataset);
