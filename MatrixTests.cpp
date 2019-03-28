#include "MatrixTests.h"
#include "Matrix.h"

void assert(int condition, const char *error_message) {
    if (!condition) {
        throw error_message;
    }
}

void default_constructor_test() {
    cout << "Testing default constructor" << endl;
    Matrix m;
    assert(m.columns() == 1, "Wrong columns created in default constructor!");
    assert(m.rows() == 1, "Wrong rows created in default constructor!");
    assert(m.get(1, 1) == 0.0, "Wrong cell value in default constructor!");
}

void size_constructor_test() {
    cout << "Testing size constructor" << endl;
    Matrix m(5, 10);
    assert(m.columns() == 10, "Wrong columns created in size constructor!");
    assert(m.rows() == 5, "Wrong rows created in size constructor!");
}

void vector_constructor_test() {
    cout << "Testing vector constructor" << endl;
    double t[6] = {1, 3, 6, 7, 8, 9};
    Matrix m1(t, 6);
    assert(m1.columns() == 6, "Wrong columns created in row constructor!");
    assert(m1.rows() == 1, "Wrong rows created in row constructor!");
    for (int i = 0; i < m1.columns(); ++i) {
        assert(m1.get(1, i + 1) == t[i], "Wrong value set in row constructor!");
    }
    Matrix m2(6, t);
    assert(m2.columns() == 1, "Wrong columns created in column constructor!");
    assert(m2.rows() == 6, "Wrong rows created in column constructor!");
    for (int i = 0; i < m2.rows(); ++i) {
        assert(m2.get(i + 1, 1) == t[i], "Wrong value set in column constructor!");
    }
}

