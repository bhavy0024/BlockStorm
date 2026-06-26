#pragma once
#include <vector>
#include <cstdlib>

using namespace std;

class Piece
{
protected:
    int posRow;
    int posCol;
    int pieceColor;
    vector<vector<int>> shapeMatrix;

public:
    Piece()
    {
        posRow = 0;
        posCol = 4;
    }

    void resetPosition()
    {
        posRow = 0;
        posCol = 4;
    }

    vector<vector<int>> getRandomPiece()
    {
        int type = rand() % 7;

        switch (type)
        {
        case 0: // O
            pieceColor = 14;
            return {
                {1, 1},
                {1, 1}};

        case 1: // I
            pieceColor = 11;
            return {
                {1, 1, 1, 1}};

        case 2: // T
            pieceColor = 13;
            return {
                {1, 1, 1},
                {0, 1, 0}};

        case 3: // L
            pieceColor = 6;
            return {
                {1, 0},
                {1, 0},
                {1, 1}};

        case 4: // J
            pieceColor = 1;
            return {
                {0, 1},
                {0, 1},
                {1, 1}};

        case 5: // S
            pieceColor = 2;
            return {
                {0, 1, 1},
                {1, 1, 0}};

        case 6: // Z
            pieceColor = 4;
            return {
                {1, 1, 0},
                {0, 1, 1}};
        }

        return {{1}};
    }
};