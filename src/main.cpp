#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;
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
    
    void print() const {
        for (const auto& row : data) {
            for (const auto& value : row) {
                cout << value << ' ';
            }
            cout << '\n';
        }
    }

    void write_to_file(const string& filename) const {
        ofstream file(filename);
        if (!file)
            throw runtime_error("Error opening file: " + filename);

        for (const auto& row : data) {
            for (const auto& value : row) {
                file << value << ' ';
            }
            file << '\n';
        }
    }

    void release() {
        data.clear();
    }

    ~Matrix() {
        release();
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


void calculate_element(const Matrix& a, const Matrix& b, Matrix& result, int i, int j) {
    int sum = 0;
    for (int k = 0; k < a.column_size(); ++k) {
        sum += a.at(i, k) * b.at(k, j);
    }
    result.set(i, j, sum);
}

void matrix_multiply(const Matrix& a, const Matrix& b, Matrix& result) {
    if (a.column_size() != b.row_size())
        throw invalid_argument("Matrix dimensions do not match for multiplication");

    result.data.resize(a.row_size(), vector<int>(b.column_size(), 0));

    vector<thread> threads;

    for (int i = 0; i < a.row_size(); ++i) {
        for (int j = 0; j < b.column_size(); ++j) {
            threads.emplace_back(calculate_element, cref(a), cref(b), ref(result), i, j);
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}


int main(int argc, char** argv) {
    
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <matrixA_file> <matrixB_file> <result_file>" << endl;
        return 1;
    }

    string filenameC = argv[3];
    
    Matrix matrixA, matrixB, matrixC;
    
    try {
        matrixA = Matrix(read_data(argv[1]));
        matrixB = Matrix(read_data(argv[2]));
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
    matrixC.print();

    try {
        matrixC.write_to_file(filenameC);
        cout << "Result matrix written to " << filenameC << endl;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    cout << "Matrices released successfully." << endl;

    return 0;
}
