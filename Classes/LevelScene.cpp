//#include "LevelScene.h"
//#include "ui/CocosGUI.h"
//#include "MenuScene.h"
//#include "LevelSelectScene.h"
//
//USING_NS_CC;
//
//Scene* LevelScene::createScene() {
//    auto scene = Scene::create();
//    auto layer = LevelScene::create();
//    scene->addChild(layer);
//    return scene;
//}
//
//bool LevelScene::init() {
//    if (!Scene::init()) {
//        return false;
//    }
//
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    createButtonLevel();
//    buttonBackHome();
//
//    return true;
//}
//
//
//void LevelScene::createButtonLevel()
//{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    Vector<MenuItem*> menuItems;
//
//    for (int i = 0; i < 3; ++i)
//    {
//        auto buttonLevel = MenuItemImage::create(
//            "ButtonLevel/DefaultUnlockLevel.png",
//            "ButtonLevel/UnlockLevel.png",
//            [=](Ref* sender){
//                auto selectScene = LevelSelectScene::create();
//                selectScene->createButtonPageLevel(i + 1);
//                addChild(selectScene);
//            });
//
//        buttonLevel->setTag(i);
//        buttonLevel->setScale(0.2 * (visibleSize.height / buttonLevel->getContentSize().height));
//        float buttonWidth = buttonLevel->getContentSize().width * buttonLevel->getScale();
//        float totalWidth = 3 * buttonWidth + 2 * 20;
//        float startX = (visibleSize.width - totalWidth) / 2;
//        buttonLevel->setPosition(Vec2(startX + i * (buttonWidth + 20), visibleSize.height / 2));
//
//        
//        auto label = Label::createWithSystemFont(std::to_string(i + 1), "Arial", 36);
//        label->setPosition(buttonLevel->getContentSize() / 2);
//        buttonLevel->addChild(label);
//
//        menuItems.pushBack(buttonLevel);
//    }
//
//    auto menuLevel = Menu::createWithArray(menuItems);
//    menuLevel->setPosition(Vec2::ZERO);
//    this->addChild(menuLevel);
//}
//
//void LevelScene::buttonBackHome()
//{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    auto homeNormal = Sprite::create("SquareButton/Home Square Button.png");
//    auto homeSelected = Sprite::create("SquareButton/Home col_Square Button.png");
//    auto homeButton = MenuItemSprite::create(homeNormal, homeSelected, 
//        [](Ref* sender) {
//            auto mainMenuScene = MenuScene::createScene();
//            Director::getInstance()->replaceScene(mainMenuScene);
//        });
//    /*homeButton->setContentSize(homeButton->getContentSize() * 0.1);*/
//    homeButton->setScale(0.1 * (visibleSize.height / homeButton->getContentSize().height));
//    homeButton->setAnchorPoint(Vec2(1, 1));
//    homeButton->setPosition(Vec2(visibleSize.width - homeButton->getContentSize().width / 2 * 9,
//        visibleSize.height - homeButton->getContentSize().width / 8)); // Đặt vị trí cho nút Home
//
//    // Tạo menu và thêm nút Home vào menu
//    auto menu = Menu::create(homeButton, nullptr);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu);
//}
