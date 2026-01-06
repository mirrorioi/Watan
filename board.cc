export module Board;
import Tile;
import Criterion;
import Goal;
import Student;
import <vector>;
import <string>;

export class Board {
    std::vector<Tile> tiles;
    std::vector<Criterion> criteria;
    std::vector<Goal> goals;
    
    void setupAdjacencies();

public:
    Board();
    
    void randomInitialize();
    void loadFromFile(const std::string& filename);
    void loadBoardLayout(const std::vector<int>& layout);
    
    void display() const;
    
    Tile& getTile(int tileNum);
    Criterion& getCriterion(int criterionNum);
    Goal& getGoal(int goalNum);
    
    bool canComplete(Student* student, int criterionNum);
    bool canImprove(Student* student, int criterionNum);
    bool canAchieve(Student* student, int goalNum);
    
    std::vector<int> getCriteriaForTile(int tileNum) const;
};
