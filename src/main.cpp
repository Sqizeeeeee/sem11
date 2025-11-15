#include "../include/matrix_utils.h"
#include "../include/standard_multiply.h"
#include "../include/strassen_multiply.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Matrix Multiplication Analysis" << std::endl;
    std::cout << "==============================" << std::endl;

    // 1. Генерация тестовых матриц
    std::cout << "\n1. Generating test matrices..." << std::endl;
    generate_all_test_matrices();

    // 2. Тестирование стандартного умножения
    std::cout << "\n2. Testing standard multiplication..." << std::endl;
    std::vector<std::pair<int, double>> standard_results = test_standard_multiply();

    // 3. Тестирование алгоритма Штрассена
    std::cout << "\n3. Testing Strassen multiplication..." << std::endl;
    std::vector<std::pair<int, double>> strassen_results = test_strassen_multiply();

    // 4. Вывод промежуточных результатов для маленькой матрицы
    std::cout << "\n4. Printing intermediate results for 4x4 matrix..." << std::endl;
    try {
        Matrix A_small = load_matrix("matrix/16x16/A.txt");
        Matrix B_small = load_matrix("matrix/16x16/B.txt");
        
        // Для демонстрации возьмем подматрицы 4x4
        Matrix A4(4, std::vector<double>(4));
        Matrix B4(4, std::vector<double>(4));
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                A4[i][j] = A_small[i][j];
                B4[i][j] = B_small[i][j];
            }
        }
        
        std::vector<Matrix> intermediates;
        Matrix C4 = strassen_multiply_recursive(A4, B4, intermediates);
        
        if (!intermediates.empty()) {
            print_intermediate_matrices(intermediates[0], intermediates[1], intermediates[2],
                                      intermediates[3], intermediates[4], intermediates[5],
                                      intermediates[6]);
        }
        
    } catch (const std::exception& e) {
        std::cout << "Error loading small matrices: " << e.what() << std::endl;
    }

    // 5. Сравнительный анализ
    std::cout << "\n5. Comparative Analysis:" << std::endl;
    std::cout << "Size\tStandard(ms)\tStrassen(ms)\tRatio" << std::endl;
    for (size_t i = 0; i < standard_results.size() && i < strassen_results.size(); ++i) {
        double ratio = strassen_results[i].second / standard_results[i].second;
        std::cout << standard_results[i].first << "x" << standard_results[i].first << "\t"
                  << standard_results[i].second << "\t\t"
                  << strassen_results[i].second << "\t\t"
                  << ratio << std::endl;
    }

    std::cout << "\nAnalysis complete! Check CSV files in ../data/" << std::endl;
    return 0;
}