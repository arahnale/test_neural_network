#!/usr/bin/env python
# -*- coding: utf8 -*-

import random
import numpy as np

INPUT_DIM = 4
OUT_DIM = 3
H_DIM = 10

def relu(t):
    return np.maximum(t, 0)

def softmax(t):
    out = np.exp(t)
    return out / np.sum(out)

def softmax_batch(t):
    out = np.exp(t)
    return out / np.sum(out, axis=1, keepdims=True)

def sparse_cross_entropy(z, y):
    return -np.log(z[0, y])

def sparse_cross_entropy_batch(z, y):
    return -np.log(np.array([z[j, y[j]] for j in range(len(y))]))

def to_full(y, num_classes):
    y_full = np.zeros((1, num_classes))
    y_full[0, y] = 1
    return y_full

def to_full_batch(y, num_classes):
    y_full = np.zeros((len(y), num_classes))
    for j, yj in enumerate(y):
        y_full[j, yj] = 1
    return y_full

def relu_deriv(t):
    return (t >= 0).astype(float)

from sklearn import datasets
iris = datasets.load_iris()
dataset = [(iris.data[i][None, ...], iris.target[i]) for i in range(len(iris.target))]

def _fill_arr(arr):
    a = -1.0
    for x in range(len(arr)):
        for y in range(len(arr[x])):
            arr[x][y] = a
            a = a + 0.05
    return arr

W1 = np.random.rand(INPUT_DIM, H_DIM)
b1 = np.random.rand(1, H_DIM)
W2 = np.random.rand(H_DIM, OUT_DIM)
b2 = np.random.rand(1, OUT_DIM)

W1 = _fill_arr(W1)
b1 = _fill_arr(b1)
W2 = _fill_arr(W2)
b2 = _fill_arr(b2)

# изменяем диапазон распределения параметров, помогает для обучения
#  W1 = (W1 - 0.5) * 2 * np.sqrt(1/INPUT_DIM)
#  b1 = (b1 - 0.5) * 2 * np.sqrt(1/INPUT_DIM)
#  W2 = (W2 - 0.5) * 2 * np.sqrt(1/H_DIM)
#  b2 = (b2 - 0.5) * 2 * np.sqrt(1/H_DIM)

# скорость обучения
ALPHA = 0.0002
NUM_EPOCHS = 400

loss_arr = []

count = 0
for ep in range(NUM_EPOCHS):
    #  random.shuffle(dataset)
    for i in range(len(dataset)):

        x, y = dataset[i]

        # Forward прямое растространение
        t1 = x @ W1 + b1
        h1 = relu(t1)
        t2 = h1 @ W2 + b2
        # вероятности предсказанные моделью
        z = softmax(t2)
        #  E = sparse_cross_entropy(z, y)

        # Backward обратное растространение
        y_full = to_full(y, OUT_DIM)
        dE_dt2 = z - y_full
        dE_dW2 = h1.T @ dE_dt2
        dE_db2 = dE_dt2
        dE_dh1 = dE_dt2 @ W2.T
        dE_dt1 = dE_dh1 * relu_deriv(t1)
        dE_dW1 = x.T @ dE_dt1
        dE_db1 = dE_dt1

        # Update обновление весов
        W1 = W1 - ALPHA * dE_dW1
        b1 = b1 - ALPHA * dE_db1
        W2 = W2 - ALPHA * dE_dW2
        b2 = b2 - ALPHA * dE_db2

        count += 1
        #  loss_arr.append(E)

def predict(x):
    t1 = x @ W1 + b1
    h1 = relu(t1)
    t2 = h1 @ W2 + b2
    print("t2" , t2)
    z = softmax(t2)
    return z

def calc_accuracy():
    correct = 0
    for x, y in dataset:
        z = predict(x)
        y_pred = np.argmax(z)
        if y_pred == y:
            correct += 1
    acc = correct / len(dataset)
    return acc

#  print("W1", W1)
#  print("b1", b1)
#  print("W2", W2)
#  print("b2", b2)

#  accuracy = calc_accuracy()
x , y = dataset[0]
z = predict(x)
print(z)
#  print("Accuracy:", accuracy)

#  import matplotlib.pyplot as plt
#  plt.plot(loss_arr)
#  plt.show()

