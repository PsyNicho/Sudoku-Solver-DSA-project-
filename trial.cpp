#include<iostream>
#include<windows.h>
#include<limits>

using namespace std;

class Sudoku {
    private:
        int grid[9][9] = {0};  // Initialize the grid with zeros (empty cells)
    
    public:
        // Function to display the grid
        void displayGrid() {
            cout << "\nCurrent Sudoku Grid:\n";
            for (int i = 0; i < 9; i++) {
                if (i % 3 == 0 && i != 0)
                    cout << "------+-------+------" << endl;
                for (int j = 0; j < 9; j++) {
                    if (j % 3 == 0 && j != 0)
                        cout << "| ";
                    cout << (grid[i][j] == 0 ? "." : to_string(grid[i][j])) << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        
    
        // Function to check if it's safe to place a number in a cell
        bool isSafe(int row, int col, int num) {
            // Check the row
            for (int x = 0; x < 9; x++) {
                if (grid[row][x] == num) {
                    return false;
                }
            }
    
            // Check the column
            for (int x = 0; x < 9; x++) {
                if (grid[x][col] == num) {
                    return false;
                }
            }
    
            // Check the 3x3 subgrid
            int startRow = row - row % 3;
            int startCol = col - col % 3;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid[i + startRow][j + startCol] == num) {
                        return false;
                    }
                }
            }
    
            return true;
        }
        
        // Function to solve the Sudoku puzzle using backtracking
        bool solve() {
            int row, col;
            bool emptyFound = false;
    
            // Find an empty cell (denoted by 0)
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (grid[i][j] == 0) {
                        row = i;
                        col = j;
                        emptyFound = true;
                        break;
                    }
                }
                if (emptyFound) {
                    break;
                }
            }
    
            // If no empty cell is found, the puzzle is solved
            if (!emptyFound) {
                return true;
            }
    
            // Try numbers from 1 to 9
            for (int num = 1; num <= 9; num++) {
                if (isSafe(row, col, num)) {
                    grid[row][col] = num;
    
                    // Recursively solve the puzzle
                    if (solve()) {
                        return true;
                    }
    
                    // If the number doesn't work, backtrack
                    grid[row][col] = 0;
                }
            }
    
            return false;  // Trigger backtracking
        }

        void hint(){
            int row, col, num;
            if (findHint(row, col, num)) {
                char applyHint;
                cout << "\nHint: Placed " << num<< " at row " << row + 1 << ", column " << col + 1 << ".\n";
                placeNumber(row, col, num);
                cout << "Hint applied to the grid.\n";
                displayGrid();
            } else {
                cout << "\nNo hint available — the puzzle may be complete or unsolvable.\n";
            }
        }

        bool findHint(int& outRow, int& outCol, int& outNum) {
            int minOptions = 10;  // To track the least number of valid options
        
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (grid[i][j] == 0) {  // Empty cell
                        int optionsCount = 0;  // To count valid numbers for the current cell
                        int firstValidNum = -1;  // To store the first valid number found for the current cell
        
                        for (int num = 1; num <= 9; num++) {
                            if (isSafe(i, j, num)) {  // Check if placing 'num' is safe
                                optionsCount++;
                                if (firstValidNum == -1) {
                                    firstValidNum = num;  // Store the first valid number
                                }
                            }
                        }
        
                        if (optionsCount > 0 && optionsCount < minOptions) {
                            minOptions = optionsCount;
                            outRow = i;
                            outCol = j;
                            outNum = firstValidNum;  
                        }
                    }
                }
            }
        
            return minOptions != 10;  // Return true if we found a hint
        }
        

        void placeNumber(int row, int col, int num) {
            grid[row][col] = num;
        }
        // Function to allow the user to input the Sudoku puzzle interactively
        void inputGrid() {
            int row, col, num, n;
            char choice;
        
            // Ask for the number of known data (pre-filled cells)
            do {
                cout << "Enter number of known data (between 0 and 81): ";
                cin >> n;
        
                // Check if the user input is a number
                if (cin.fail()) {
                    cin.clear(); // Clears error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discards invalid input
                    cout << "Invalid input! Please enter a number between 0 and 81.\n";
                    continue; // Ask again
                }
        
                // Validate the number of known entries
                if (n < 0 || n > 81) {
                    cout << "Invalid number of known data. The value must be between 0 and 81.\n";
                }
            } while (n < 0 || n > 81);  // Keep asking until the number is valid
        
            // Now, allow user to input n known data
            for (int i = 0; i < n; i++) {
                do {
                    cout << "Enter row (1-9), column (1-9), and number (1-9): ";
                    cin >> row >> col >> num;
        
                    // Check if the user input is a number
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Please enter valid row, column, and number (1-9).\n";
                        continue; // Ask again
                    }
        
                    if (row >= 1 && row <= 9 && col >= 1 && col <= 9 && num >= 1 && num <= 9) {
                        row -= 1;  // Adjust to 0-based index
                        col -= 1;
                        if (grid[row][col] == 0) {  // Ensure that the cell is empty
                            if (isSafe(row, col, num)) {
                                grid[row][col] = num;
                                cout << "Number placed successfully." << endl;
                                break;  // Exit the loop and move to the next input
                            } else {
                                cout << "The number cannot be placed here due to a conflict." << endl;
                            }
                        } else {
                            cout << "The cell is already filled with a number. Please choose an empty cell." << endl;
                        }
                    } else {
                        cout << "Invalid input! Please ensure the row and column are between 1 and 9, and the number is between 1 and 9." << endl;
                    }
                } while (true);  // Repeat for valid input
            }
        }   
};


int main() {
    Sudoku s;
    char choice;
    char cont;

    do {
        cout << "\n=== Sudoku Solver ===";
        cout << "\n1. Input data to grid";
        cout << "\n2. Get hint";
        cout << "\n3. Solve Completely";
        cout << "\n4. Display grid";
        cout << "\n5. Exit";
        cout << "\nEnter your choice (1-5): ";
        cin >> choice;

        switch(choice) {
            case '1':
                s.inputGrid();
                break;
            case '2':
                s.hint();
                break;
            case '3':
                if (s.solve()) {
                    cout << "\nSudoku solved successfully!\n";
                    s.displayGrid();
                } else {
                    cout << "\nNo solution exists for this Sudoku puzzle.\n";
                }
                break;
            case '4':
                s.displayGrid();
                break;
            case '5':
                system("cls");
                cout << "Thank you for using the program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please choose 1-5.\n";
        }

        cout << "Do you want to continue? (Y/N): ";
        cin >> cont;
    } while(cont == 'Y' || cont == 'y');

    return 0;
}
