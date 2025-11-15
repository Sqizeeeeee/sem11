#include "../include/strassen_multiply.h"
#include "../include/standard_multiply.h"
#include <iostream>
#include <fstream>

// Вспомогательные функции для операций с матрицами
Matrix add_matrix(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix result(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
    return result;
}

Matrix subtract_matrix(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix result(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
    return result;
}

// Разбиение матрицы на 4 блока
void split_matrix(const Matrix& source, Matrix& A11, Matrix& A12, 
                 Matrix& A21, Matrix& A22) {
    int n = source.size() / 2;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A11[i][j] = source[i][j];
            A12[i][j] = source[i][j + n];
            A21[i][j] = source[i + n][j];
            A22[i][j] = source[i + n][j + n];
        }
    }
}

// Сборка матрицы из 4 блоков
Matrix combine_matrix(const Matrix& C11, const Matrix& C12, 
                     const Matrix& C21, const Matrix& C22) {
    int n = C11.size();
    Matrix result(2 * n, std::vector<double>(2 * n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = C11[i][j];
            result[i][j + n] = C12[i][j];
            result[i + n][j] = C21[i][j];
            result[i + n][j + n] = C22[i][j];
        }
    }
    return result;
}

// Основная рекурсивная функция Штрассена
Matrix strassen_multiply_recursive(const Matrix& A, const Matrix& B, 
                                  std::vector<Matrix>& intermediates) {
    int n = A.size();
    
    // Базовый случай
    if (n <= 64) {
        return standard_multiply(A, B);
    }
    
    // Разбиваем матрицы на блоки
    int new_size = n / 2;
    Matrix A11(new_size, std::vector<double>(new_size));
    Matrix A12(new_size, std::vector<double>(new_size));
    Matrix A21(new_size, std::vector<double>(new_size));
    Matrix A22(new_size, std::vector<double>(new_size));
    
    Matrix B11(new_size, std::vector<double>(new_size));
    Matrix B12(new_size, std::vector<double>(new_size));
    Matrix B21(new_size, std::vector<double>(new_size));
    Matrix B22(new_size, std::vector<double>(new_size));
    
    split_matrix(A, A11, A12, A21, A22);
    split_matrix(B, B11, B12, B21, B22);
    
    // Вычисляем промежуточные матрицы M1-M7
    Matrix M1 = strassen_multiply_recursive(add_matrix(A11, A22), add_matrix(B11, B22), intermediates);
    Matrix M2 = strassen_multiply_recursive(add_matrix(A21, A22), B11, intermediates);
    Matrix M3 = strassen_multiply_recursive(A11, subtract_matrix(B12, B22), intermediates);
    Matrix M4 = strassen_multiply_recursive(A22, subtract_matrix(B21, B11), intermediates);
    Matrix M5 = strassen_multiply_recursive(add_matrix(A11, A12), B22, intermediates);
    Matrix M6 = strassen_multiply_recursive(subtract_matrix(A21, A11), add_matrix(B11, B12), intermediates);
    Matrix M7 = strassen_multiply_recursive(subtract_matrix(A12, A22), add_matrix(B21, B22), intermediates);
    
    // Сохраняем промежуточные матрицы только на первом уровне рекурсии
    if (intermediates.empty() && n == A.size()) {
        intermediates.push_back(M1);
        intermediates.push_back(M2);
        intermediates.push_back(M3);
        intermediates.push_back(M4);
        intermediates.push_back(M5);
        intermediates.push_back(M6);
        intermediates.push_back(M7);
    }
    
    // Вычисляем результирующие блоки
    Matrix C11 = add_matrix(subtract_matrix(add_matrix(M1, M4), M5), M7);
    Matrix C12 = add_matrix(M3, M5);
    Matrix C21 = add_matrix(M2, M4);
    Matrix C22 = add_matrix(subtract_matrix(add_matrix(M1, M3), M2), M6);
    
    return combine_matrix(C11, C12, C21, C22);
}

// Основная функция умножения Штрассена
Matrix strassen_multiply(const Matrix& A, const Matrix& B) {
    std::vector<Matrix> intermediates;
    return strassen_multiply_recursive(A, B, intermediates);
}

// Функции для вывода промежуточных результатов
void print_intermediate_matrices(const Matrix& M1, const Matrix& M2, 
                                const Matrix& M3, const Matrix& M4,
                                const Matrix& M5, const Matrix& M6,
                                const Matrix& M7) {
    std::cout << "\n=== Intermediate Matrices M1-M7 ===" << std::endl;
    
    std::cout << "M1:" << std::endl;
    for (const auto& row : M1) {
        for (double val : row) std::cout << val << " ";
        std::cout << std::endl;
    }
    
    std::cout << "M2:" << std::endl;
    for (const auto& row : M2) {
        for (double val : row) std::cout << val << " ";
        std::cout << std::endl;
    }
    
    // Аналогично для M3-M7...
    std::cout << "..." << std::endl;
}

void print_result_blocks(const Matrix& C11, const Matrix& C12, 
                        const Matrix& C21, const Matrix& C22) {
    std::cout << "\n=== Result Blocks ===" << std::endl;
    
    std::cout << "C11:" << std::endl;
    for (const auto& row : C11) {
        for (double val : row) std::cout << val << " ";
        std::cout << std::endl;
    }
    
    // Аналогично для C12, C21, C22...
    std::cout << "..." << std::endl;
}

long long theoretical_operations_count_strassen(int n) {
    // Для алгоритма Штрассена:
    // Количество операций ~ 7 * T(n/2) + 18*(n/2)²
    // Решение: O(n^log2(7)) ≈ O(n^2.807)
    if (n <= 64) { // базовый случай - стандартное умножение
        return theoretical_operations_count(n);
    }
    return 7 * theoretical_operations_count_strassen(n/2) + 18 * (n/2) * (n/2);
}

// Тестирование алгоритма Штрассена
std::vector<std::pair<int, double>> test_strassen_multiply() {
    std::vector<int> sizes = {16, 32, 64, 128, 256, 512, 1024, 2048};
    std::vector<std::pair<int, double>> results;
    
    for (int size : sizes) {
        std::string dir_path = "matrix/" + std::to_string(size) + "x" + std::to_string(size);
        
        try {
            Matrix A = load_matrix(dir_path + "/A.txt");
            Matrix B = load_matrix(dir_path + "/B.txt");
            
            auto start = std::chrono::high_resolution_clock::now();
            Matrix C = strassen_multiply(A, B);
            auto end = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> duration = end - start;
            results.push_back({size, duration.count()});
            
            std::cout << "Strassen " << size << "x" << size << ": " 
                      << duration.count() << " ms" << std::endl;
                      
        } catch (const std::exception& e) {
            std::cout << "Error for size " << size << ": " << e.what() << std::endl;
        }
    }
    
    // Сохраняем результаты в CSV
    std::ofstream file("data/strassen_results.csv");
    file << "size,time_ms,theoretical_ops\n";
    for (const auto& result : results) {
        file << result.first << "," << result.second << "," 
             << theoretical_operations_count_strassen(result.first) << "\n";
    }
    
    return results;
}