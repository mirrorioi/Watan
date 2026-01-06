import Geese;

Geese::Geese() : position(-1) {}

void Geese::moveTo(int tileNum) {
    position = tileNum;
}

int Geese::getPosition() const {
    return position;
}

bool Geese::isOnBoard() const {
    return position >= 0;
}
