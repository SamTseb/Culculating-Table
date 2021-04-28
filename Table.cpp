//
// Created by Sam on 27.04.2021.
//

#include "Table.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>


#ifdef WIN32
#define EXTRA_CH 0
#endif

#ifdef __linux__
#define EXTRA_CH 1
#endif

using namespace std;

Table::Table(char *path) {
    extract(path);
    validation();
}

void Table::extract(char *path) {
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("File is not open.");
    }
    string line;
    string word;
    vector<string> temp_vector;

    if (file.peek() != EOF) {
        while (getline(file, line)) {
            table_array.push_back(proccess_line(line));
        }
    } else
        throw runtime_error("File is empty.");;
}

void Table::print_done_table() {
    if (table_array.size() != 0) {
        for (int i = 0; i < table_array[0].size(); i++)
            cout << table_array[0][i] << '\t';
        cout << '\n';

        for (int i = 1; i < table_array.size(); i++) {
            cout << table_array[i][0] << '\t';
            for (int j = 1; j < table_array[i].size(); j++)
                cout << execute(table_array[i][j]) << '\t';
            cout << '\n';
        }
    }

}

int Table::execute(string &elem_) {
    string elem = elem_.substr(1, elem_.npos - 1);
    if (std::find(exe_chain.begin(), exe_chain.end(), elem) != exe_chain.end())
        throw runtime_error("Cycle address.");
    exe_chain.push_back(elem);
    if (elem_[0] != '=') {
        try {
            int num = stoi(elem_);
            exe_chain.erase(std::remove(exe_chain.begin(), exe_chain.end(), elem), exe_chain.end());
            return num;
        }
        catch (invalid_argument except) {
            throw runtime_error("Wrong data in cell");
        }
    } else {
        int res;
        size_t pos = elem.find("+");
        if (pos != elem.npos) {
            return operation('+', elem, pos);
        } else {
            pos = elem.find("-");
            if (pos != elem.npos) {
                return operation('-', elem, pos);
            } else {
                pos = elem.find("*");
                if (pos != elem.npos) {
                    return operation('*', elem, pos);
                } else {
                    pos = elem.find("/");
                    if (pos != elem.npos) {
                        return operation('/', elem, pos);
                    } else
                        throw runtime_error("The operation is not definite.");
                }
            }
        }
    }
}

int Table::find_cell(string elem) {
    try {
        int num = stoi(elem);
        return num;
    }
    catch (invalid_argument except) {
        string error_mess("Wrong cell format -> ");
        error_mess += elem;
        string collum_name, row_name;
        size_t pos = elem.npos, new_pos;
        int collum, row;


        for (char i = '0'; i <= '9'; ++i) {
            new_pos = elem.find(i);
            if (pos > new_pos)
                pos = new_pos;
        }
        if (pos == elem.npos)
            throw runtime_error(error_mess.c_str());

        row_name = elem.substr(pos, elem.length() - pos);
        collum_name = elem.substr(0, pos);

        auto it = find(table_array[0].begin(), table_array[0].end(), collum_name);
        collum = it - table_array[0].begin();
        row = vertical_search(row_name);
        if (row == -1 || it == table_array[0].end()) {
            error_mess = "This cell doesn't exist ";
            error_mess += elem;
            throw runtime_error(error_mess.c_str());
        }
        return execute(table_array[row][collum]);
    }
}

vector<string> Table::proccess_line(string &str) {
    size_t pos = str.find(","), start = 0;
    vector<string> temp_vector;
    while (pos != str.npos) {
        temp_vector.push_back(str.substr(start, pos - start));
        start = pos + 1;
        pos = str.find(",", start);
    }
    temp_vector.push_back(str.substr(start, str.length() - start - EXTRA_CH));
    return temp_vector;
}

int Table::vertical_search(string &elem) {
    for (int i = 1; i < table_array.size(); ++i) {
        if (elem == table_array[i][0]) {
            return i;
        }
    }
    return -1;
}

int Table::operation(char op, string &elem, int pos) {
    int res;
    int divisor;
    switch (op) {
        case '+':
            res = find_cell(elem.substr(0, pos)) + find_cell(elem.substr(pos + 1, elem.npos - pos - 1));
            exe_chain.erase(std::remove(exe_chain.begin(), exe_chain.end(), elem), exe_chain.end());
            return res;
        case '-':
            res = find_cell(elem.substr(0, pos)) - find_cell(elem.substr(pos + 1, elem.npos - pos - 1));
            exe_chain.erase(std::remove(exe_chain.begin(), exe_chain.end(), elem), exe_chain.end());
            return res;
        case '*':
            res = find_cell(elem.substr(0, pos)) * find_cell(elem.substr(pos + 1, elem.npos - pos - 1));
            exe_chain.erase(std::remove(exe_chain.begin(), exe_chain.end(), elem), exe_chain.end());
            return res;
        case '/':
            divisor = find_cell(elem.substr(pos + 1, elem.npos - pos - 1));
            if (divisor == 0)
                throw runtime_error("The division by zero!");
            else
                res = find_cell(elem.substr(0, pos)) / divisor;
            exe_chain.erase(std::remove(exe_chain.begin(), exe_chain.end(), elem), exe_chain.end());
            return res;
        default:
            throw runtime_error("The operation is not definite.");
    }
}

void Table::validation_elem(int i, int j) {
    string error_mess("Error table format -> ");
    if(i + j == 0 && table_array[i][j] != "")
        throw runtime_error((error_mess + to_string(i) + ' ' + to_string(j)));
    else if(i == 0){
        for (int k = 0; k < table_array[i][j].length(); ++k) {
            if (table_array[i][j][k] < 'A' || table_array[i][j][k] > 'z')
                throw runtime_error((error_mess + to_string(i) + ' ' + to_string(j)));
        }
    } else if(j == 0) {
        for (int k = 0; k < table_array[i][j].length(); ++k) {
            if (table_array[i][j][k] < '0' || table_array[i][j][k] > '9')
                throw runtime_error((error_mess + to_string(i) + ' ' + to_string(j)));
        }
    }
}

void Table::validation() {
    for (int i = 0; i < table_array[0].size(); ++i) {
        validation_elem(0,i);
    }
    for (int i = 1; i < table_array.size(); ++i) {
        validation_elem(i,0);
    }
}

