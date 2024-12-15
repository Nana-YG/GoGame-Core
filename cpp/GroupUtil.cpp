//
// Created by nana on 12/15/24.
//

#include "Board.h"
#include <vector>

inline bool isConnected(Stone stone1, Stone stone2) {
    StonePosition diff = stone1.pos - stone2.pos;
    return std::abs(diff.row) <= 1 && std::abs(diff.col) <= 1;
}

inline void combine(Group group1, Group group2) {
}

inline void combine(Group group1, Group group2, Group group3) {
}

inline void combine(Group group1, Group group2, Group group3, Group group4) {
}