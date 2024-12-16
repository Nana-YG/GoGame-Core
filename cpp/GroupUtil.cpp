//
// Created by nana on 12/15/24.
//

#include "Board.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

/**
 * The connecting stone must be put at an empty spot, which is counted
 * as a liberty of the combining groups.
 */
Group* combined(std::vector<Group*> groups, int offset) {
    // offset: liberties of the connecting stone
    Group* newGroup = new Group();
    newGroup->color = groups[0]->color;

    // Combine stones and calculate liberties
    newGroup->liberty = offset; // Start with offset for the connecting stone
    for (const auto& group : groups) {
        // Add stones from the group
        newGroup->stones.insert(newGroup->stones.end(), group->stones.begin(), group->stones.end());
        // Sum up liberties
        newGroup->liberty += group->liberty;
    }

    // Adjust liberties: subtract the connecting spot (equal to the number of groups)
    newGroup->liberty -= groups.size();

    // Update the group pointer for each stone in the new group
    for (Stone* stone : newGroup->stones) {
        stone->group = newGroup;
    }

    return newGroup;
}

Group* combined(Group* group1, Group* group2, int offset) {
    // offset: liberties of the connecting stone
    Group* newGroup = new Group();
    newGroup->color = group1->color;

    // Combine stones
    newGroup->stones = group1->stones;
    newGroup->stones.insert(newGroup->stones.end(), group2->stones.begin(), group2->stones.end());
    for (Stone* stone : newGroup->stones) {
        stone->group = newGroup;
    }

    // Combine liberties
    newGroup->liberty = group1->liberty + group2->liberty + offset - 2;

    return newGroup;
}

Group* combined(Group* group1, Group* group2, Group* group3, int offset) {
    // offset: liberties of the connecting stone
    Group* newGroup = new Group();
    newGroup->color = group1->color;

    // Combine stones
    newGroup->stones = group1->stones;
    newGroup->stones.insert(newGroup->stones.end(), group2->stones.begin(), group2->stones.end());
    newGroup->stones.insert(newGroup->stones.end(), group3->stones.begin(), group3->stones.end());
    for (Stone* stone : newGroup->stones) {
        stone->group = newGroup;
    }

    // Combine liberties
    newGroup->liberty = group1->liberty + group2->liberty + group3->liberty + offset - 3;

    return newGroup;
}

Group* combined(Group* group1, Group* group2, Group* group3, Group* group4, int offset) {
    // offset: liberties of the connecting stone
    Group* newGroup = new Group();
    newGroup->color = group1->color;

    // Combine stones
    newGroup->stones = group1->stones;
    newGroup->stones.insert(newGroup->stones.end(), group2->stones.begin(), group2->stones.end());
    newGroup->stones.insert(newGroup->stones.end(), group3->stones.begin(), group3->stones.end());
    newGroup->stones.insert(newGroup->stones.end(), group4->stones.begin(), group4->stones.end());
    for (Stone* stone : newGroup->stones) {
        stone->group = newGroup;
    }

    // Combine liberties
    newGroup->liberty = group1->liberty + group2->liberty + group3->liberty + group4->liberty + offset - 4;

    return newGroup;
}

void printGroups(Board board) {
    int size = board.getSize();
    std::vector<std::vector<char>> display(size * 2 - 1, std::vector<char>(size * 2 - 1, ' '));

    // Initialize symbols
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Stone* stone = board.getBoard()[i][j];
            if (stone->color == BLACK) {
                display[i * 2][j * 2] = 'X';
            } else if (stone->color == WHITE) {
                display[i * 2][j * 2] = 'O';
            } else {
                display[i * 2][j * 2] = '.'; // Empty spots
            }
        }
    }

    // Add Group connectors
    for (int i = (size - 1); i >= 0; i--) {
        for (int j = (size - 1); j >= 0; j--) {
            Stone* stone = board.getBoard()[i][j];
            if (stone->color == EMPTY || stone->group == nullptr) {
                continue;
            }

            // Connect to the right
            if (j + 1 < size && board.getBoard()[i][j + 1]->group == stone->group) {
                display[i * 2][j * 2 + 1] = '-';
            }
            // Connect down3
            if (i + 1 < size && board.getBoard()[i + 1][j]->group == stone->group) {
                display[i * 2 + 1][j * 2] = '|';
            }
        }
    }

    // Print result
    for (const auto& row : display) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << '\n';
    }
}
