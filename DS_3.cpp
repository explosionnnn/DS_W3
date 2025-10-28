#include<iostream>
#include<string>

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
        char** BuildMaze(int x, int y) {
            char** maze = new char*[x];
            for (int i = 0; i < x; i++) {
                maze[i] = new char[y];
            }
            return maze;
        }

        void DeleteMaze(char** maze, int x) {
            for (int i = 0; i < x; i++) {
                delete[] maze[i];
            }
            delete[] maze;
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
            StackNode* del = top;
            top = top -> next;
            delete top;
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