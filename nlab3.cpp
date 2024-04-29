#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include<chrono>
#include<algorithm>
#include<sstream>
#include<string>
struct Elem {
    int row, col, val;
    Elem(int row, int col, int val) : row(row), col(col), val(val) {}
};
void rearrangeMatrix(std::vector<Elem>& matrix, int b) {
    std::vector<int> rows;
    std::vector<int> cols;
    std::vector<int> valuesGreaterThanB;
    std::vector<int> valuesLessThanOrEqualB;
    // Сохраняем значения row, col и val для всех ненулевых элементов
    for (const auto& elem : matrix) {
        if (elem.val > b) {
            rows.push_back(elem.row);
            cols.push_back(elem.col);
            valuesGreaterThanB.push_back(elem.val);
        }
        else {
            rows.push_back(elem.row);
            cols.push_back(elem.col);
            valuesLessThanOrEqualB.push_back(elem.val);
        }
    }
    matrix.clear();
    // Помещаем сначала элементы с val > b, а затем с val <= b
    for (size_t i = 0; i < valuesGreaterThanB.size(); ++i) {
        matrix.push_back({ rows[i], cols[i], valuesGreaterThanB[i] });
    }
    for (size_t i = 0; i < valuesLessThanOrEqualB.size(); ++i) {
        matrix.push_back({ rows[i + valuesGreaterThanB.size()], cols[i + valuesGreaterThanB.size()], valuesLessThanOrEqualB[i] });
    }
}
std::vector<Elem> generate_sparse_mat(int row, int col, int count_of_elems) {
    std::vector<Elem> mat;
    std::vector<int> rows;
    std::vector<int> cols;
    std::vector<std::pair<int, int>> all_indicies;
    while (mat.size() < count_of_elems) {
        int rand_row = rand() % row;
        int rand_col = rand() % col;
        if (std::count(all_indicies.begin(), all_indicies.end(), std::make_pair(rand_row, rand_col)) == 0 &&
            std::count(rows.begin(), rows.end(), rand_row) <= row && std::count(cols.begin(), cols.end(), rand_col) <= col) {
            all_indicies.emplace_back(rand_row, rand_col);
            rows.emplace_back(rand_row);
            cols.emplace_back(rand_col);
            mat.push_back(Elem(rand_row, rand_col, 1 + rand() % 9));
        }
    }
    return mat;
}
void print_sparse_mat(std::vector<Elem> mat, int row, int col) {
    std::cout << "Printed entire matrix\n";
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            bool found = false;
            for (const auto& elem : mat) {
                if (elem.row == i && elem.col == j) {
                    std::cout << elem.val << " ";
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
}
void write_custom_to_file(std::string& filename) {
    std::string mat_line;
    std::vector<std::string> mat_lines;
    std::ofstream custom(filename);
    std::cin.get();
    while (std::getline(std::cin, mat_line)) {
        mat_lines.push_back(mat_line);
        if (mat_line == "-1") break;
    }
    mat_lines.pop_back();
    for (const std::string& line : mat_lines)
        custom << line << std::endl;
}
std::vector<Elem> read_custom_from_file(std::ifstream& mat_file, int row, int col) {
    std::vector<Elem> custom_mat;
    std::string temp_line;
    while (std::getline(mat_file, temp_line)) {
        std::istringstream mat_line(temp_line);
        int row = 0, col = 0, val = 0;
        mat_line >> row >> col >> val;
        custom_mat.push_back(Elem(row, col, val));
    }
    return custom_mat;
}
int main() {
    srand(time(0));
    std::vector<Elem> mat_1;
    int rows_1 = 0, columns_1 = 0, count_of_elems = 0, res_row = 0, res_col = 0, rows_2 = 0, columns_2 = 0, fullness = 0;
    std::cout << "\t\t\t\t\n\n"
        << "Two ways to create matrix:\n\n"
        << "1 - Generation by parameters\n"
        << "2 - Create by a given struct\n\n";

    int choice = 0;
    std::cin >> choice;
    std::string file_name;

    if (choice == 1) {
        std::cout << "Enter the dimension of first matrix separated by space: ";
        std::cin >> rows_1 >> columns_1;
        res_row = rows_1;
        count_of_elems = rows_1 * columns_1;
        std::cout << "Enter % of not null elements for  matrix: ";
        std::cin >> fullness;
        count_of_elems = count_of_elems * fullness / 100;
        mat_1 = generate_sparse_mat(rows_1, columns_1, count_of_elems);
        std::vector<Elem> temp;
        auto start1 = std::chrono::high_resolution_clock::now();
        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();


        std::cout << "\nMatrix: " << std::endl;
        print_sparse_mat(mat_1, rows_1, columns_1);
        int nonzero_count = 0;
        for (const Elem& elem : mat_1) {
            if (elem.val != 0) {
                nonzero_count++;
            }
        }

        // Вывод размерности разреженной матрицы
        int sparse_size = nonzero_count * 6;
        std::cout << "Sparse matrix size: " << sparse_size << " bytes" << std::endl;

       
    }

    
    if (choice == 2) {
        std::cout << "Enter the dimension of first matrix separated by space: ";
        std::cin >> rows_1 >> columns_1;
        res_row = rows_1;
        count_of_elems = rows_1 * columns_1;

        file_name = "custom_1.txt";
        std::cout << "Enter a value for first matrix separated by space on each line: \n";
        write_custom_to_file(file_name);
        std::ifstream custom1(file_name);
        mat_1 = read_custom_from_file(custom1, rows_1, columns_1);

        std::vector<Elem> temp;
        auto start2 = std::chrono::high_resolution_clock::now();
        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();
        std::cout << "Matrix: " << std::endl;
        print_sparse_mat(mat_1, rows_1, columns_1);
        int nonzero_count = 0;
        for (const Elem& elem : mat_1) {
            if (elem.val != 0) {
                nonzero_count++;
            }
        }

        // Вывод размерности разреженной матрицы
        int sparse_size = nonzero_count * 6;
        std::cout << "Sparse matrix size: " << sparse_size << " bytes" << std::endl;

        
    }
    int b;
    std::cout << "Enter number b:";
    std::cin >> b;
    rearrangeMatrix(mat_1, b);
    print_sparse_mat(mat_1, rows_1, columns_1);
    return 0;
}