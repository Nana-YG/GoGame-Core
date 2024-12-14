#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "Config.h"
#include <iostream>
#include <chrono>

int main() {
    // Load configuration
    Config& config = Config::getInstance();
    if (!config.loadFromFile("/home/nana/Gyyyyy/Projects/MyAlphaGo/cpp/config.json")) {
        std::cerr << "Failed to load configuration. Using default settings." << std::endl;
    }

    // Initialize game
    Board board;
    Player playerB("Alice", BLACK);
    Player playerW("Bob", WHITE);
    Game game(&playerB, &playerW);
    game.init();

    // Start game loop
    bool gameOver = false;
    while (!gameOver) {
        // Display board
        std::cout << board.showBoard();

        // Current player makes a move
        Player* currentPlayer = (game.getMoveCount() % 2 == 0) ? &playerB : &playerW;
        std::cout << currentPlayer->getName() << "'s turn ("
                  << (currentPlayer->getColor() == BLACK ? "BLACK" : "WHITE") << "):" << std::endl;

        int row, col;
        std::cout << "Enter your move (row and column): ";
        std::cin >> row >> col;

        auto start = std::chrono::high_resolution_clock::now();

        StonePosition pos = {row - 1, col - 1}; // Adjust for 0-based indexing
        if (!currentPlayer->makeMove(&game, board, pos)) {
            std::cout << "Invalid move. Try again." << std::endl;
            continue;
        }

        // 停止计时
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);


        // 输出时间消耗
        std::cout << "Move processed in " << duration.count() << " μs." << std::endl;


        // Update game state
        game.addBoard(board);
        gameOver = board.isOver();
    }

    // Game over
    std::cout << "Game over! Final board state:" << std::endl;
    std::cout << board.showBoard();

    return 0;
}
