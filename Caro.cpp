#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <string>
#include <Windows.h>

using namespace std;

// Định nghĩa enum class ConsoleColor để biểu diễn màu sắc trên console
enum class ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Yellow = 6,
    White = 7
};

// Hàm SetConsoleColor: Thiết lập màu cho văn bản trên cửa sổ console
void SetConsoleColor(ConsoleColor foreground, ConsoleColor background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = static_cast<int>(foreground) + (static_cast<int>(background) * 16);
    SetConsoleTextAttribute(hConsole, color);
}

// Hàm flushInputBuffer: Xóa bộ đệm đầu vào của cin
void flushInputBuffer() {
    cin.clear();
    while (cin.get() != '\n') continue;
}

// Lớp đại diện cho bảng trò chơi
class Board {
private:
    vector<vector<char>> board;

public:
    // Hàm khởi tạo bảng trò chơi
    Board() : board(3, vector<char>(3, ' ')) {}

    // Phương thức draw: Vẽ bảng trò chơi trên màn hình console
    void draw() {
        cout << "     1   2   3  " << endl;
        cout << "   +---+---+---+" << endl;
        for (int i = 0; i < 3; ++i) {
            cout << " " << i + 1 << " | ";
            for (int j = 0; j < 3; ++j) {
                // Thiết lập màu cho ô trên bảng dựa trên ký hiệu 'X' hoặc 'O'
                if (board[i][j] == 'X') {
                    SetConsoleColor(ConsoleColor::Green, ConsoleColor::Black);
                }
                else if (board[i][j] == 'O') {
                    SetConsoleColor(ConsoleColor::Red, ConsoleColor::Black);
                }
                else {
                    SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
                }
                cout << board[i][j];
                SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
                cout << " | ";
            }
            cout << endl << "   +---+---+---+" << endl;
        }
    }

    // Phương thức checkWin: Kiểm tra xem một người chơi có thắng hay không
    bool checkWin(char symbol) {
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol ||
                board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) {
                return true;
            }
        }
        return (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
            (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol);
    }

    // Phương thức isFull: Kiểm tra xem bảng trò chơi đã đầy chưa
    bool isFull() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }

    // Phương thức placeSymbol: Đặt ký hiệu của một người chơi vào ô trên bảng
    bool placeSymbol(int row, int col, char symbol) {
        if (row < 0 || row > 2 || col < 0 || col > 2) {
            cout << "Vị trí không hợp lệ. Vui lòng nhập lại." << endl;
            return false;
        }

        if (board[row][col] != ' ') {
            return false;
        }

        board[row][col] = symbol;
        return true;
    }

    // Phương thức clear: Xóa bảng trò chơi
    void clear() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = ' ';
            }
        }
    }
};

// Lớp đại diện cho một người chơi
class Player {
protected:
    char symbol;
    string name;

public:
    // Hàm khởi tạo người chơi
    Player(char symbol, string name) : symbol(symbol), name(name) {}

    // Phương thức ảo để người chơi thực hiện nước đi
    virtual void makeMove(Board& board) = 0;

    // Phương thức để lấy ký hiệu của người chơi
    char getSymbol() const {
        return symbol;
    }

    // Phương thức để lấy tên của người chơi
    string getName() const {
        return name;
    }
};

// Lớp con đại diện cho người chơi là con người
class HumanPlayer : public Player {
public:
    // Hàm khởi tạo người chơi con người
    HumanPlayer(char symbol, string name) : Player(symbol, name) {}

    // Phương thức để người chơi con người thực hiện nước đi
    void makeMove(Board& board) {
        bool validMove = false;
        do {
            int row, col;
            cout << name << ", nhập dòng và cột (1-3): ";
            cin >> row >> col;

            // Kiểm tra xem người dùng đã nhập đúng định dạng (số từ 1 đến 3) chưa
            if (cin.fail() || row < 1 || row > 3 || col < 1 || col > 3) {
                cout << "Vị trí không hợp lệ. Vui lòng nhập lại." << endl;
                flushInputBuffer();
                continue;
            }
            // Chuyển đổi từ index 1-based thành 0-based
            row--;
            col--;

            validMove = board.placeSymbol(row, col, symbol);

            if (!validMove) {
                cout << "Ô này đã được đánh dấu. Vui lòng chọn ô khác." << endl;
            }

        } while (!validMove);
    }
};

