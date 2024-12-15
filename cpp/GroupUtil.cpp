//
// Created by nana on 12/15/24.
//

#include "Board.h"
#include <vector>

inline bool isConnected(Stone stone1, Stone stone2) {
    StonePosition diff = stone1.pos - stone2.pos;
    return std::abs(diff.row) <= 1 && std::abs(diff.col) <= 1;
}


/**
 * The connecting stone must be put at an empty spot, which is counted
 * as a liberty of the combining groups.
 */
inline Group combined(std::vector<Group> groups, int offset) {
    // offset: liberties of the connecting stone
    Group newGroup;
    newGroup.color = groups[0].color;

    // Combine stones and calculate liberties
    newGroup.liberties = offset; // Start with offset for the connecting stone
    for (const auto& group : groups) {
        // Add stones from the group
        newGroup.stones.insert(newGroup.stones.end(), group.stones.begin(), group.stones.end());
        // Sum up liberties
        newGroup.liberties += group.liberties;
    }

    // Adjust liberties: subtract the connecting spot (equal to the number of groups)
    newGroup.liberties -= groups.size();

    // Update the group pointer for each stone in the new group
    for (Stone& stone : newGroup.stones) {
        stone.group = &newGroup;
    }

    return newGroup;
}

inline Group combined(Group group1, Group group2, int offset) {
    // offset: liberties of the connecting stone
    Group newGroup;
    newGroup.color = group1.color;

    // Combine stones
    newGroup.stones = group1.stones;
    newGroup.stones.insert(newGroup.stones.end(), group2.stones.begin(), group2.stones.end());
    for (Stone& stone : newGroup.stones) {
        stone.group = &newGroup;
    }

    // Combine liberties
    newGroup.liberties = group1.liberties + group2.liberties + offset - 2;

    return newGroup;
}

inline Group combined(Group group1, Group group2, Group group3, int offset) {
    // offset: liberties of the connecting stone
    Group newGroup;
    newGroup.color = group1.color;

    // Combine stones
    newGroup.stones = group1.stones;
    newGroup.stones.insert(newGroup.stones.end(), group2.stones.begin(), group2.stones.end());
    newGroup.stones.insert(newGroup.stones.end(), group3.stones.begin(), group3.stones.end());
    for (Stone& stone : newGroup.stones) {
        stone.group = &newGroup;
    }

    // Combine liberties
    newGroup.liberties = group1.liberties + group2.liberties + group3.liberties + offset - 3;

    return newGroup;
}

inline Group combined(Group group1, Group group2, Group group3, Group group4, int offset) {
    // offset: liberties of the connecting stone
    Group newGroup;
    newGroup.color = group1.color;

    // Combine stones
    newGroup.stones = group1.stones;
    newGroup.stones.insert(newGroup.stones.end(), group2.stones.begin(), group2.stones.end());
    newGroup.stones.insert(newGroup.stones.end(), group3.stones.begin(), group3.stones.end());
    newGroup.stones.insert(newGroup.stones.end(), group4.stones.begin(), group4.stones.end());
    for (Stone& stone : newGroup.stones) {
        stone.group = &newGroup;
    }

    // Combine liberties
    newGroup.liberties = group1.liberties + group2.liberties + group3.liberties + group4.liberties + offset - 4;

    return newGroup;
}