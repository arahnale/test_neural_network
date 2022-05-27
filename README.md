# Описание

Репозиторий сделан с целью тестирования производительности python и pure C

# Источники

Исходный код python взят с видео по нейронкам 

https://www.youtube.com/watch?v=bXGBeRzM87g&t=507s 

https://github.com/dkorobchenko-nv/nn-python/blob/main/train.py

# Тесты производительности 

Процессор: Apple M1 Max

Сборка и выполнение не оптимизированного алгоритма на **pure C**

```
clang -g utils.c main.c && time ./a.out
./a.out  0,10s user 0,00s system 36% cpu 0,269 total
```

Выполнение не оптимизированного алгоритма на **python**

```
~/Documents/projects/test_neural_network ➜ (master) ᚫ time python3 main2.py
python3 main2.py  1,39s user 2,16s system 320% cpu 1,105 total
```

Выполнение оптимизированного алгоритма на **python**

```
Accuracy: 0.9733333333333334
python3 main1.py  0,78s user 2,11s system 660% cpu 0,437 total
```
