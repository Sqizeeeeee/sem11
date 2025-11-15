#ifndef STRASSEN_MULTIPLY_H
#define STRASSEN_MULTIPLY_H

#include "matrix_utils.h"
#include <chrono>

// Основная функция умножения Штрассена
Matrix strassen_multiply(const Matrix& A, const Matrix& B);

// Функции для вывода промежуточных результатов
void print_intermediate_matrices(const Matrix& M1, const Matrix& M2, 
                                const Matrix& M3, const Matrix& M4,
                                const Matrix& M5, const Matrix& M6,
                                const Matrix& M7);

void print_result_blocks(const Matrix& C11, const Matrix& C12, 
                        const Matrix& C21, const Matrix& C22);

// Тестирование алгоритма Штрассена
std::vector<std::pair<int, double>> test_strassen_multiply();

// Теоретическая сложность Штрассена: O(n^log2(7)) ≈ O(n^2.807)
long long theoretical_operations_count_strassen(int n);


Matrix strassen_multiply_recursive(const Matrix& A, const Matrix& B, 
                                  std::vector<Matrix>& intermediates);

#endif