#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include "Board.h"
#include "Piece.h"
#include "ScoreManager.h"
using namespace std;

class BlockStorm : public Board, public Piece, public ScoreManager
{

public:
    vector<vector<int>> holdShape;
    int highScore = 0;
    bool holdUsed = false;
    vector<vector<int>> nextShape;
    int level = 1;
    bool gameEnded = false;
    bool paused = false;
    int holdColor = 15;
    void drawBoard()
    {
        system("cls");

        vector<vector<int>> tempGrid = gridMap;
        int ghostRow = getGhostRow();
        for (int r = 0; r < shapeMatrix.size(); r++)
        {
            for (int c = 0; c < shapeMatrix[r].size(); c++)
            {
                if (shapeMatrix[r][c])
                {
                    int nr = ghostRow + r;
                    int nc = posCol + c;

                    if (nr >= 0 && nr < rows &&
                        nc >= 0 && nc < cols &&
                        tempGrid[nr][nc] == 0)
                    {
                        tempGrid[nr][nc] = -1;
                    }
                }
            }
        }
        for (int r = 0; r < shapeMatrix.size(); r++)
        {
            for (int c = 0; c < shapeMatrix[r].size(); c++)
            {
                if (shapeMatrix[r][c])
                {
                    int newRow = posRow + r;
                    int newCol = posCol + c;

                    if (newRow < rows && newCol < cols)
                        tempGrid[newRow][newCol] = pieceColor;
                }
            }
        }
        for (int i = 0; i < cols * 2 + 2; i++)
            cout << "=";

        cout << "\n";
        cout << "Score : " << totalPoints << "\n";
        // cout << "High Score : " << highScore << "\n";
        cout << "Level : " << level << "\n";
        cout << "\nHold:\n";
        for (int r = 0; r < holdShape.size(); r++)
        {
            for (int c = 0; c < holdShape[r].size(); c++)
            {
                if (holdShape[r][c])
                    cout << "[]";
                else
                    cout << "  ";
            }
            cout << "\n";
        }
        cout << "\nNext:\n";

        for (int r = 0; r < nextShape.size(); r++)
        {
            for (int c = 0; c < nextShape[r].size(); c++)
            {
                if (nextShape[r][c])
                    cout << "[]";
                else
                    cout << "  ";
            }
            cout << "\n";
        }

        cout << "\n";
        for (int r = 0; r < rows; r++)
        {
            cout << "|";

            for (int c = 0; c < cols; c++)
            {
                if (tempGrid[r][c] == -1)
                {
                    setColor(8);
                    cout << "[]";
                    setColor(15);
                }
                else if (tempGrid[r][c] > 0)
                {
                    setColor(tempGrid[r][c]);
                    cout << "[]";
                    setColor(15);
                }
                else
                {
                    cout << "  ";
                }
            }

            cout << "|\n";
        }

        for (int i = 0; i < cols * 2 + 2; i++)
            cout << "=";

        cout << "\n";
    }
    int getGhostRow()
    {
        int ghostRow = posRow;

        while (true)
        {
            bool ok = true;

            for (int r = 0; r < shapeMatrix.size(); r++)
            {
                for (int c = 0; c < shapeMatrix[r].size(); c++)
                {
                    if (shapeMatrix[r][c])
                    {
                        int nr = ghostRow + r + 1;
                        int nc = posCol + c;

                        if (nr >= rows || isOccupied(nr, nc))
                        {
                            ok = false;
                            break;
                        }
                    }
                }

                if (!ok)
                    break;
            }

            if (!ok)
                break;

            ghostRow++;
        }

        return ghostRow;
    }
    BlockStorm()
    {
        loadHighScore();
        nextShape = generatePiece();
        spawnNewPiece();
    }
    int getHighScore()
    {
        return highScore;
    }
    int getScore()
    {
        return totalPoints;
    }
    void loadHighScore()
    {
        ifstream fin("highscore.txt");

        if (fin)
            fin >> highScore;

        fin.close();
    }
    void saveHighScore()
    {
        ofstream fout("highscore.txt");

        fout << highScore;

        fout.close();
    }
    void updateHighScore()
    {
        if (totalPoints > highScore)
        {
            highScore = totalPoints;
            saveHighScore();
        }
    }
    bool tryWallKick()
    {
        if (canMove(0, 0))
            return true;

        posCol--;
        if (canMove(0, 0))
            return true;

        posCol += 2;
        if (canMove(0, 0))
            return true;

        posCol -= 3;
        if (canMove(0, 0))
            return true;

        posCol += 4;
        if (canMove(0, 0))
            return true;

        posCol -= 2;
        return false;
    }
    void restartGame()
    {
        clearBoard();

        totalPoints = 0;
        level = 1;

        gameEnded = false;
        paused = false;

        holdShape.clear();
        nextShape = generatePiece();

        holdUsed = false;

        spawnNewPiece();
    }
    int getSpeed()
    {
        int speed = 1000 - (level - 1) * 100;

        if (speed < 100)
            speed = 100;

        return speed;
    }
    void updateLevel()
    {
        level = totalPoints / 100 + 1;
    }
    void lockCurrentPiece()
    {
        for (int r = 0; r < shapeMatrix.size(); r++)
        {
            for (int c = 0; c < shapeMatrix[r].size(); c++)
            {
                if (shapeMatrix[r][c])
                {
                    placeCell(posRow + r, posCol + c, pieceColor);
                }
            }
        }
        holdUsed = false;
    }
    void holdPiece()
    {
        if (holdUsed)
            return;

        holdUsed = true;

        if (holdShape.empty())
        {
            holdShape = shapeMatrix;
            holdColor = pieceColor;

            shapeMatrix = nextShape;
            nextShape = generatePiece();

            posRow = 0;
            posCol = 4;
        }
        else
        {
            swap(shapeMatrix, holdShape);
            swap(pieceColor, holdColor);

            posRow = 0;
            posCol = 4;
        }
    }
    void resetPosition()
    {
        posRow = 0;
        posCol = 4;
    }
    vector<vector<int>> generatePiece()
    {
        return getRandomPiece();
    }
    void spawnNewPiece()
    {
        resetPosition();

        shapeMatrix = nextShape;
        nextShape = generatePiece();

        if (!canMove(0, 0))
        {
            updateHighScore();
            gameEnded = true;
        }
    }
    void setColor(int color)
    {
        SetConsoleTextAttribute(
            GetStdHandle(STD_OUTPUT_HANDLE),
            color);
    }
    bool collisionBelow()
    {
        for (int r = 0; r < shapeMatrix.size(); r++)
        {
            for (int c = 0; c < shapeMatrix[r].size(); c++)
            {
                if (shapeMatrix[r][c])
                {
                    int nr = posRow + r + 1;
                    int nc = posCol + c;

                    if (nr >= rows)
                        return true;

                    if (isOccupied(nr, nc))
                        return true;
                }
            }
        }

        return false;
    }
    void clearRows()
    {
        for (int r = rows - 1; r >= 0; r--)
        {
            bool full = true;

            for (int c = 0; c < cols; c++)
            {
                if (gridMap[r][c] == 0)
                {
                    full = false;
                    break;
                }
            }

            if (full)
            {
                for (int rr = r; rr > 0; rr--)
                {
                    gridMap[rr] = gridMap[rr - 1];
                }

                gridMap[0] = vector<int>(cols, 0);

                totalPoints += 100;
                updateLevel();
                r++;
            }
        }
    }
    void rotatePieceLeft()
    {
        vector<vector<int>> backup = shapeMatrix;

        int r = shapeMatrix.size();
        int c = shapeMatrix[0].size();

        vector<vector<int>> rotated(c, vector<int>(r));

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                rotated[c - 1 - j][i] = shapeMatrix[i][j];
            }
        }

        shapeMatrix = rotated;
        if (!tryWallKick())
        {
            shapeMatrix = backup;
        }
    }
    void rotatePieceRight()
    {
        vector<vector<int>> backup = shapeMatrix;

        int r = shapeMatrix.size();
        int c = shapeMatrix[0].size();

        vector<vector<int>> rotated(c, vector<int>(r));

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                rotated[j][r - 1 - i] = shapeMatrix[i][j];
            }
        }

        shapeMatrix = rotated;

        if (!tryWallKick())
        {
            shapeMatrix = backup;
        }
    }
    void moveDown()
    {
        if (!canMove(1, 0))
        {
            lockCurrentPiece();

            clearRows();

            spawnNewPiece();
        }
        else
        {
            posRow++;
            totalPoints++;
            updateLevel();
        }
    }
    bool canMove(int dr, int dc)
    {
        for (int r = 0; r < shapeMatrix.size(); r++)
        {
            for (int c = 0; c < shapeMatrix[r].size(); c++)
            {
                if (shapeMatrix[r][c])
                {
                    int nr = posRow + r + dr;
                    int nc = posCol + c + dc;

                    if (nc < 0 || nc >= cols || nr < 0 || nr >= rows)
                        return false;

                    if (isOccupied(nr, nc))
                        return false;
                }
            }
        }
        return true;
    }
    void hardDrop()
    {
        while (canMove(1, 0))
        {
            posRow++;
            totalPoints += 2; // bonus points for fast drop
            updateLevel();
        }

        lockCurrentPiece();
        clearRows();
        spawnNewPiece();
    }
    void togglePause()
    {
        paused = !paused;
    }
    void moveLeft()
    {
        if (canMove(0, -1))
            posCol--;
    }

    void moveRight()
    {
        if (canMove(0, 1))
            posCol++;
    }
    bool isPaused()
    {
        return paused;
    }
    void handleInput()
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == 'p' || key == 'P')
            {
                togglePause();
                return;
            }

            if (paused)
                return;
            if (key == 'a' || key == 'A')
                moveLeft();
            if (key == 'w' || key == 'W')
                rotatePieceRight(); // clockwise
            if (key == 's' || key == 'S')
                rotatePieceLeft(); // counter-clockwise
            if (key == 'd' || key == 'D')
                moveRight();
            if (key == ' ')
                hardDrop();
            if (key == 'c' || key == 'C')
                holdPiece();
            if (key == 'p' || key == 'P')
                togglePause();
            if (key == 27)
                exit(0);
        }
    }
    bool gameOver()
    {
        return gameEnded;
    }
};