void string_constructor_test() {
    cout << "Testing string constructor" << endl;
    Matrix m1("{ {1, 0, 0}, {0, 1, 0.5}}");
    assert(m1.columns() == 3, "Wrong columns created in string constructor!");
    assert(m1.rows() == 2, "Wrong rows created in string constructor!");
    assert(m1.get(1, 1) == 1.0, "Wrong value set in string constructor!");
    assert(m1.get(2, 3) == 0.5, "Wrong value set in string constructor!");
    Matrix m2("{    {   1   , 0,    0    }, {0, 1, 0.5}, {2, 1, 0.5}, {3.5, 1.7, 0.5}}");
    assert(m2.columns() == 3, "Wrong columns created in string constructor!");
    assert(m2.rows() == 4, "Wrong rows created in string constructor!");
    try {
        Matrix m3(" {1, 0, 0}, {0, 1, 0.5}}");
        assert(false, "Missing { was not detected in string constructor!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        Matrix m4("{ {1, 0, 0}, 0, 1, 0.5}}");
        assert(false, "Missing { was not detected in string constructor!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        Matrix m5("{ {1, 0 0}, {0, 1, 0.5}}");
        assert(false, "Missing , was not detected in string constructor!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        Matrix m6("{ {1, 0 0} {0, 1, 0.5}}");
        assert(false, "Missing , was not detected in string constructor!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        Matrix m7("{ {1, 0, 0, {0, 1, 0.5}}");
        assert(false, "Missing } was not detected in string constructor!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        Matrix m8("{ {1, 0, 0}, {0, 1, 0.5}");
        assert(false, "Missing } was not detected in string constructor!");
    } catch (Matrix::MatrixException e) {
    }
}

void static_methods_test() {
    cout << "Testing static methods" << endl;
    Matrix m1 = Matrix::identity(3);
    assert(m1.columns() == 3, "Wrong columns created in identity!");
    assert(m1.rows() == 3, "Wrong rows created in identity!");
    for (int i = 0; i < m1.rows(); ++i) {
        for (int j = 0; j < m1.columns(); ++j) {
            assert(m1.get(i + 1, j + 1) == (i == j ? 1.0 : 0.0), "Wrong value set in identity!");
        }
    }
    double t[6] = {1, 3, 6, 7, 8, 9};
    Matrix m2 = Matrix::diagonal(t, 6);
    assert(m2.columns() == 6, "Wrong columns created in diagonal!");
    assert(m2.rows() == 6, "Wrong rows created in diagonal!");
    for (int i = 0; i < m2.rows(); ++i) {
        for (int j = 0; j < m2.columns(); ++j) {
            assert(m2.get(i + 1, j + 1) == (i == j ? t[i] : 0.0), "Wrong value set in diagonal!");
        }
    }
}

void set_get_functions_test() {
    cout << "Testing set and get functions" << endl;
    Matrix m(5, 10);
    m.set(4, 8, 4);
    assert(m.get(4, 8) == 4.0, "Wrong value set or get!");
    try {
        m.set(4, 18, 4);
        assert(false, "Exception in set was not raised!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        m.get(4, 18);
        assert(false, "Exception in get was not raised!");
    } catch (Matrix::MatrixException e) {
    }
}

void copy_operator_test() {
    cout << "Testing copy operator" << endl;
    Matrix m1 = Matrix::identity(3);
    Matrix m2(4, 4);
    m2 = m1;
    assert(m2.columns() == m1.columns(), "Wrong columns after copy!");
    assert(m2.rows() == m1.rows(), "Wrong rows after copy!");
    for (int i = 0; i < m2.rows(); ++i) {
        for (int j = 0; j < m2.columns(); ++j) {
            assert(m2.get(i + 1, j + 1) == m1.get(i + 1, j + 1), "Wrong value after copy!");
        }
    }
}

void index_operator_test() {
    cout << "Testing index operator" << endl;
    Matrix m(4, 7);
    m.set(3, 6, 9);
    assert(m[3].get(1, 6) == 9.0, "Wrong index operator for a row!");
    assert(m[6].get(3, 1) == 9.0, "Wrong index operator for a column!");
    assert(m[3][6].get(1, 1) == 9.0, "Wrong index operator for a group!");
    assert(m[6][3].get(1, 1) == 9.0, "Wrong index operator for a group!");
    try {
        cout << m[10];
        assert(false, "Exception in index operator was not raised!");
    } catch (Matrix::MatrixException e) {
    }
}

void scalar_multiplication_test() {
    cout << "Testing scalar multiplication" << endl;
    Matrix m1("{ {1, 0, 3.6}, {0, 1, 0.5}}"), m2;
    m2 = m1 * 3.4;
    for (int i = 0; i < m2.rows(); ++i) {
        for (int j = 0; j < m2.columns(); ++j) {
            assert(m2.get(i + 1, j + 1) == m1.get(i + 1, j + 1) * 3.4, "Wrong value after scalar multiplication!");
        }
    }
    m2 = m1;
    m2 *= 2.3;
    for (int i = 0; i < m2.rows(); ++i) {
        for (int j = 0; j < m2.columns(); ++j) {
            assert(m2.get(i + 1, j + 1) == m1.get(i + 1, j + 1) * 2.3, "Wrong value after scalar multiplication!");
        }
    }
}

void default_tests_basic() {
    default_constructor_test();
    size_constructor_test();
    vector_constructor_test();
    string_constructor_test();
    static_methods_test();
    set_get_functions_test();
    copy_operator_test();
    index_operator_test();
    scalar_multiplication_test();
}

void addition_operator_test() {
    cout << "Testing addition operator" << endl;
    Matrix m1("{ {1, 0, 0}, {0, 1, 0.5} }");
    Matrix m2("{ {10.2, 2.3, 0}, {7.8, 1, 0.5} }");
    Matrix m3(4, 4);
    try {
        cout << m1 + m3;
        assert(false, "Incompatible sizes exception was not raised in addition operator!");
    } catch (Matrix::MatrixException e) {
    }
    m3 = m2 + m1;
    for (int i = 0; i < m3.rows(); ++i) {
        for (int j = 0; j < m3.columns(); ++j) {
            assert(m3.get(i + 1, j + 1) == m1.get(i + 1, j + 1) + m2.get(i + 1, j + 1), "Wrong value after addition!");
        }
    }
    m2 += m1;
    for (int i = 0; i < m3.rows(); ++i) {
        for (int j = 0; j < m3.columns(); ++j) {
            assert(m3.get(i + 1, j + 1) == m2.get(i + 1, j + 1), "Wrong value after self-addition!");
        }
    }
}

void substraction_operator_test() {
    cout << "Testing substraction operator" << endl;
    Matrix m1("{ {1, 0, 0}, {0, 1, 0.5} }");
    Matrix m2("{ {10.2, 2.3, 0}, {7.8, 1, 0.5} }");
    Matrix m3(4, 4);
    try {
        cout << m1 - m3;
        assert(false, "Incompatible sizes exception was not raised in substraction operator!");
    } catch (Matrix::MatrixException e) {
    }
    m3 = m2 - m1;
    for (int i = 0; i < m3.rows(); ++i) {
        for (int j = 0; j < m3.columns(); ++j) {
            assert(m3.get(i + 1, j + 1) == m2.get(i + 1, j + 1) - m1.get(i + 1, j + 1), "Wrong value after substraction!");
        }
    }
    (m3 += m1) -= m1;
    for (int i = 0; i < m3.rows(); ++i) {
        for (int j = 0; j < m3.columns(); ++j) {
            assert(m3.get(i + 1, j + 1) == m2.get(i + 1, j + 1) - m1.get(i + 1, j + 1), "Wrong value after self-substraction!");
        }
    }
}

void multiplication_operator_test() {
    cout << "Testing multiplication operator" << endl;
    Matrix m1("{ {2, 0, 4, -1}, {1, -1, 1, 0} }");
    Matrix m2("{ {2}, {1}, {0}, {-2} }");
    Matrix m3("{ {-1, 1}, {2, 0}, {0, 3} }");
    Matrix m4("{ {3, 1, 2}, {0, -1, 4} }");
    Matrix m5, m6, r1("{{6}, {1}}"), r2("{{-3, -2, 2}, {6, 2, 4}, {0, -3, 12}}"), r3("{{-1, 9}, {-2, 12}}");
    try {
        cout << m2 * m1;
        assert(false, "Incompatible sizes exception was not raised in multiplication operator!");
    } catch (Matrix::MatrixException e) {
    }
    m5 = m1 * m2;
    for (int i = 0; i < m5.rows(); ++i) {
        for (int j = 0; j < m5.columns(); ++j) {
            assert(m5.get(i + 1, j + 1) == r1.get(i + 1, j + 1), "Wrong value after M x N * N x K multiplication!");
        }
    }
    m5 = m1;
    m5 *= m2;
    for (int i = 0; i < m5.rows(); ++i) {
        for (int j = 0; j < m5.columns(); ++j) {
            assert(m5.get(i + 1, j + 1) == r1.get(i + 1, j + 1), "Wrong value after self-multiplication!");
        }
    }
    m6 = m3 * m4;
    for (int i = 0; i < m6.rows(); ++i) {
        for (int j = 0; j < m6.columns(); ++j) {
            assert(m6.get(i + 1, j + 1) == r2.get(i + 1, j + 1), "Wrong value after M x N * N x M multiplication!");
        }
    }
    m6 = m4 * m3;
    for (int i = 0; i < m6.rows(); ++i) {
        for (int j = 0; j < m6.columns(); ++j) {
            assert(m6.get(i + 1, j + 1) == r3.get(i + 1, j + 1), "Wrong value after N x M * M x N multiplication!");
        }
    }
}

void unary_operator_test() {
    cout << "Testing unary operator" << endl;
    Matrix m1("{ {1, 0, 0}, {0, 1, 0.5} }");
    Matrix m2(4, 4);
    m2 = -m1;
    for (int i = 0; i < m2.rows(); ++i) {
        for (int j = 0; j < m2.columns(); ++j) {
            assert(m2.get(i + 1, j + 1) == -m1.get(i + 1, j + 1), "Wrong value after unary operator!");
        }
    }
}

void equal_operator_test() {
    cout << "Testing equal operator" << endl;
    Matrix m1("{ {1, 0, 0}, {0, 1, 0.5} }");
    Matrix m2("{ {10.2, 2.3, 0}, {7.8, 1, 0.5} }");
    Matrix m3(4, 4);
    try {
        cout << (m3 == m1);
        assert(false, "Incompatible sizes exception was not raised in equal operator!");
    } catch (Matrix::MatrixException e) {
    }
    assert(m2 != m1, "Equal for unequal matrices!");
    assert(m1 == m1, "Unequal for equal matrices!");
}

void concatenation_operator_test() {
    cout << "Testing concatenation operator" << endl;
    Matrix m1("{ {2, 0, 1}, {1, -1, 1} }");
    Matrix m2("{ {1, 1, 1}}");
    Matrix m3("{ {-1}, {2}}");
    Matrix m4("{ {3, 1}, {-1, 4} }");
    Matrix m5, m6, r1("{{1, 1, 1}, {2, 0, 1}, {1, -1, 1}}"), r2("{{-1, 3, 1}, {2, -1, 4}}");
    try {
        cout << (m1 | m2);
        assert(false, "Incompatible sizes exception was not raised in vertical concatenation operator!");
    } catch (Matrix::MatrixException e) {
    }
    try {
        cout << (m3 / m4);
        assert(false, "Incompatible sizes exception was not raised in horizontal concatenation operator!");
    } catch (Matrix::MatrixException e) {
    }
    m5 = m2 / m1;
    assert(m5 == r1, "Wrong value after horizontal concatenation!");
    m6 = m3 | m4;
    assert(m6 == r2, "Wrong value after vertical concatenation!");
}

void default_tests_advanced() {
    addition_operator_test();
    substraction_operator_test();
    multiplication_operator_test();
    unary_operator_test();
    equal_operator_test();
    concatenation_operator_test();
}