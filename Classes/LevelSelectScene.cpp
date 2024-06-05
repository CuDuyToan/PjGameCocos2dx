#include "LevelSelectScene.h"
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

    if (level > 1) {
        auto reducedLevel = ui::Button::create("res/UI-73.png", "res/UI-81.png");
        reducedLevel->setScale(0.5);
        reducedLevel->setPosition(Vec2(visibleSize.width / 2 - 250 + origin.x, visibleSize.height / 2 - 250 + origin.y));
        reducedLevel->addClickEventListener([=](Ref* sender) {
            createButtonPageLevel(level - 1);
            });
        this->addChild(reducedLevel);
    }

    auto playLevel = ui::Button::create("res/UI-74.png", "res/UI-82.png");
    playLevel->setScale(0.5);
    playLevel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 5 + origin.y));
    playLevel->addClickEventListener([=](Ref* sender) {
        auto nextScene = GameScene::create();
        Director::getInstance()->replaceScene(nextScene);
        });
    this->addChild(playLevel);

    auto nextLevel = ui::Button::create("res/UI-74.png", "res/UI-82.png");
    nextLevel->setScale(0.5);
    nextLevel->setPosition(level == 1 ? Vec2(visibleSize.width / 10 * 8 + origin.x, visibleSize.height / 5 + origin.y) :
        Vec2(visibleSize.width / 10 * 8 + origin.x, visibleSize.height / 5 + origin.y));
    nextLevel->addClickEventListener([=](Ref* sender) {
        createButtonPageLevel(level + 1);
        });
    nextLevel->setVisible(loadLevel() >= level);
    this->addChild(nextLevel);
}

void LevelSelectScene::createButtonChangeLevel(const std::string& nameLevel)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto tableLevel = Sprite::create("res/UI-59.png");
    tableLevel->setScale(0.8);
    tableLevel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(tableLevel);

    auto label = Label::createWithTTF(nameLevel, "fonts/Marker Felt.ttf", 50);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 10 * 7));
    label->setTextColor(Color4B::BLACK);
    this->addChild(label);
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
    homeButton->setScale(0.35);
    homeButton->setAnchorPoint(Vec2(1, 1));
    homeButton->setPosition(Vec2(visibleSize.width - homeButton->getContentSize().width / 10,
        visibleSize.height - homeButton->getContentSize().width / 10)); // Đặt vị trí cho nút Home

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