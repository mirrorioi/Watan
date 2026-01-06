export module Game;
import Board;
import Student;
import Geese;
import <vector>;
import <string>;
import <memory>;

export class Game {
    Board board;
    std::vector<std::unique_ptr<Student>> students;
    Geese geese;
    int currentTurn;
    bool gameWon;
    std::string winner;
    bool diceRolled;
    
    void initStudents();
    void setupInitialCriteria();
    Student* getCurrentStudent();
    void distributeResources(int rollValue);
    void handleSevenRoll();
    void handleTrade(const std::string& targetColour, const std::string& give, const std::string& take);
    bool checkWin();
    void printHelp();
    
public:
    Game();
    
    void initGame();
    void loadGame(const std::string& filename);
    void loadBoard(const std::string& filename);
    void saveGame(const std::string& filename);
    
    void run();
    void beginTurn();
    void endTurn();
    
    void printBoard();
    void printStatus();
    void printCriteria();
    
    bool completeGoal(int goalNum);
    bool completeCriterion(int criterionNum);
    bool improveCriterion(int criterionNum);
};
