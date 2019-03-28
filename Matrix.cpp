#define _CRT_SECURE_NO_WARNINGS

#include "Matrix.h"
#include "string.h"
#include <sstream>

const double Matrix::EPS = 0.1;

Matrix::MatrixException::MatrixException(const char *_message) {
    message = new char[strlen(_message) + 1];
    strcpy(message, _message);
}

Matrix::MatrixException::MatrixException(const MatrixException& exception) {
    message = new char[strlen(exception.message) + 1];
    strcpy(message, exception.message);
}

Matrix::MatrixException::~MatrixException() {
    if (message) {
        delete[] message;
    }
    message = NULL;
}

char* Matrix::MatrixException::getMessage() const {
    return message;
}

Matrix::Matrix() {
    init(1, 1);
}

Matrix::Matrix(int n, int m) {
    init(n, m);
}

Matrix::Matrix(double d) {
    init(1, 1);
    set(1, 1, d);
}

Matrix::Matrix(double *row, int m) {
    init(1, m);
    for (int i = 0; i < m; ++i) {
        data[0][i] = row[i];
    }
}

Matrix::Matrix(int n, double *col) {
    init(n, 1);
    for (int i = 0; i < n; ++i) {
        data[i][0] = col[i];
    }
}

Matrix::Matrix(const char *str) {
    int n = 0, m = 0, i = 0;
    while (str[i] != '\0') {
        if (str[i] == '{') n++;
        if (str[i] == ',') m++;
        i++;
    }
    n -= 1;
    if (n == 0) n = 1; // just to handle a divizion by zero exception
    m = (m - n + 1) / n + 1;
    init(n, m);
    char tmp;
    stringstream ss(str);
    ss >> skipws >> tmp;
    if (tmp != '{') throw MatrixException("Missing first { in matrix string format");
    tmp = '\0';
    for (int i = 0; i < n; ++i) {
        ss >> tmp;
        if (tmp != '{') throw MatrixException("Missing open line { in matrix string format");
        for (int j = 0; j < m; ++j) {
            ss >> data[i][j];
            if (j < m - 1) {
                ss >> tmp;
                if (tmp != ',') throw MatrixException("Missing , between numbers in line in matrix string format");
            }
        }
        ss >> tmp;
        if (tmp != '}') throw MatrixException("Missing close line } in matrix string format");
        if (i < n - 1) {
            ss >> tmp;
            if (tmp != ',') throw MatrixException("Missing , between matrix lines in matrix string format");
        }
    }
    tmp = '\0';
    ss >> tmp;
    if (tmp != '}') throw MatrixException("Missing last } in matrix string format");
}

