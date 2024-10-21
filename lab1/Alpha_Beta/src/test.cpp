#include <fstream>
#include "node.h"

//#define DEBUG

using namespace ChineseChess;

int main() 
{
    int testID = 9;
    int depth = 5;

    std::string inputfilename = "../input/" + std::to_string(testID) + ".txt";
    std::string outputfilename = "../output/output_" + std::to_string(testID) + ".txt";
    std::ifstream inputfile(inputfilename);
    std::vector<std::vector<char>> board;

    std::string line;
    int n = 0;
    while (std::getline(inputfile, line)) 
    {
        std::vector<char> row;
        for (char ch : line) 
        {
            row.push_back(ch);
        }
        board.push_back(row);
        n = n + 1;
        if (n >= 10) break;
    }

    inputfile.close();
    GameTreeNode* root = new GameTreeNode(true, board, std::numeric_limits<int>::min());

    //TODO
    
    int max = root->alphaBeta(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), depth, true);
    printf("max = %d\n", max);
    std::string str = root->GetPath(depth);
    std::cout << str;

    std::ofstream outputfile(outputfilename, std::ios::out | std::ios::trunc);
    if (outputfile.is_open())
    {
        outputfile << str;
        outputfile.close();
    }
    else 
    {
        std::cout << "Failed to open the outputfile." << std::endl;
    }

    printf("sum=%d", sum);

    //代码测试
#ifdef DEBUG
    ChessBoard _board = root->getBoardClass();
    std::vector<std::vector<char>> cur_board = _board.getBoard();

    for (int i = 0; i < cur_board.size(); i++) {
        for (int j = 0; j < cur_board[0].size(); j++) {
            std::cout << cur_board[i][j];
        }
        std::cout << std::endl;
    }

    std::vector<Move> red_moves = _board.getMoves(true);
    std::vector<Move> black_moves = _board.getMoves(false);

    for (int i = 0; i < red_moves.size(); i++) {
        std::cout << "init: " << red_moves[i].init_x << " " << red_moves[i].init_y << std::endl;
        std::cout << "next: " << red_moves[i].next_x << " " << red_moves[i].next_y << std::endl;
        std::cout << "score " << red_moves[i].score << std::endl;
    }
    for (int i = 0; i < black_moves.size(); i++) {
        std::cout << "init: " << black_moves[i].init_x << " " << black_moves[i].init_y << std::endl;
        std::cout << "next: " << black_moves[i].next_x << " " << black_moves[i].next_y << std::endl;
        std::cout << "score " << black_moves[i].score << std::endl;
    }
#endif // DEBUG
    return 0;
}