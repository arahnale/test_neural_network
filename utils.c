#include "utils.h"

long double randfrom(long double min, long double max) {
    long double range = (max - min); 
    long double div = RAND_MAX / range;
    return min + (rand() / div);
}

static double_array ** arrays = NULL;
static int arrays_count = 0;

// создание двумерного массива double
double_array * arr_d2_create (int x , int y) {
  if (x < 1) exit(1);
  if (y < 1) exit(1);

  double_array * arr = malloc(sizeof(double_array));
  if (arr == NULL) exit(1);

  arr->x = x;
  arr->y = y;

  arr->a = (long double **) malloc (sizeof(long double) * x);
  for (int i = 0 ; i < x  ; i++) {
    arr->a[i] = (long double *) malloc (sizeof(long double) * y);
    if (arr->a[i] == NULL) exit(1);
    for (int j = 0 ; j < y  ; j++) {
      /** printf("arr[%d][%d] = %d\n" , i , j , 10 * i + j); */
      arr->a[i][j] = (long double) 0.0;
    }
  }

  if (arrays == NULL) {
    arrays = malloc(sizeof(double_array *));
    if (arrays == NULL) {
      printf("error malloc arrays\n");
      exit(1);
    }
  } else {
    arrays = realloc(arrays , sizeof(double_array *) * (arrays_count + 1));
    if (arrays == NULL) {
      printf("error malloc arrays\n");
      exit(1);
    }
  }
  arrays[arrays_count++] = arr;

  return arr;
}

