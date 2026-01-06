export module Criterion;
import Student;
import <string>;
import <vector>;

export class Criterion {
    int criterionNum;
    int completionLevel; // 0 = none, 1 = assignment, 2 = midterm, 3 = exam
    Student* owner;
    std::vector<int> adjacentCriteria;
    std::vector<int> adjacentGoals;

public:
    Criterion(int num);
    
    void addAdjacentCriterion(int criterionNum);
    void addAdjacentGoal(int goalNum);
    
    void complete(Student* student);
    void improve(Student* student);
    
    int getNumber() const;
    int getCompletionLevel() const;
    Student* getOwner() const;
    std::string getOwnerColor() const;
    
    const std::vector<int>& getAdjacentCriteria() const;
    const std::vector<int>& getAdjacentGoals() const;
    
    std::string display() const;
};
