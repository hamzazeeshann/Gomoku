#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>

using namespace std;

// BAKI SAARE FUNCTIONS
void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

void init(int& Turn, int& dim, int& nop, int& wc, char* &Psymbol, string* &names, char** &B) {
    cout << "How many Players? ";
    cin >> nop;
    names = new string[nop]{};
    Psymbol = new char[nop]{};
    for (int i = 0; i < nop; i++) {
        cout << "\nEnter the name of " << i+1 << "th Player: ";
        cin >> names[i];
        cout << "\nChoose a Symbol: ";
        cin>>Psymbol[i];
    }
    cout << "\nEnter the dimension of the board: ";
    cin >> dim;
    cout << "How many consecutive symbols to win? (Win Count): ";
    cin >> wc;
    cout << "Which Player will start? ";
    cin >> Turn;
    Turn = Turn - 1;
    B = new char*[dim];
    for (int i = 0; i < dim; i++) {
        B[i] = new char[dim];
        for (int j = 0; j < dim; j++) {
            B[i][j] = '-';
        }
    }
    system("cls");
}

void PrintBoard(char** B, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            cout << B[i][j];
        }
        cout << endl;
    }
}

void PrintMessage(string Name) {
    gotoRowCol(10, 25);
    cout << Name << "'s Turn, Click a box";
}

bool isValidTurn(int ri, int ci, char** B, int dim) {
    return (ri >= 0 and ci >= 0 and ri < dim and ci < dim and B[ri][ci] == '-');
}

void BoardUpdate(char** B, int ri, int ci, char sym) {
    B[ri][ci] = sym;
}

// WIN-CHECK FUNCTIONS
bool hasWonHor(char** B, int ri, int ci, int dim,int wc, char sym) {
    if (ci > dim - wc)
        return false;
    for (int i = 0; i < wc; i++) {
        if (B[ri][ci+i] != sym)
            return false;
    }
    return true;
}
bool hasWonVert(char** B, int ri, int ci, int dim, int wc, char sym) {
    if (ri > dim - wc)
        return false;
    for (int i = 0; i <wc; i++) {
        if (B[ri+i][ci] != sym)
            return false;
    }
    return true;
}
bool hasWonDiagLeft(char** B, int ri, int ci, int dim, int wc, char sym) {
    if (ci < wc - 1 || ri > dim - wc)
        return false;
    for (int i = 0; i < wc; i++) {
        if (B[ri + i][ci - i] != sym)
            return false;
    }
    return true;
}
bool hasWonDiagRight(char** B, int ri, int ci, int dim, int wc, char sym) {
    if (ci > dim - wc || ri > dim - wc)
        return false;
    for (int i = 0; i < wc; i++) {
        if (B[ri + i][ci + i] != sym)
            return false;
    }
    return true;
}
bool hasWon(char** B, int dim, char sym, int wc) {
    for (int ri = 0; ri < dim; ri++) {
        for (int ci = 0; ci < dim; ci++) {
            if (hasWonDiagLeft(B, ri, ci, dim, wc, sym) || hasWonDiagRight(B, ri, ci, dim, wc, sym)
                || hasWonVert(B, ri, ci, dim, wc, sym) || hasWonHor(B, ri, ci, dim, wc, sym))
                return true;
        }
    }
    return false;
}

bool isDraw(char** B, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (B[i][j] == '-') 
                return false;
        }
    }
    return true;
}

int main() {
    int ri, ci;
    int turn, dim, nop, wc;
    string* names = nullptr;
    char* Psymbol=nullptr;
    char** B = nullptr;
    init(turn, dim, nop, wc, Psymbol, names, B);
    cout << endl;
    do {
        gotoRowCol(0, 0);
        PrintBoard(B, dim);
        PrintMessage(names[turn]);

        do {
            getRowColbyLeftClick(ri, ci);
        } while (!isValidTurn(ri, ci, B, dim));
        BoardUpdate(B, ri, ci, Psymbol[turn]);

        if (hasWon(B, dim, Psymbol[turn], wc)) {
            system("cls");
            gotoRowCol(0, 0);
            PrintBoard(B, dim);
            gotoRowCol(20, 25);
            cout << names[turn] << " has won the game!" << endl;
            Psymbol[turn] = ':';
            //I am updating the PSYMBOL to check wether the next turn has already won or not, if yes then change turn again.
      
        }
        if (isDraw(B, dim)) {
            system("cls");
            PrintBoard(B, dim);
            gotoRowCol(20, 25);
            cout << "The game is a draw!" << endl;
            break;
        }

        do {
            turn = (turn + 1) % nop;
        } while (Psymbol[turn] == ':');

    } while (true);

    for (int i = 0; i < dim; i++) {
        delete[] B[i];
    }
    delete[] B;
    delete[] names;
    delete[] Psymbol;

    return 0;
}