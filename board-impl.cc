import Board;
import Student;
import Tile;
import Criterion;
import Goal;
import <iostream>;
import <fstream>;
import <sstream>;
import <algorithm>;
import <random>;
import <vector>;

Board::Board() {
    for (int i = 0; i < 54; ++i) {
        criteria.emplace_back(i);
    }
    
    for (int i = 0; i < 72; ++i) {
        goals.emplace_back(i);
    }
    
    setupAdjacencies();
}

void Board::setupAdjacencies() {
    // Simplified adjacency setup based on board structure
    std::vector<std::pair<int,int>> criterionsLinks = {
        {0,1},{0,3},{1,4},{2,3},{4,5},{2,7},{3,8},{4,9},{5,10},
        {6,7},{8,9},{10,11},{6,12},{7,13},{8,14},{9,15},{10,16},
        {11,17},{13,14},{15,16},{12,18},{13,19},{14,20},{15,21},
        {16,22},{17,23},{18,19},{20,21},{22,23},{18,24},{19,25},
        {20,26},{21,27},{22,28},{23,29},{25,26},{27,28},{24,30},
        {25,31},{26,32},{27,33},{28,34},{29,35},{30,31},{32,33},
        {34,35},{30,36},{31,37},{32,38},{33,39},{34,40},{35,41},
        {37,38},{39,40},{36,42},{37,43},{38,44},{39,45},{40,36},
        {41,47},{42,43},{44,45},{46,47},{43,48},{44,49},{45,50},
        {46,51},{48,49},{50,51},{52,53}
    };

    // format {criterion, goal}
    std::vector<std::pair<int,int>> criterionGoalLinks = {
        {0,0},{0,1},{1,0},{1,2},{2,3},{2,5},{3,1},{3,3},{3,6},
        {4,2},{4,4},{4,7},{5,4},{5,8},{6,9},{6,12},{7,5},{7,9},
        {7,13},{8,6},{8,10},{8,14},{9,7},{9,10},{9,15},{10,8},
        {10,11},{10,16},{11,11},{11,17},{12,12},{12,20},{13,13},
        {13,18},{13,21},{14,14},{14,18},{14,22},{15,15},{15,19},
        {15,23},{16,16},{16,19},{16,24},{17,17},{17,25},{18,20},
        {18,26},{28,29},{19,21},{19,26},{19,30},{20,22},{20,27},
        {20,31},{21,23},{21,27},{21,32},{22,24},{22,28},{22,33},
        {23,25},{23,28},{23,34},{24,29},{24,37},{25,30},{25,35},
        {25,38},{26,31},{26,35},{26,39},{27,32},{27,36},{27,40},
        {28,33},{28,36},{28,41},{29,34},{29,42},{30,37},{30,43},
        {30,46},{31,38},{31,43},{31,47},{32,39},{32,44},{32,48},
        {33,40},{33,44},{33,49},{34,41},{34,45},{34,50},{35,42},
        {35,45},{35,51},{36,46},{36,54},{37,47},{37,52},{37,55},
        {38,48},{38,52},{38,56},{39,49},{39,53},{39,57},{40,50},
        {40,53},{40,58},{41,51},{41,59},{42,54},{42,60},{43,55},
        {43,60},{43,63},{44,56},{44,61},{44,64},{45,57},{45,61},
        {45,65},{46,58},{46,62},{46,66},{47,59},{47,62},{48,63},
        {48,67},{49,64},{49,67},{49,69},{50,65},{50,68},{50,70},
        {51,66},{51,68},{52,69},{52,71},{53,70},{53,71}
    };

    std::vector<std::pair<int,int>> goalLinks = {
        {0,1},{0,2},{3,5},{3,6},{1,3},{1,6},{2,4},{2,7},{4,7},
        {4,8},{9,12},{5,9},{5,13},{9,13},{6,10},{10,14},{6,14},
        {7,10},{7,15},{10,15},{8,11},{11,16},{8,16},{11,17},
        {12,20},{13,18},{13,21},{18,21},{14,18},{14,22},{18,22},
        {15,19},{15,23},{19,23},{16,19},{16,24},{19,24},{17,25},
        {20,26},{20,29},{26,29},{21,26},{21,30},{26,30},{22,27},
        {22,31},{27,31},{23,27},{23,32},{27,32},{24,28},{24,33},
        {28,33},{25,28},{25,34},{28,34},{29,37},{30,35},{30,38},
        {35,38},{31,35},{31,39},{35,39},{32,36},{32,40},{36,40},
        {33,36},{33,41},{36,41},{34,42},{37,43},{37,46},{43,46},
        {38,43},{38,47},{43,47},{39,44},{39,48},{44,48},{40,44},
        {44,49},{40,49},{41,45},{41,50},{45,50},{42,51},{46,54},
        {47,52},{47,55},{52,55},{48,52},{48,56},{52,56},{49,53},
        {49,57},{53,57},{50,53},{50,58},{53,58},{51,59},{54,60},
        {55,60},{55,63},{63,60},{56,61},{56,64},{61,64},{57,61},
        {57,65},{61,65},{58,62},{58,66},{62,66},{59,62},{63,67},
        {64,67},{64,69},{67,69},{65,68},{65,70},{68,70},{66,68},
        {69,71},{70,71}
    };

    
    for (const auto& edge : criterionsLinks) {
        criteria[edge.first].addAdjacentCriterion(edge.second);
        criteria[edge.second].addAdjacentCriterion(edge.first);
    }

    for (const auto& edge : criterionGoalLinks) {
        criteria[edge.first].addAdjacentGoal(edge.second);
        goals[edge.second].addAdjacentCriterion(edge.first);
    }

    for (const auto& edge : goalLinks) {
        goals[edge.first].addAdjacentGoal(edge.second);
        goals[edge.second].addAdjacentGoal(edge.first);
    }

}

void Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return;
    
    std::vector<int> layout;
    int val;
    while (file >> val && layout.size() < 38) {
        layout.push_back(val);
    }
    loadBoardLayout(layout);
}

void Board::randomInitialize() {
    std::vector<std::string> resources = {
        "CAFFEINE", "CAFFEINE", "CAFFEINE", "CAFFEINE",
        "LAB", "LAB", "LAB", "LAB",
        "LECTURE", "LECTURE", "LECTURE", "LECTURE",
        "STUDY", "STUDY", "STUDY",
        "TUTORIAL", "TUTORIAL", "TUTORIAL",
        "NETFLIX"
    };
    
    std::vector<int> values = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};

    // random generator
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(resources.begin(), resources.end(), rng);
    std::shuffle(values.begin(), values.end(), rng);
    tiles.clear();
    int i = 0, j = 0;
    while (i < 19 && j < 18) {
        if (resources[i] == "NETFLIX") {
            tiles.emplace_back(i, resources[i], 7);
            i++;
        } else {
            tiles.emplace_back(i, resources[i], values[j]);
            i++;
            j++;
        }
    }
    
    // O(1) initialization since the board is fixed
    std::vector<std::vector<int>> tileToCriteria = {
        {0,1,3,4,8,9}, {2,3,7,8,13,14}, {4,5,9,10,15,16},
        {6,7,12,13,18,19}, {8,9,14,15,20,21}, {10,11,16,17,22,23},
        {13,14,19,20,25,26}, {15,16,21,22,27,28}, {18,19,24,25,30,31},
        {20,21,26,27,32,33}, {22,23,28,29,34,35}, {25,26,31,32,37,38},
        {27,28,33,34,39,40}, {30,31,36,37,42,43}, {32,33,38,39,44,45}, 
        {34,35,40,41,46,47}, {37,38,43,44,48,49}, {39,40,45,46,50,51}, 
        {44,45,49,50,52,53}
    };
    
    for (int i = 0; i < 19; ++i) {
        for (int critId : tileToCriteria[i]) {
            tiles[i].addAdjacentCriterion(critId);
        }
    }
}

