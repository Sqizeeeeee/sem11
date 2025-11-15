cmake_minimum_required(VERSION 3.10)
project(MatrixMultiplicationAnalysis)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение папки с исходниками
include_directories(include)

# Создание исполняемого файла
add_executable(matrix_analysis
    src/main.cpp
    src/matrix_utils.cpp
    src/standard_multiply.cpp
    src/strassen_multiply.cpp
)

# Настройка выходной директории
set_target_properties(matrix_analysis PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Основная цель - полный пайплайн анализа
add_custom_target(run_analysis
    COMMAND ${CMAKE_COMMAND} -E echo "=== Creating directories ==="
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/graphs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running C++ matrix multiplication ==="
    COMMAND $<TARGET_FILE:matrix_analysis>
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running Python analysis ==="
    COMMAND python3 ${CMAKE_SOURCE_DIR}/analysis.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  # Запускаем из корня проекта!
    DEPENDS matrix_analysis
    COMMENT "Running complete analysis pipeline: C++ computation -> Python visualization"
)

# Цель только для генерации тестовых данных
add_custom_target(generate_matrices
    COMMAND ${CMAKE_COMMAND} -E echo "Generating test matrices..."
    COMMAND $<TARGET_FILE:matrix_analysis> --generate-only
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS matrix_analysis
    COMMENT "Generating test matrices for all sizes"
)

# Цель только для Python анализа (если данные уже есть)
add_custom_target(plot_only
    COMMAND ${CMAKE_COMMAND} -E echo "Running Python analysis..."
    COMMAND python3 ${CMAKE_SOURCE_DIR}/analysis.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Python analysis only (requires existing data)"
)

# Цель для очистки всех сгенерированных данных
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning generated data..."
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/graphs
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/matrix
    COMMENT "Cleaning all generated data and graphs"
)

# Цель для показа структуры проекта
add_custom_target(show_structure
    COMMAND ${CMAKE_COMMAND} -E echo "Project structure:"
    COMMAND ${CMAKE_COMMAND} -E echo "matrix_multiply_cpp/"
    COMMAND ${CMAKE_COMMAND} -E echo "├── src/                 - C++ source files"
    COMMAND ${CMAKE_COMMAND} -E echo "├── include/             - C++ header files"
    COMMAND ${CMAKE_COMMAND} -E echo "├── matrix/              - Test matrices"
    COMMAND ${CMAKE_COMMAND} -E echo "├── data/                - CSV results"
    COMMAND ${CMAKE_COMMAND} -E echo "├── graphs/              - Output graphs"
    COMMAND ${CMAKE_COMMAND} -E echo "├── analysis.py          - Python analysis"
    COMMAND ${CMAKE_COMMAND} -E echo "└── CMakeLists.txt       - Build configuration"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Showing project structure"
)

# Информация о использовании
add_custom_target(show_help
    COMMAND ${CMAKE_COMMAND} -E echo "Available targets:"
    COMMAND ${CMAKE_COMMAND} -E echo "  make run_analysis    - Full pipeline (C++ + Python)"
    COMMAND ${CMAKE_COMMAND} -E echo "  make generate_matrices - Generate test matrices only"
    COMMAND ${CMAKE_COMMAND} -E echo "  make plot_only       - Run Python analysis only"
    COMMAND ${CMAKE_COMMAND} -E echo "  make clean_all       - Clean all generated data"
    COMMAND ${CMAKE_COMMAND} -E echo "  make show_structure  - Show project structure"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Run ./bin/matrix_analysis for C++ only"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Showing available targets"
)