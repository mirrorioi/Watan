export module Tile;
import <string>;
import <vector>;

export class Tile {
    int tileId;
    std::string resourceType;
    int value;
    bool hasGeeseOnTile;
    std::vector<int> adjacentCriteria; // 6 criteria around the tile

public:
    Tile(int id, const std::string& resource, int val);
    
    int getId() const;
    std::string getResourceType() const;
    int getValue() const;
    bool hasGeese() const;
    void setGeese(bool geese);
    
    void addAdjacentCriterion(int criterionNum);
    const std::vector<int>& getAdjacentCriteria() const;
};
