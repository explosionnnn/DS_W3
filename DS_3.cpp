/*11327121 侯至謙, 11327155 黃宇謙*/
#include<iostream>
#include<string>
#include <fstream>

using namespace std;

enum Direction {
    RIGHT = 0,
    DOWN = 1,
    LEFT = 2,
    UP = 3
};

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
        int x; //長
        int y; //寬
    public:
        Maze(int x, int y) {
            this->x = x;
            this->y = y;
            maze = new char*[x];
            for (int i = 0; i < x; i++) {
                maze[i] = new char[y];
            }
        }

        ~Maze() {
            DeleteMaze();
        }

        void DeleteMaze() {
            for (int i = 0; i < x; i++) {
                delete[] maze[i];
            }
            delete[] maze;
            maze = nullptr;
        }

        int GetX() {
            return x;
        }

        int GetY() {
            return y;
        }

        char GetBlock() {
            return maze[x][y];
        }

        char GetBlock(int nx, int ny) {
            return maze[nx][ny];
        }

        void SetMaze(int pos_x, int pos_y, char maze_word) {
            maze[pos_x][pos_y] = maze_word;
        }

        void PrintMaze() {
            for (int i = 0; i < x; i++) {
                for (int j = 0; j < y; j++) {
                    cout << maze[i][j];
                }
                cout << endl;
            }
        }

        void PrintVisitedRoute() {
            for (int i = 0; i < x; i++) {
                for (int j = 0; j < y; j++) {
                    if (maze[i][j] == 'R') {  //R也算在visited route
                        cout << 'V';
                    } else {
                        cout << maze[i][j];
                    }
                }
                cout << endl;
            }
        }

        void PrintReachRoute() {
            for (int i = 0; i < x; i++) {
                for (int j = 0; j < y; j++) {
                    if (maze[i][j] == 'V') {
                        cout << 'E';
                    } else {
                        cout << maze[i][j];
                    }
                }
                cout << endl;
            }
        }





};

class RecordMap { // stack
    private:
        StackNode* top;
    public:
        RecordMap() {
            top = nullptr;
        }

        ~RecordMap() {
            while (!IsEmpty()) {
                pop();
            }
        }

        void pop() {
            if (!IsEmpty()) {
                StackNode* del = top;
                top = top -> next;
                delete del;
            }
        }

        void push(int x, int y) {
            StackNode* newnode = new StackNode;
            newnode -> pos.x = x;
            newnode -> pos.y = y;
            newnode -> next = top;
            top = newnode;
        }

        bool IsEmpty() {
            return (top == nullptr);
        }

        void GetTop(Pos &pos) {
            if (!IsEmpty()) {
                pos.x = top -> pos.x;
                pos.y = top -> pos.y;
            }
        }


};

class Mouse {
    private:
        Pos pos;
        Direction dir;
        Maze &in_this_maze;        /*用reference避免Mouse被刪除時*/
        RecordMap &visited_route;  /*連帶刪除外部Maze和RecordMap指向的物件*/
        bool finish = false;
        bool back = false;
    public:
    Mouse(Maze& maze, RecordMap& map)
        : pos{0, 0}, dir(RIGHT), in_this_maze(maze), visited_route(map) {}

    void ChangeDir() {
        dir = static_cast<Direction>(dir + 1);
        if (dir == 4) {
            dir = RIGHT;
        }
    }

    void Walk() {
        if (dir == RIGHT) {
            pos.x++;
        } else if (dir == DOWN) {
            pos.y++;
        } else if (dir == LEFT) {
            pos.x--;
        } else if (dir == UP) {
            pos.y--;
        }
        visited_route.push(pos.x, pos.y);
        in_this_maze.SetMaze(pos.x, pos.y, 'R');  //走到就設為R
    }

    bool CanWalkTo(int nx, int ny) {
        if (nx < 0 || ny < 0) {
            return false;
        }
        if (nx >= in_this_maze.GetX() || ny >= in_this_maze.GetY()) {
            return false;
        }
        char block = in_this_maze.GetBlock(nx, ny);
        if (block == 'O' || block == 'V' || block == 'R') {  // 這三個都不能走
            return false;
        }
        return true;

    }

    bool Step() {
    // 試四個方向
    for (int i = 0; i < 4; ++i) {
        int nx = pos.x;
        int ny = pos.y;

        // 依目前 dir 算下一格
        if (dir == RIGHT)      nx++;
        else if (dir == DOWN)  ny++;
        else if (dir == LEFT)  nx--;
        else if (dir == UP)    ny--;

        // 可以走就走
        if (CanWalkTo(nx, ny)) {
            return true;    // 這一步成功
        }

        // 不行走就換方向再試
        ChangeDir();
    }

    // 四個方向都不能走 → 死路 → 回去
    Back();
    return false;
}

    void Back() {
        in_this_maze.SetMaze(pos.x, pos.y, 'V');  // 退回來變成V
        visited_route.GetTop(pos);  // 退到堆疊的top
        ChangeDir();
        visited_route.pop();
    }

    bool Finish() {
        return (in_this_maze.GetBlock(pos.x, pos.y) == 'G');
    }

    void FindGoal() { //T1
        bool try_step;
        visited_route.push(pos.x, pos.y);
        while (!visited_route.IsEmpty()) {
            try_step = Step();
            if (try_step) {
                Walk();
                if (Finish()) {
                    in_this_maze.PrintMaze();
                    break;
                }
            }
        }
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
    if (x <= 0 || y <= 0) {
        return 1;
    }
    Maze maze(x, y);
    while (infile >> maze_dot) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                maze.SetMaze(i, j, maze_dot);
            }
        }
    }

}