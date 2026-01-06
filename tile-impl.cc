import Tile;
import <string>;
import <vector>;

Tile::Tile(int id, const std::string& resource, int val)
    : tileId(id), resourceType(resource), value(val), hasGeeseOnTile(false) {}

int Tile::getId() const { return tileId; }
std::string Tile::getResourceType() const { return resourceType; }
int Tile::getValue() const { return value; }
bool Tile::hasGeese() const { return hasGeeseOnTile; }
void Tile::setGeese(bool geese) { hasGeeseOnTile = geese; }

void Tile::addAdjacentCriterion(int criterionNum) {
    adjacentCriteria.push_back(criterionNum);
}

const std::vector<int>& Tile::getAdjacentCriteria() const {
    return adjacentCriteria;
}
