#pragma once

#include <vector>

using namespace std;

class Board
{
protected:
    int rows;
    int cols;
    vector<vector<int>> gridMap;

public:
    Board()
    {
        rows = 20;
        cols = 10;

        gridMap.assign(rows, vector<int>(cols, 0));
    }
    bool isOccupied(int r, int c)
    {
        if (r < 0 || r >= rows || c < 0 || c >= cols)
            return true;

        return gridMap[r][c] != 0;
    }
    void placeCell(int r, int c, int color)
    {
        if (r >= 0 && r < rows &&
            c >= 0 && c < cols)
        {
            gridMap[r][c] = color;
        }
    }
    void clearBoard()
    {
        gridMap.assign(rows, vector<int>(cols, 0));
    }
};