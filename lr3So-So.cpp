// Задача 1. Даны две разреженные матрицы CS. Перемножить их и результат занести в разреженную матрицу CRS.

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
std::vector<Elem> mult(std::vector<Elem> mat_1, std::vector<Elem> mat_2) {
    std::vector<Elem> result;
    for (int i = 0; i < mat_1.size(); i++) {
        for (int j = 0; j < mat_2.size(); j++) {
            if (mat_1[i].col == mat_2[j].row)
                result.push_back(Elem(mat_1[i].row, mat_2[j].col, mat_1[i].val * mat_2[j].val));
        }
    }
    return result;
}
std::vector<Elem> mult_helper(std::vector<Elem> temp) {
    std::vector<Elem> result;
    size_t end = temp.size();
    for (int i = 0; i < end; i++) {
        for (int j = 0; j < end; j++) {
            if (temp[i].col == temp[j].col && temp[i].row == temp[j].row && i != j) {
                temp[i].val += temp[j].val;
                temp.erase(temp.begin() + j);
                end--;
                j--;
            }
        }
        result.push_back(temp[i]);
    }
    return result;
}
std::vector<std::vector<int>> convert_to_ccs_mat(std::vector<Elem> mat, int row, int col) {
    std::vector<std::vector<int>> ccs_mat(3);
    ccs_mat[0].reserve(mat.size());
    ccs_mat[1].reserve(mat.size());
    ccs_mat[2].resize(col + 1, 0);
    int cnt = 0;
    for (int j = 0; j < col; ++j) {
        for (int i = 0; i < row; ++i) {
            bool found = false;
            for (const auto& elem : mat) {
                if (elem.row == i && elem.col == j) {
                    ccs_mat[0].push_back(elem.val);
                    ccs_mat[1].push_back(elem.row);
                    found = true;
                    cnt++;
                    break;
                }
            }
        }
        ccs_mat[2][j + 1] = cnt;
    }
    return ccs_mat;
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
    std::cout << " Print entire matrix\n";
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
void write_ccs_to_file(std::string& filename, std::vector<std::vector<int>> ccs_mat) {
    std::ofstream custom(filename);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < ccs_mat[i].size(); j++)
            custom << ccs_mat[i][j] << " ";
        custom << "\n";
    }
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
void print_ccs_mat(std::vector<std::vector<int>> mat) {
    std::cout << "\nMatrix CCS:\n";
    std::cout << "Array of values: { ";
    for (int i = 0; i < mat[0].size(); i++)
        std::cout << mat[0][i] << " ";
    std::cout << "}\n";
    std::cout << "Array of row index: { ";
    for (int i = 0; i < mat[1].size(); i++)
        std::cout << mat[1][i] << " ";
    std::cout << "}\n";
    std::cout << "Array of column pointers: { ";
    for (int i = 0; i < mat[2].size(); i++)
        std::cout << mat[2][i] << " ";
    std::cout << "}\n";
}
int main() {
    srand(time(0));
    setlocale(LC_ALL, "ru");
    std::vector<Elem> mat_1;
    std::vector<Elem> mat_2;
    int rows_1 = 0, columns_1 = 0, count_of_elems = 0, res_row = 0, res_col = 0, rows_2 = 0, columns_2 = 0, fullness = 0;
    std::cout << "\t\t\t\t----------- Информация по программе -----------\n\n"
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
        std::cout << "Enter % of not null eements for first matrix: ";
        std::cin >> fullness;
        count_of_elems = count_of_elems * fullness / 100;
        mat_1 = generate_sparse_mat(rows_1, columns_1, count_of_elems);

        std::cout << "Enter the dimension of second matrix separated by space: ";
        std::cin >> rows_2 >> columns_2;
        res_col = columns_2;
        count_of_elems = rows_2 * columns_2;
        std::cout << "Enter % of not null eements for first matrix:: ";
        std::cin >> fullness;
        count_of_elems = count_of_elems * fullness / 100;
        mat_2 = generate_sparse_mat(rows_2, columns_2, count_of_elems);

        std::vector<Elem> temp;
        std::vector<Elem> result;
        auto start1 = std::chrono::high_resolution_clock::now();
        temp = mult(mat_1, mat_2);
        result = mult_helper(temp);
        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();


        int memoryCS1 = mat_1.size() * sizeof(Elem); //
        std::cout << "First Matrix: " << std::endl;
        std::cout << "Memory usage for the first matrix in CS format: " << memoryCS1 << " bytes" << std::endl;
        print_sparse_mat(mat_1, rows_1, columns_1);

        int memoryCS2 = mat_2.size() * sizeof(Elem); //
        std::cout << "\nSecond Matrix: " << std::endl;
        std::cout << "Memory usage for the second matrix in CS format: " << memoryCS2 << " bytes" << std::endl;
        print_sparse_mat(mat_2, rows_2, columns_2);
        std::cout << "\nFinish matrix: " << std::endl;
        print_sparse_mat(result, res_row, res_col);
        std::cout << "\nTime: " << duration1 << " мкс.\n";
        std::vector<std::vector<int>> ccs_result = convert_to_ccs_mat(result, res_row, res_col);
        int nnzResult = result.size(); // Количество ненулевых элементов в результирующей матрице
        int memoryCCS = nnzResult * (sizeof(int) * 2) + (res_col + 1) * sizeof(int); // Память для результирующей матрицы в формате 
        std::cout << "Memory usage for the result matrix in CCS format: " << memoryCCS << " bytes" << std::endl;
        print_ccs_mat(ccs_result);
        file_name = "ccs_res.txt";
        write_ccs_to_file(file_name, ccs_result);

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

        std::cout << "Enter the dimension of second matrix separated by space: ";
        std::cin >> rows_2 >> columns_2;
        res_col = columns_2;
        count_of_elems = rows_2 * columns_2;

        file_name = "custom_2.txt";
        std::cout << "Enter a value for second matrix separated by space on each line: \n";
        write_custom_to_file(file_name);
        std::ifstream custom2(file_name);
        mat_2 = read_custom_from_file(custom2, rows_2, columns_2);

        std::vector<Elem> temp;
        std::vector<Elem> result;
        auto start2 = std::chrono::high_resolution_clock::now();
        temp = mult(mat_1, mat_2);
        result = mult_helper(temp);
        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();
        
        int memoryCS1 = mat_1.size() * sizeof(Elem); //
        std::cout << "First Matrix: " << std::endl;
        std::cout << "Memory usage for the first matrix in CS format: " << memoryCS1 << " bytes" << std::endl;
        print_sparse_mat(mat_1, rows_1, columns_1);

        int memoryCS2 = mat_2.size() * sizeof(Elem); //
        std::cout << "\nSecond Matrix: " << std::endl;
        std::cout << "Memory usage for the second matrix in CS format: " << memoryCS2 << " bytes" << std::endl;
        print_sparse_mat(mat_2, rows_2, columns_2);
        std::cout << "\nFinal Matrix: " << std::endl;
        print_sparse_mat(result, res_row, res_col);
        std::cout << "\nTime: " << duration2 << " мкс.\n";
        std::vector<std::vector<int>> ccs_result = convert_to_ccs_mat(mat_1, res_row, res_col);
        int nnzResult = result.size(); // Количество ненулевых элементов в результирующей матрице
        int memoryCCS = nnzResult * (sizeof(int) * 2) + (res_col + 1) * sizeof(int); // Память для результирующей матрицы в формате CCS
        std::cout << "Memory usage for the result matrix in CCS format: " << memoryCCS << " bytes" << std::endl;
        print_ccs_mat(ccs_result);
        file_name = "ccs_res.txt";
        write_ccs_to_file(file_name, ccs_result);
    }
    return 0;
}