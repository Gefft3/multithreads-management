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

void calculate_row(const Matrix& a, const Matrix& b, Matrix& result, int row) {
    for (int j = 0; j < b.column_size(); ++j) {
        int sum = 0;
        for (int k = 0; k < a.column_size(); ++k) {
            sum += a.at(row, k) * b.at(k, j);
        }
        result.set(row, j, sum);
    }
}

void matrix_multiply_parallel(const Matrix& a, const Matrix& b, Matrix& result) {
    if (a.column_size() != b.row_size())
        throw invalid_argument("Matrix dimensions do not match for multiplication");

    result.data.resize(a.row_size(), vector<int>(b.column_size(), 0));

    vector<thread> threads;

    for (int i = 0; i < a.row_size(); ++i) {
        threads.emplace_back(calculate_row, cref(a), cref(b), ref(result), i);
    }

    for (auto& t : threads) {
        t.join();
    }
}

void matrix_multiply_serial(const Matrix& a, const Matrix& b, Matrix& result) {
    if (a.column_size() != b.row_size())
        throw invalid_argument("Matrix dimensions do not match for multiplication");

    result.data.resize(a.row_size(), vector<int>(b.column_size(), 0));

    for (int i = 0; i < a.row_size(); ++i) {
        for (int j = 0; j < b.column_size(); ++j) {
            calculate_element(a, b, result, i, j);
        }
    }
}

void write_statistics(
    const string& filename, 
    duration<double> duration_serial,
    duration<double> duration_parallel, 
    const Matrix& matrixA, 
    const Matrix& matrixB, 
    const Matrix& matrixC) 
{

    ofstream stats_file(filename);
    if (!stats_file)
        throw runtime_error("Error opening statistics file: " + filename);

    stats_file << "Matrix A size: " << matrixA.row_size() << "x" << matrixA.column_size() << endl;
    stats_file << "Matrix B size: " << matrixB.row_size() << "x" << matrixB.column_size() << endl;
    stats_file << "Result Matrix size: " << matrixC.row_size() << "x" << matrixC.column_size() << endl;
    stats_file << "teste" << endl;
    stats_file << "Time taken for serial multiplication: " << duration_serial.count() << " microseconds" << endl;
    stats_file << "Time taken for parallel multiplication: " << duration_parallel.count() << " microseconds" << endl;
    
    double speedup = duration_serial.count() / duration_parallel.count();
    stats_file << "Speedup: " << speedup << "x" << endl;
    
    stats_file.close();
}


int main(int argc, char** argv) {
    
    if (argc != 7) {
        cerr << "Usage: " << argv[0] << " <matrixA_file> <matrixB_file> <matrixC_file> <generate_output> <statistics_file> <method>" << endl;
        return 1;
    }
    
    Matrix matrixA, matrixB, matrixC;
    
    try {
        matrixA = Matrix(read_data(argv[1]));
        matrixB = Matrix(read_data(argv[2]));
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    
    duration<double> var_duration_serial, var_duration_parallel; 
    
    auto start_serial = high_resolution_clock::now();
    matrix_multiply_serial(matrixA, matrixB, matrixC);
    auto end_serial = high_resolution_clock::now();
    var_duration_serial = end_serial - start_serial; 
    cout << "Serial multiplication completed in " << var_duration_serial.count() << " microseconds." << endl;

    auto start_parallel = high_resolution_clock::now();
    matrix_multiply_parallel(matrixA, matrixB, matrixC);
    auto end_parallel = high_resolution_clock::now();
    var_duration_parallel = end_parallel - start_parallel; 
    cout << "Parallel multiplication completed in " << var_duration_parallel.count() << " microseconds." << endl;

    // cout << "Resultant Matrix:" << endl;
    // matrixC.print();

    string filenameC = argv[3];
    bool generate_output = (string(argv[4]) == "true");
    string statistics_file = argv[5];

    if (generate_output) {
        try {
            matrixC.write_to_file(filenameC);
            cout << "Result matrix written to " << filenameC << endl;
        } catch (const exception& e) {
            cerr << e.what() << endl;
            return 1;
        }

        try {
            write_statistics(statistics_file, var_duration_serial, var_duration_parallel, matrixA, matrixB, matrixC);
            cout << "Statistics written to " << statistics_file << endl;
        } catch (const exception& e) {
            cerr << e.what() << endl;
            return 1;
        }
    } 

    cout << "Matrices released successfully." << endl;

    return 0;
}
