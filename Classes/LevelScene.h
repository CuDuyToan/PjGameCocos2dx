#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"

class LevelScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void createButtonLevel();

    void buttonBackHome();

    CREATE_FUNC(LevelScene);

private:
    int level = 1;

};

#endif // __LEVEL_SCENE_H__
