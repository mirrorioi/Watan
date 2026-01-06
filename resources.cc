export module Resources;
import <string>;

export class Resources {
    int caffeine;
    int lab;
    int lecture;
    int study;
    int tutorial;

public:
    Resources();
    
    int getCaffeine() const;
    int getLab() const;
    int getLecture() const;
    int getStudy() const;
    int getTutorial() const;
    
    void setCaffeine(int amount);
    void setLab(int amount);
    void setLecture(int amount);
    void setStudy(int amount);
    void setTutorial(int amount);
    
    void add(const std::string& type, int amount);
    void subtract(const std::string& type, int amount);
    bool hasEnough(const std::string& type, int amount) const;
    int getTotalCount() const;
    void loseHalf();
};
