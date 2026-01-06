import Criterion;
import Student;
import <sstream>;
import <vector>;

Criterion::Criterion(int num) 
    : criterionNum(num), completionLevel(0), owner(nullptr) {}

void Criterion::addAdjacentCriterion(int criterionNum) {
    adjacentCriteria.push_back(criterionNum);
}

void Criterion::addAdjacentGoal(int goalNum) {
    adjacentGoals.push_back(goalNum);
}

void Criterion::complete(Student* student) {
    owner = student;
    completionLevel = 1;
}

void Criterion::improve(Student* student) {
    if (owner == student && completionLevel < 3) {
        completionLevel++;
    }
}

int Criterion::getNumber() const { return criterionNum; }
int Criterion::getCompletionLevel() const { return completionLevel; }
Student* Criterion::getOwner() const { return owner; }

std::string Criterion::getOwnerColor() const {
    if (!owner) return "";
    return owner->getColour();
}

const std::vector<int>& Criterion::getAdjacentCriteria() const {
    return adjacentCriteria;
}

const std::vector<int>& Criterion::getAdjacentGoals() const {
    return adjacentGoals;
}

std::string Criterion::display() const {
    if (completionLevel == 0) {
        std::ostringstream oss;
        if (criterionNum < 10) oss << " ";
        oss << criterionNum;
        return oss.str();
    }
    
    char colorChar = getOwnerColor()[0];
    char levelChar = 'A';
    if (completionLevel == 1) levelChar = 'A';
    else if (completionLevel == 2) levelChar = 'M';
    else if (completionLevel == 3) levelChar = 'E';
    
    std::string result;
    result += colorChar;
    result += levelChar;
    return result;
}
