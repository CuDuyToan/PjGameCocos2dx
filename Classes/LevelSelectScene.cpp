﻿#include "LevelSelectScene.h"
#include "MenuScene.h"
#include "GameScene.h"

#include "ui/CocosGUI.h"

USING_NS_CC;

cocos2d::Scene* LevelSelectScene::createScene() {
    auto scene = LevelSelectScene::create();
    return scene;
}

bool LevelSelectScene::init() {
    if (!Scene::init()) {
        return false;
    }


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    mainBackGround();
    menuButton();

    /*if (loadLevel() >= 2) {
        createButtonPageLevel2();
    }
    else {
        createButtonPageLevel1();
    }*/

    createButtonPageLevel(1);

    return true;
}

void LevelSelectScene::saveNextLevelButtonState(bool state) {
    UserDefault::getInstance()->setBoolForKey("next_level_button_state", state);
    UserDefault::getInstance()->flush();
}

bool LevelSelectScene::loadNextLevelButtonState() {
    return UserDefault::getInstance()->getBoolForKey("next_level_button_state", false);
}

void LevelSelectScene::saveLevel(int level) {
    UserDefault::getInstance()->setIntegerForKey("current_level", level);
    UserDefault::getInstance()->flush();
}

int LevelSelectScene::loadLevel() {
    return UserDefault::getInstance()->getIntegerForKey("current_level", 1);
}

void LevelSelectScene::mainBackGround()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto background = Sprite::create("BackGround/backGround.png");
    auto background = Sprite::create("BackGround/HoloBackGroundBlur.png");

    background->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    // Thiết lập kích thước của background bằng kích thước hiển thị của cửa sổ
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background, -1); // Đặt background ở layer thấp hơn để nó nằm dưới các phần tử khác của Scene
}

void LevelSelectScene::createButtonPageLevel(int level)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createButtonChangeLevel("Level " + std::to_string(level));

    auto storyBook = this->getChildByName("story book");

    if (storyBook)
    {
        if (storyBook->getChildByName("next level"))
        {
            storyBook->removeChildByName("next level");
        }
        if (storyBook->getChildByName("play level"))
        {
            storyBook->removeChildByName("play level");
        }
        if (storyBook->getChildByName("reduced level"))
        {
            storyBook->removeChildByName("reduced level");
        }
        if (level > 1) {

            auto reducedLevel = ui::Button::create("select level/button return.png", "select level/button return2.png");
            reducedLevel->setScale(0.08 * (sizeTable / reducedLevel->getContentSize().height));
            reducedLevel->setPosition(Vec2(reducedLevel->getContentSize().width,
                reducedLevel->getContentSize().height));
            reducedLevel->addClickEventListener([=](Ref* sender) {
                createButtonPageLevel(level - 1);
                });

            reducedLevel->setName("reduced level");
            storyBook->addChild(reducedLevel, 1);
        }

        auto playLevel = ui::Button::create("SquareButton/Play Square Button.png", "SquareButton/Play col_Square Button.png");
        playLevel->setScale(0.08 * (sizeTable / playLevel->getContentSize().height));
        playLevel->setPosition(Vec2(storyBook->getContentSize().width / 2, storyBook->getContentSize().height / 10));
        playLevel->addClickEventListener([=](Ref* sender) {
            auto nextScene = GameScene::create();
            Director::getInstance()->replaceScene(nextScene);
            });
        playLevel->setName("play level");
        storyBook->addChild(playLevel);

        auto nextLevel = ui::Button::create("select level/button next.png", "select level/button next2.png");
        nextLevel->setScale(0.08 * (sizeTable / nextLevel->getContentSize().height));
        nextLevel->setPosition(Vec2(storyBook->getContentSize().width - nextLevel->getContentSize().width,
            nextLevel->getContentSize().height));
        nextLevel->addClickEventListener([=](Ref* sender) {
            createButtonPageLevel(level + 1);
            });
        nextLevel->setVisible(loadLevel() >= level);
        nextLevel->setName("next level");
        storyBook->addChild(nextLevel);
    }
    else
    {
        CCLOG("cant found story book");
    }


}

void LevelSelectScene::createButtonChangeLevel(const std::string& nameLevel)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (!this->getChildByName("story book"))
    {
        auto tableLevel = Sprite::create("select level/storyBook.png");
        sizeTable = tableLevel->getContentSize().height;
        tableLevel->setScale(0.8 * (visibleSize.height / tableLevel->getContentSize().height));

        tableLevel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        tableLevel->setName("story book");
        addChild(tableLevel, 0);

        auto label = Label::createWithTTF(nameLevel, "fonts/Marker Felt.ttf", 24);      
        label->setAnchorPoint(Vec2(0.5, 0.5));
        label->setScale(0.05 * (sizeTable / label->getContentSize().height));
        label->setPosition(Vec2(tableLevel->getContentSize().width / 2,
            tableLevel->getContentSize().height - (label->getContentSize().height * 2)));
        label->setTextColor(Color4B::BLACK);

        label->setName("level name");
        tableLevel->addChild(label);
    }
    else
    {
        auto tableLevel = this->getChildByName("story book");
        auto labelOld = tableLevel->getChildByName("level name");
        if (labelOld)
        {
            labelOld->removeFromParentAndCleanup(true);
        }


        auto label = Label::createWithTTF(nameLevel, "fonts/Marker Felt.ttf", 24);
        label->setAnchorPoint(Vec2(0.5, 0.5));
        label->setScale(0.05 * (sizeTable / label->getContentSize().height));
        label->setPosition(Vec2(tableLevel->getContentSize().width / 2,
            tableLevel->getContentSize().height - (label->getContentSize().height * 2)));
        label->setTextColor(Color4B::BLACK);

        label->setName("level name");
        tableLevel->addChild(label);
    }
}



void LevelSelectScene::menuButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Tạo MenuItemImage cho nút Home
    auto homeNormal = Sprite::create("SquareButton/Home Square Button.png");
    auto homeSelected = Sprite::create("SquareButton/Home col_Square Button.png");
    auto homeButton = MenuItemSprite::create(homeNormal, homeSelected, CC_CALLBACK_1(LevelSelectScene::homeButton, this));
    /*homeButton->setContentSize(homeButton->getContentSize() * 0.1);*/
    homeButton->setScale(0.1 * (visibleSize.height / homeButton->getContentSize().height));
    homeButton->setAnchorPoint(Vec2(1, 1));
    homeButton->setPosition(Vec2(visibleSize.width - homeButton->getContentSize().width / 8,
        visibleSize.height - homeButton->getContentSize().width / 8)); // Đặt vị trí cho nút Home

    // Tạo menu và thêm nút Home vào menu
    auto menu = Menu::create(homeButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void LevelSelectScene::homeButton(cocos2d::Ref* pSender)
{
    // Tạo một MainMenu Scene mới hoặc lấy ra Scene đã tồn tại
    auto mainMenuScene = MenuScene::createScene();

    // Thay thế Scene hiện tại bằng MainMenu Scene
    Director::getInstance()->replaceScene(mainMenuScene);
}