#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// exp log
#include <math.h>
#include "utils.h"


#define INPUT_DIM 4
#define OUT_DIM 3
#define H_DIM 10


double_array * relu(double_array * ret , double_array * arr) {
  if ( ret == NULL ) {
    ret = arr_d2_create(arr->x , arr->y);
  }

  for(int i = 0; i < arr->x; i++) {
    for(int j = 0; j < arr->y; j++) {
      ret->a[i][j] = (arr->a[i][j] < 0) ? 0 : arr->a[i][j] ;
    }
  }
  return ret;
}

double_array * softmax(double_array * ret , double_array * arr) {
  if ( ret == NULL ) {
    ret = arr_d2_equating(ret , arr);
  }

  for(int i = 0; i < arr->x; i++) {
    for(int j = 0; j < arr->y; j++) {
      ret->a[i][j] = (long double) exp((long double)arr->a[i][j]);
    }
  }
  
  long double * tmp_arr;
  tmp_arr = malloc(sizeof(long double) * arr->x);
  for(int i = 0; i < ret->x; i++) {
    long double sum = (long double) 0.0;
    for(int j = 0; j < ret->y; j++) {
      sum += (long double) ret->a[i][j];
    }
    tmp_arr[i] = (long double) sum;
  }

  for(int i = 0; i < arr->x; i++) {
    for(int j = 0; j < arr->y; j++) {
      ret->a[i][j] = (long double) ret->a[i][j] / (long double) tmp_arr[i] ;
    }
  }

  free(tmp_arr);
  return ret;
}

long double sparse_cross_entropy(long double a) {
  return (long double) log(a) * -1.0;
}

double_array * to_full(double_array * arr , int y) {
  for (int i = 0 ; i < arr->x ; i++) {
    for (int j = 0 ; j < arr->y ; j++) {
      arr->a[i][j] = (long double) 0.0;
    }
  }
  arr->a[0][y] = (long double) 1.0;
  return arr;
}

// функция активации
double_array * relu_deriv(double_array * ret , double_array * arr) {
  if ( ret == NULL ) {
    ret = arr_d2_create(arr->x , arr->y);
  }

  for(int i = 0; i < arr->x; i++) {
    for(int j = 0; j < arr->y; j++) {
      ret->a[i][j] = (arr->a[i][j] > 0) ? (long double) 1.0 : (long double) 0.0; 
    }
  }

  return ret;
}

double_array * _fill_arr(double_array * arr) {
  double a = - 1.0;

  for (int x = 0 ; x < arr->x ; x++) {
    for (int y = 0 ; y < arr->y ; y++) {
      arr->a[x][y] = a;
      a += (long double) 0.05;
    }
  }

  return arr;
}