Matrix::Matrix(const Matrix& other) {
    init(other.lines, other.cols);
    for (int i = 0; i < other.lines; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

void Matrix::init(int n, int m) {
    lines = n;
    cols = m;
    data = new double*[n];
    for (int i = 0; i < n; ++i) {
        data[i] = new double[m];
        for (int j = 0; j < m; ++j) {
            data[i][j] = 0.0;
        }
    }
}

void Matrix::deinit() {
    if (data) {
        for (int i = 0; i < lines; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
    data = NULL;
}

Matrix::~Matrix() {
    deinit();
}

Matrix Matrix::identity(int n) {
    Matrix result(n, n);
    for (int i = 0; i < n; ++i) {
        result.data[i][i] = 1.0;
    }
    return result;
}

Matrix Matrix::diagonal(double *vals, int n) {
    Matrix result(n, n);
    for (int i = 0; i < n; ++i) {
        result.data[i][i] = vals[i];
    }
    return result;
}

int Matrix::rows() const {
    return lines;
}

int Matrix::columns() const {
    return cols;
}

void Matrix::set(int i, int j, double val) {
    if (i > lines || j > cols) {
        stringstream ss;
        ss << "Attempting to set (" << i << ',' << j << ") element for (" << lines << ',' << cols << ") matrix!";
        throw MatrixException(ss.str().c_str());
    }
    data[i - 1][j - 1] = val;
}

double Matrix::get(int i, int j) {
    if (i > lines || j > cols) {
        stringstream ss;
        ss << "Attempting to get (" << i << ',' << j << ") element from (" << lines << ',' << cols << ") matrix!";
        throw MatrixException(ss.str().c_str());
    }
    return data[i - 1][j - 1];
}

Matrix& Matrix::operator=(const Matrix& other) {
    deinit();
    init(other.lines, other.cols);
    for (int i = 0; i < other.lines; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator[](int val) const {
    if (val <= lines) {
        Matrix result(1, cols);
        for (int i = 0; i < cols; ++i) {
            result.data[0][i] = data[val - 1][i];
        }
        return result;
    } else if (val <= cols) {
        Matrix result(lines, 1);
        for (int i = 0; i < lines; ++i) {
            result.data[i][0] = data[i][val - 1];
        }
        return result;
    } else {
        stringstream ss;
        ss << "Attempting to get " << val << "th line from (" << lines << ',' << cols << ") matrix!";
        throw MatrixException(ss.str().c_str());
    }
}

Matrix& Matrix::operator*=(double scalar) {
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix Matrix::operator*(double scalar) {
    Matrix result(*this);
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] *= scalar;
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& other) const {
    Matrix result(*this);
    result += other;
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (lines != other.lines || cols != other.cols) {
        stringstream ss;
        ss << "Attempting to add (" << lines << ',' << cols << ") and (" << other.lines << ',' << other.cols << ") matrices!";
        throw MatrixException(ss.str().c_str());
    }

    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator-(const Matrix& other) const {
    Matrix result(*this);
    result -= other;
    return result;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (lines != other.lines || cols != other.cols) {
        stringstream ss;
        ss << "Attempting to substract (" << lines << ',' << cols << ") and (" << other.lines << ',' << other.cols << ") matrices!";
        throw MatrixException(ss.str().c_str());
    }

    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix& other) const {
    Matrix result(*this);
    result *= other;
    return result;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (cols != other.lines) {
        stringstream ss;
        ss << "Attempting to multiply (" << lines << ',' << cols << ") and (" << other.lines << ',' << other.cols << ") matrices!";
        throw MatrixException(ss.str().c_str());
    }

    Matrix tmp(lines, other.cols);
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                tmp.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    *this = tmp;
    return *this;
}

Matrix Matrix::operator-() const {
    Matrix result(*this);
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = -data[i][j];
        }
    }
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if (lines != other.lines || cols != other.cols) {
        stringstream ss;
        ss << "Attempting to compare (" << lines << ',' << cols << ") and (" << other.lines << ',' << other.cols << ") matrices!";
        throw MatrixException(ss.str().c_str());
    }

    bool result = true;
    for (int i = 0; (i < lines && result); ++i) {
        for (int j = 0; j < cols; ++j) {
            if (abs(data[i][j] - other.data[i][j]) > EPS) {
                result = false;
                break;
            }
        }
    }
    return result;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

Matrix Matrix::operator|(const Matrix& other) const {
    if (lines != other.lines) {
        stringstream ss;
        ss << "Attempting to vertically concatenate (" << lines << ',' << cols << ") and (" << other.lines << ',' << other.cols << ") matrices!";
        throw MatrixException(ss.str().c_str());
    }
    Matrix result(lines, cols + other.cols);
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j];
        }
    }
    for (int i = 0; i < other.lines; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            result.data[i][j + cols] = other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator/(const Matrix& other) const {
    if (cols != other.cols) {
        stringstream ss;
        ss << "Attempting to horizontally concatenate (" << lines << ',' << cols << ") and (" << other.lines << ',' << other.cols << ") matrices!";
        throw MatrixException(ss.str().c_str());
    }
    Matrix result(lines + other.lines, cols);
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j];
        }
    }
    for (int i = 0; i < other.lines; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            result.data[i + lines][j] = other.data[i][j];
        }
    }
    return result;
}

ostream& operator <<(ostream& stream, const Matrix& matrix) {
    auto old_precision = stream.precision(1);
    auto old_flags = stream.flags();
    stream << fixed;
    stream << endl;
    for (int i = 0; i < matrix.lines; ++i) {
        stream << "|  ";
        for (int j = 0; j < matrix.cols; ++j) {
            stream << matrix.data[i][j] << "  ";
        }
        stream << "|" << endl;
    }
    stream << endl;
    stream.precision(old_precision);
    stream.flags(old_flags);
    return stream;
}