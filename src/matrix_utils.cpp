#include "../include/matrix_utils.h"
#include <fstream>
#include <random>
#include <iostream>
#include <filesystem>



// Генерация случайной матрицы
Matrix generate_matrix(int rows, int cols, double min_val, double max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min_val, max_val);
    
    Matrix matrix(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dist(gen);
        }
    }
    return matrix;
}

// Сохранение матрицы в файл
void save_matrix(const Matrix& matrix, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int rows = matrix.size();
    int cols = matrix[0].size();
    
    file << rows << " " << cols << "\n";
    for (const auto& row : matrix) {
        for (double val : row) {
            file << val << " ";
        }
        file << "\n";
    }
}

// Загрузка матрицы из файла
Matrix load_matrix(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int rows, cols;
    file >> rows >> cols;
    
    Matrix matrix(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> matrix[i][j];
        }
    }
    
    return matrix;
}

// Генерация всех тестовых матриц
void generate_all_test_matrices() {
    std::vector<int> sizes = {16, 32, 64, 128, 256, 512, 1024, 2048};  // Степени двойки
    
    for (int size : sizes) {
        std::string dir_path = "matrix/" + std::to_string(size) + "x" + std::to_string(size);
        
        Matrix A = generate_matrix(size, size);
        Matrix B = generate_matrix(size, size);
        
        save_matrix(A, dir_path + "/A.txt");
        save_matrix(B, dir_path + "/B.txt");
        
        std::cout << "Generated matrices for size: " << size << "x" << size << std::endl;
    }
}