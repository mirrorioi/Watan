import Game;
import Board;
import Student;
import Geese;
import Tile;
import Criterion;
import Goal;
import Resources;
import Dice;
import <iostream>;
import <fstream>;
import <sstream>;
import <algorithm>;
import <map>;
import <memory>;
import <vector>;
import <stdexcept>;

Game::Game() : currentTurn(0), gameWon(false), diceRolled(false) {
    initStudents();
}

void Game::initStudents() {
    students.clear();
    // RAII Used feature
    students.push_back(std::make_unique<Student>("Blue", 0));
    students.push_back(std::make_unique<Student>("Red", 1));
    students.push_back(std::make_unique<Student>("Orange", 2));
    students.push_back(std::make_unique<Student>("Yellow", 3));
}

Student* Game::getCurrentStudent() {
    return students[currentTurn].get();
}

void Game::initGame() {
    board.randomInitialize();
    setupInitialCriteria();
}

void Game::setupInitialCriteria() {
    std::vector<int> order = {0, 1, 2, 3, 3, 2, 1, 0};
    
    for (int studentNum : order) {
        Student* student = students[studentNum].get();
        std::cout << "Student " << student->getColour() 
                  << ", where do you want to complete an Assignment?" << std::endl;
        
        int criterionNum;
        while (true) {
            std::cout << "> ";
            if (!(std::cin >> criterionNum)) {
                if (std::cin.eof()) {
                    saveGame("backup.sv");
                    throw std::runtime_error("End of input");
                }
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            
            if (criterionNum < 0 || criterionNum >= 54) {
                std::cout << "You cannot build here." << std::endl;
                continue;
            }
            
            if (!board.canComplete(student, criterionNum)) {
                std::cout << "You cannot build here." << std::endl;
                continue;
            }
            
            break;
        }
        
        Criterion& crit = board.getCriterion(criterionNum);
        crit.complete(student);
        student->addCriterion(criterionNum);
    }
    
    printBoard();
}

void Game::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return;
    
    file >> currentTurn;
    
    for (int i = 0; i < 4; ++i) {
        int caff, lab, lec, study, tut;
        file >> caff >> lab >> lec >> study >> tut;
        
        students[i]->getResources().setCaffeine(caff);
        students[i]->getResources().setLab(lab);
        students[i]->getResources().setLecture(lec);
        students[i]->getResources().setStudy(study);
        students[i]->getResources().setTutorial(tut);
        
        char g;
        file >> g;
        
        int goalNum;
        std::string temp;
        std::getline(file, temp);
        std::istringstream iss(temp);
        while (iss >> temp) {
            if (temp == "c") break;
            goalNum = std::stoi(temp);
            Goal& goal = board.getGoal(goalNum);
            goal.achieve(students[i].get());
            students[i]->addGoal(goalNum);
        }
    }
    
    std::vector<int> layout;
    int val;
    while (file >> val && layout.size() < 38) {
        layout.push_back(val);
    }
    board.loadBoardLayout(layout);
    
    int geesePos;
    file >> geesePos;
    if (geesePos >= 0 && geesePos < 19) {
        geese.moveTo(geesePos);
        board.getTile(geesePos).setGeese(true);
    }
}

void Game::loadBoard(const std::string& filename) {
    board.loadFromFile(filename);
}

void Game::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    
    file << currentTurn << std::endl;
    
    for (int i = 0; i < 4; ++i) {
        const Resources& res = students[i]->getResources();
        file << res.getCaffeine() << " " << res.getLab() << " " 
             << res.getLecture() << " " << res.getStudy() << " " 
             << res.getTutorial() << " g";
        
        for (int goalId : students[i]->getOwnedGoalIds()) {
            file << " " << goalId;
        }
        
        file << " c";
        for (int critId : students[i]->getOwnedCriteriaIds()) {
            Criterion& crit = board.getCriterion(critId);
            file << " " << critId << " " << crit.getCompletionLevel();
        }
        file << std::endl;
    }
    
    std::vector<std::string> resourceMap = {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL", "NETFLIX"};
    for (int i = 0; i < 19; ++i) {
        Tile& tile = board.getTile(i);
        std::string resType = tile.getResourceType();
        int resNum = std::find(resourceMap.begin(), resourceMap.end(), resType) - resourceMap.begin();
        file << resNum << " " << tile.getValue();
        if (i < 18) file << " ";
    }
    file << std::endl;
    
    file << geese.getPosition() << std::endl;
}

