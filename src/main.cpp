#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Matrix {
public:
    vector<vector<int>> data;

    Matrix() = default;

    Matrix(const vector<vector<int>>& input_data) : data(input_data) {}

    int row_size() const { return data.size(); }
    int column_size() const { return data.empty() ? 0 : data[0].size(); }

    int at(int row, int column) const {
        if (row < 0 || row >= row_size() || column < 0 || column >= column_size())
            throw out_of_range("Index out of range");
        return data[row][column];
    }
    void set(int row, int column, int value) {
        if (row < 0 || row >= row_size() || column < 0 || column >= column_size())
            throw out_of_range("Index out of range");
        data[row][column] = value;
    }
    
    void show_matrix() const {
        for (const auto& row : data) {
            for (const auto& value : row) {
                cout << value << " ";
            }
            cout << endl;
        }
    }
};

vector<vector<int>> read_data(const string& filename) {
    ifstream file(filename);
    if (!file)
        throw runtime_error("Error opening file: " + filename);

    vector<vector<int>> matrix_data;
    string line;

    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        int value;
        while (ss >> value) {
            row.push_back(value);
        }
        if (!row.empty())
            matrix_data.push_back(row);
    }

    return matrix_data;
}


void matrix_multiply(const Matrix& a, const Matrix& b, Matrix& result) {
    if (a.column_size() != b.row_size())
        throw invalid_argument("Matrix dimensions do not match for multiplication");

    result.data.resize(a.row_size(), vector<int>(b.column_size(), 0));

    for (int i = 0; i < a.row_size(); ++i) {
        for (int j = 0; j < b.column_size(); ++j) {
            for (int k = 0; k < a.column_size(); ++k) {
                result.data[i][j] += a.at(i, k) * b.at(k, j);
            }
        }
    }
}


int main(int argc, char** argv) {

    Matrix matrixA, matrixB, matrixC;

    try {
        string path_to_A = argv[1];
        string path_to_B = argv[2];
        matrixA = Matrix(read_data(path_to_A));
        matrixB = Matrix(read_data(path_to_B));
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    
    try {
        matrix_multiply(matrixA, matrixB, matrixC);
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    cout << "Resultant Matrix:" << endl;
    matrixC.show_matrix();

    return 0;
}