int main() {
  // инициализация весов
  srand(time(NULL));

  // веса
  double_array * W1 = arr_d2_create_random(INPUT_DIM , H_DIM);
  double_array * b1 = arr_d2_create_random(1 , H_DIM);
  double_array * W2 = arr_d2_create_random(H_DIM , OUT_DIM);
  double_array * b2 = arr_d2_create_random(1 , OUT_DIM);
  
  // специально заполняю данные чтобы проверить работоспособность
  W1 = _fill_arr(W1);
  b1 = _fill_arr(b1);
  W2 = _fill_arr(W2);
  b2 = _fill_arr(b2);
  /** arr_d2_print("W1" , W1); */
  /** arr_d2_print("b1" , b1); */
  /** arr_d2_print("W2" , W2); */
  /** arr_d2_print("b2" , b2); */

  // входящие параметры
  /** double_array * x  = arr_d2_create_random(1 , INPUT_DIM); */
  double_array * x  = arr_d2_create(1 , INPUT_DIM);
  int y;
  // вычисляемые параметры
  double_array * t1 = NULL;
  double_array * h1 = NULL;
  // транспонированный
  double_array * h1_T = NULL;
  double_array * t2 = NULL;
  // полученный результат
  double_array * z  = NULL;
  // ошибка
  double E;
  double_array * y_full = arr_d2_create(1 , OUT_DIM);
  double_array * dE_dt2   = NULL;
  double_array * dE_dW2   = NULL;
  double_array * dE_db2   = NULL;
  double_array * W2_T     = NULL;
  double_array * dE_dh1   = NULL;
  double_array * t1_rd    = NULL;
  double_array * t1_rd_T  = NULL;
  double_array * dE_dt1   = NULL;
  double_array * x_T      = NULL;
  double_array * dE_dW1   = NULL;
  double_array * dE_db1   = NULL;
  double_array * A_dE_dW1 = NULL;

  double_array * A_dE_dW2 = NULL;
  double_array * A_dE_db1 = NULL;
  double_array * A_dE_db2 = NULL;

  long double ALPHA = (long double) 0.0002;
  // входные данные (потом возьму нормальные)
  // выходные данные
  /** y = 2; */

  dataset_t ** dataset = read_dataset("./dataset.txt");

  /** for (int epoha = 0 ; epoha < 100 ; epoha++) { */
  /**   dataset_t ** dataset_p = dataset; */
  /**   int count = 0; */
  /**   while(*dataset_p) { */
  /**     dataset_t * d = *dataset_p; */
  /**     printf("%d %d %lf\n" , epoha , count++, d->d1); */
  /**     dataset_p++; */
  /**   } */
  /** } */
  /** return 0; */

  int count = 0;
  for (int epoha = 0 ; epoha < 400 ; epoha++) {
    dataset_t ** dataset_p = dataset;

    while(*dataset_p) {
      dataset_t * d = *dataset_p;
      x->a[0][0] = (long double) d->d1;
      x->a[0][1] = (long double) d->d2;
      x->a[0][2] = (long double) d->d3;
      x->a[0][3] = (long double) d->d4;
      y = d->i;

      /** printf("Forward прямое распространение\n"); */
      /** printf("t1 = x @ W1 + b1 \n"); */
      t1 = arr_d2_multiply_matrix(t1, x , W1 );
      t1 = arr_d2_increase(t1 , b1);

      /** printf("h1 = relu(t1) \n"); */
      h1 = relu(h1, t1);

      /** printf("t2 = h1 @ W2 + b2 \n"); */
      t2 = arr_d2_multiply_matrix(t2 , h1 , W2 );
      t2 = arr_d2_increase(t2 , b2);

      // результат
      z = softmax(z , t2);

      // вычисление ошибки
      /** printf("E = sparse_cross_entropy(z , y) \n"); */
      /** E = sparse_cross_entropy(z->a[0][y]); */
      /** printf("ошибка %lf \n" , E); */

      // Backward обратное растространение
      // One hot encoding - приведение массива ответов к виду 0 , 0 , 1
      // где 1 - это верный ответ
      /** printf("y_full = to_full(y , OUT_DIM) \n"); */
      y_full = to_full(y_full , y);

      /** printf("dE_dt2 = z - y_full \n"); */
      dE_dt2 = arr_d2_decrease(dE_dt2, z , y_full);

      /** printf("dE_dW2 = h1.T @ dE_dt2 \n"); */
      /** h1.T - транспонированный */
      h1_T = arr_d2_transparent(h1_T , h1);
      dE_dW2 = arr_d2_multiply_matrix(dE_dW2 , h1_T , dE_dt2);

      /** printf("dE_db2 = dE_dt2 \n"); */
      dE_db2 =  arr_d2_equating(dE_db2  , dE_dt2);

      /** printf("dE_dh1 = dE_dt2 @ W2.T \n"); */
      W2_T = arr_d2_transparent(W2_T , W2);
      dE_dh1 = arr_d2_multiply_matrix(dE_dh1 , dE_dt2 , W2_T);

      /** printf("dE_dt1 = dE_dh1 * relu_deriv(t1) \n"); */
      t1_rd = relu_deriv(t1_rd , t1);
      dE_dt1 = arr_d2_multiply(dE_dt1  , dE_dh1 , t1_rd);

      /** printf("dE_dW1 = x.T @ dE_dt1 \n"); */
      x_T = arr_d2_transparent(x_T , x);
      dE_dW1 = arr_d2_multiply_matrix(dE_dW1 , x_T , dE_dt1);

      /** printf("dE_db1 = dE_dt1 \n"); */
      dE_db1 = arr_d2_equating(dE_db1 , dE_dt1);

      // Update Обновление весов
      /** printf("W1 = W1 - ALPHA * dE_dW1 \n"); */
      A_dE_dW1 = arr_d2_d_multiply(A_dE_dW1 , dE_dW1 , ALPHA);
      W1 = arr_d2_decrease(W1 , W1 , A_dE_dW1);

      /** printf("b1 = b1 - ALPHA * dE_db1 \n"); */
      A_dE_db1 = arr_d2_d_multiply(A_dE_db1 , dE_db1 , ALPHA);
      b1 = arr_d2_decrease(b1 , b1 , A_dE_db1);
      
      /** printf("W2 = W2 - ALPHA * dE_dW2 \n"); */
      A_dE_dW2 = arr_d2_d_multiply(A_dE_dW2 , dE_dW2 , ALPHA);
      W2 = arr_d2_decrease(W2 , W2 , A_dE_dW2);

      /** printf("b2 = b2 - ALPHA * dE_db2 \n"); */
      A_dE_db2 = arr_d2_d_multiply(A_dE_db2 , dE_db2 , ALPHA);
      b2 = arr_d2_decrease(b2 , b2 , A_dE_db2);

      dataset_p++;
      count++;
    }
  }
  /** arr_d2_print("W1" , W1); */
  /** arr_d2_print("b1" , b1); */
  /** arr_d2_print("W2" , W2); */
  /** arr_d2_print("b2" , b2); */

  // провожу тестирование
  x->a[0][0] = dataset[0]->d1;
  x->a[0][1] = dataset[0]->d2;
  x->a[0][2] = dataset[0]->d3;
  x->a[0][3] = dataset[0]->d4;

  /** printf("t1 = x @ W1 + b1 \n"); */
  t1 = arr_d2_multiply_matrix(t1, x , W1 );
  t1 = arr_d2_increase(t1 , b1);

  /** printf("h1 = relu(t1) \n"); */
  h1 = relu(h1, t1);

  /** printf("t2 = h1 @ W2 + b2 \n"); */
  t2 = arr_d2_multiply_matrix(t2 , h1 , W2 );
  t2 = arr_d2_increase(t2 , b2);

  // результат
  /** printf("z = softmax(t2) \n"); */
  z = softmax(z , t2);
  arr_d2_print("z" , z);
  /** printf("правильный ответ %d\n" , dataset[0]->i); */

}
