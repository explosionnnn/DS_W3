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
        char** maze; // 注意:maze[y][x] (先取寬(縱軸)再取長(橫軸))
        int x; //長
        int y; //寬
    public:
        Maze(int x, int y) {
            this->x = x;
            this->y = y;
            maze = new char*[y];            // y rows         
            for (int i = 0; i < y; i++) {
                maze[i] = new char[x];      // x columns
            }
        }

        Maze(const Maze& other) : Maze(other.x, other.y) { //Copy Constructor
        for (int i = 0; i < y; i++)
            for (int j = 0; j < x; j++)
                maze[i][j] = other.maze[i][j];
        }

        ~Maze() {
            DeleteMaze();
        }

        void DeleteMaze() {
            for (int i = 0; i < y; i++) {
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

        char GetBlock() { // 這個好像沒用到
            return maze[y][x];
        }

        char GetBlock(int nx, int ny) {
            return maze[ny][nx];
        }

        void SetMaze(int pos_x, int pos_y, char maze_word) {
            if (pos_x >= 0 && pos_x < x && pos_y >= 0 && pos_y < y) {
                maze[pos_y][pos_x] = maze_word;
            }
        }

        void PrintMaze() {
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    cout << maze[i][j];
                }
                cout << endl;
            }
        }

        void PrintVisitedRoute() {
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
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
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    if (maze[i][j] == 'V') {
                        cout << 'E';
                    } else {
                        cout << maze[i][j];
                    }
                }
                cout << endl;
            }
        }

        void Reset(Maze &original_maze) {
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    maze[i][j] = original_maze.GetBlock(j, i);
                }
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

        StackNode* GetTop() {
            if (!IsEmpty()) {
                return top;
            }
            return nullptr;
        }

        int GetTopX() {
            if (!IsEmpty()) {
                return top -> pos.x;
            }
            return 0;
        }

        int GetTopY() {
            if (!IsEmpty()) {
                return top -> pos.y;
            }
            return 0;
        }

        void Clear() {
            while (!IsEmpty()) {
                pop();
            }
        }

};

class Mouse {
    private:
        Pos pos;
        Direction dir;
        Maze in_this_maze;        /*Maze已實作Copy Constructor，不用reference*/
        Maze original_maze; //每次task結束，還原
        RecordMap &visited_route;  /*用reference避免Mouse被刪除時連帶刪除外部RecordMap指向的物件*/
        bool finish = false;
        bool back = false;
    public:
    Mouse(Maze& maze, RecordMap& map)
        : pos{0, 0}, dir(RIGHT), in_this_maze(maze), original_maze(maze), visited_route(map) {}

    void ChangeDir() {
        dir = static_cast<Direction>(dir + 1);
        if (dir == 4) {
            dir = RIGHT;
        }
    }

    void Walk(const Pos& next_pos) {
        pos = next_pos;
        visited_route.push(pos.x, pos.y);
        if (in_this_maze.GetBlock(pos.x, pos.y) != 'G') // 保護 G
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

    bool Step(Pos& next_pos) {
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
            next_pos = {nx, ny};
            return true;    // 這一步成功
        }

        // 不行走就換方向再試
        ChangeDir();
    }

    // 四個方向都不能走
    return false;
}

    void Back() {
        in_this_maze.SetMaze(pos.x, pos.y, 'V'); 
        if (!visited_route.IsEmpty()) {
             visited_route.pop();
            if (!visited_route.IsEmpty()) {
                StackNode* top = visited_route.GetTop(); 
                pos.x = top->pos.x;
                pos.y = top->pos.y;
            }
            ChangeDir();
        }
    }

    bool Finish() {
        return (in_this_maze.GetBlock(pos.x, pos.y) == 'G');
    }

    void FindGoal() { //T1 and T4
        visited_route.push(pos.x, pos.y);
        if (in_this_maze.GetBlock(pos.x, pos.y) == 'E') {
            in_this_maze.SetMaze(pos.x, pos.y, 'R');
        }
        while (!visited_route.IsEmpty()) {
            Pos next;
            if (Step(next)) {
                Walk(next);
                if (Finish()) {
                    in_this_maze.PrintVisitedRoute();
                    in_this_maze.PrintReachRoute();
                    return;
                }
            } else {
                Back();
            }
        }
        in_this_maze.PrintVisitedRoute();
        in_this_maze.Reset(original_maze);
    }

    void PutGoal(StackNode* goal) {
        while (goal != nullptr) {
            in_this_maze.SetMaze(goal->pos.x, goal->pos.y, 'G');
            goal = goal -> next;
        }
    }

    void FindGoalRequired(int goal_number) { //T2
        int count = 0;
        visited_route.push(pos.x, pos.y);
        if (in_this_maze.GetBlock(pos.x, pos.y) == 'E') {
            in_this_maze.SetMaze(pos.x, pos.y, 'R');
        }
        while (!visited_route.IsEmpty() && count < goal_number) {
            Pos next;
            if (Step(next)) {
                Walk(next);
                if (Finish()) {
                    count++;
                }
            } else {
                Back();
            }
        }
        in_this_maze.PrintVisitedRoute();
        if (count == goal_number) {
           in_this_maze.PrintReachRoute();
        }
        in_this_maze.Reset(original_maze);
    }

    void FindAllGoal() { //T3
        int count = 0;
        visited_route.push(pos.x, pos.y);
        if (in_this_maze.GetBlock(pos.x, pos.y) == 'E') {
            in_this_maze.SetMaze(pos.x, pos.y, 'R');
        }
        while (!visited_route.IsEmpty()) {
            Pos next;
            if (Step(next)) {
                Walk(next);
                if (Finish()) count++;
            } else {
                Back();
            }
        }
        in_this_maze.PrintVisitedRoute();
        cout << "The maze has " << count << " goal(s) in total" << endl;
        in_this_maze.Reset(original_maze);
    }

    void SetMaze(Maze& maze) {
        in_this_maze.Reset(maze);
        original_maze.Reset(maze);
    }

};

