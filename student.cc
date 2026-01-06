export module Student;
import Resources;
import Dice;
import <string>;
import <vector>;
import <memory>;

export class Student {
    std::string colour;
    int playerNum;
    Resources resources;
    std::vector<int> ownedCriteriaIds;
    std::vector<int> ownedGoalIds;
    std::unique_ptr<Dice> dice;

public:
    Student(const std::string& col, int num);
    
    std::string getColour() const;
    int getPlayerNum() const;
    Resources& getResources();
    
    std::vector<int>& getOwnedCriteriaIds();
    std::vector<int>& getOwnedGoalIds();
    
    void addCriterion(int criterionId);
    void addGoal(int goalId);
    
    void gainResource(const std::string& type, int amount);
    void spendResource(const std::string& type, int amount);
    bool hasEnough(const std::string& type, int amount) const;
    
    void setDiceLoaded(bool loaded);
    int rollDice();
    
    void printStatus() const;
    void printCompleted() const;
    void printResources() const;
    
    int getCriteriaCount() const;
};
