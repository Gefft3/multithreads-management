#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Matrix {
public:
    vector<vector<int>> data;

    explicit Matrix(const vector<vector<int>>& input_data) : data(input_data) {}

    int row_size() const { return data.size(); }
    int column_size() const { return data.empty() ? 0 : data[0].size(); }

    int at(int row, int column) const {
        if (row < 0 || row >= row_size() || column < 0 || column >= column_size())
            throw out_of_range("Index out of range");
        return data[row][column];
    }
};

vector<vector<int>> read_data(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> matrix_data;
    string line;

    if (!file)
        throw runtime_error("Error opening file: " + filename);

    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(stoi(value));
        }
        matrix_data.push_back(row);
    }
    return matrix_data;
}

int main(int argc, char** argv) {

    try {
        Matrix matrix1(read_data(argv[1]));
        Matrix matrix2(read_data(argv[2]));
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
