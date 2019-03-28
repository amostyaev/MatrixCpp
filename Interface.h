class Interface {
private:
    char *current_line;

    void print_help();
    bool line_is_empty();
    char* read_line();

    void run_default_tests();
    void run_addition();
public:
    Interface() : current_line(NULL) {};
    ~Interface() {
        if (current_line) {
            delete[] current_line;
        }
    }

    bool make_line();
};
