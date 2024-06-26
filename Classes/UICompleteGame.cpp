﻿#include "UICompleteGame.h"
#include "GameScene.h"
#include "LevelScene.h"
#include "LevelSelectScene.h"

USING_NS_CC;

bool UICompleteGame::init() {
    if (!Layer::init())
    {
        return false;
    }

    //this->setVisible(false);

    //createButtons();

    return true;
}

void UICompleteGame::createButtons(int currentLevel)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Clear any existing buttons
    this->removeAllChildren();

    createPanel();
    
    // Create "Chơi Lại" button
    auto replayButton = MenuItemImage::create(
        "UICompleteGame/DefaultRepeat.png",
        "UICompleteGame/Repeat.png",
        [](Ref* sender) {
            
            Director::getInstance()->replaceScene(GameScene::create());
        });
    replayButton->setScale(0.2);
    replayButton->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - 20));

    // Create "Next Level" button
    auto nextLevelButton = MenuItemImage::create(
        "UICompleteGame/DefaultNextLevel.png",
        "UICompleteGame/NextLevel.png",
        [currentLevel](Ref* sender) {
            auto nextScene = LevelSelectScene::create();
            nextScene->createButtonPageLevel(currentLevel + 1);
            Director::getInstance()->replaceScene(nextScene);
        
        });
    nextLevelButton->setScale(0.2);
    nextLevelButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 20));

    auto backLevelButton = MenuItemImage::create(
        "UICompleteGame/DefaultListLevel.png",
        "UICompleteGame/ListLevel.png",
        [](Ref* sender) {
            auto levelScene = LevelScene::createScene();
            Director::getInstance()->replaceScene(levelScene);
        });
    backLevelButton->setScale(0.2);
    backLevelButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - 20));


    // Create menu and add buttons
    auto menu = Menu::create(replayButton, nextLevelButton, backLevelButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void UICompleteGame::createPanel()
{   
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroundOverlay = LayerColor::create(Color4B(0, 0, 0, 150), visibleSize.width, visibleSize.height);
    backgroundOverlay->setPosition(Vec2(origin.x, origin.y));
    this->addChild(backgroundOverlay);

    auto panel = Sprite::create("UICompleteGame/PanelMedium.png");
    panel->setScale(0.2f);
    panel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(panel);

    /*auto label = Label::createWithSystemFont("Level Complete", "fonts / Marker Felt.ttf", 150);
    label->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - label->getContentSize().height));
    panel->addChild(label);*/
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 150, GlyphCollection::DYNAMIC);
    ttfConfig.bold = true;
    auto label = Label::createWithTTF(ttfConfig, "Level Complete");
    label->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - label->getContentSize().height));
    panel->addChild(label);
}