void Board::loadBoardLayout(const std::vector<int>& layout) {
    tiles.clear();
    
    std::vector<std::string> resourceNames = {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL", "NETFLIX"};
    
    for (int i = 0; i < 19; ++i) {
        int resourceType = layout[i * 2];
        int value = layout[i * 2 + 1];
        tiles.emplace_back(i, resourceNames[resourceType], value);
    }
    
    // Maybe use global variable to avoid duplication?
    std::vector<std::vector<int>> tileToCriteria = {
        {0,1,3,4,8,9}, {2,3,7,8,13,14}, {4,5,9,10,15,16},
        {6,7,12,13,18,19}, {8,9,14,15,20,21}, {10,11,16,17,22,23},
        {13,14,19,20,25,26}, {15,16,21,22,27,28}, {18,19,24,25,30,31},
        {20,21,26,27,32,33}, {22,23,28,29,34,35}, {25,26,31,32,37,38},
        {27,28,33,34,39,40}, {30,31,36,37,42,43}, {32,33,38,39,44,45}, 
        {34,35,40,41,46,47}, {37,38,43,44,48,49}, {39,40,45,46,50,51}, 
        {44,45,49,50,52,53}
    };
    
    for (int i = 0; i < 19; ++i) {
        for (int critId : tileToCriteria[i]) {
            tiles[i].addAdjacentCriterion(critId);
        }
    }
}

Tile& Board::getTile(int tileNum) {
    return tiles[tileNum];
}

Criterion& Board::getCriterion(int criterionNum) {
    return criteria[criterionNum];
}

Goal& Board::getGoal(int goalNum) {
    return goals[goalNum];
}

// Have not check resources, only checks board conditions
bool Board::canComplete(Student* student, int criterionNum) {
    Criterion& crit = criteria[criterionNum];
    
    if (crit.getOwner() != nullptr) return false;
    
    for (int adjCrit : crit.getAdjacentCriteria()) {
        if (criteria[adjCrit].getOwner() != nullptr && criteria[adjCrit].getOwner() != student) {
            return false;
        }
    }
    
    return true;
}

bool Board::canImprove(Student* student, int criterionNum) {
    Criterion& crit = criteria[criterionNum];
    
    if (crit.getOwner() != student) return false;
    if (crit.getCompletionLevel() >= 3) return false;
    
    return true;
}

bool Board::canAchieve(Student* student, int goalNum) {
    Goal& goal = goals[goalNum];
    
    if (goal.hasOwner()) return false;
    
    for (int adjCrit : goal.getAdjacentCriteria()) {
        if (criteria[adjCrit].getOwner() == student) {
            return true;
        }
    }
    
    for (int adjGoal : goal.getAdjacentGoals()) {
        if (goals[adjGoal].getOwner() == student) {
            return true;
        }
    }
    
    return false;
}

std::vector<int> Board::getCriteriaForTile(int tileNum) const {
    return tiles[tileNum].getAdjacentCriteria();
}

void Board::display() const {
    auto formatCrit = [&](int num) -> std::string {
        std::string display = criteria[num].display();
        if (display.length() == 1) {
            return display;
        }
        return display;
    };
    
    auto formatGoal = [&](int num) -> std::string {
        std::string display = goals[num].display();
        return display;
    };

    auto tileVal = [&](int tileNum) -> std::string {
        int val = tiles[tileNum].getValue();
        if (val == 7) return "  ";
        std::string result = (val < 10) ? " " + std::to_string(val) : std::to_string(val);
        return result;
    };

    auto tileIndex = [&](int tileNum) -> std::string {
            int val = tileNum;
            std::string result = (val < 10) ? " " + std::to_string(val) : std::to_string(val);
            return result;
        };

    
    auto tileRes = [&](int tileNum) -> std::string {
        std::string res = tiles[tileNum].getResourceType();
        if (tiles[tileNum].hasGeese()) res += "(G)";
        while (res.length() < 11) res += " ";
        return res;
    };
    
    std::cout << "                                   |" << formatCrit(0) << "|--" << formatGoal(0) << "--|" << formatCrit(1) << "|" << std::endl;
    std::cout << "                                   /            \\" << std::endl;
    std::cout << "                                 " << formatGoal(1) << "      " << tileIndex(0) << "     " << formatGoal(2) << std::endl;
    std::cout << "                                 /     " << tileRes(0) << "\\" << std::endl;
    std::cout << "                    |" << formatCrit(2) << "|--" << formatGoal(3) << "--|" << formatCrit(3) << "|       " << tileVal(0) << "       |" << formatCrit(4) << "|--" << formatGoal(4) << "--|" << formatCrit(5) << "|" << std::endl;
    std::cout << "                    /            \\                /            \\" << std::endl;
    std::cout << "                  " << formatGoal(5) << "      " << tileIndex(1) << "     " << formatGoal(6) << "             " << formatGoal(7) << "      " << tileIndex(2) << "     " << formatGoal(8) << std::endl;
    std::cout << "                  /     " << tileRes(1) << "\\            /     " << tileRes(2) << "\\" << std::endl;
    std::cout << "     |" << formatCrit(6) << "|--" << formatGoal(9) << "--|" << formatCrit(7) << "|       " << tileVal(1) << "       |" << formatCrit(8) << "|--" << formatGoal(10) << "--|" << formatCrit(9) << "|       " << tileVal(2) << "       |" << formatCrit(10) << "|--" << formatGoal(11) << "--|" << formatCrit(11) << "|" << std::endl;
    std::cout << "     /            \\                /            \\                /            \\" << std::endl;
    std::cout << "   " << formatGoal(12) << "      " << tileIndex(3) << "     " << formatGoal(13) << "             " << formatGoal(14) << "      " << tileIndex(4) << "     " << formatGoal(15) << "             " << formatGoal(16) << "      " << tileIndex(5) << "     " << formatGoal(17) << std::endl;
    std::cout << "   /     " << tileRes(3) << "\\            /     " << tileRes(4) << "\\            /     " << tileRes(5) << "\\" << std::endl;
    std::cout << "|" << formatCrit(12) << "|       " << tileVal(3) << "       |" << formatCrit(13) << "|--" << formatGoal(18) << "--|" << formatCrit(14) << "|       " << tileVal(4) << "       |" << formatCrit(15) << "|--" << formatGoal(19) << "--|" << formatCrit(16) << "|       " << tileVal(5) << "       |" << formatCrit(17) << "|" << std::endl;
    std::cout << "   \\                /            \\                /            \\                /" << std::endl;
    std::cout << "   " << formatGoal(20) << "             " << formatGoal(21) << "      " << tileIndex(6) << "     " << formatGoal(22) << "             " << formatGoal(23) << "      " << tileIndex(7) << "     " << formatGoal(24) << "             " << formatGoal(25) << std::endl;
    std::cout << "     \\            /     " << tileRes(6) << "\\            /     " << tileRes(7) << "\\            /" << std::endl;
    std::cout << "     |" << formatCrit(18) << "|--" << formatGoal(26) << "--|" << formatCrit(19) << "|       " << tileVal(6) << "       |" << formatCrit(20) << "|--" << formatGoal(27) << "--|" << formatCrit(21) << "|       " << tileVal(7) << "       |" << formatCrit(22) << "|--" << formatGoal(28) << "--|" << formatCrit(23) << "|" << std::endl;
    std::cout << "     /            \\                /            \\                /            \\" << std::endl;
    std::cout << "   " << formatGoal(29) << "      " << tileIndex(8) << "     " << formatGoal(30) << "             " << formatGoal(31) << "      " << tileIndex(9) << "     " << formatGoal(32) << "             " << formatGoal(33) << "      " << tileIndex(10) << "     " << formatGoal(34) << std::endl;
    std::cout << "   /     " << tileRes(8) << "\\            /     " << tileRes(9) << "\\            /     " << tileRes(10) << "\\" << std::endl;
    std::cout << "|" << formatCrit(24) << "|       " << tileVal(8) << "       |" << formatCrit(25) << "|--" << formatGoal(35) << "--|" << formatCrit(26) << "|       " << tileVal(9) << "       |" << formatCrit(27) << "|--" << formatGoal(36) << "--|" << formatCrit(28) << "|       " << tileVal(10) << "       |" << formatCrit(29) << "|" << std::endl;
    std::cout << "   \\                /            \\                /            \\                /" << std::endl;
    std::cout << "   " << formatGoal(37) << "             " << formatGoal(38) << "       " << tileIndex(11) << "    " << formatGoal(39) << "             " << formatGoal(40) << "      " << tileIndex(12) << "     " << formatGoal(41) << "             " << formatGoal(42) << std::endl;
    std::cout << "     \\            /     " << tileRes(11) << "\\            /     " << tileRes(12) << "\\            /" << std::endl;
    std::cout << "     |" << formatCrit(30) << "|--" << formatGoal(43) << "--|" << formatCrit(31) << "|       " << tileVal(11) << "       |" << formatCrit(32) << "|--" << formatGoal(44) << "--|" << formatCrit(33) << "|       " << tileVal(12) << "       |" << formatCrit(34) << "|--" << formatGoal(45) << "--|" << formatCrit(35) << "|" << std::endl;
    std::cout << "     /            \\                /            \\                /            \\" << std::endl;
    std::cout << "   " << formatGoal(43) << "      " << tileIndex(13) << "     " << formatGoal(44) << "             " << formatGoal(45) << "      " << tileIndex(14) << "     " << formatGoal(46) << "             " << formatGoal(47) << "      " << tileIndex(15) << "     " << formatGoal(48) << std::endl;
    std::cout << "   /     " << tileRes(13) << "\\            /     " << tileRes(14) << "\\            /     " << tileRes(15) << "\\" << std::endl;
    std::cout << "|" << formatCrit(36) << "|       " << tileVal(13) << "       |" << formatCrit(37) << "|--" << formatGoal(52) << "--|" << formatCrit(38) << "|       " << tileVal(14) << "      |" << formatCrit(39) << "|--" << formatGoal(53) << "--|" << formatCrit(40) << "|        " << tileVal(15) << "       |" << formatCrit(41) << "|" << std::endl;
    std::cout << "   \\                /            \\                /            \\                /" << std::endl;
    std::cout << "   " << formatGoal(54) << "             " << formatGoal(55) << "      " << tileIndex(16) << "     " << formatGoal(56) << "             " << formatGoal(57) << "      " << tileIndex(17) << "     " << formatGoal(58) << "             " << formatGoal(59) << std::endl;
    std::cout << "     \\            /     " << tileRes(16) << "\\            /     " << tileRes(17) << "\\            /" << std::endl;
    std::cout << "     |" << formatCrit(42) << "|--" << formatGoal(60) << "--|" << formatCrit(43) << "|       " << tileVal(16) << "       |" << formatCrit(44) << "|--" << formatGoal(61) << "--|" << formatCrit(45) << "|       " << tileVal(17) << "       |" << formatCrit(46) << "|--" << formatGoal(62) << "--|" << formatCrit(47) << "|" << std::endl;
    std::cout << "                  \\                /            \\                /" << std::endl;
    std::cout << "                  " << formatGoal(63) << "             " << formatGoal(64) << "      " << tileIndex(18) << "     " << formatGoal(65) << "             " << formatGoal(66) << std::endl;
    std::cout << "                    \\            /     " << tileRes(18) << "\\            /" << std::endl;
    std::cout << "                    |" << formatCrit(48) << "|--" << formatGoal(67) << "--|" << formatCrit(49) << "|       " << tileVal(18) << "       |" << formatCrit(50) << "|--" << formatGoal(68) << "--|" << formatCrit(51) << "|" << std::endl;
    std::cout << "                                 \\                /" << std::endl;
    std::cout << "                                 " << formatGoal(69) << "             " << formatGoal(70) << std::endl;
    std::cout << "                                   \\            /" << std::endl;
    std::cout << "                                   |" << formatCrit(52) << "|--" << formatGoal(71) << "--|" << formatCrit(53) << "|" << std::endl;
}

