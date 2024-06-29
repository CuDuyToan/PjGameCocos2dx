#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "cocos2d.h"

class Settings : public cocos2d::Layer {
public:

    virtual bool init();

    void createSettingsButton();

    void createButtons();

    void createPanel();

    CREATE_FUNC(Settings);

};

#endif // __SETTINGS_H__
