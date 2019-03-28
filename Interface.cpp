#include "Matrix.h"
#include "MatrixTests.h"
#include "Interface.h"
#include "string.h"
#include <sstream>

#define DEFAULT_BUFLEN 100

using namespace std;

void Interface::print_help() {
    cout << "Type the mode you want to launch:" << endl;
    cout << "1 - launch predefined tests" << endl;
    cout << "2 - launch matrix addition" << endl;
}

bool Interface::line_is_empty() {
    if (!current_line) return true;

    bool result = true;
    int i = 0;
    while (result && current_line[i] != '\0') {
        if (!isspace(current_line[i])) {
            result = false;
        }
        i++;
    }
    return result;
}

char* Interface::read_line() {
    int i = 0;
    int len = DEFAULT_BUFLEN;
    char *buffer = new char[len], *tmp;
    char c = getchar();
    while (c != '\n' && c != EOF) {
        buffer[i] = c;
        i++;
        if (i == len - 1 /* -1 for \0 */) {
            len += DEFAULT_BUFLEN;
            tmp = buffer;
            buffer = new char[len];
            for (int j = 0; j < i; ++j) {
                buffer[j] = tmp[j];
            }
            delete[] tmp;
        }
        c = getchar();
    }
    buffer[i] = '\0';
    return buffer;
}

void Interface::run_default_tests() {
    default_tests_basic();
    cout << endl;
    default_tests_advanced();
    cout << endl;
    cout << "No unhandled exceptions! That\'s good!" << endl;
}

void Interface::run_addition() {
    cout << "Enter matrix 1 as the standard string ( {{0, 1}, {1, 0}} )" << endl;
    char *str1 = read_line();
    Matrix m1(str1);
    cout << "Enter matrix 2 as the standard string ( {{0, 1}, {1, 0}} )" << endl;
    char *str2 = read_line();
    Matrix m2(str2);
    cout << "The sum is:" << endl << m1 + m2;
}

bool Interface::make_line() {
    cout << "> ";

    if (current_line) delete[] current_line;
    current_line = read_line();
    if (line_is_empty()) return true;
    if (strcmp(current_line, "exit") == 0) return false;

    if (strcmp(current_line, "help") == 0) {
        print_help();
    } else {
        int mode = 0;
        stringstream ss(current_line);
        ss >> mode;
        try {
            switch (mode) {
            case 1:
                run_default_tests();
                break;
            case 2:
                run_addition();
                break;
            default:
                cout << "Unimplemented mode: " << mode << endl;
                print_help();
                break;
            }
        } catch (const char *s) {
            cout << s << endl;
            cout << "An exception has been thrown. Closing the mode..." << endl;
        } catch (const Matrix::MatrixException& e) {
            cout << e.getMessage() << endl;
            cout << "An exception has been thrown. Closing the mode..." << endl;
        }
    }
    return true;
}

int main() {
    Interface in;
    while (in.make_line());
}