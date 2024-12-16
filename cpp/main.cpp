#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "GTP.h"
#include <iostream>
#include <chrono>


#include "GroupUtil.h"

int main() {
    /*
    // Load configuration
    Config& config = Config::getInstance();
    if (!config.loadFromFile("/home/nana/Gyyyyy/Projects/GTP-Core/cpp/config.json")) {
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

        // std::cout << "Liberties:" << std::endl;
        // std::cout << board.showLiberties();

        // Current player makes a move
        Player* currentPlayer = (game.getMoveCount() % 2 == 0) ? &playerB : &playerW;
        std::cout << currentPlayer->getName() << "'s turn ("
                  << (currentPlayer->getColor() == BLACK ? "BLACK" : "WHITE") << "):" << std::endl;

        int row, col;
        std::cout << "Enter your move (row and column): ";
        std::cin >> row >> col;

        // Start timmer
        auto start = std::chrono::high_resolution_clock::now();

        StonePosition pos = {row - 1, col - 1}; // Adjust for 0-based indexing
        if (!currentPlayer->makeMove(&game, board, pos)) {
            std::cout << "Invalid move. Try again." << std::endl;
            continue;
        }

        // std::cout << "Linked Board:" << std::endl;
        // printGroups(board);

        // Stop timmer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);


        // Time Consume for history query
        std::cout << "Move processed in " << duration.count() << " μs." << std::endl;


        // Update game state
        game.addBoard(board);
        gameOver = board.isOver();
    }

    // Game over
    std::cout << "Game over! Final board state:" << std::endl;
    std::cout << board.showBoard();

    return 0;
    */

    GTP gtp;
    gtp.run(); // Start the GTP loop
    return 0;


}
