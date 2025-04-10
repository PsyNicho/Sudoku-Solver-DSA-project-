#include <iostream>
using namespace std;

const int SIZE = 9;

void printSudokuGrid() {
    
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) 
            cout << "------+-------+------" << endl;
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) 
                cout << "| ";                      
            cout << ". ";
        }
        cout << endl;   
    }
}

int main() {
    cout << "Empty Sudoku Grid:" << endl;
    printSudokuGrid();
    return 0;
}