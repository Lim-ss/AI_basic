#include <vector>
#include <map>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>

#define ALPHA_BETA

namespace ChineseChess
{
    int sum = 0;
    //棋力评估，这里的棋盘方向和输入棋盘方向不同，在使用时需要仔细
    //生成合法动作代码部分已经使用，经过测试是正确的，大家可以参考
    std::vector<std::vector<int>> JiangPosition = {
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {1, -8, -9, 0, 0, 0, 0, 0, 0, 0},
                                                {5, -8, -9, 0, 0, 0, 0, 0, 0, 0},
                                                {1, -8, -9, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                            };

    std::vector<std::vector<int>> ShiPosition = {
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                            };
        
    std::vector<std::vector<int>> XiangPosition = {
                                                {0, 0, -2, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 3, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, -2, 0, 0, 0, 0, 0, 0, 0},
                                            };

    std::vector<std::vector<int>> MaPosition = {
                                                {0, -3, 5, 4, 2, 2, 5, 4, 2, 2},
                                                {-3, 2, 4, 6, 10, 12, 20, 10, 8, 2},
                                                {2, 4, 6, 10, 13, 11, 12, 11, 15, 2},
                                                {0, 5, 7, 7, 14, 15, 19, 15, 9, 8},
                                                {2, -10, 4, 10, 15, 16, 12, 11, 6, 2},
                                                {0, 5, 7, 7, 14, 15, 19, 15, 9, 8},
                                                {2, 4, 6, 10, 13, 11, 12, 11, 15, 2},
                                                {-3, 2, 4, 6, 10, 12, 20, 10, 8, 2},
                                                {0, -3, 5, 4, 2, 2, 5, 4, 2, 2},
                                            };

    std::vector<std::vector<int>> PaoPosition = {
                                                {0, 0, 1, 0, -1, 0, 0, 1, 2, 4},
                                                {0, 1, 0, 0, 0, 0, 3, 1, 2, 4},
                                                {1, 2, 4, 0, 3, 0, 3, 0, 0, 0},
                                                {3, 2, 3, 0, 0, 0, 2, -5, -4, -5},
                                                {3, 2, 5, 0, 4, 4, 4, -4, -7, -6},
                                                {3, 2, 3, 0, 0, 0, 2, -5, -4, -5},
                                                {1, 2, 4, 0, 3, 0, 3, 0, 0, 0},
                                                {0, 1, 0, 0, 0, 0, 3, 1, 2, 4},
                                                {0, 0, 1, 0, -1, 0, 0, 1, 2, 4},
                                            };

    std::vector<std::vector<int>> JuPosition = {
                                                {-6, 5, -2, 4, 8, 8, 6, 6, 6, 6},
                                                {6, 8, 8, 9, 12, 11, 13, 8, 12, 8},
                                                {4, 6, 4, 4, 12, 11, 13, 7, 9, 7},
                                                {12, 12, 12, 12, 14, 14, 16, 14, 16, 13},
                                                {0, 0, 12, 14, 15, 15, 16, 16, 33, 14},
                                                {12, 12, 12, 12, 14, 14, 16, 14, 16, 13},
                                                {4, 6, 4, 4, 12, 11, 13, 7, 9, 7},
                                                {6, 8, 8, 9, 12, 11, 13, 8, 12, 8},
                                                {-6, 5, -2, 4, 8, 8, 6, 6, 6, 6},
                                            };

    std::vector<std::vector<int>> BingPosition = {
                                                {0, 0, 0, -2, 3, 10, 20, 20, 20, 0},
                                                {0, 0, 0, 0, 0, 18, 27, 30, 30, 0},
                                                {0, 0, 0, -2, 4, 22, 30, 45, 50, 0},
                                                {0, 0, 0, 0, 0, 35, 40, 55, 65, 2},
                                                {0, 0, 0, 6, 7, 40, 42, 55, 70, 4},
                                                {0, 0, 0, 0, 0, 35, 40, 55, 65, 2},
                                                {0, 0, 0, -2, 4, 22, 30, 45, 50, 0},
                                                {0, 0, 0, 0, 0, 18, 27, 30, 30, 0},
                                                {0, 0, 0, -2, 3, 10, 20, 20, 20, 0},
                                            };

    //棋子价值评估
    std::map<std::string, int> piece_values = {
                                            {"Jiang", 10000},
                                            {"Shi", 10},
                                            {"Xiang", 30},
                                            {"Ma", 300},
                                            {"Ju", 500},
                                            {"Pao", 300},
                                            {"Bing", 90}
                                        };

    //行棋可能性评估，这里更多是对下一步动作的评估
    std::map<std::string, int> next_move_values = {
                                            {"Jiang", 9999},
                                            {"Ma", 100},
                                            {"Ju", 500},
                                            {"Pao", 100},
                                            {"Bing", -20}
                                        };
    
    //动作结构体，每个动作设置score，可以方便剪枝
    struct Move {
        int init_x;
        int init_y;
        int next_x;
        int next_y;
        int score;
    };

    // 定义棋盘上的棋子结构体
    struct ChessPiece {
        char name; // 棋子名称
        int init_x, init_y; // 棋子的坐标
        bool color;   //棋子阵营 true为红色、false为黑色
    };

    // 定义棋盘类
    class ChessBoard {
    private:
        int sizeX, sizeY;   //棋盘大小，固定
        std::vector<ChessPiece> pieces;   //棋盘上所有棋子
        std::vector<std::vector<char>> board;    //当前棋盘、二维数组表示
        std::vector<Move> red_moves;    //红方棋子的合法动作
        std::vector<Move> black_moves;   //黑方棋子的合法动作
    public:
        // 初始化棋盘，提取棋盘上棋子，并生成所有合法动作
        void initializeBoard(const std::vector<std::vector<char>>& init_board) {
            board = init_board;
            sizeX = board.size();
            sizeY = board[0].size();

            for (int i = 0; i < sizeX; ++i) {
                for (int j = 0; j < sizeY; ++j) {
                    char pieceChar = board[i][j];
                    if (pieceChar == '.') continue;

                    ChessPiece piece;
                    piece.init_x = j;
                    piece.init_y = i;
                    piece.color = (pieceChar >= 'A' && pieceChar <= 'Z');
                    piece.name = pieceChar;
                    pieces.push_back(piece);

                    switch (pieceChar) {
                        case 'R':
                            generateJuMoves(j, i, piece.color);
                            break;
                        case 'C':
                            generatePaoMoves(j, i, piece.color);
                            break;
                        case 'N':
                            generateMaMoves(j, i, piece.color);
                            break;
                        case 'B':
                            generateXiangMoves(j, i, piece.color);
                            break;
                        case 'A':
                            generateShiMoves(j, i, piece.color);
                            break;
                        case 'K':
                            generateJiangMoves(j, i, piece.color);
                            break;
                        case 'P':
                            generateBingMoves(j, i, piece.color);
                            break;
                        case 'r':
                            generateJuMoves(j, i, piece.color);
                            break;
                        case 'c':
                            generatePaoMoves(j, i, piece.color);
                            break;
                        case 'n':
                            generateMaMoves(j, i, piece.color);
                            break;
                        case 'b':
                            generateXiangMoves(j, i, piece.color);
                            break;
                        case 'a':
                            generateShiMoves(j, i, piece.color);
                            break;
                        case 'k':
                            generateJiangMoves(j, i, piece.color);
                            break;
                        case 'p':
                            generateBingMoves(j, i, piece.color);
                            break;
                        default:
                            break;
                    }
                } 
            }
        }
        
        //生成车的合法动作
        void generateJuMoves(int x, int y, bool color) {
            //前后左右分别进行搜索，遇到棋子停止，不同阵营可以吃掉
            std::vector<Move> JuMoves;
            for(int i = x + 1; i < sizeY; i++) {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = i;
                cur_move.next_y = y;
                cur_move.score = 0;
                if (board[y][i] != '.') {
                    bool cur_color = (board[y][i] >= 'A' && board[y][i] <= 'Z');
                    if (cur_color != color) {
                        JuMoves.push_back(cur_move);
                    }
                    break;
                }
                JuMoves.push_back(cur_move);
            }

            for(int i = x - 1; i >= 0; i--) {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = i;
                cur_move.next_y = y;
                cur_move.score = 0;
                if (board[y][i] != '.') {
                    bool cur_color = (board[y][i] >= 'A' && board[y][i] <= 'Z');
                    if (cur_color != color) {
                        JuMoves.push_back(cur_move);
                    }
                    break;
                }
                JuMoves.push_back(cur_move);
            }

            for(int j = y + 1; j < sizeX; j++) {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = x;
                cur_move.next_y = j;
                cur_move.score = 0;
                if (board[j][x] != '.') {
                    bool cur_color = (board[j][x] >= 'A' && board[j][x] <= 'Z');
                    if (cur_color != color) {
                        JuMoves.push_back(cur_move);
                    }
                    break;
                }
                JuMoves.push_back(cur_move);
            }

            for(int j = y - 1; j >= 0; j--) {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = x;
                cur_move.next_y = j;
                cur_move.score = 0;
                if (board[j][x] != '.') {
                    bool cur_color = (board[j][x] >= 'A' && board[j][x] <= 'Z');
                    if (cur_color != color) {
                        JuMoves.push_back(cur_move);
                    }
                    break;
                }
                JuMoves.push_back(cur_move); 
            }
            for (int i = 0; i < JuMoves.size(); i++) {
                if(color) {
                    JuMoves[i].score = JuPosition[JuMoves[i].next_x][9 - JuMoves[i].next_y] - JuPosition[x][9 -y];
                    red_moves.push_back(JuMoves[i]);
                }
                else {
                    JuMoves[i].score = JuPosition[JuMoves[i].next_x][JuMoves[i].next_y] - JuPosition[x][y];
                    black_moves.push_back(JuMoves[i]);
                }
            }
        }

        //生成马的合法动作
        void generateMaMoves(int x, int y, bool color) {
            //遍历所有可能动作，筛选
            std::vector<Move> MaMoves;
            int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
            int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
            int majiao_x[] = { 1, 0, 0, -1, -1, 0, 0, 1 };
            int majiao_y[] = { 0, 1, 1, 0, 0, -1, -1, 0 };
            //简化，不考虑拌马脚
            //TODO 可以实现拌马脚过程
            for(int i = 0; i < 8; i++) {
                Move cur_move;
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx < 0 || nx >= 9 || ny < 0 || ny >= 10) continue;
                if (board[y + majiao_y[i]][x + majiao_x[i]] != '.') continue;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = nx;
                cur_move.next_y = ny;
                cur_move.score = 0;
                if (board[ny][nx] != '.') {
                    //注意棋盘坐标系，这里nx、ny相反是正确的
                    bool cur_color = (board[ny][nx] >= 'A' && board[ny][nx] <= 'Z');
                    if (cur_color != color) {
                        MaMoves.push_back(cur_move);
                    }
                    continue;
                }
                MaMoves.push_back(cur_move);
            }
            for (int i = 0; i < MaMoves.size(); i++) {
                if(color) {
                    MaMoves[i].score = MaPosition[MaMoves[i].next_x][9 - MaMoves[i].next_y] - MaPosition[x][9 - y];
                    red_moves.push_back(MaMoves[i]);
                }
                else {
                    MaMoves[i].score = MaPosition[MaMoves[i].next_x][MaMoves[i].next_y] - MaPosition[x][y];
                    black_moves.push_back(MaMoves[i]);
                }
            }
        }

        //生成炮的合法动作
        void generatePaoMoves(int x, int y, bool color) {
            //和车生成动作相似，需要考虑炮翻山吃子的情况
            std::vector<Move> PaoMoves;
            //TODO
            for (int i = x + 1; i < sizeY; i++) 
            {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = i;
                cur_move.next_y = y;
                cur_move.score = 0;
                if (board[y][i] != '.') 
                {
                    i++;
                    while (i < sizeY)
                    {
                        if (board[y][i] != '.')
                        {
                            bool cur_color = (board[y][i] >= 'A' && board[y][i] <= 'Z');
                            if (cur_color != color) 
                            {
                                cur_move.next_x = i;
                                cur_move.next_y = y;
                                PaoMoves.push_back(cur_move);
                            }
                            break;
                        }
                        i++;
                    }
                    break;
                }
                PaoMoves.push_back(cur_move);
            }
            for (int i = x - 1; i >= 0; i--) 
            {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = i;
                cur_move.next_y = y;
                cur_move.score = 0;
                if (board[y][i] != '.')
                {
                    i--;
                    while (i >= 0)
                    {
                        if (board[y][i] != '.')
                        {
                            bool cur_color = (board[y][i] >= 'A' && board[y][i] <= 'Z');
                            if (cur_color != color)
                            {
                                cur_move.next_x = i;
                                cur_move.next_y = y;
                                PaoMoves.push_back(cur_move);
                            }
                            break;
                        }
                        i--;
                    }
                    break;
                }
                PaoMoves.push_back(cur_move);
            }
            for (int j = y + 1; j < sizeX; j++)
            {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = x;
                cur_move.next_y = j;
                cur_move.score = 0;
                if (board[j][x] != '.')
                {
                    j++;
                    while (j < sizeX)
                    {
                        if (board[j][x] != '.')
                        {
                            bool cur_color = (board[j][x] >= 'A' && board[j][x] <= 'Z');
                            if (cur_color != color)
                            {
                                cur_move.next_x = x;
                                cur_move.next_y = j;
                                PaoMoves.push_back(cur_move);
                            }
                            break;
                        }
                       j++;
                    }
                    break;
                }
                PaoMoves.push_back(cur_move);
            }
            for (int j = y - 1; j >= 0; j--)
            {
                Move cur_move;
                cur_move.init_x = x;
                cur_move.init_y = y;
                cur_move.next_x = x;
                cur_move.next_y = j;
                cur_move.score = 0;
                if (board[j][x] != '.')
                {
                    j--;
                    while (j >= 0)
                    {
                        if (board[j][x] != '.')
                        {
                            bool cur_color = (board[j][x] >= 'A' && board[j][x] <= 'Z');
                            if (cur_color != color)
                            {
                                cur_move.next_x = x;
                                cur_move.next_y = j;
                                PaoMoves.push_back(cur_move);
                            }
                            break;
                        }
                        j--;
                    }
                    break;
                }
                PaoMoves.push_back(cur_move);
            }

            for (int i = 0; i < PaoMoves.size(); i++) {
                if(color) {
                    PaoMoves[i].score = PaoPosition[PaoMoves[i].next_x][9 - PaoMoves[i].next_y] - PaoPosition[x][9 -y];
                    red_moves.push_back(PaoMoves[i]);
                }
                else {
                    PaoMoves[i].score = PaoPosition[PaoMoves[i].next_x][PaoMoves[i].next_y] - PaoPosition[x][y];
                    black_moves.push_back(PaoMoves[i]);
                }
            }
        }

        //生成相的合法动作
        void generateXiangMoves(int x, int y, bool color) {
            std::vector<Move> XiangMoves;
            //TODO
            Move cur_move;
            cur_move.init_x = x;
            cur_move.init_y = y;
            cur_move.score = 0;
            if (color)//红棋
            {
                if (x + 2 < sizeY && y + 2 < sizeX)
                {
                    if (board[y + 1][x + 1] != '.')
                    {
                        
                        cur_move.next_x = x + 2;
                        cur_move.next_y = y + 2;
                        if (board[y + 2][x + 2] == '.' || !(board[y + 2][x + 2] >= 'A' && board[y + 2][x + 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
                if (x + 2 < sizeY && y - 2 >= sizeX / 2)
                {
                    if (board[y - 1][x + 1] != '.')
                    {
                        cur_move.next_x = x + 2;
                        cur_move.next_y = y - 2;
                        if (board[y - 2][x + 2] == '.' || (board[y - 2][x + 2] >= 'A' && board[y - 2][x + 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
                if (x - 2 >= 0 && y + 2 < sizeX)
                {
                    if (board[y + 1][x - 1] != '.')
                    {
                        cur_move.next_x = x - 2;
                        cur_move.next_y = y + 2;
                        if (board[y + 2][x - 2] == '.' || (board[y + 2][x - 2] >= 'A' && board[y + 2][x - 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
                if (x - 2 >= 0 && y - 2 >= sizeX / 2)
                {
                    if (board[y - 1][x - 1] != '.')
                    {
                        cur_move.next_x = x - 2;
                        cur_move.next_y = y - 2;
                        if (board[y - 2][x - 2] == '.' || (board[y - 2][x - 2] >= 'A' && board[y - 2][x - 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
            }
            else//黑棋
            {
                if (x + 2 < sizeY && y + 2 < sizeX / 2)
                {
                    if (board[y + 1][x + 1] != '.')
                    {
                        cur_move.next_x = x + 2;
                        cur_move.next_y = y + 2;
                        if (board[y + 2][x + 2] == '.' || (board[y + 2][x + 2] >= 'A' && board[y + 2][x + 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
                if (x + 2 < sizeY && y - 2 >= 0)
                {
                    if (board[y - 1][x + 1] != '.')
                    {
                        cur_move.next_x = x + 2;
                        cur_move.next_y = y - 2;
                        if (board[y - 2][x + 2] == '.' || (board[y - 2][x + 2] >= 'A' && board[y - 2][x + 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
                if (x - 2 >= 0 && y + 2 < sizeX / 2)
                {
                    if (board[y + 1][x - 1] != '.')
                    {
                        cur_move.next_x = x - 2;
                        cur_move.next_y = y + 2;
                        if (board[y + 2][x - 2] == '.' || (board[y + 2][x - 2] >= 'A' && board[y + 2][x - 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
                if (x - 2 >= 0 && y - 2 >= 0)
                {
                    if (board[y - 1][x - 1] != '.')
                    {
                        cur_move.next_x = x - 2;
                        cur_move.next_y = y - 2;
                        if (board[y - 2][x - 2] == '.' || (board[y - 2][x - 2] >= 'A' && board[y - 2][x - 2] <= 'Z'))
                        {
                            XiangMoves.push_back(cur_move);
                        }
                    }
                }
            }
            
            for (int i = 0; i < XiangMoves.size(); i++) {
                if(color) {
                    XiangMoves[i].score = XiangPosition[XiangMoves[i].next_x][9 - XiangMoves[i].next_y] - XiangPosition[x][9 -y];
                    red_moves.push_back(XiangMoves[i]);
                }
                else {
                    XiangMoves[i].score = XiangPosition[XiangMoves[i].next_x][XiangMoves[i].next_y] - XiangPosition[x][y];
                    black_moves.push_back(XiangMoves[i]);
                }
            }
        }

        //生成士的合法动作
        void generateShiMoves(int x, int y, bool color) {
            std::vector<Move> ShiMoves;
            //TODO
            Move cur_move;
            cur_move.init_x = x;
            cur_move.init_y = y;
            cur_move.score = 0;
            if (color)//红棋
            {
                if (x == 4)//在中间
                {
                    if (board[9][3] == '.' || !(board[9][3] >= 'A' && board[9][3] <= 'Z'))
                    {
                        cur_move.next_x = 3;
                        cur_move.next_y = 9;
                        ShiMoves.push_back(cur_move);
                    }
                    if (board[9][5] == '.' || !(board[9][5] >= 'A' && board[9][5] <= 'Z'))
                    {
                        cur_move.next_x = 5;
                        cur_move.next_y = 9;
                        ShiMoves.push_back(cur_move);
                    }
                    if (board[7][3] == '.' || !(board[7][3] >= 'A' && board[7][3] <= 'Z'))
                    {
                        cur_move.next_x = 3;
                        cur_move.next_y = 7;
                        ShiMoves.push_back(cur_move);
                    }
                    if (board[7][5] == '.' || !(board[7][5] >= 'A' && board[7][5] <= 'Z'))
                    {
                        cur_move.next_x = 5;
                        cur_move.next_y = 7;
                        ShiMoves.push_back(cur_move);
                    }
                }
                else//不在中间
                {
                    if (board[8][4] == '.' || !(board[8][4] >= 'A' && board[8][4] <= 'Z'))
                    {
                        cur_move.next_x = 4;
                        cur_move.next_y = 8;
                        ShiMoves.push_back(cur_move);
                    }
                }
            }
            else//黑棋
            {
                if (x == 4)//在中间
                {
                    if (board[2][3] == '.' || (board[2][3] >= 'A' && board[2][3] <= 'Z'))
                    {
                        cur_move.next_x = 3;
                        cur_move.next_y = 2;
                        ShiMoves.push_back(cur_move);
                    }
                    if (board[2][5] == '.' || (board[2][5] >= 'A' && board[2][5] <= 'Z'))
                    {
                        cur_move.next_x = 5;
                        cur_move.next_y = 2;
                        ShiMoves.push_back(cur_move);
                    }
                    if (board[0][3] == '.' || (board[0][3] >= 'A' && board[0][3] <= 'Z'))
                    {
                        cur_move.next_x = 3;
                        cur_move.next_y = 0;
                        ShiMoves.push_back(cur_move);
                    }
                    if (board[0][5] == '.' || (board[0][5] >= 'A' && board[0][5] <= 'Z'))
                    {
                        cur_move.next_x = 5;
                        cur_move.next_y = 0;
                        ShiMoves.push_back(cur_move);
                    }
                }
                else//不在中间
                {
                    if (board[8][4] == '.' || (board[8][4] >= 'A' && board[8][4] <= 'Z'))
                    {
                        cur_move.next_x = 4;
                        cur_move.next_y = 1;
                        ShiMoves.push_back(cur_move);
                    }
                }
            }

            for (int i = 0; i < ShiMoves.size(); i++) {
                if(color) {
                    ShiMoves[i].score = ShiPosition[ShiMoves[i].next_x][9 - ShiMoves[i].next_y] - ShiPosition[x][9 -y];
                    red_moves.push_back(ShiMoves[i]);
                }
                else {
                    ShiMoves[i].score = ShiPosition[ShiMoves[i].next_x][ShiMoves[i].next_y] - ShiPosition[x][y];
                    black_moves.push_back(ShiMoves[i]);
                }
            }
        }

        //生成将的合法动作
        void generateJiangMoves(int x, int y, bool color) {
            std::vector<Move> JiangMoves;
            //TODO
            Move cur_move;
            cur_move.init_x = x;
            cur_move.init_y = y;
            cur_move.score = 0;
            if (color)//红棋
            {
                cur_move.next_x = x + 1;
                cur_move.next_y = y;
                if (cur_move.next_x <= 5)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                cur_move.next_x = x - 1;
                cur_move.next_y = y;
                if (cur_move.next_x >= 3)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                cur_move.next_x = x;
                cur_move.next_y = y + 1;
                if (cur_move.next_y <= 9)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                cur_move.next_x = x;
                cur_move.next_y = y - 1;
                if (cur_move.next_y >= 7)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                {
                    //飞将
                    int j;
                    for (j = y - 1;j >= 0 && board[j][x] == '.';j--)
                        ;
                    if (j >= 0 && board[j][x] == 'k')
                    {
                        cur_move.next_x = x;
                        cur_move.next_y = j;
                        JiangMoves.push_back(cur_move);
                    }
                }
                
            }
            else//黑棋
            {
                cur_move.next_x = x + 1;
                cur_move.next_y = y;
                if (cur_move.next_x <= 5)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        (board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                cur_move.next_x = x - 1;
                cur_move.next_y = y;
                if (cur_move.next_x >= 3)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        (board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                cur_move.next_x = x;
                cur_move.next_y = y + 1;
                if (cur_move.next_y <= 2)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        (board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                cur_move.next_x = x;
                cur_move.next_y = y - 1;
                if (cur_move.next_y >= 0)
                {
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        (board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        JiangMoves.push_back(cur_move);
                    }
                }
                {
                    //飞将
                    int j;
                    for (j = y + 1;j < sizeX && board[j][x] == '.';j++)
                        ;
                    if (j < sizeX && board[j][x] == 'K')
                    {
                        cur_move.next_x = x;
                        cur_move.next_y = j;
                        JiangMoves.push_back(cur_move);
                    }
                }
            }

            for (int i = 0; i < JiangMoves.size(); i++) {
                if(color) {
                    JiangMoves[i].score = JiangPosition[JiangMoves[i].next_x][9 - JiangMoves[i].next_y] - JiangPosition[x][9 -y];
                    red_moves.push_back(JiangMoves[i]);
                }
                else {
                    JiangMoves[i].score = JiangPosition[JiangMoves[i].next_x][JiangMoves[i].next_y] - JiangPosition[x][y];
                    black_moves.push_back(JiangMoves[i]);
                }
            }
        }

        //生成兵的合法动作
        void generateBingMoves(int x, int y, bool color) {
            //需要分条件考虑，小兵在过楚河汉界之前只能前进，之后可以左右前
            std::vector<Move> BingMoves;
            //TODO
            Move cur_move;
            cur_move.init_x = x;
            cur_move.init_y = y;
            cur_move.score = 0;
            if (color)//红棋·
            {
                if (y > 4)//没过河
                {
                    cur_move.next_x = x;
                    cur_move.next_y = y - 1;
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        BingMoves.push_back(cur_move);
                    }
                }
                else
                {
                    if (y - 1 >= 0)
                    {
                        cur_move.next_x = x;
                        cur_move.next_y = y - 1;
                        if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                            !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                        {
                            BingMoves.push_back(cur_move);
                        }
                    }
                    if (x - 1 >= 0)
                    {
                        cur_move.next_x = x - 1;
                        cur_move.next_y = y;
                        if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                            !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                        {
                            BingMoves.push_back(cur_move);
                        }
                    }
                    if (x + 1 < sizeY)
                    {
                        cur_move.next_x = x + 1;
                        cur_move.next_y = y;
                        if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                            !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                        {
                            BingMoves.push_back(cur_move);
                        }
                    }
                }
            }
            else//黑棋
            {
                if (y <= 4)//没过河
                {
                    cur_move.next_x = x;
                    cur_move.next_y = y + 1;
                    if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                        !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                    {
                        BingMoves.push_back(cur_move);
                    }
                }
                else
                {
                    if (y + 1 < sizeX)
                    {
                        cur_move.next_x = x;
                        cur_move.next_y = y + 1;
                        if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                            !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                        {
                            BingMoves.push_back(cur_move);
                        }
                    }
                    if (x - 1 >= 0)
                    {
                        cur_move.next_x = x - 1;
                        cur_move.next_y = y;
                        if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                            !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                        {
                            BingMoves.push_back(cur_move);
                        }
                    }
                    if (x + 1 < sizeY)
                    {
                        cur_move.next_x = x + 1;
                        cur_move.next_y = y;
                        if (board[cur_move.next_y][cur_move.next_x] == '.' ||
                            !(board[cur_move.next_y][cur_move.next_x] >= 'A' && board[cur_move.next_y][cur_move.next_x] <= 'Z'))
                        {
                            BingMoves.push_back(cur_move);
                        }
                    }
                }
            }

            for (int i = 0; i < BingMoves.size(); i++) {
                if(color) {
                    BingMoves[i].score = BingPosition[BingMoves[i].next_x][9 - BingMoves[i].next_y] - BingPosition[x][9 -y];
                    red_moves.push_back(BingMoves[i]);
                }
                else {
                    BingMoves[i].score = BingPosition[BingMoves[i].next_x][BingMoves[i].next_y] - BingPosition[x][y];
                    black_moves.push_back(BingMoves[i]);
                }
            }
        }
    
        //终止判断
        bool judgeTermination() {
            //TODO
            return false;
        }

        //棋盘分数评估，根据当前棋盘进行棋子价值和棋力评估，max玩家减去min玩家分数
        int evaluateNode() {
            //TODO
            int score = 0;
            for (int i = 0;i < pieces.size();i++)
            {
                auto piece = pieces[i];
                if (piece.color)//红棋
                {
                    if (piece.name == 'R')
                        score += JuPosition[piece.init_x][9 - piece.init_y] + piece_values["Ju"];
                    if (piece.name == 'N')
                        score += MaPosition[piece.init_x][9 - piece.init_y] + piece_values["Ma"];
                    if (piece.name == 'B')
                        score += XiangPosition[piece.init_x][9 - piece.init_y] + piece_values["Xiang"];
                    if (piece.name == 'A')
                        score += ShiPosition[piece.init_x][9 - piece.init_y] + piece_values["Shi"];
                    if (piece.name == 'K')
                        score += JiangPosition[piece.init_x][9 - piece.init_y] + piece_values["Jiang"];
                    if (piece.name == 'C')
                        score += PaoPosition[piece.init_x][9 - piece.init_y] + piece_values["Pao"];
                    if (piece.name == 'P')
                        score += BingPosition[piece.init_x][9 - piece.init_y] + piece_values["Bing"];
                }
                else//黑棋
                {
                    if (piece.name == 'r')
                        score -= JuPosition[piece.init_x][9 - piece.init_y] + piece_values["Ju"];
                    if (piece.name == 'n')
                        score -= MaPosition[piece.init_x][9 - piece.init_y] + piece_values["Ma"];
                    if (piece.name == 'b')
                        score -= XiangPosition[piece.init_x][9 - piece.init_y] + piece_values["Xiang"];
                    if (piece.name == 'a')
                        score -= ShiPosition[piece.init_x][9 - piece.init_y] + piece_values["Shi"];
                    if (piece.name == 'k')
                        score -= JiangPosition[piece.init_x][9 - piece.init_y] + piece_values["Jiang"];
                    if (piece.name == 'c')
                        score -= PaoPosition[piece.init_x][9 - piece.init_y] + piece_values["Pao"];
                    if (piece.name == 'p')
                        score -= BingPosition[piece.init_x][9 - piece.init_y] + piece_values["Bing"];
                }
            }
            return score;
        }

        //测试接口
        std::vector<Move> getMoves(bool color) {
            if(color) return red_moves;
            return black_moves;
        }
    
        std::vector<ChessPiece> getChessPiece() {
            return pieces;
        }
    
        std::vector<std::vector<char>> getBoard() {
            return board;
        }
    };

    // 定义博弈树节点类
    class GameTreeNode {
    private:
        bool color; // 当前玩家类型，true为红色方、false为黑色方
        ChessBoard board; // 当前棋盘状态
        std::vector<GameTreeNode*> children; // 子节点列表
        int evaluationScore; // 棋盘评估分数
        int alpha;
        int beta;

    public:
        // 构造函数
        GameTreeNode(bool color, std::vector<std::vector<char>> initBoard, int evaluationScore)
            : color(color), evaluationScore(evaluationScore), alpha(std::numeric_limits<int>::min()), beta(std::numeric_limits<int>::max())
        {
            board.initializeBoard(initBoard);
            children.clear();
            //std::vector<Move> moves = board.getMoves(color);
            //std::vector<std::vector<char>> cur_board = board.getBoard();


            //先不要创建所有子节点
            
            // 为合法动作创建子节点
            // for (int i = 0; i < moves.size(); i++) 
            // {
            //     GameTreeNode* child = updateBoard(cur_board, moves[i], color);
            //     children.push_back(child);
            // }
        }

        //根据当前棋盘和动作构建新棋盘（子节点）
        GameTreeNode* updateBoard(std::vector<std::vector<char>> cur_board, Move move, bool color) 
        {
            //TODO
            cur_board[move.next_y][move.next_x] = cur_board[move.init_y][move.init_x];
            cur_board[move.init_y][move.init_x] = '.';
            if (color)//构造出黑棋先走的棋盘
            {
                GameTreeNode* child = new GameTreeNode(!color, cur_board, std::numeric_limits<int>::max());
                children.push_back(child);
                return child;
            }
            else//构造出红棋先走的棋盘
            {
                GameTreeNode* child = new GameTreeNode(!color, cur_board, std::numeric_limits<int>::min());
                children.push_back(child);
                return child;
            }
        }

        // 博弈树搜索，depth为搜索深度
        // 如果是Maximizer，返回最大值，如果是Minimizer，则返回最小的，由于存在剪枝，返回的最大最小值在不影响父节点的情况下不一定是最大/最小值
        int alphaBeta(int parent_alpha, int parent_beta, int depth, bool isMaximizer) //isMaximizer等价于color == true
        {
            sum++;
            if (depth == 0)
            {
                int value = getEvaluationScore();
                alpha = value;
                beta = value;
                return value;
            }
            //判断棋局是否已经结束（是不是还有两个将）
            {
                auto pieces = board.getChessPiece();
                bool klive = false;
                bool Klive = false;
                for (int i = 0;i < pieces.size();i++)
                {
                    if (pieces[i].name == 'k')
                        klive = true;
                    else if (pieces[i].name == 'K')
                        Klive = true;
                }
                if (!klive)
                {
                    alpha = 10000;
                    beta = 10000;
                    return 10000;
                }
                if (!Klive)
                {
                    alpha = -10000;
                    beta = -10000;
                    return -10000;
                }
            }
            //TODO alpha-beta剪枝过程
            auto board = getBoardClass();
            auto move = board.getMoves(color);
            if(depth > 1)
            printf("depth:%d,a:%d,b:%d\n", depth, parent_alpha, parent_beta);
            if (isMaximizer)
            {
                int i = 0;
#ifdef ALPHA_BETA
                while (i < move.size() && alpha < parent_beta)
#endif // ALPHA_BETA
#ifndef ALPHA_BETA
                while (i < move.size())
#endif // !ALPHA_BETA
                {
                    GameTreeNode* child = updateBoard(board.getBoard(), move[i], color);
                    int min = child->alphaBeta(alpha, beta, depth - 1, !color);//由于可能存在剪枝，不一定是最小值，但是可以当成最小值看待
                    if (min > alpha)
                        alpha = min;
                    i++;
                }
                if (i == move.size())
                {
                    beta = alpha;
                }
                return alpha;
            }
            else
            {
                int i = 0;
#ifdef ALPHA_BETA
                while (i < move.size() && beta > parent_alpha)
#endif // ALPHA_BETA
#ifndef ALPHA_BETA
                while (i < move.size())
#endif // !ALPHA_BETA
                {
                    GameTreeNode* child = updateBoard(board.getBoard(), move[i], color);
                    int max = child->alphaBeta(alpha, beta, depth - 1, !color);//由于可能存在剪枝，不一定是最大值，但是可以当成最大值看待
                    if (max < beta)
                        beta = max;
                    i++;
                }
                if (i == move.size())
                {
                    alpha = beta;
                }
                return beta;
            }

            return 0;
        }

        std::string GetPath(int depth)
        {
            std::string path = "";
            bool isMaximizer = true;
            auto node = this;
            while (depth != 0)
            {
                //在子节点中查找正确的路径
                if (isMaximizer == true)
                {
                    auto children = node->children;
                    for (int i = 0;i < children.size();i++)
                    {
                        if (children[i]->beta == alpha)
                        {
                            path += CompareBoard(node, children[i]);
                            node = children[i];
                            break;
                        }
                    }
                    isMaximizer = !isMaximizer;
                }
                else
                {
                    auto children = node->children;
                    for (int i = 0;i < children.size();i++)
                    {
                        if (children[i]->alpha == beta)
                        {
                            path += CompareBoard(node, children[i]);
                            node = children[i];
                            break;
                        }
                    }
                    isMaximizer = !isMaximizer;
                }
                depth--;
            }
            return path;
        }

        //比较后时刻的棋谱，反求行棋
        std::string CompareBoard(GameTreeNode* currentNode, GameTreeNode* nextNode)
        {
            std::stringstream ss;
            auto cur_board = currentNode->board.getBoard();
            auto next_board = nextNode->board.getBoard();
            int sizeX = cur_board.size();
            int sizeY = cur_board[0].size();
            std::pair<int, int> vary[2];
            int t = 0;
            for (int i = 0;i < sizeX;i++)
            {
                for (int j = 0;j < sizeY;j++)
                {
                    if (cur_board[i][j] != next_board[i][j])
                    {
                        vary[t] = std::make_pair(i, j);
                        t++;
                        if (t >= 2)
                            break;
                    }
                    if (t >= 2)
                        break;
                }
            }
            if (cur_board[vary[0].first][vary[0].second] == next_board[vary[1].first][vary[1].second] && next_board[vary[1].first][vary[1].second] != '.')
            {
                ss << next_board[vary[1].first][vary[1].second] << " (" << vary[0].second << "," << vary[0].first << ") (" << vary[1].second << "," << vary[1].first << ")" << std::endl;
                //printf("%c (%d,%d) (%d,%d)\n", next_board[vary[1].first][vary[1].second], vary[0].second, vary[0].first, vary[1].second, vary[1].first);
            }
            else
            {
                ss << next_board[vary[0].first][vary[0].second] << " (" << vary[1].second << "," << vary[1].first << ") (" << vary[0].second << "," << vary[0].first << ")" << std::endl;
                //printf("%c (%d,%d) (%d,%d)\n", next_board[vary[0].first][vary[0].second], vary[1].second, vary[1].first, vary[0].second, vary[0].first);
            }
            return ss.str();
        }

        //返回节点评估分数
        int getEvaluationScore()
        {
            evaluationScore = board.evaluateNode();
            return evaluationScore;
        }

        //返回棋盘类
        ChessBoard getBoardClass()
        {
            return board;
        }
        
        ~GameTreeNode()
        {
            for (GameTreeNode* child : children)
            {
                delete child;
            }
        }

    };
    
}