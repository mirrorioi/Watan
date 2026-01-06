import Goal;
import Student;
import <sstream>;
import <vector>;

Goal::Goal(int num) : goalNum(num), owner(nullptr) {}

void Goal::addAdjacentCriterion(int criterionNum) {
    adjacentCriteria.push_back(criterionNum);
}

void Goal::addAdjacentGoal(int goalNum) {
    adjacentGoals.push_back(goalNum);
}

void Goal::achieve(Student* student) {
    owner = student;
}

int Goal::getNumber() const { return goalNum; }
Student* Goal::getOwner() const { return owner; }
bool Goal::hasOwner() const { return owner != nullptr; }

std::string Goal::getOwnerColor() const {
    if (!owner) return "";
    return owner->getColour();
}

const std::vector<int>& Goal::getAdjacentCriteria() const {
    return adjacentCriteria;
}

const std::vector<int>& Goal::getAdjacentGoals() const {
    return adjacentGoals;
}

std::string Goal::display() const {
    if (!owner) {
        std::ostringstream oss;
        if (goalNum < 10) oss << " ";
        oss << goalNum;
        return oss.str();
    }
    
    char colorChar = getOwnerColor()[0];
    std::string result;
    result += colorChar;
    result += 'A';
    return result;
}
