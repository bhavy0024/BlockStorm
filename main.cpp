#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "Board.h"
#include "Piece.h"
#include "Game.h"
#include <ctime>
using namespace std;

int main()
{
    srand(time(0));

    BlockStorm game;

    // game.spawnNewPiece();
    while (!game.gameOver())
    {
        game.handleInput();

        if (!game.isPaused())
        {
            game.drawBoard();
            game.moveDown();
        }
        else
        {
            game.drawBoard(); // optional: show PAUSED screen
        }

        Sleep(game.getSpeed());
    }
    while (true)
    {
        cout << "\n=====================\n";
        cout << "      GAME OVER\n";
        cout << "=====================\n";

        cout << "Final Score : " << game.getScore() << "\n";
        cout << "High Score  : " << game.getHighScore() << "\n\n";

        cout << "R - Restart Game\n";
        cout << "Q - Quit\n";

        char ch;
        cin >> ch;

        if (ch == 'R' || ch == 'r')
        {
            game.restartGame();

            while (!game.gameOver())
            {
                game.handleInput();

                if (!game.isPaused())
                {
                    game.drawBoard();
                    game.moveDown();
                }

                Sleep(game.getSpeed());
            }
        }
        else if (ch == 'Q' || ch == 'q')
        {
            break;
        }
    }
    return 0;
}