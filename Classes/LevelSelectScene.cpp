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

    if (loadLevel() >= 2) {
        createButtonPageLevel2();
    }
    else {
        createButtonPageLevel1();
    }

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


void LevelSelectScene::levelSelectedCallback(cocos2d::Ref* pSender) {
    auto button = dynamic_cast<cocos2d::MenuItem*>(pSender);

    if (button) {
        int selectedLevel = button->getTag(); // Lấy tag của nút đã chọn
        // Chuyển đến màn chơi của cấp độ đã chọn
        // Ví dụ: 
        // cocos2d::Director::getInstance()->replaceScene(GameScene::createScene(selectedLevel));

        auto playLevel = GameScene::createPhysicsWorld();
        Director::getInstance()->replaceScene(playLevel);


    }
    else
    {
        CCLOG("can not select level");
    }
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

void LevelSelectScene::createLevelText()
{

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Số lượng hàng và cột
    int numRows = 3;
    int numCols = 5;
    int maxLevel = 1;

    // Tạo một menu
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    // Vị trí ban đầu của hàng nút
    auto startButton = Sprite::create("CasualButton/CGB03-blue_M_btn.png");
    startButton->setContentSize(startButton->getContentSize() * 0.35f);

    auto labelStart = cocos2d::Label::createWithTTF("Level 00", "fonts/arial.ttf", 24);

    Vec2 startPos((origin.x + visibleSize.width / 2) - labelStart->getContentSize().width * 3,
        (origin.y + (visibleSize.height / 3) * 2) /*- startSprite->getContentSize().height * 3*/);

    float startX = startPos.x;
    float startY = startPos.y;

    // Kích thước của mỗi nút
    Size buttonSize(labelStart->getContentSize().width, labelStart->getContentSize().height);

    // Vòng lặp để tạo các sprite và xếp chúng vào grid
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (maxLevel <= a)
            {
                // Tạo văn bản cho nút
                std::string labelText = "Level " + std::to_string((col + 1) + row * 5);
                auto label = cocos2d::Label::createWithTTF(labelText, "fonts/arial.ttf", 24);
                label->setAnchorPoint(Vec2(1, 1));

                // Tạo nút với văn bản
                auto menuItem = cocos2d::MenuItemLabel::create(label,
                    CC_CALLBACK_1(LevelSelectScene::levelSelectedCallback, this));
                menuItem->setContentSize(startButton->getContentSize());
                menuItem->setTag((col + 1) + row * 5);

                // Đặt vị trí cho nút
                menuItem->setPosition(Vec2(startX + col * (startButton->getContentSize().width + startButton->getContentSize().width / 2),
                    startY - (row * (startButton->getContentSize().height + startButton->getContentSize().height / 2))));

                // Thêm nút vào menu
                menu->addChild(menuItem);
                maxLevel++;
            }
        }
    }

}

void LevelSelectScene::createCasualButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Số lượng hàng và cột
    int numRows = 3;
    int numCols = 5;


    // Tọa độ ban đầu cho sprite
    auto startSprite = Sprite::create("CasualButton/CGB03-blue_M_btn.png");
    startSprite->setContentSize(startSprite->getContentSize() * 0.35f);

    Vec2 startPos((origin.x + visibleSize.width / 2) - startSprite->getContentSize().width * 3,
        (origin.y + (visibleSize.height / 3) * 2) /*- startSprite->getContentSize().height * 3*/);

    // Vòng lặp để tạo các sprite và xếp chúng vào grid
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            // Tạo sprite cho mỗi level
            auto sprite = Sprite::create("CasualButton/CGB03-blue_M_btn.png");
            sprite->setContentSize(sprite->getContentSize() * 0.35f);

            // Tính toán vị trí cho sprite
            Vec2 spritePos = startPos + Vec2((sprite->getContentSize().width + (sprite->getContentSize().width) / 2) * col,
                -((sprite->getContentSize().height + (sprite->getContentSize().height) / 2) * row));


            // Đặt vị trí cho sprite
            sprite->setPosition(spritePos);

            // Thêm sprite vào Scene
            this->addChild(sprite);
        }
    }


    //auto casualButton1 = Sprite::create("CasualButton/CGB03-blue_M_btn.png");
    //auto casualButton2 = Sprite::create("CasualButton/CGB03-blue_M_btn.png");

    //casualButton1->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
    //casualButton1->setScale(0.35);
    //this->addChild(casualButton1);

    /*auto menu = cocos2d::Menu::create(level1Button, level2Button, nullptr);
    menu->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
    menu->alignItemsVertically();
    this->addChild(menu);*/
}

void LevelSelectScene::createButtonPageLevel1()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createButtonChangeLevel("Level 1");

    /*auto reducedLevel = ui::Button::create("res/UI-73.png", "res/UI-81.png");
    reducedLevel->setScale(0.3);
    reducedLevel->setPosition(Vec2(visibleSize.width / 2 - 250 + origin.x, visibleSize.height / 2 - 250 + origin.y));
    reducedLevel->addClickEventListener([=](Ref* sender) {

    });
    this->addChild(reducedLevel);*/

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
    nextLevel->setPosition(Vec2(visibleSize.width / 10 * 8 + origin.x, visibleSize.height / 5 + origin.y));
    nextLevel->addClickEventListener([=](Ref* sender) {
        createButtonPageLevel2();
        });
    nextLevel->setVisible(false);
    this->addChild(nextLevel);

    if (loadLevel() >= 1) {
        nextLevel->setVisible(true);
    }
}

void LevelSelectScene::createButtonPageLevel2()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createButtonChangeLevel("Level 2");

    auto reducedLevel = ui::Button::create("res/UI-73.png", "res/UI-81.png");
    reducedLevel->setScale(0.5);
    reducedLevel->setPosition(Vec2(visibleSize.width / 2 - 250 + origin.x, visibleSize.height / 2 - 250 + origin.y));
    reducedLevel->addClickEventListener([=](Ref* sender) {
        createButtonPageLevel1();
        });
    this->addChild(reducedLevel);

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
    nextLevel->setPosition(Vec2(visibleSize.width / 2 + 250 + origin.x, visibleSize.height / 2 - 250 + origin.y));
    nextLevel->addClickEventListener([=](Ref* sender) {
        createButtonPageLevel3();
        });
    nextLevel->setVisible(false);
    this->addChild(nextLevel);

    if (loadLevel() >= 2) {
        nextLevel->setVisible(true);
    }
}

void LevelSelectScene::createButtonPageLevel3()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createButtonChangeLevel("Level 3");

    auto reducedLevel = ui::Button::create("res/UI-73.png", "res/UI-81.png");
    reducedLevel->setScale(0.5);
    reducedLevel->setPosition(Vec2(visibleSize.width / 2 - 250 + origin.x, visibleSize.height / 2 - 250 + origin.y));
    reducedLevel->addClickEventListener([=](Ref* sender) {
        createButtonPageLevel2();
        });
    this->addChild(reducedLevel);

    /*auto nextLevel = ui::Button::create("res/UI-74.png", "res/UI-82.png");
    nextLevel->setScale(0.3);
    nextLevel->setPosition(Vec2(visibleSize.width / 2 + 250 + origin.x, visibleSize.height / 2 - 250 + origin.y));
    nextLevel->addClickEventListener([=](Ref* sender) {

        });
    this->addChild(nextLevel);*/
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
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 10 * 7));
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