class ReadFileParser {
    private:
        int x;
        int y;
        char maze_dot;
    public:
        ReadFileParser() {}

        ~ReadFileParser() {}

        Maze ReadMaze(const char* filename) {
            ifstream infile("input" + string(filename) + ".txt");
            if (!infile) {
                cout << "input" << filename << ".txt does not exist!" << endl;
                return Maze(0, 0);
            }
            infile >> x;
            infile >> y;
            if (x <= 0 || y <= 0) {
                cout << "Invalid maze size!" << endl;
                return Maze(0, 0);
            }
            Maze maze(x, y);
            string line;
            for (int i = 0; i < y; i++) {          // y 行
                infile >> line;
                for (int j = 0; j < x; j++) {      // x 列
                    maze.SetMaze(j, i, line[j]);
                }
            }
            return maze;
        }
};

class MissionGenerator {
    private:
        int mission_type;
        Maze maze123;
        RecordMap stack123; 
    public:
        MissionGenerator() : maze123(0,0) {}

        ~MissionGenerator() {}

        void Mission1() {
            cout << "Input a file number: ";
            string filename;
            cin >> filename;
            ReadFileParser parser;
            Maze new_maze = parser.ReadMaze(filename.c_str());
            if (new_maze.GetX() == 0 || new_maze.GetY() == 0) {
                return;
            }
            maze123 = new_maze; 
            stack123.Clear();                
            Mouse mouse123(maze123, stack123);
            mouse123.FindGoal();
        }

        void Mission2() {
            if (maze123.GetX() == 0 || maze123.GetY() == 0) {
                cout << "### Execute command 1 to load a maze! ###" << endl;
                return;
            }
            stack123.Clear();
            Mouse mouse123(maze123, stack123);
            int goal_number;
            cout << "Number of G (goals): ";
            cin >> goal_number;
            mouse123.FindGoalRequired(goal_number);
        }

        void Mission3() {
            if (maze123.GetX() == 0 || maze123.GetY() == 0) {
                cout << "### Execute command 1 to load a maze! ###" << endl;
                return;
            }
            stack123.Clear();
            Mouse mouse123(maze123, stack123);
            mouse123.FindAllGoal();
        }

        void Mission4() {
            cout << "Input a file number: ";
            string filename;
            cin >> filename;
            ReadFileParser parser;
            Maze maze4 = parser.ReadMaze(filename.c_str());
            if (maze4.GetX() == 0 || maze4.GetY() == 0) {
                return;
            }
            RecordMap stack4;
            Mouse mouse(maze4, stack4);
            mouse.FindGoal();
        }

        void GenerateMissions() {
            while (true) {
                cout << "*** (^_^) Data Structure (^o^) ***" << endl;
                cout << "*** Find the Goal(s) in a Maze ***" << endl;
                cout << "* 0. Quit                        *" << endl;
                cout << "* 1. Find one goal               *" << endl;
                cout << "* 2. Find goal(s) as requested   *" << endl;
                cout << "* 3. How many goals?             *" << endl;
                cout << "* 4. Shortest path to one goal   *" << endl;
                cout << "**********************************" << endl;
                cout << "Input a command(0, 1, 2, 3, 4): ";
                cin >> mission_type;
                if (mission_type == 1) {
                    Mission1();
                } else if (mission_type == 2) {
                    Mission2();
                } else if (mission_type == 3) {
                    Mission3();
                } else if (mission_type == 4) {
                    Mission4();
                } else if (mission_type == 0) {
                    return;
                } else {
                    cout << "Command does not exist!" << endl;
                }
            }
        }
};

int main() {
    MissionGenerator generator;
    generator.GenerateMissions();
    return 0;
}
    
