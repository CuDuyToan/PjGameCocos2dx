//#include "MenuLayer.h"
//#include "MenuScene.h"
////#include "SimpleAudioEngine.h"
//
//USING_NS_CC;
//
//bool MenuLayer::init() {
//    if (!Layer::init()) {
//        return false;
//    }
//
//    createHomeButton();
//
//
//
//    //this->schedule(CC_SCHEDULE_SELECTOR(MenuLayer::updateHomePosition), 0.0f);
//    return true;
//}
//
//void MenuLayer::createHomeButton() {
//
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    auto uiLayer = Layer::create();
//    this->addChild(uiLayer, 100);
//
//    auto homeButton = Sprite::create("LargeButton/Options Button.png");
//    homeButton->setScale(0.3);
//
//    uiLayer->addChild(homeButton, 1);
//}
//
//void MenuLayer::updateHomePosition(float dt) {
//    //this->setPosition( + Vec2(0, 200));
//}
//
//void MenuLayer::goToMenuScene(Ref* pSender) {
//    // Tạo một MainMenu Scene mới hoặc lấy ra Scene đã tồn tại
//    auto mainMenuScene = MenuScene::createScene();
//
//    // Thay thế Scene hiện tại bằng MainMenu Scene
//    Director::getInstance()->replaceScene(mainMenuScene);
//}
