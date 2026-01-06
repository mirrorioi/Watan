export module Goal;
import Student;
import <string>;
import <vector>;

export class Goal {
    int goalNum;
    Student* owner;
    std::vector<int> adjacentCriteria;
    std::vector<int> adjacentGoals;

public:
    Goal(int num);
    
    void addAdjacentCriterion(int criterionNum);
    void addAdjacentGoal(int goalNum);
    
    void achieve(Student* student);
    
    int getNumber() const;
    Student* getOwner() const;
    std::string getOwnerColor() const;
    bool hasOwner() const;
    
    const std::vector<int>& getAdjacentCriteria() const;
    const std::vector<int>& getAdjacentGoals() const;
    
    std::string display() const;
};
