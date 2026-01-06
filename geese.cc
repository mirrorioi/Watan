export module Geese;

export class Geese {
    int position; // -1 means not on board, otherwise tile number 0-18

public:
    Geese();
    
    void moveTo(int tileNum);
    int getPosition() const;
    bool isOnBoard() const;
};
