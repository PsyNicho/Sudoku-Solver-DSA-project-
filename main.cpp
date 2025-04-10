#include<iostream>
#include<windows.h>
#include<limits>

using namespace std;

int *posx,*posy;

void dash() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width,i;
    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi)) {
        cout<<"Error getting console screen buffer info.";
        return;
    }
    width=csbi.dwSize.X;
    cout<<endl;
    for (i=0;i<width;i++) {
        cout<<"-";
    }
    cout<<endl<<endl;
    fflush(stdout);
}

void gotoxy(int x,int y) {
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    if(hConsole==INVALID_HANDLE_VALUE) {
        cout<<"Error getting console handle.";
        exit(1);
	}
    COORD cursorPos;
    cursorPos.X=x;
    cursorPos.Y=y;
    if(!SetConsoleCursorPosition(hConsole,cursorPos)) {
        cout<<"Error setting console cursor position.";
        exit(1);
    }
}

char confirm(char s[],int *x,int *y) {
	char c;	
	gotoxy(*x,*y);
	cout<<s;
	cin>>c;
	return(c);
}

class Sudoku {
    private:
        int grid[9][9] = {0};
        int x,y; 
    public:
        void displayGrid(int x, int y) {
            if(y==1)
                system("cls");
            gotoxy(x,y);
            cout<<"Current Sudoku Grid :\n";
            for (int i = 0; i < 9; i++) {
                if (i % 3 == 0 && i != 0){
                    gotoxy(x,++y);
                    cout << "------+-------+------" << endl;
                }  
                gotoxy(x,++y); 
                for (int j = 0; j < 9; j++) { 
                    if (j % 3 == 0 && j != 0){
                        
                        cout << "| ";
                    }
                    cout << (grid[i][j] == 0 ? "." : to_string(grid[i][j])) << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        
        bool isSafe(int row, int col, int num) {
            for (int x = 0; x < 9; x++) {
                if (grid[row][x] == num) {
                    return false;
                }
            }
            for (int x = 0; x < 9; x++) {
                if (grid[x][col] == num) {
                    return false;
                }
            }
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
        
        bool solve() {
            int row, col;
            bool emptyFound = false;
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
            if (!emptyFound) {
                return true;
            }
            for (int num = 1; num <= 9; num++) {
                if (isSafe(row, col, num)) {
                    grid[row][col] = num;
                    if (solve()) 
                        return true;
                    grid[row][col] = 0;
                }
            }   
            return false;
        }

        void hint(){
            int row, col, num;
            if (findHint(row, col, num)) {
                x=35,y=1;
                system("cls");
                gotoxy(x,y);
                cout << "Hint: Placed " << num<< " at row " << row + 1 << ", column " << col + 1 << ".\n";
                placeNumber(row, col, num);
                gotoxy(x,++y);
                cout << "Hint applied to the grid.\n";
                displayGrid(35,3);
            } 
            else 
                cout << "\nNo hint available — the puzzle may be complete or unsolvable.\n";
            
        }

        bool findHint(int& outRow, int& outCol, int& outNum) {
            int minOptions = 10; 
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (grid[i][j] == 0) {  
                        int optionsCount = 0;  
                        int firstValidNum = -1;  
                        for (int num = 1; num <= 9; num++) {
                            if (isSafe(i, j, num)) {  
                                optionsCount++;
                                if (firstValidNum == -1) 
                                    firstValidNum = num;                   
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
            return minOptions != 10; 
        }
        

        void placeNumber(int row, int col, int num) {
            grid[row][col] = num;
        }
        void inputGrid() {
            int row, col, num, n;
            char choice;
            do {
                x=35,y=1;
                system("cls");
                gotoxy(x,++y);
                cout << "Enter number of known data (between 0 and 81): ";
                cin >> n;
                if (cin.fail()) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    gotoxy(x,++y);
                    cout << "Invalid input! Please enter a number between 0 and 81.\n";
                    continue; 
                }
                if (n < 0 || n > 81) {
                    gotoxy(x,++y);
                    cout << "Invalid number of known data. The value must be between 0 and 81.\n";
                }
            } while (n < 0 || n > 81);  
            for (int i = 0; i < n; i++) {
                do {
                    gotoxy(x,++y);
                    cout << "Enter row (1-9), column (1-9), and number (1-9): ";
                    cin >> row >> col >> num;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        gotoxy(x,++y);
                        cout << "Invalid input! Please enter valid row, column, and number (1-9).\n";
                        continue; 
                    }
                    if (row >= 1 && row <= 9 && col >= 1 && col <= 9 && num >= 1 && num <= 9) {
                        row -= 1;  
                        col -= 1;
                        if (grid[row][col] == 0) {  
                            if (isSafe(row, col, num)) {
                                grid[row][col] = num;
                                // gotoxy(x,++y);
                                // cout << "Number placed successfully." << endl;
                                break;  
                            } 
                            else{
                                gotoxy(x,++y);
                                cout << "The number cannot be placed here due to a conflict." << endl;
                            } 
                                
                        } 
                        else {
                            gotoxy(x,++y);
                            cout << "The cell is already filled with a number. Please choose an empty cell." << endl; 
                        }
                    } 
                    else {
                        gotoxy(x,++y);
                        cout << "Invalid input! Please ensure the row and column are between 1 and 9, and the number is between 1 and 9." << endl;
                    }
                } while (true);  
            }
        }   
};

void displayMenu() {
    system("cls");
    int x=50,y=4;
    gotoxy(x,y);
    dash();
    y+=3;
    gotoxy(x,y);
    cout<<"|\tSudoku Solver \t|";
    gotoxy(x,++y);
    dash();
    y+=3;
    gotoxy(x,y);
    cout<<"1. Input data to grid";
    gotoxy(x,++y);
    cout<<"2. Get hint";
    gotoxy(x,++y);
    cout<<"3. Solve Completely";
    gotoxy(x,++y);
    cout<<"4. Display grid";
    gotoxy(x,++y);
    cout<<"5. Exit";
}

int main() {
    int x=35,y=20;
    Sudoku s;
    char c;
    posx=&x;
	posy=&y;
    do {
        displayMenu();
        c=confirm("Which operation do you want to perform?: ",&x,&y); 
        switch(c) {
            case '1':
            s.inputGrid();
                break;
            case '2':
                s.hint();
                break;
            case '3':
                if (s.solve()) {
                    system("cls");
                    gotoxy(35,1);
                    cout << "Sudoku solved successfully!";
                    s.displayGrid(35,3);
                } 
                else 
                cout << "\nNo solution exists for this Sudoku puzzle.\n";
                break;
            case '4':
                s.displayGrid(35,1);
                break;
            case '5':
            	system("cls");
                gotoxy(x,4);
				dash();
				gotoxy(45,8);
				printf("Thank you for using the program.");
				gotoxy(x,10);
				dash();
				return 0;
				break;
            default:
            	y+=2;
                gotoxy(x,y);
				printf("Invalid choice. Please choose 1 or 2.");
				*posy=23;
        }
        c=confirm("Do you want to continue? [Y/N] : ",posx,posy);
        (*posy)=20;
    } while(c=='y' || c=='Y');   
}

