#include<vector>
#include<iostream>
#include<queue>
#include<map>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>

using namespace std;

int sum = 0;

struct Map_Cell
{
    int type;
    // TODO: 定义地图信息
};

struct Search_Cell
{
    int h;
    int g;
    // TODO: 定义搜索状态
    int x;
    int y;
    int food;
    Search_Cell* parent;

    int Heuristic_Funtion(int x, int y, pair<int, int> end_point);
    Search_Cell(int x,int y,int food,Search_Cell* parent, pair<int, int> end_point);

    bool operator==(const Search_Cell& other) const
    {
        return x == other.x && y == other.y && food == other.food;
    }
};

// TODO: 定义启发式函数
int Search_Cell::Heuristic_Funtion(int x, int y, pair<int, int> end_point)
{
    int dx = std::abs(x - end_point.first);
    int dy = std::abs(y - end_point.second);
    return dx + dy;
    //return 0;//一致代价搜索
}

Search_Cell::Search_Cell(int x, int y, int food, Search_Cell* parent, pair<int, int> end_point)
{
    this->x = x;
    this->y = y;
    this->food = food;
    this->parent = parent;
    this->g = (this->parent == nullptr) ? 0 : this->parent->g + 1;
    this->h = Heuristic_Funtion(x, y, end_point);
}

// 自定义比较函数对象，按照 Search_Cell 结构体的 g + h 属性进行比较，用于open_list的排序
struct CompareF1 
{
    bool operator()(const Search_Cell *a, const Search_Cell *b) const {
        return (a->g + a->h) > (b->g + b->h); // 较小的 g + h 值优先级更高
    }
};
// 自定义比较函数对象，按照 Search_Cell 结构体的 x, y, food 属性进行比较，如果都相同则返回true，用于close_list的查找
struct CompareF2 
{
    bool operator()(const Search_Cell* a, const Search_Cell* b) const {
        if (a->x == b->x && a->y == b->y && a->food == b->food)
            return true;
        else
            return false;
    }
};


