#ifndef __LEVEL_SELECT_H__
#define __LEVEL_SELECT_H__

#include "cocos2d.h"

using namespace cocos2d;

class LevelSelectScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // callback khi người chơi chọn một cấp độ cụ thể
    void levelSelectedCallback(cocos2d::Ref* pSender);
    void homeButton(cocos2d::Ref* pSender);

    void menuButton();
    void createCasualButton();
    void createLevelText();
    void mainBackGround();

    void createButtonChangeLevel(const std::string& nameLevel);

    void readStory(int level);
    void showTextWithEffect(const std::string& text, int level);
    void addCharacterByCharacter(const std::string& text, float delay, int level);

    void saveNextLevelButtonState(bool state);

    bool loadNextLevelButtonState();

    void saveLevel(int level);

    int loadLevel();
    bool checkPathExists(int level);

    void selectLevel(int levelSelect);

    void createButtonPageLevel(int level);

    CREATE_FUNC(LevelSelectScene);

private:
    bool stopFlag = false;
    float sizeTable = 1;
    int a = 2;
};


#endif // __LEVEL_SELECT_H__