void Game::distributeResources(int rollValue) {
    std::map<int, std::map<std::string, int>> studentGains;
    // search: studentGains(ID, resourceType) = amount
    
    for (int i = 0; i < 19; ++i) {
        Tile& tile = board.getTile(i);
        
        if (tile.getValue() != rollValue) continue;
        if (tile.hasGeese()) continue;
        
        std::string resourceType = tile.getResourceType();
        if (resourceType == "NETFLIX") continue;
        
        for (int critId : tile.getAdjacentCriteria()) {
            Criterion& crit = board.getCriterion(critId);
            Student* owner = crit.getOwner();
            
            if (owner) {
                int amount = crit.getCompletionLevel();
                studentGains[owner->getPlayerNum()][resourceType] += amount;
            }
        }
    }
    
    if (studentGains.empty()) {
        std::cout << "No students gained resources." << std::endl;
        return;
    }
    
    std::vector<std::string> resourceOrder = {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"};
    
    for (int i = 0; i < 4; ++i) {
        if (studentGains.count(i) == 0) continue;
        
        std::cout << "Student " << students[i]->getColour() << " gained:" << std::endl;
        
        for (const std::string& resType : resourceOrder) {
            if (studentGains[i].count(resType) > 0) {
                int amount = studentGains[i][resType];
                students[i]->gainResource(resType, amount);
                std::cout << amount << " " << resType << std::endl;
            }
        }
    }
}

void Game::handleSevenRoll() {
    std::vector<std::string> resourceTypes = {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"};
    
    for (auto& student : students) {
        int total = student->getResources().getTotalCount();
        if (total >= 10) {
            int toLose = total / 2;
            std::cout << "Student " << student->getColour() << " loses " 
                      << toLose << " resources to the geese. They lose:" << std::endl;
            
            std::vector<std::string> available;
            for (const auto& resType : resourceTypes) {
                const Resources& res = student->getResources();
                int count = 0;
                if (resType == "CAFFEINE") count = res.getCaffeine();
                else if (resType == "LAB") count = res.getLab();
                else if (resType == "LECTURE") count = res.getLecture();
                else if (resType == "STUDY") count = res.getStudy();
                else if (resType == "TUTORIAL") count = res.getTutorial();
                
                for (int i = 0; i < count; ++i) {
                    available.push_back(resType);
                }
            }
            
            std::map<std::string, int> lost;
            for (int i = 0; i < toLose; ++i) {
                int idx = getRandom(0, available.size() - 1);
                std::string resType = available[idx];
                lost[resType]++;
                available.erase(available.begin() + idx);
            }
            
            for (const auto& resType : resourceTypes) {
                if (lost.count(resType) > 0) {
                    std::cout << lost[resType] << " " << resType << std::endl;
                    student->spendResource(resType, lost[resType]);
                }
            }
        }
    }
    
    std::cout << "Choose where to place the GEESE." << std::endl;
    int tileNum;
    while (true) {
        std::cout << "> ";
        if (!(std::cin >> tileNum)) {
            if (std::cin.eof()) {
                saveGame("backup.sv");
                throw std::runtime_error("End of input");
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid command." << std::endl;
            continue;
        }
        
        if (tileNum < 0 || tileNum >= 19) {
            std::cout << "Invalid command." << std::endl;
            continue;
        }
        
        if (geese.isOnBoard() && geese.getPosition() == tileNum) {
            std::cout << "Invalid command." << std::endl;
            continue;
        }
        
        break;
    }
    
    if (geese.isOnBoard()) {
        board.getTile(geese.getPosition()).setGeese(false);
    }
    
    geese.moveTo(tileNum);
    board.getTile(tileNum).setGeese(true);
    
    std::vector<Student*> victims;
    for (int critId : board.getTile(tileNum).getAdjacentCriteria()) {
        Criterion& crit = board.getCriterion(critId);
        Student* owner = crit.getOwner();
        
        if (owner && owner != getCurrentStudent() && owner->getResources().getTotalCount() > 0) {
            if (std::find(victims.begin(), victims.end(), owner) == victims.end()) {
                victims.push_back(owner);
            }
        }
    }
    
    if (victims.empty()) {
        std::cout << "Student " << getCurrentStudent()->getColour() 
                  << " has no students to steal from." << std::endl;
        return;
    }
    
    std::sort(victims.begin(), victims.end(), [](Student* a, Student* b) {
        return a->getPlayerNum() < b->getPlayerNum();
    });
    
    std::cout << "Student " << getCurrentStudent()->getColour() 
              << " can choose to steal from";
    for (size_t i = 0; i < victims.size(); ++i) {
        if (i > 0) std::cout << ",";
        std::cout << " " << victims[i]->getColour();
    }
    std::cout << "." << std::endl;
    
    std::cout << "Choose a student to steal from." << std::endl;
    
    Student* victim = nullptr;
    while (true) {
        std::cout << "> ";
        std::string colour;
        if (!(std::cin >> colour)) {
            if (std::cin.eof()) {
                saveGame("backup.sv");
                throw std::runtime_error("End of input");
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid command." << std::endl;
            continue;
        }
        
        for (Student* v : victims) {
            if (v->getColour() == colour) {
                victim = v;
                break;
            }
        }
        
        if (victim) break;
        std::cout << "Invalid command." << std::endl;
    }
    
    std::vector<std::string> availableRes;
    std::vector<std::string> resTypes = {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL"};
    
    for (const auto& resType : resTypes) {
        const Resources& res = victim->getResources();
        int count = 0;
        if (resType == "CAFFEINE") count = res.getCaffeine();
        else if (resType == "LAB") count = res.getLab();
        else if (resType == "LECTURE") count = res.getLecture();
        else if (resType == "STUDY") count = res.getStudy();
        else if (resType == "TUTORIAL") count = res.getTutorial();
        
        for (int i = 0; i < count; ++i) {
            availableRes.push_back(resType);
        }
    }
    
    if (!availableRes.empty()) {
        int idx = getRandom(0, availableRes.size() - 1);
        std::string stolen = availableRes[idx];
        
        victim->spendResource(stolen, 1);
        getCurrentStudent()->gainResource(stolen, 1);
        
        std::cout << "Student " << getCurrentStudent()->getColour() 
                  << " steals " << stolen << " from student " 
                  << victim->getColour() << "." << std::endl;
    }
}

void Game::beginTurn() {
    Student* current = getCurrentStudent();
    std::cout << "Student " << current->getColour() << "'s turn." << std::endl;
    current->printStatus();
    
    diceRolled = false;
    
    while (!diceRolled) {
        std::cout << "> ";
        std::string command;
        if (!(std::cin >> command)) {
            if (std::cin.eof()) {
                saveGame("backup.sv");
                throw std::runtime_error("End of input");
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid command." << std::endl;
            continue;
        }
        
        if (command == "load") {
            current->setDiceLoaded(true);
        } else if (command == "fair") {
            current->setDiceLoaded(false);
        } else if (command == "roll") {
            int rollValue = current->rollDice();
            diceRolled = true;
            
            if (rollValue == 7) {
                handleSevenRoll();
            } else {
                distributeResources(rollValue);
            }
        } else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}

void Game::endTurn() {
    while (true) {
        std::cout << "> ";
        std::string command;
        std::cin >> command;
        
        if (std::cin.eof()) {
            saveGame("backup.sv");
            throw std::runtime_error("End of input");
        }
        
        if (command == "board") {
            printBoard();
        } else if (command == "status") {
            printStatus();
        } else if (command == "criteria") {
            printCriteria();
        } else if (command == "achieve") {
            int goalNum;
            std::cin >> goalNum;
            completeGoal(goalNum);
        } else if (command == "complete") {
            int criterionNum;
            std::cin >> criterionNum;
            completeCriterion(criterionNum);
        } else if (command == "improve") {
            int criterionNum;
            std::cin >> criterionNum;
            improveCriterion(criterionNum);
        } else if (command == "trade") {
            std::string colour, give, take;
            std::cin >> colour >> give >> take;
            handleTrade(colour, give, take);
        } else if (command == "next") {
            currentTurn = (currentTurn + 1) % 4;
            break;
        } else if (command == "save") {
            std::string filename;
            std::cin >> filename;
            saveGame(filename);
        } else if (command == "help") {
            printHelp();
        } else {
            std::cout << "Invalid command." << std::endl;
        }
        
        if (checkWin()) {
            gameWon = true;
            break;
        }
    }
}

bool Game::completeGoal(int goalNum) {
    Student* current = getCurrentStudent();
    
    if (goalNum < 0 || goalNum >= 72) {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
    
    if (!board.canAchieve(current, goalNum)) {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
    
    if (!current->hasEnough("STUDY", 1) || !current->hasEnough("TUTORIAL", 1)) {
        std::cout << "You do not have enough resources." << std::endl;
        return false;
    }
    
    current->spendResource("STUDY", 1);
    current->spendResource("TUTORIAL", 1);
    
    Goal& goal = board.getGoal(goalNum);
    goal.achieve(current);
    current->addGoal(goalNum);
    
    return true;
}

bool Game::completeCriterion(int criterionNum) {
    Student* current = getCurrentStudent();
    
    if (criterionNum < 0 || criterionNum >= 54) {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
    
    if (!board.canComplete(current, criterionNum)) {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
    
    if (!current->hasEnough("CAFFEINE", 1) || !current->hasEnough("LAB", 1) ||
        !current->hasEnough("LECTURE", 1) || !current->hasEnough("TUTORIAL", 1)) {
        std::cout << "You do not have enough resources." << std::endl;
        return false;
    }
    
    current->spendResource("CAFFEINE", 1);
    current->spendResource("LAB", 1);
    current->spendResource("LECTURE", 1);
    current->spendResource("TUTORIAL", 1);
    
    Criterion& crit = board.getCriterion(criterionNum);
    crit.complete(current);
    current->addCriterion(criterionNum);
    
    return true;
}

bool Game::improveCriterion(int criterionNum) {
    Student* current = getCurrentStudent();
    
    if (criterionNum < 0 || criterionNum >= 54) {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
    
    if (!board.canImprove(current, criterionNum)) {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
    
    Criterion& crit = board.getCriterion(criterionNum);
    int level = crit.getCompletionLevel();
    
    if (level == 1) {
        if (!current->hasEnough("LECTURE", 2) || !current->hasEnough("STUDY", 3)) {
            std::cout << "You do not have enough resources." << std::endl;
            return false;
        }
        current->spendResource("LECTURE", 2);
        current->spendResource("STUDY", 3);
    } else if (level == 2) {
        if (!current->hasEnough("CAFFEINE", 3) || !current->hasEnough("LAB", 2) ||
            !current->hasEnough("LECTURE", 2) || !current->hasEnough("TUTORIAL", 1) ||
            !current->hasEnough("STUDY", 2)) {
            std::cout << "You do not have enough resources." << std::endl;
            return false;
        }
        current->spendResource("CAFFEINE", 3);
        current->spendResource("LAB", 2);
        current->spendResource("LECTURE", 2);
        current->spendResource("TUTORIAL", 1);
        current->spendResource("STUDY", 2);
    }
    
    crit.improve(current);
    return true;
}

void Game::handleTrade(const std::string& targetColour, const std::string& give, const std::string& take) {
    Student* current = getCurrentStudent();
    Student* target = nullptr;
    
    for (auto& student : students) {
        if (student->getColour() == targetColour) {
            target = student.get();
            break;
        }
    }
    
    if (!target || target == current) {
        std::cout << "Invalid command." << std::endl;
        return;
    }
    
    if (!current->hasEnough(give, 1)) {
        std::cout << "You do not have enough resources." << std::endl;
        return;
    }
    
    if (!target->hasEnough(take, 1)) {
        std::cout << "You do not have enough resources." << std::endl;
        return;
    }
    
    std::cout << current->getColour() << " offers " << target->getColour() 
              << " one " << give << " for one " << take << "." << std::endl;
    std::cout << "Does " << target->getColour() << " accept this offer?" << std::endl;
    
    std::cout << "> ";
    std::string response;
    std::cin >> response;
    
    if (response == "yes") {
        current->spendResource(give, 1);
        current->gainResource(take, 1);
        target->gainResource(give, 1);
        target->spendResource(take, 1);
    }
}

bool Game::checkWin() {
    Student* current = getCurrentStudent();
    if (current->getCriteriaCount() >= 10) {
        winner = current->getColour();
        std::cout << "Student " << winner << " has won!" << std::endl;
        return true;
    }
    return false;
}

void Game::printBoard() {
    board.display();
}

void Game::printStatus() {
    for (auto& student : students) {
        student->printStatus();
    }
}

void Game::printCriteria() {
    Student* current = getCurrentStudent();
    std::cout << current->getColour() << " has completed:" << std::endl;
    
    for (int critId : current->getOwnedCriteriaIds()) {
        Criterion& crit = board.getCriterion(critId);
        std::cout << critId << " " << crit.getCompletionLevel() << std::endl;
    }
}

void Game::printHelp() {
    std::cout << "Valid commands:" << std::endl;
    std::cout << "board" << std::endl;
    std::cout << "status" << std::endl;
    std::cout << "criteria" << std::endl;
    std::cout << "achieve <goal>" << std::endl;
    std::cout << "complete <criterion>" << std::endl;
    std::cout << "improve <criterion>" << std::endl;
    std::cout << "trade <colour> <give> <take>" << std::endl;
    std::cout << "next" << std::endl;
    std::cout << "save <file>" << std::endl;
    std::cout << "help" << std::endl;
}

void Game::run() {
    while (!gameWon) {
        beginTurn();
        endTurn();
    }
    
    std::cout << "Would you like to play again?" << std::endl;
    std::cout << "> ";
    std::string response;
    std::cin >> response;
    
    if (response == "yes") {
        students.clear();
        initStudents();
        geese = Geese();
        board = Board();
        currentTurn = 0;
        gameWon = false;
        winner = "";
        initGame();
        run();
    }
}