void Astar_search(const string input_file, int &step_nums, string &way)
{
    ifstream file(input_file);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    getline(file, line); // 读取第一行
    stringstream ss(line);
    string word;
    vector<string> words;
    while (ss >> word) {
        words.push_back(word);
    }
    int M = stoi(words[0]);
    int N = stoi(words[1]);
    int T = stoi(words[2]);

    pair<int, int> start_point; // 起点
    pair<int, int> end_point;   // 终点
    Map_Cell **Map = new Map_Cell *[M];
    // 加载地图
    for(int i = 0; i < M; i++)
    {
        Map[i] = new Map_Cell[N];
        getline(file, line);
        stringstream ss(line);
        string word;
        vector<string> words;
        while (ss >> word) {
            words.push_back(word);
        }
        for(int j = 0; j < N; j++)
        {
            Map[i][j].type = stoi(words[j]);
            if(Map[i][j].type == 3)
            {
                start_point = {i, j};
            }
            else if(Map[i][j].type == 4)
            {
                end_point = {i, j};
            }
        }
    }
    // 以上为预处理部分
    // ------------------------------------------------------------------

    Search_Cell* search_cell = new Search_Cell(start_point.first, start_point.second, T, nullptr, end_point);
    priority_queue<Search_Cell *, vector<Search_Cell *>, CompareF1> open_list;
    vector<Search_Cell *> close_list;
    open_list.push(search_cell);

    while(!open_list.empty())
    {
        // TODO: A*搜索过程实现
        sum++;//用于检查性能
        Search_Cell* best = open_list.top();
        open_list.pop();
        if (best->x == end_point.first && best->y == end_point.second)
        {
            //找到最短路径
            step_nums = 0;
            while (best->parent != nullptr)
            {
                step_nums++;
                if (best->parent->x < best->x)
                    way += "D";
                else if (best->parent->x > best->x)
                    way += "U";
                else if (best->parent->y < best->y)
                    way += "R";
                else
                    way += "L";
                best = best->parent;
            }
            std::reverse(way.begin(), way.end());
            return;
        }
        else
        {
            if (best->x + 1 < M && Map[best->x + 1][best->y].type != 1)//向下有路
            {
                int food;
                if (Map[best->x + 1][best->y].type == 2)
                    food = T;
                else
                    food = best->food - 1;
                if (food > 0)//向下物资不会耗尽
                {
                    Search_Cell* D = new Search_Cell(best->x + 1, best->y, food, best, end_point);
                    if (std::find_if(close_list.begin(), close_list.end(), [&](const Search_Cell* ptr) {
                        return (ptr->x) == (D->x) && (ptr->y) == (D->y) && (ptr->food) == (D->food);
                        }) == close_list.end())//新状态没被探索过
                    {
                        close_list.push_back(D);
                        open_list.push(D);
                    }
                    else
                    {
                        delete D;
                    }
                }
            }
            if (best->x - 1 >=0 && Map[best->x - 1][best->y].type != 1)//向上有路
            {
                int food;
                if (Map[best->x - 1][best->y].type == 2)
                    food = T;
                else
                    food = best->food - 1;
                if (food > 0)//向上物资不会耗尽
                {
                    Search_Cell* U = new Search_Cell(best->x - 1, best->y, food, best, end_point);
                    if (std::find_if(close_list.begin(), close_list.end(), [&](const Search_Cell* ptr){
                            return (ptr->x) == (U->x) && (ptr->y) == (U->y) && (ptr->food) == (U->food);
                        }) == close_list.end())//新状态没被探索过
                    {
                        close_list.push_back(U);
                        open_list.push(U);
                    }
                    else
                    {
                        delete U;
                    }
                }
            }
            if (best->y + 1 < M && Map[best->x][best->y + 1].type != 1)//向右有路
            {
                int food;
                if (Map[best->x][best->y + 1].type == 2)
                    food = T;
                else
                    food = best->food - 1;
                if (food > 0)//向右物资不会耗尽
                {
                    Search_Cell* R = new Search_Cell(best->x, best->y + 1, food, best, end_point);
                    if (std::find_if(close_list.begin(), close_list.end(), [&](const Search_Cell* ptr) {
                        return (ptr->x) == (R->x) && (ptr->y) == (R->y) && (ptr->food) == (R->food);
                        }) == close_list.end())//新状态没被探索过
                    {
                        close_list.push_back(R);
                        open_list.push(R);
                    }
                    else
                    {
                        delete R;
                    }
                }
            }
            if (best->y - 1 >=0 && Map[best->x][best->y - 1].type != 1)//向左有路
            {
                int food;
                if (Map[best->x][best->y - 1].type == 2)
                    food = T;
                else
                    food = best->food - 1;
                if (food > 0)//向左物资不会耗尽
                {
                    Search_Cell* L = new Search_Cell(best->x, best->y - 1, food, best, end_point);
                    if (std::find_if(close_list.begin(), close_list.end(), [&](const Search_Cell* ptr) {
                        return (ptr->x) == (L->x) && (ptr->y) == (L->y) && (ptr->food) == (L->food);
                        }) == close_list.end())//新状态没被探索过
                    {
                        close_list.push_back(L);
                        open_list.push(L);
                    }
                    else
                    {
                        delete L;
                    }
                }
            }
        }

    }

    // ------------------------------------------------------------------
    // TODO: 填充step_nums与way
    step_nums = -1;
    way = "";

    // ------------------------------------------------------------------
    // 释放动态内存
    for(int i = 0; i < M; i++)
    {
        delete[] Map[i];
    }
    delete[] Map;
    while(!open_list.empty())
    {
        auto temp = open_list.top();
        delete[] temp;
        open_list.pop();
    }
    for(int i = 0; i < close_list.size(); i++)
    {
        delete[] close_list[i];
    }

    return;
}

void output(const string output_file, int &step_nums, string &way)
{
    ofstream file(output_file);
    if(file.is_open())
    {
        file << step_nums << endl;
        if(step_nums >= 0)
        {
            file << way << endl;
        }

        file.close();
    }
    else
    {
        cerr << "Can not open file: " << output_file << endl;
    }
    return;
}

int main(int argc, char *argv[])
{
    string input_base = "../input/input_";
    string output_base = "../output/output_";
    // input_0为讲义样例，此处不做测试
    for(int i = 0; i < 11; i++)
    {
        int step_nums = -1;
        string way = "";
        Astar_search(input_base + to_string(i) + ".txt", step_nums, way);
        output(output_base + to_string(i) + ".txt", step_nums, way);
    }
    printf("sum = %d\n", sum);
    return 0;
}