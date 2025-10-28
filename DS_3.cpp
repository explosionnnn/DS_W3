#include<iostream>
#include<string>
#include <fstream>

using namespace std;
struct Pos {
    int x;
    int y;
};

struct StackNode {
    Pos pos;
    StackNode* next;
};
class Maze {
    private:
        char** maze;
        int x;
        int y;
    public:
        Maze (int x, int y) {
            this -> x = x;
            this -> y = y;
            maze = new char*[x];
            for (int i = 0; i < x; i++) {
                maze[i] = new char[y];
            }
        }
        void DeleteMaze() {
            for (int i = 0; i < x; i++) {
                delete[] maze[i];
            }
            delete[] maze;
            maze = nullptr;
        }
        void SetMaze(char maze_word) {
            maze[x][y] = maze_word;
        }

        void PrintMaze() {
            for (int i = 0; i < x; i++) {
                for (int j = 0; j < y; j++) {
                    cout << maze[x][y];
                }
                cout << endl;
            }
        }


};
class Stack {
    private:
        StackNode* top;
    public:
        Stack() {
            top = nullptr;
        }
        ~Stack() {
            while (!IsEmpty()) {
                pop();
            }
        }
        void pop() {
            if (!IsEmpty()) {
                StackNode* del = top;
                top = top -> next;
                delete top;
            }
        }

        void push() {
            StackNode* newnode;
            newnode -> next = top;
            top = newnode;
        }

        bool IsEmpty() {
            if (top == nullptr) {
                return true;
            }
            return false;
        }





};

int main() {
    int x;
    int y;
    char maze_dot;
    ifstream infile("data.txt");
    if (!infile) {
        return 1;
    }
    infile >> x;
    infile >> y;
    Maze maze(x, y);
    while (infile >> maze_dot) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                maze.SetMaze(maze_dot);
            }
        }
    }

}