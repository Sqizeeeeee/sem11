#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <vector>
#include <string>

// Определяем тип матрицы для удобства
using Matrix = std::vector<std::vector<double>>;

// Генерация случайной матрицы размера rows x cols
Matrix generate_matrix(int rows, int cols, double min_val = 0.0, double max_val = 1.0);

// Сохранение матрицы в файл
void save_matrix(const Matrix& matrix, const std::string& filename);

// Загрузка матрицы из файла
Matrix load_matrix(const std::string& filename);

// Генерация всех тестовых матриц для разных размеров
void generate_all_test_matrices();

#endif