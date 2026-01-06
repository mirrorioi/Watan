import Game;
import Dice;
import <iostream>;
import <string>;
import <random>;
import <stdexcept>;

int main(int argc, char* argv[]) {
    std::random_device rd;
    seed(rd());
    
    std::string loadFile = "";
    std::string boardFile = "";
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-seed" && i + 1 < argc) {
            int s = std::stoi(argv[i + 1]);
            seed(s);
            ++i;
        } else if (arg == "-load" && i + 1 < argc) {
            loadFile = argv[i + 1];
            ++i;
        } else if (arg == "-board" && i + 1 < argc) {
            boardFile = argv[i + 1];
            ++i;
        }
    }
    
    try {
        Game game;
        
        if (!loadFile.empty()) {
            game.loadGame(loadFile);
        } else {
            if (!boardFile.empty()) {
                game.loadBoard(boardFile);
            } else {
                game.initGame();
            }
        }
        
        game.run();
    } catch (const std::runtime_error& e) {
        return 0;
    }
    
    return 0;
}
