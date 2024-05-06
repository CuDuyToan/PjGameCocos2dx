#include "MenuScene.h"
#include "GameScene.h"
#include "LevelSelectScene.h"
#include "AudioEngine.h"
#include "Player.h"

USING_NS_CC;

Scene* MenuScene::createScene() {
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    createButton();
    playBackGroundMusic();

    //sprite in mainmenu
    createGuraFallingSprite();
    createAhhSharkSprite();
    createImageGura();

    //GameScene::createTileMap();



    CCLOG("main menu 32323232 ");

    return true;
}



bool  MenuScene::isBackgroundMusicPlaying()
{
    return AudioEngine::getState(musicID) == AudioEngine::AudioState::PLAYING;
}

void MenuScene::playBackGroundMusic()
{
    if (isBackgroundMusicPlaying() == false)
    {

        // Phát âm thanh nền
        musicID = cocos2d::AudioEngine::play2d("Music/Gura-BGM2.mp3", true);
    }
    else
    {
        CCLOG("no background sound");
    }


}

//createBG error
void MenuScene::createBackGround(const std::string& backgroundPath)
{
    const char* cstr = backgroundPath.c_str();

    // In ra chuỗi bằng hàm CCLOG
    CCLOG("Message: %s", cstr);

    // Tạo Sprite từ đường dẫn
    auto background = Sprite::create(cstr);

    // Kiểm tra xem background có tồn tại không
    if (background != nullptr) {
        // Lấy kích thước màn hình hiển thị
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Lấy toạ độ gốc của toạ độ trực quan
        auto origin = Director::getInstance()->getVisibleOrigin();

        // Đặt vị trí của background là ở giữa màn hình
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

        // Thêm background vào Scene
        Director::getInstance()->getRunningScene()->addChild(background, -1);
    }
    else {
        // Nếu không thể tạo được background từ đường dẫn, thông báo lỗi
        log("Error: Cannot load background from %s", backgroundPath.c_str());
    }
}

void MenuScene::mainBackGround()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto background = Sprite::create("BackGround/backGround.png");
    auto background = Sprite::create("BackGround/HoloBackGround.png");

    background->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    // Thiết lập kích thước của background bằng kích thước hiển thị của cửa sổ
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background, -1); // Đặt background ở layer thấp hơn để nó nằm dưới các phần tử khác của Scene
}

void MenuScene::createButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    mainBackGround();

    //tao nut "exit"
    auto exitItem = MenuItemImage::create(
        "LargeButton/Exit Button.png",
        "LargeButton/Exit  col_Button.png",
        CC_CALLBACK_1(MenuScene::menuExitCallback, this));
    exitItem->setScale(0.3);
    exitItem->setTag(0);

    // Tạo nút "Play" và thêm vào menu
    auto playItem = MenuItemImage::create(
        "LargeButton/Play Button.png",
        "LargeButton/Play col_Button.png",
        CC_CALLBACK_1(MenuScene::menuPlayCallback, this));
    playItem->setScale(0.3);
    playItem->setTag(2);
    // tao nut "option"
    auto optionsItem = MenuItemImage::create(
        "LargeButton/Options Button.png",
        "LargeButton/Options  col_Button.png",
        CC_CALLBACK_1(MenuScene::menuOptionsCallback, this));
    optionsItem->setScale(0.3);
    optionsItem->setTag(3);

    //tao menu va them cac nut
    auto menu = Menu::create(playItem, optionsItem, exitItem, nullptr);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    menu->alignItemsVertically();

    addChild(menu);
}

void MenuScene::menuPlayCallback(Ref* pSender) {
    auto selectLevel = LevelSelectScene::createScene();
    Director::getInstance()->replaceScene(selectLevel);
}

void MenuScene::menuOptionsCallback(cocos2d::Ref* pSender)
{
    CCLOG("Option");
    increaseBackgroundMusicVolume(0.5);
}

void MenuScene::increaseBackgroundMusicVolume(float volumeDelta) {
    CCLOG("volume + %f", volumeDelta);
    float newVolume = AudioEngine::getVolume(musicID) + volumeDelta;
    AudioEngine::setVolume(musicID, newVolume);
}

void MenuScene::menuExitCallback(cocos2d::Ref* pSender)
{
    CCLOG("Exit");
    Director::getInstance()->end();
}

void MenuScene::createGuraFallingSprite()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/GuraFalling.plist",
        "GuraPlayer/GuraFalling.png");

    Vector<SpriteFrame*> frames;
    for (int i = 2; i <= 3; ++i) {
        std::string frameName = "GuraMemeFalling (" + std::to_string(i) + ").png";
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
        }
    }

    Animation* animation = Animation::createWithSpriteFrames(frames, 0.2f);

    Animate* animate = Animate::create(animation);
    Sprite* sprite = Sprite::create();
    sprite->setPosition(Vec2(origin.x + visibleSize.width - visibleSize.width / 10 * 9,
        origin.y + visibleSize.height - visibleSize.height / 10 * 9));
    this->addChild(sprite);
    sprite->runAction(RepeatForever::create(animate));

}

void MenuScene::createAhhSharkSprite()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuraPlayer/AhhShark.plist",
        "GuraPlayer/AhhShark.png");

    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 12; ++i) {
        std::string frameName = "GuraMemeA (" + std::to_string(i) + ").png";
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
        }
    }

    Animation* animation = Animation::createWithSpriteFrames(frames, 0.2f);

    Animate* animate = Animate::create(animation);
    Sprite* sprite = Sprite::create();
    sprite->setPosition(Vec2(origin.x + visibleSize.width - visibleSize.width / 4,
        origin.y + visibleSize.height - visibleSize.height / 10));
    this->addChild(sprite);
    sprite->runAction(RepeatForever::create(animate));
}

void MenuScene::createImageGura()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto background = Sprite::create("BackGround/backGround.png");
    auto image = Sprite::create("imageGura/GuraMeme (160).png");

    image->setPosition(origin.x + visibleSize.width - visibleSize.width / 10,
        origin.y + visibleSize.height - (visibleSize.height - image->getContentSize().height / 2));
    //image->setScale(2);
    auto rotateAction = RepeatForever::create(RotateBy::create(2.0f, -360)); // 360 độ trong 1 giây
    image->runAction(rotateAction);
    this->addChild(image); // Đặt background ở layer thấp hơn để nó nằm dưới các phần tử khác của Scene
}