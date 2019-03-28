#include <iostream>

using namespace std;

class Matrix {
public:
    class MatrixException: exception {
    private:
        char *message;
    public:
        MatrixException(const char *_message);
        MatrixException(const MatrixException& exception);
        ~MatrixException();
        char* getMessage() const;
    };
private:
    static const double EPS;

    int cols, lines;
    double **data;

    void init(int n, int m);
    void deinit();
public:
    Matrix();
    Matrix(int n, int m);
    Matrix(double d);
    Matrix(double *row, int m);
    Matrix(int n, double *col);
    Matrix(const char *str);
    Matrix(const Matrix& other);
    ~Matrix();

    static Matrix identity(int n);
    static Matrix diagonal(double *vals, int n);

    int rows() const;
    int columns() const;
    void set(int i, int j, double val);
    double get(int i, int j);

    Matrix& operator=(const Matrix& other);
    Matrix operator[](int val) const;
    Matrix& operator*=(double scalar);
    Matrix operator*(double scalar);
    Matrix operator+(const Matrix& other) const;
    Matrix& operator+=(const Matrix& other);
    Matrix operator-(const Matrix& other) const;
    Matrix& operator-=(const Matrix& other);
    Matrix operator*(const Matrix& other) const;
    Matrix& operator*=(const Matrix& other);
    Matrix operator-() const;
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    Matrix operator|(const Matrix& other) const;
    Matrix operator/(const Matrix& other) const;
    friend ostream& operator<< (ostream& stream, const Matrix& matrix);
};
