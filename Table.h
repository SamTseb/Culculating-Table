//
// Created by Sam on 27.04.2021.
//

#ifndef NOKIA_TEST_TASK_TABLE_H
#define NOKIA_TEST_TASK_TABLE_H

#include <string>
#include <vector>

using namespace std;


class Table {
private:
    vector<vector<string>> table_array;
    vector<string> exe_chain;

    void extract(char* path);
    vector<string> proccess_line(string& str);
    int execute(string& elem);
    void validation_elem(int i, int j);
    void validation();
    int find_cell(string elem);
    int vertical_search(string& elem);
    int operation(char op, string &elem, int pos);
public:
    Table(char* path);
    void print_done_table();
};


#endif //NOKIA_TEST_TASK_TABLE_H
