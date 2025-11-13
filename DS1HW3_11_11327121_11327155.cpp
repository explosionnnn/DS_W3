/*11327121 侯至謙, 11327155 黃宇謙*/
#include<iostream>
#include<string>        
#include <fstream>
#include <limits>

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

        void Initialize(int x, int y) {
            this->x = x;
            this->y = y;
            maze = new char*[y];            // y rows
            for (int i = 0; i < y; i++) {
                maze[i] = new char[x];      // x columns
            }
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

        void SetMazeXY(int x, int y) {
            this -> x = x;
            this -> y = y;
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

class IntMaze {
    private:
        int** maze; // 注意:maze[y][x] (先取寬(縱軸)再取長(橫軸))
        int x; //長
        int y; //寬
    public:
        IntMaze(int x, int y) {
            this->x = x;
            this->y = y;
            maze = new int*[y];            // y rows
            for (int i = 0; i < y; i++) {
                maze[i] = new int[x];      // x columns
            }
        }

        IntMaze() {}

        IntMaze(const IntMaze& other) : IntMaze(other.x, other.y) { //Copy Constructor
        for (int i = 0; i < y; i++)
            for (int j = 0; j < x; j++)
                maze[i][j] = other.maze[i][j];
        }

        ~IntMaze() {
            DeleteMaze();
        }

        void DeleteMaze() {
            for (int i = 0; i < y; i++) {
                delete[] maze[i];
            }
            delete[] maze;
            maze = nullptr;
        }

        void Initialize(int x, int y) {
            this->x = x;
            this->y = y;
            maze = new int*[y];            // y rows
            for (int i = 0; i < y; i++) {
                maze[i] = new int[x];      // x columns
            }
        }

        int GetX() {
            return x;
        }

        int GetY() {
            return y;
        }

        int GetBlock() { // 這個好像沒用到
            return maze[y][x];
        }

        int GetBlock(int nx, int ny) {
            return maze[ny][nx];
        }

        void SetMaze(int pos_x, int pos_y, int maze_word) {
            if (pos_x >= 0 && pos_x < x && pos_y >= 0 && pos_y < y) {
                maze[pos_y][pos_x] = maze_word;
            }
        }

        void SetMazeXY(int x, int y) {
            this -> x = x;
            this -> y = y;
        }

        void PrintMaze() {
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    cout << maze[i][j];
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

        void AllSet(int num) {
            if (x > 0 && y > 0) {
                for (int i = 0; i < y; i++) {
                    for (int j = 0; j < x; j++) {
                        SetMaze(j, i, num);
                    }
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

        void AppendOtherFromTop(RecordMap &other) {
            StackNode* node = other.GetTop();
            while (node != nullptr) {
                push(node->pos.x, node->pos.y);
                node = node->next;
            }
        }

        int Length() {
            int length = 0;
            StackNode* node = top;
            while (node != nullptr) {
                length++;
                node = node->next;
            }
            return length;
        }

        void Copy(RecordMap &v_route) {
            if (!IsEmpty()) {
                Clear();
            }
            StackNode *cu = v_route.GetTop();
            while (cu != nullptr) {
                push(cu -> pos.x, cu -> pos.y);
                cu = cu -> next;
            }
        }

        bool Contain(int x, int y) {
            StackNode *cu = GetTop();
            while (cu != nullptr) {
                if (x == cu -> pos.x && y == cu -> pos.y) {
                    return true;
                }
                cu = cu -> next;
            }
            return false;
        }
};

class Mouse {
    private:
        Pos pos;
        Direction dir;
        Maze in_this_maze;        /*Maze已實作Copy Constructor，不用reference*/
        Maze original_maze; //每次task結束，還原
        RecordMap &visited_route;  /*用reference避免Mouse被刪除時連帶刪除外部RecordMap指向的物件*/
        void AllReset() {
            in_this_maze.Reset(original_maze);
            pos.x = 0;
            pos.y = 0;
            dir = RIGHT;
            visited_route.Clear();
        }
        void Walk(const Pos& next_pos) {
            pos = next_pos;
            visited_route.push(pos.x, pos.y);
            if (in_this_maze.GetBlock(pos.x, pos.y) != 'G') {// 保護 G
                in_this_maze.SetMaze(pos.x, pos.y, 'R');  //走到就設為R
            }
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
            return false;
        }

        bool Step(Pos& next_pos, IntMaze& step_arr, int step, int shortest_path) {
        // 先記下現在的方向，結束要還原
            Direction orig_dir = dir;

            for (int i = 0; i < 4; ++i) {
                int nx = pos.x;
                int ny = pos.y;

            // 依目前 dir 算下一格
                if (dir == RIGHT)      nx++;
                else if (dir == DOWN)  ny++;
                else if (dir == LEFT)  nx--;
                else if (dir == UP)    ny--;

            // 準備走的是「下一步」，所以要用 step+1 去比
                int next_step = step + 1;

            // 可以走就走
                if (CanWalkTo(nx, ny)
                    && next_step < shortest_path              // 不要比目前找到的最短還長
                    && next_step < step_arr.GetBlock(nx, ny)) {        // 這格目前沒有更好的走法
                    next_pos = {nx, ny};
                    return true;    // 這一步成功（dir 保持在成功的方向）
                }

            // 不行走就換方向再試
                ChangeDir();
            }

        // 四個方向都不能走，把方向還原
            dir = orig_dir;
            return false;
        }

    void Back() {
        if (in_this_maze.GetBlock(pos.x, pos.y) != 'G') // 保護 G
            in_this_maze.SetMaze(pos.x, pos.y, 'V');  //走到就設為R
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

    void Back_f() {
        if (in_this_maze.GetBlock(pos.x, pos.y) == 'R')
            in_this_maze.SetMaze(pos.x, pos.y, 'E');
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

    void ChangeDir() {
        dir = static_cast<Direction>(dir + 1);
        if (dir == 4) {
            dir = RIGHT;
        }
    }

    void PutGoal(StackNode* goal) {
        StackNode *cu = goal;
        while (cu != nullptr) {
            in_this_maze.SetMaze(cu->pos.x, cu->pos.y, 'G');
            cu = cu -> next;
        }
    }

    void PutR(RecordMap &shortest_route) {
        // cout << "aaaaaaa" << endl;
        StackNode *cu = shortest_route.GetTop();
        while (cu != nullptr) {
            in_this_maze.SetMaze(cu -> pos.x, cu -> pos.y, 'R');
            cu = cu -> next;
        }
    }

    void PutV(RecordMap &visit_route) {
        // cout << "aaaaaaa" << endl;
        StackNode *cu = visit_route.GetTop();
        while (cu != nullptr) {
            in_this_maze.SetMaze(cu -> pos.x, cu -> pos.y, 'V');
            cu = cu -> next;
        }
    }

    void SetMaze(Maze& maze) {
            in_this_maze.Reset(maze);
            original_maze.Reset(maze);
        }

    void MarkStackAsRoute() {
        StackNode* node = visited_route.GetTop();
        while (node != nullptr) {
            if (in_this_maze.GetBlock(node->pos.x, node->pos.y) != 'G') {
                in_this_maze.SetMaze(node->pos.x, node->pos.y, 'R');
            }
            node = node->next;
        }
    }



    public:
    Mouse(Maze& maze, RecordMap& map)
        : pos{0, 0}, dir(RIGHT), in_this_maze(maze), original_maze(maze), visited_route(map) {}


    void FindGoal() { //T1
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
                    cout << endl;
                    in_this_maze.PrintReachRoute();
                    AllReset();
                    return;
                }
            } else {
                Back();
            }
        }
        in_this_maze.PrintVisitedRoute();
        AllReset();
    }

    void FindGoalRequired(int goal_number) { //T2
        int count = 0;
        visited_route.push(pos.x, pos.y);
        if (in_this_maze.GetBlock(pos.x, pos.y) == 'E') {
            in_this_maze.SetMaze(pos.x, pos.y, 'R');
        }
        RecordMap goal;
        while (!visited_route.IsEmpty() && count < goal_number) {
            Pos next;
            if (Step(next)) {
                Walk(next);
                if (Finish()) {
                    in_this_maze.SetMaze(pos.x, pos.y, 'R'); //標記已找到的goal為R，避免重複計數
                    goal.push(pos.x, pos.y);
                    count++;
                    if (count == goal_number) {
                        MarkStackAsRoute();
                        PutGoal(goal.GetTop());
                        in_this_maze.PrintVisitedRoute();
                        cout << endl;
                        in_this_maze.PrintReachRoute();
                        in_this_maze.Reset(original_maze);
                        return;
                    }
                }
            } else {
                Back();
            }
        }
        PutGoal(goal.GetTop());
        in_this_maze.PrintVisitedRoute();
        in_this_maze.Reset(original_maze);
    }

    void FindAllGoal() { //T3
        int count = 0;
        RecordMap goal;
        visited_route.push(pos.x, pos.y);
        if (in_this_maze.GetBlock(pos.x, pos.y) == 'E') {
            in_this_maze.SetMaze(pos.x, pos.y, 'R');
        }
        while (!visited_route.IsEmpty()) {
            Pos next;
            if (Step(next)) {
                Walk(next);
                if (Finish()) {
                    count++;
                    goal.push(pos.x, pos.y);
                    in_this_maze.SetMaze(pos.x, pos.y, 'R');
                }
            } else {
                Back();
            }
        }
        PutGoal(goal.GetTop());
        in_this_maze.PrintVisitedRoute();
        cout << endl << "The maze has " << count << " goal(s) in total." << endl;
        AllReset();
    }

    void FindGoalLength() { //T4
        int step = 1;
        int shortest_path = 10000;
        RecordMap visit_route; //記拜訪過的路
        RecordMap shortest_route; // 記目前最短的路
        RecordMap goal;
        IntMaze step_arr;
        step_arr.Initialize(in_this_maze.GetX(), in_this_maze.GetY());
        step_arr.AllSet(std::numeric_limits<int>::max());
        step_arr.SetMaze(pos.x, pos.y, 1);
        visited_route.push(pos.x, pos.y);
        visit_route.push(pos.x, pos.y);
        while (!visited_route.IsEmpty()) {
            Pos next;
            if (Step(next, step_arr, step, shortest_path)) {
                Walk(next);
                step++;
                step_arr.SetMaze(pos.x, pos.y, step);
                visit_route.push(pos.x, pos.y);
                if (Finish()) {
                    goal.push(pos.x, pos.y);
                    shortest_path = step;
                    shortest_route.Copy(visited_route);
                }
            } else {
                if (in_this_maze.GetBlock(pos.x, pos.y) != 'G') {
                    in_this_maze.SetMaze(pos.x, pos.y, 'E');  // 先還原這一格
                }
                Back_f();
                step--;
            }
        }
        if (shortest_route.IsEmpty()) {
            PutV(visit_route);
            in_this_maze.PrintVisitedRoute();
            cout << endl << endl << "### There is no path to find a goal! ### ";
            return;
        }
        PutV(visit_route);
        PutR(shortest_route);
        PutGoal(goal.GetTop());
        in_this_maze.PrintVisitedRoute();
        cout << endl;
        in_this_maze.PrintReachRoute();
        cout << endl << "Shortest path length = " << shortest_path;
        AllReset();
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

        void ReadMaze(const char* filename, Maze &maze) {
            ifstream infile("input" + string(filename) + ".txt");
            if (!infile) {
                cout << endl << "input" << filename << ".txt does not exist!" << endl << endl;
                return maze.SetMazeXY(0, 0);
            }
            infile >> x;
            infile >> y;
            if (x <= 0 || y <= 0) {
                cout << "Invalid maze size!" << endl;
                maze.SetMazeXY(0,0);
                return;
            }
            maze.Initialize(x,y);
            char block;
            for (int i = 0; i < y; i++) {          // y 行
                for (int j = 0; j < x; j++) {
                    infile >> block;     // x 列
                    maze.SetMaze(j, i, block);
                }
            }
        }
};

class MissionGenerator {
    private:
        string mission_type;
        Maze maze123;
        RecordMap stack123;
        void Mission1() {
            cout << endl;
            cout << "Input a file number: ";
            string filename;
            cin >> filename;
            ReadFileParser parser;
            parser.ReadMaze(filename.c_str(), maze123);
            if (maze123.GetX() == 0 || maze123.GetY() == 0) {
                return;
            }
            stack123.Clear();
            Mouse mouse123(maze123, stack123);
            mouse123.FindGoal();
            cout << endl << endl;
        }

        void Mission2() {
            cout << endl;
            if (maze123.GetX() == 0 || maze123.GetY() == 0) {
                cout << "### Execute command 1 to load a maze! ###" << endl << endl;
                return;
            }
            stack123.Clear();
            Mouse mouse123(maze123, stack123);
            string goal_number;
            int g_num;
            while (true) {
                cout << "Number of G (goals): ";
                cin >> goal_number;
                try {
                    g_num = stoi(goal_number);
                    if (g_num < 0) {
                        cout << endl;
                        continue;
                    }
                    if (g_num > 100 || g_num == 0) {
                        cout << endl <<"### The number must be in [1,100] ###" << endl << endl;
                        continue;
                    }
                    break;
                    cout << endl;
                } catch(std::invalid_argument&) {       // 非數字輸入)
                    cout << endl;
                    continue;
                }
            }
            mouse123.FindGoalRequired(g_num);
            cout << endl << endl;
        }

        void Mission3() {
            if (maze123.GetX() == 0 || maze123.GetY() == 0) {
                cout << endl <<  "### Execute command 1 to load a maze! ###" << endl << endl;
                return;
            }
            stack123.Clear();
            Mouse mouse123(maze123, stack123);
            mouse123.FindAllGoal();
            cout << endl;
        }

        void Mission4() {
            cout << endl;
            cout << "Input a file number: ";
            string filename;
            cin >> filename;
            ReadFileParser parser;
            Maze maze4(0, 0);
            parser.ReadMaze(filename.c_str(), maze4);
            if (maze4.GetX() == 0 || maze4.GetY() == 0) {
                return;
            }
            RecordMap stack4;
            Mouse mouse(maze4, stack4);
            mouse.FindGoalLength();
            cout << endl << endl;
        }
    public:
        MissionGenerator() : maze123(0,0) {}

        ~MissionGenerator() {}

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
                if (mission_type == "1") {
                    Mission1();
                } else if (mission_type == "2") {
                    Mission2();
                } else if (mission_type == "3") {
                    Mission3();
                } else if (mission_type == "4") {
                    Mission4();
                } else if (mission_type == "0") {
                    return;
                } else {
                    cout << endl << "Command does not exist!";
                    cout << endl << endl;;
                }
            }
        }
};

int main() {
    MissionGenerator generator;
    generator.GenerateMissions();
    return 0;
}

