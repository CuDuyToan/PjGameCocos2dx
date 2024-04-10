//#include "CreateButton.h"
//#include "MenuScene.h"
//#include "LevelSelectScene.h"
//#include "GameScene.h"
//
//USING_NS_CC;
//
//CreateButton* CreateButton::createButton()
//{
//	return CreateButton::create();
//
//}
//
//bool CreateButton::init()
//{
//	if (!Node::init())
//	{
//		return false;
//	}
//
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    Size designSize = Director::getInstance()->getWinSize();
//
//    ratioX = visibleSize.width / 1024;
//    ratioY = visibleSize.height / 768;
//
//
//	return true;
//}
//
//void CreateButton::createGroupButtonMainMenu()
//{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    //tao nut "exit"
//    auto exitItem = MenuItemImage::create(
//        "LargeButton/Exit Button.png",
//        "LargeButton/Exit  col_Button.png",
//        CC_CALLBACK_1(CreateButton::exitGameCallBack, this));
//    exitItem->setScaleX(0.3 * ratioX);
//    exitItem->setScaleY(0.3 * ratioY);
//    exitItem->setTag(0);
//
//    // Tạo nút "Play" và thêm vào menu
//    auto playItem = MenuItemImage::create(
//        "LargeButton/Play Button.png",
//        "LargeButton/Play col_Button.png",
//        CC_CALLBACK_1(CreateButton::selectLevelGameCallBack, this));
//    playItem->setScaleX(0.3 * ratioX);
//    playItem->setScaleY(0.3 * ratioY);
//    playItem->setTag(1);
//    // tao nut "option"
//    auto optionsItem = MenuItemImage::create(
//        "LargeButton/Options Button.png",
//        "LargeButton/Options  col_Button.png",
//        CC_CALLBACK_1(CreateButton::optionMenuCallBack, this));
//    optionsItem->setScaleX(0.3 * ratioX);
//    optionsItem->setScaleY(0.3 * ratioY);
//    optionsItem->setTag(2);
//
//    //tao menu va them cac nut
//    auto menu = Menu::create(playItem, optionsItem, exitItem, nullptr);
//    menu->setPosition(Vec2(origin.x + visibleSize.width - playItem->getContentSize().width / 2,
//        origin.y + visibleSize.height / 2));
//    menu->alignItemsVertically();
//
//    addChild(menu);
//}
//
//void CreateButton::createButtonReturnMainMenu()
//{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    // Tạo MenuItemImage cho nút Home
//    auto homeNormal = Sprite::create("SquareButton/Home Square Button.png");
//    auto homeSelected = Sprite::create("SquareButton/Home col_Square Button.png");
//    auto homeButton = MenuItemSprite::create(homeNormal, homeSelected,
//        CC_CALLBACK_1(CreateButton::returnMainMenuCallBack, this));
//    /*homeButton->setContentSize(homeButton->getContentSize() * 0.1);*/
//    homeButton->setScaleX(0.25 * ratioX);
//    homeButton->setScaleY(0.25 * ratioY);
//    homeButton->setAnchorPoint(Vec2(1, 1));
//    homeButton->setPosition(Vec2(origin.x + visibleSize.width - homeButton->getContentSize().width / 10,
//        origin.y + visibleSize.height - homeButton->getContentSize().width / 10)); // Đặt vị trí cho nút Home
//
//    // Tạo menu và thêm nút Home vào menu
//    auto menu = Menu::create(homeButton, nullptr);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu);
//}
//
//void CreateButton::createButtonPauseMenu()
//{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    // Tạo MenuItemImage cho nút Home
//    /*auto homeNormal = Sprite::create("SquareButton/Home Square Button.png");
//    auto homeSelected = Sprite::create("SquareButton/Home col_Square Button.png");
//    auto homeButton = MenuItemSprite::create(homeNormal, homeSelected,
//        CC_CALLBACK_1(CreateButton::returnMainMenuCallBack, this));*/
//
//    auto pauseButton = MenuItemImage::create(
//        "LargeButton/Pause Square Button.png",
//        "LargeButton/Pause col_Square Button.png",
//        CC_CALLBACK_1(CreateButton::pauseMenuCallBack, this));
//    /*homeButton->setContentSize(homeButton->getContentSize() * 0.1);*/
//    pauseButton->setScaleX(0.25 * ratioX);
//    pauseButton->setScaleY(0.25 * ratioY);
//    pauseButton->setAnchorPoint(Vec2(1, 1));
//    pauseButton->setPosition(Vec2(origin.x + visibleSize.width - pauseButton->getContentSize().width / 10,
//        origin.y + visibleSize.height - pauseButton->getContentSize().width / 10)); // Đặt vị trí cho nút Home
//
//    // Tạo menu và thêm nút Home vào menu
//    auto menu = Menu::create(pauseButton, nullptr);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu);
//}
//
//void CreateButton::createGroupButtonPauseMenu()
//{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    //tao nut "exit"
//    auto resume = MenuItemImage::create(
//        "LargeButton/Play Square Button.png",
//        "LargeButton/Play col_Square Button.png",
//        CC_CALLBACK_1(CreateButton::resumeGameCallBack, this));
//    resume->setScaleX(0.25 * ratioX);
//    resume->setScaleY(0.25 * ratioY);
//    resume->setTag(1);
//
//    // Tạo nút "Play" và thêm vào menu
//    auto home = MenuItemImage::create(
//        "LargeButton/Home Square Button.png",
//        "LargeButton/Home col_Square Button.png",
//        CC_CALLBACK_1(CreateButton::returnMainMenuCallBack, this));
//    home->setScaleX(0.25 * ratioX);
//    home->setScaleY(0.25 * ratioY);
//    home->setTag(2);
//
//    // tao nut "option"
//    auto replay = MenuItemImage::create(
//        "LargeButton/Return Square Button.png",
//        "LargeButton/Return col_Square Button.png",
//        CC_CALLBACK_1(CreateButton::replayLevelCallBack, this));
//    replay->setScaleX(0.25 * ratioX);
//    replay->setScaleY(0.25 * ratioY);
//    replay->setTag(3);
//
//    //tao menu va them cac nut
//    auto menu = Menu::create(resume, replay, home, nullptr);
//    //menu->setAnchorPoint(Vec2(0.5, 0.5));
//    menu->setPosition(Vec2(origin.x + visibleSize.width - resume->getContentSize().width / 2,
//        origin.y + visibleSize.height / 2));
//    menu->alignItemsHorizontally();
//
//    //can sua them
//
//    addChild(menu);
//}
//
/////////////////
//
//void CreateButton::exitGameCallBack(cocos2d::Ref* pSender)
//{
//    CCLOG("Exit");
//    Director::getInstance()->end();
//}
//
//void CreateButton::selectLevelGameCallBack(cocos2d::Ref* pSender)
//{
//    auto selectLevel = LevelSelectScene::createScene();
//    Director::getInstance()->replaceScene(selectLevel);
//}
//
//void CreateButton::optionMenuCallBack(cocos2d::Ref* pSender)
//{
//    CCLOG("option");
//}
//
//void CreateButton::returnMainMenuCallBack(cocos2d::Ref* pSender)
//{
//    auto mainMenu = MenuScene::createScene();
//    Director::getInstance()->replaceScene(mainMenu);
//}
//
//void CreateButton::pauseMenuCallBack(cocos2d::Ref* pSender)
//{
//    GameScene* gameScene = GameScene::create();
//    gameScene->unscheduleAllCallbacks();
//    gameScene->stopAllActions();
//    gameScene->setPosition(Vec2::ZERO);
//    addChild(gameScene);
//}
//
//void CreateButton::resumeGameCallBack(cocos2d::Ref* pSender)
//{
//
//}