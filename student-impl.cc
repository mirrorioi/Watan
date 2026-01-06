import Student;
import Dice;
import Resources;
import <iostream>;
import <vector>;
import <memory>;

Student::Student(const std::string& col, int num) 
    : colour(col), playerNum(num), dice(std::make_unique<FairDice>()) {}

std::string Student::getColour() const { return colour; }
int Student::getPlayerNum() const { return playerNum; }
Resources& Student::getResources() { return resources; }

std::vector<int>& Student::getOwnedCriteriaIds() { return ownedCriteriaIds; }
std::vector<int>& Student::getOwnedGoalIds() { return ownedGoalIds; }

void Student::addCriterion(int criterionId) {
    ownedCriteriaIds.push_back(criterionId);
}

void Student::addGoal(int goalId) {
    ownedGoalIds.push_back(goalId);
}

void Student::gainResource(const std::string& type, int amount) {
    resources.add(type, amount);
}

void Student::spendResource(const std::string& type, int amount) {
    resources.subtract(type, amount);
}

bool Student::hasEnough(const std::string& type, int amount) const {
    return resources.hasEnough(type, amount);
}

void Student::setDiceLoaded(bool loaded) {
    if (loaded) {
        dice = std::make_unique<LoadedDice>();
    } else {
        dice = std::make_unique<FairDice>();
    }
}

int Student::rollDice() {
    return dice->roll();
}

void Student::printStatus() const {
    std::cout << colour << " has " << ownedCriteriaIds.size() 
              << " course criteria, " << resources.getCaffeine() << " caffeines, "
              << resources.getLab() << " labs, " << resources.getLecture() 
              << " lectures, " << resources.getStudy() << " studies, and "
              << resources.getTutorial() << " tutorials." << std::endl;
}

void Student::printCompleted() const {
    std::cout << colour << " has completed:" << std::endl;
}

void Student::printResources() const {
    std::cout << resources.getCaffeine() << " caffeines, "
              << resources.getLab() << " labs, " << resources.getLecture() 
              << " lectures, " << resources.getStudy() << " studies, "
              << resources.getTutorial() << " tutorials" << std::endl;
}

int Student::getCriteriaCount() const {
    return ownedCriteriaIds.size();
}