// Lớp con đại diện cho người chơi là máy tính
class ComputerPlayer : public Player {
public:
    // Hàm khởi tạo người chơi máy tính
    ComputerPlayer(char symbol, string name) : Player(symbol, name) {}

    // Phương thức để người chơi máy tính thực hiện nước đi
    void makeMove(Board& board) {
        int row, col;
        do {
            row = rand() % 3;
            col = rand() % 3;
        } while (!board.placeSymbol(row, col, symbol));
        cout << "Máy đã đặt '" << symbol << "' ở hàng " << row + 1 << ", cột " << col + 1 << endl;
    }
};
// Lớp đại diện cho trò chơi
class Game {
private:
    Board board;
    Player* player1;
    Player* player2;

public:
    // Hàm khởi tạo trò chơi
    Game(Player* p1, Player* p2) : player1(p1), player2(p2) {}

    // Phương thức start: Bắt đầu trò chơi
    void start() {
        board.clear();
        Player* currentPlayer = player1;
        bool gameEnded = false;
        while (!gameEnded) {
            board.draw();
            cout << "Lượt của " << currentPlayer->getName() << ": " << endl;
            currentPlayer->makeMove(board);
            if (board.checkWin(currentPlayer->getSymbol())) {
                board.draw();
                cout << currentPlayer->getName() << " thắng!" << endl;
                gameEnded = true;
            }
            else if (board.isFull()) {
                board.draw();
                cout << "Hòa!" << endl;
                gameEnded = true;
            }
            currentPlayer = (currentPlayer == player1) ? player2 : player1;
        }
    }
};

// Hàm menu: Hiển thị menu và bắt đầu trò chơi
void menu() {
    int choice;
    do {
        cout << "Chọn chế độ chơi:" << endl;
        cout << "1. Người chơi với máy" << endl;
        cout << "2. Người chơi với người" << endl;
        cout << "3. Máy chơi với máy" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại." << endl;
            flushInputBuffer();
            continue;
        }

        if (choice < 1 || choice > 3) {
            cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại." << endl;
        }
    } while (choice < 1 || choice > 3);

    Player* player1 = nullptr;
    Player* player2 = nullptr;

    string symbolChoice;
    bool validSymbol = false;
    do {
        cout << "Chọn ký hiệu (X hoặc O) cho player 1: ";
        cin >> symbolChoice;

        if (symbolChoice != "X" && symbolChoice != "O") {
            cout << "Ký hiệu không hợp lệ. Vui lòng chọn lại." << endl;
        }
        else {
            validSymbol = true;
        }
    } while (!validSymbol);

    player1 = new HumanPlayer(symbolChoice[0], "player 1");

    if (choice == 1) {
        player2 = new ComputerPlayer((symbolChoice[0] == 'X' ? 'O' : 'X'), "player 2");
    }
    else if (choice == 3) {
        player1 = new ComputerPlayer('X', "player 1");
        player2 = new ComputerPlayer('O', "player 2");
    }
    else if (choice == 2) {
        player2 = new HumanPlayer((symbolChoice[0] == 'X' ? 'O' : 'X'), "Người chơi 2");
    }

    if (player1 && player2) {
        Game game(player1, player2);
        game.start();
        delete player1;
        delete player2;
    }
}

// Hàm main: Bắt đầu chương trình
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    bool returnToMenu = false;
    do {
        menu();
        int option;
        do {
            cout << "Bạn muốn quay lại menu chọn chế độ chơi không?" << endl;
            cout << "1. Có" << endl;
            cout << "2. Không" << endl;
            cout << "Nhập lựa chọn: ";
            cin >> option;
            if (cin.fail()) {
                cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại." << endl;
                flushInputBuffer();
                continue;
            }

            if (option < 1 || option > 2) {
                cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại." << endl;
            }
        } while (option < 1 || option > 2);

        returnToMenu = (option == 1);

    } while (returnToMenu);

    return 0;
}

