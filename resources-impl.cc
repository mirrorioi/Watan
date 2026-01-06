import Resources;
import <algorithm>;
import <string>;

Resources::Resources() : caffeine(0), lab(0), lecture(0), study(0), tutorial(0) {}

int Resources::getCaffeine() const { return caffeine; }
int Resources::getLab() const { return lab; }
int Resources::getLecture() const { return lecture; }
int Resources::getStudy() const { return study; }
int Resources::getTutorial() const { return tutorial; }

void Resources::setCaffeine(int amount) { caffeine = amount; }
void Resources::setLab(int amount) { lab = amount; }
void Resources::setLecture(int amount) { lecture = amount; }
void Resources::setStudy(int amount) { study = amount; }
void Resources::setTutorial(int amount) { tutorial = amount; }

void Resources::add(const std::string& type, int amount) {
    if (type == "CAFFEINE") caffeine += amount;
    else if (type == "LAB") lab += amount;
    else if (type == "LECTURE") lecture += amount;
    else if (type == "STUDY") study += amount;
    else if (type == "TUTORIAL") tutorial += amount;
}

void Resources::subtract(const std::string& type, int amount) {
    if (type == "CAFFEINE") caffeine = std::max(0, caffeine - amount);
    else if (type == "LAB") lab = std::max(0, lab - amount);
    else if (type == "LECTURE") lecture = std::max(0, lecture - amount);
    else if (type == "STUDY") study = std::max(0, study - amount);
    else if (type == "TUTORIAL") tutorial = std::max(0, tutorial - amount);
}

bool Resources::hasEnough(const std::string& type, int amount) const {
    if (type == "CAFFEINE") return caffeine >= amount;
    if (type == "LAB") return lab >= amount;
    if (type == "LECTURE") return lecture >= amount;
    if (type == "STUDY") return study >= amount;
    if (type == "TUTORIAL") return tutorial >= amount;
    return false;
}

int Resources::getTotalCount() const {
    return caffeine + lab + lecture + study + tutorial;
}

void Resources::loseHalf() {
    caffeine /= 2;
    lab /= 2;
    lecture /= 2;
    study /= 2;
    tutorial /= 2;
}
