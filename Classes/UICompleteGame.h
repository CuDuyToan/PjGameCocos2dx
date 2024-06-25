#ifndef __UI_COMPLETE_GAME_H__
#define __UI_COMPLETE_GAME_H__

#include "cocos2d.h"
#include "LevelScene.h"

class UICompleteGame : public cocos2d::Layer {
public:

    virtual bool init();

    void createButtons();

    CREATE_FUNC(UICompleteGame);


private:
    void createPanel();

};

#endif // __MENU_SCENE_H__