// вывод  двумерного массива double
int arr_d2_print (char * name , double_array * arr) {
  if (arr == NULL) {
    printf("Error: %s array is NULL\n" , name);
    return 0;
  }
  printf("%s x = %d , y = %d\n" , name , arr->x , arr->y);
  for (int i = 0 ; i < arr->x ; i++) {
    for (int j = 0 ; j < arr->y ; j++) {
      printf("%Lf " , arr->a[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}

// наполнение массива рандомными значениями
void arr_d2_set_random (double_array * arr) {
  for (int i = 0 ; i < arr->x ; i++) {
    for (int j = 0 ; j < arr->y ; j++) {
      arr->a[i][j] = (long double) rand() / RAND_MAX;
    }
  }
}

// создание двумерного массива double с рандомными значениями
double_array * arr_d2_create_random (int x , int y) {
  double_array * arr = malloc(sizeof(double_array));

  arr->x = x;
  arr->y = y;

  arr->a = (long double **) malloc (sizeof(long double) * x);
  for (int i = 0 ; i < x  ; i++) {
    arr->a[i] = (long double *) malloc (sizeof(long double) * y);
    for (int j = 0 ; j < y  ; j++) {
      /** printf("arr[%d][%d] = %d\n" , i , j , 10 * i + j); */
      arr->a[i][j] = randfrom(-1.0 , 1.0);
    }
  }

  if (arrays == NULL) {
    arrays = malloc(sizeof(double_array *));
    if (arrays == NULL) {
      printf("error malloc arrays\n");
      exit(1);
    }
  } else {
    arrays = realloc(arrays , sizeof(double_array *) * (arrays_count + 1));
    if (arrays == NULL) {
      printf("error malloc arrays\n");
      exit(1);
    }
  }
  arrays[arrays_count++] = arr;

  return arr;
}

// умножение двумерных double матриц
double_array * arr_d2_multiply_matrix(double_array * arr , double_array * A , double_array * B) {
  if (arr == NULL) {
    arr = arr_d2_create(A->x , B->y);
  }

  /** double_array * C = arr_d2_create(A->x , B->y); */
  if (arr->x != A->x) {
    printf("Error arr_d2_multiply: arr->x (%d) != A->x (%d) \n" , arr->x , A->x);
    exit(1);
  }
  if (arr->y != B->y) {
    printf("Error arr_d2_multiply: arr->y (%d) != B->y (%d)\n" , arr->y , B->y);
    exit(1);
  }
  if (A->y != B->x) {
    printf("Error arr_d2_multiply: A->y (%d) != B->x (%d)\n" , A->y , B->x);
    exit(1);
  }

  for(int i = 0; i < A->x; i++) {
    for(int j = 0; j < B->y; j++) {
      arr->a[i][j] = (long double) 0.0;
      for(int k = 0; k < A->y; k++) {
        /** printf("arr->a[i%d][j%d] += A->a[i%d][k%d] * B->a[k%d][j%d]\n" , i , j , i , k , k , j); */
        arr->a[i][j] += (long double) A->a[i][k] * (long double) B->a[k][j];
      }
    }
  }

  return arr;
}


// умножение двумерных double массивов
double_array * arr_d2_multiply(double_array * arr , double_array * A , double_array * B) {
  if (arr == NULL) {
    arr = arr_d2_create(A->x , B->y);
  }

  /** double_array * C = arr_d2_create(A->x , B->y); */
  if (arr->x != A->x) {
    printf("Error arr_d2_multiply: arr->x (%d) != A->x (%d) \n" , arr->x , A->x);
    exit(1);
  }
  if (arr->y != B->y) {
    printf("Error arr_d2_multiply: arr->y (%d) != B->y (%d)\n" , arr->y , B->y);
    exit(1);
  }
  if (A->x != B->x) {
    printf("Error arr_d2_multiply: A->x (%d) != B->x (%d)\n" , A->x , B->x);
    exit(1);
  }

  for(int i = 0; i < A->x; i++) {
    for(int j = 0; j < B->y; j++) {
      arr->a[i][j] = (long double) A->a[i][j] * (long double) B->a[i][j];
    }
  }

  return arr;
}

// умножение двумерного массива на число double
double_array * arr_d2_d_multiply(double_array * arr , double_array * A , long double B) {
  if (arr == NULL) {
    /** printf("arr_d2_multiply: create x = %d , y = %d\n" , A->x , A->y); */
    arr = arr_d2_create(A->x , A->y);
  }

  if (arr->x != A->x) {
    printf("Error: arr_d2_d_multiply arr->x != A->x \n");
    exit(1);
  }
  if (arr->y != A->y) {
    printf("Error: arr_d2_d_multiply arr->y != A->y\n");
    exit(1);
  }
  /** double_array * C = arr_d2_create(A->x , A->y); */

  for(int i = 0; i < A->x; i++) {
    for(int j = 0; j < A->y; j++) {
      arr->a[i][j] = (long double) A->a[i][j] * (long double) B;
    }
  }

  return arr;
}


// вычитание A - B
double_array * arr_d2_decrease(double_array * arr , double_array * A , double_array * B) {
  if (arr == NULL) {
    /** printf("arr_d2_decrease: create array x = %d , y = %d\n" , A->x , A->y); */
    arr = arr_d2_create(A->x , A->y);
  }

  /** double_array * arr = arr_d2_create(A->x , A->y); */
  if (A->x != B->x) {
    printf("Error arr_d2_decrease: A->x != B->x \n");
    exit(1);
  }

  if (A->y != B->y) {
    printf("Error arr_d2_decrease: A->y != B->y \n");
    exit(1);
  }

  for (int i = 0 ; i < arr->x ; i++) {
    for (int j = 0 ; j < arr->y ; j++) {
      /** printf("arr->a[%d][%d] = A->a[%d][%d] - B->a[%d][%d]\n" , i , j , i , j , i , j); */
      arr->a[i][j] = (long double) A->a[i][j] - (long double) B->a[i][j];
    }
  }

  return arr;
}

// транспонирование двумерного double массива
double_array * arr_d2_transparent(double_array * ret , double_array * arr) {
  if (ret == NULL) {
    /** printf("arr_d2_transparent: create array x = %d , y = %d\n" , arr->y , arr->x); */
    ret = arr_d2_create(arr->y , arr->x);
  }

  if (ret->x != arr->y) {
    printf("Error arr_d2_transparent: ret->x != arr->y \n");
    exit(1);
  }

  if (ret->y != arr->x) {
    printf("Error arr_d2_transparent: ret->y != arr->x\n");
    exit(1);
  }

  for (int i = 0; i < arr->x; i++) {
    for (int j = 0; j < arr->y; j++) {
      ret->a[j][i] = (long double) arr->a[i][j];
    }
  }

  return ret;
}

// приравнивание массива
double_array * arr_d2_equating(double_array * arr , double_array * A) {
  if (arr == NULL) {
    arr = arr_d2_create(A->x , A->y);
  }

  if (arr->x != A->x) {
    printf("Error arr_d2_equating: arr->x != A->x\n");
    exit(1);
  }
  if (arr->y != A->y) {
    printf("Error arr_d2_equating: arr->y != A->y\n");
    exit(1);
  }

  // может быть поулчится использовать копирование памяти
  for (int i = 0; i < arr->x; i++) {
    for (int j = 0; j < arr->y; j++) {
      arr->a[i][j] = (long double) A->a[i][j];
    }
  }

  return arr;
}

// сложение массива
double_array * arr_d2_increase(double_array * arr , double_array * A) {
  if (arr == NULL) {
    arr = arr_d2_create(A->x , A->y);
  }

  if (arr->x != A->x) {
    printf("Error arr_d2_increase: arr->x != A->x\n");
    exit(1);
  }
  if (arr->y != A->y) {
    printf("Error arr_d2_increase: arr->y != A->y\n");
    exit(1);
  }

  // может быть поулчится использовать копирование памяти
  for (int i = 0; i < arr->x; i++) {
    for (int j = 0; j < arr->y; j++) {
      arr->a[i][j] += (long double) A->a[i][j];
    }
  }

  return arr;
}

int arr_d2_free() {
  for (int i = 0 ; i < arrays_count ; i++) {
    /** printf("x = %d , y = %d\n" , arrays[i]->x , arrays[i]->y); */
    for (int x = 0 ; x < arrays[i]->x ; x++) {
      free(arrays[i]->a[x]);
    }
    free(arrays[i]->a);
    free(arrays[i]);
  }

  return 0;
}

// чтение датасета с данными для обучения из файла
dataset_t ** read_dataset(char * filename) {
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  int count = 0;
  dataset_t ** dataset = malloc(sizeof(dataset_t *));
  while ((read = getline(&line, &len, fp)) != -1) {
    dataset[count] = malloc(sizeof(dataset_t));
    sscanf(line , "%Lf %Lf %Lf %Lf %d" , &dataset[count]->d1 , &dataset[count]->d2 , &dataset[count]->d3 , &dataset[count]->d4 , &dataset[count]->i);
    /** printf("Retrieved line of length %zu:\n", read); */
    /** printf("%s", line); */
    count++;
    dataset = realloc(dataset , (count + 2) * sizeof(dataset_t *));
  }
  dataset[count] = NULL;

  fclose(fp);
  if (line)
    free(line);

  return dataset;
}

int dataset_free(dataset_t ** dataset) {
  free(dataset);
  return 0;
}
