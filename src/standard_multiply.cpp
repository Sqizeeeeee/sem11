#include "../include/standard_multiply.h"
#include <iostream>
#include <fstream>

Matrix standard_multiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();
    
    Matrix C(n, std::vector<double>(p, 0.0));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return C;
}

long long theoretical_operations_count(int n) {
    // Умножений: n^3, Сложений: n^3 - n^2
    return 2 * n * n * n - n * n;
}

std::vector<std::pair<int, double>> test_standard_multiply() {
    std::vector<int> sizes = {16, 32, 64, 128, 256, 512, 1024, 2048};
    std::vector<std::pair<int, double>> results;
    
    for (int size : sizes) {
        std::string dir_path = "matrix/" + std::to_string(size) + "x" + std::to_string(size);
        
        try {
            Matrix A = load_matrix(dir_path + "/A.txt");
            Matrix B = load_matrix(dir_path + "/B.txt");
            
            auto start = std::chrono::high_resolution_clock::now();
            Matrix C = standard_multiply(A, B);
            auto end = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> duration = end - start;
            results.push_back({size, duration.count()});
            
            std::cout << "Standard " << size << "x" << size << ": " 
                      << duration.count() << " ms" << std::endl;
                      
        } catch (const std::exception& e) {
            std::cout << "Error for size " << size << ": " << e.what() << std::endl;
        }
    }
    
    // Сохраняем результаты в CSV
    std::ofstream file("data/standard_results.csv");
    file << "size,time_ms,theoretical_ops\n";
    for (const auto& result : results) {
        file << result.first << "," << result.second << "," 
             << theoretical_operations_count(result.first) << "\n";
    }
    
    return results;
}