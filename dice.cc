export module Dice;

export void seed(int s);
export int getRandom(int min, int max);

export class Dice {
public:
    virtual ~Dice() = default;
    virtual int roll() = 0;
};

export class FairDice : public Dice {
public:
    int roll() override;
};

export class LoadedDice : public Dice {
public:
    int roll() override;
};
