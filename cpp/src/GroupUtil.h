//
// Created by nana on 12/15/24.
//

#ifndef GROUPUTIL_H
#define GROUPUTIL_H
#include <vector>
#include "Board.h"


Group* combined(std::vector<Group*> groups, int offset);
Group* combined(Group* group1, Group* group2, int offset);
Group* combined(Group* group1, Group* group2, Group* group3, int offset);
Group* combined(Group* group1, Group* group2, Group* group3, Group* group4, int offset);

void printGroups(Board board);


#endif //GROUPUTIL_H
