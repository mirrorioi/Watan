module Dice;
import <random>;
import <iostream>;

static std::mt19937 rng;

void seed(int s) {
    rng.seed(s);
}

int getRandom(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

int FairDice::roll() {
    return getRandom(1, 6) + getRandom(1, 6);
}

int LoadedDice::roll() {
    int rollValue;
    while (true) {
        std::cout << "Input a roll between 2 and 12:" << std::endl;
        std::cout << "> ";
        if (!(std::cin >> rollValue)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid roll." << std::endl;
            continue;
        }
        if (rollValue < 2 || rollValue > 12) {
            std::cout << "Invalid roll." << std::endl;
            continue;
        }
        break;
    }
    return rollValue;
}
