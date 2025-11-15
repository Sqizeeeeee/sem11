#ifndef STANDARD_MULTIPLY_H
#define STANDARD_MULTIPLY_H

#include "matrix_utils.h"
#include <chrono>

// Стандартное умножение матриц
Matrix standard_multiply(const Matrix& A, const Matrix& B);

// Тестирование стандартного умножения на разных размерах
// Возвращает вектор пар: {размер_матрицы, время_выполнения_в_миллисекундах}
std::vector<std::pair<int, double>> test_standard_multiply();

// Теоретическое количество операций для матрицы n x n
// Умножений: n^3, Сложений: n^3 - n^2, Всего: ~2n^3
long long theoretical_operations_count(int n);

#endif