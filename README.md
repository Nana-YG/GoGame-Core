# GoGame-Core

GoGame-Core is an open-source implementation of a light-weight Go game engine, built with C++ and designed for educational and collaborative purposes. This project includes essential game components such as players, a board, and a game manager, and is extendable for advanced features like AI integration or SGF file support.

## Why Building Another Go Software?

While there are many Go software options available, MyAlphaGo is designed with the following goals in mind:

1. **Educational Value**: Most Go software like GNU Go already has massive code space with an integrated AI engine. This project serves as a resource for learning about Go game mechanics, C++ programming, and software design.
2. **Modularity**: A clean and modular architecture allows for easy experimentation and feature additions.
3. **Community Collaboration**: Encouraging open-source contributions to create a robust and flexible Go engine.
4. **Customization**: Provides developers and enthusiasts the ability to tweak and adapt the software to their needs, whether for research or casual play.


## Features

- **Game Mechanics:**
  - Fully functional Go game engine with rules implementation.
  - Board state management, including legal move checking and super-ko rule handling.

- **Configurable Settings:**
  - Customizable board size, komi, and other game parameters via `config.json`.

- **Extensibility:**
  - Modular design makes it easy to integrate AI engines or expand features (e.g., SGF file parsing & editing).

## Requirements

- **Operating System:** Linux (tested on Ubuntu)
- **Language:** C++20
- **Build System:** CMake (version 3.29 or later)
- **Dependencies:**
  - [nlohmann/json](https://github.com/nlohmann/json) for configuration file parsing.

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/<your-username>/MyAlphaGo.git
   cd MyAlphaGo
   ```

2. Build the project:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. Run the executable:

   ```bash
   ./MyAlphaGo
   ```

## Configuration

The game settings are defined in a `config.json` file. Below is an example configuration:

```json
{
  "Game": {
    "BoardSize": 19,
    "Komi": 6.5,
    "Handicap": 0,
    "Rule": "Japanese"
  }
}
```

Place this file in the same directory as the executable.

## Usage

Run the program and follow the instructions in the terminal to play the game. Players will take turns to place stones on the board until the game ends.

## File Structure

- **`main.cpp`**: Entry point of the program.
- **`Player.cpp` / `Player.h`**: Defines the Player class and player actions.
- **`Board.cpp` / `Board.h`**: Manages the game board and rules.
- **`Game.cpp` / `Game.h`**: Handles the game state and flow.
- **`Config.cpp` / `Config.h`**: Loads and manages game configuration.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributions

Contributions are welcome! Here's how you can help:

1. Fork the repository.
2. Create a new branch for your feature/bugfix.
3. Submit a pull request with a detailed explanation of your changes.

Feel free to open issues for bugs, feature requests, or general questions.

## Contact

If you have any questions or suggestions, feel free to contact me via [GitHub Issues](https://github.com/<Nana-YG>/MyAlphaGo/issues).

Happy coding and enjoy Go!
