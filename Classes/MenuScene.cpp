#include "MenuScene.h"
#include "GameScene.h"
//#include "LevelSelectScene.h"
#include "AudioEngine.h"
#include "Player.h"
#include  "LevelScene.h"
#include "ui/CocosGUI.h"

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
	createImageGura();
	createGuraFallingSprite();
	createAhhSharkSprite();


	//GameScene::createTileMap();



	CCLOG("main menu 321");

	return true;
}



bool  MenuScene::isBackgroundMusicPlaying()
{
	return AudioEngine::getState(musicID) == AudioEngine::AudioState::PLAYING;
}

void MenuScene::saveMusicID()
{
	UserDefault::getInstance()->setIntegerForKey("Back_ground_music_ID", musicID);
}

void MenuScene::playBackGroundMusic()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	bool isMusicPlaying = UserDefault::getInstance()->getBoolForKey("is_music_playing", true);
	musicID = UserDefault::getInstance()->getIntegerForKey("Back_ground_music_ID", -1);

	if (musicID == -1) {
		// Chưa có âm thanh nào đang phát, phát âm thanh mới
		musicID = cocos2d::AudioEngine::play2d("Music/Gura-BGM2.mp3", true);
		UserDefault::getInstance()->setIntegerForKey("Back_ground_music_ID", musicID);
	}
	else {
		if (isMusicPlaying && AudioEngine::getState(musicID) != AudioEngine::AudioState::PLAYING) {
			musicID = cocos2d::AudioEngine::play2d("Music/Gura-BGM2.mp3", true);
			UserDefault::getInstance()->setIntegerForKey("Back_ground_music_ID", musicID);
		}
		else if (!isMusicPlaying && AudioEngine::getState(musicID) == AudioEngine::AudioState::PLAYING) {
			AudioEngine::pause(musicID);
		}
	}

	auto musicBox = ui::CheckBox::create(
		"ButtonLevel/DefaultSoundOff.png",
		"ButtonLevel/SoundOff.png",
		"ButtonLevel/DefaultSoundOn.png",
		"ButtonLevel/DefaultSoundOn.png",
		"ButtonLevel/SoundOn.png"
	);
	musicBox->setSelected(isMusicPlaying);

	musicBox->addEventListener(
		[&](Ref* sender, ui::CheckBox::EventType type)
		{
			switch (type)
			{
			case ui::CheckBox::EventType::SELECTED:

				if (AudioEngine::getState(musicID) != AudioEngine::AudioState::PLAYING) {
					musicID = cocos2d::AudioEngine::play2d("Music/Gura-BGM2.mp3", true);
					UserDefault::getInstance()->setIntegerForKey("Back_ground_music_ID", musicID);
				}
				UserDefault::getInstance()->setBoolForKey("is_music_playing", true);

				break;
			case ui::CheckBox::EventType::UNSELECTED:
				AudioEngine::pause(musicID);
				UserDefault::getInstance()->setBoolForKey("is_music_playing", false);
				break;
			default:
				break;
			}
			/*UserDefault::getInstance()->setBoolForKey("is_music_playing", isMusicPlaying);
			UserDefault::getInstance()->flush();*/
		}
	);
	musicBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 300));
	this->addChild(musicBox);

}


void MenuScene::createBackGround(const std::string& backgroundPath)
{
	const char* cstr = backgroundPath.c_str();

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
		log("Error: Cannot load background from %s", backgroundPath.c_str());
	}
}

void MenuScene::mainBackGround()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("BackGround/HoloBackGround.png");

	background->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	this->addChild(background, -1);
}


void MenuScene::createButton()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	mainBackGround();

	auto exitItem = MenuItemImage::create(
		"LargeButton/Exit Button.png",
		"LargeButton/Exit  col_Button.png",
		CC_CALLBACK_1(MenuScene::menuExitCallback, this));
	exitItem->setTag(0);
	exitItem->setScale(0.2 * (visibleSize.height / exitItem->getContentSize().height));

	// Tạo nút "Play" và thêm vào menu
	auto playItem = MenuItemImage::create(
		"LargeButton/Play Button.png",
		"LargeButton/Play col_Button.png",
		CC_CALLBACK_1(MenuScene::menuPlayCallback, this));
	playItem->setTag(2);
	playItem->setScale(0.2 * (visibleSize.height / playItem->getContentSize().height));
	// tao nut "option"
	auto optionsItem = MenuItemImage::create(
		"LargeButton/Options Button.png",
		"LargeButton/Options  col_Button.png",
		CC_CALLBACK_1(MenuScene::menuOptionsCallback, this));
	optionsItem->setTag(3);
	optionsItem->setScale(0.2 * (visibleSize.height / playItem->getContentSize().height));

	//tao menu va them cac nut
	auto menu = Menu::create(playItem, optionsItem, exitItem, nullptr);
	menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	menu->alignItemsVertically();

	addChild(menu);
}

void MenuScene::menuPlayCallback(Ref* pSender) {
	auto selectLevel = LevelScene::createScene();
	Director::getInstance()->replaceScene(selectLevel);
}

void MenuScene::menuOptionsCallback(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setIntegerForKey("select_level", 1);
	UserDefault::getInstance()->setIntegerForKey("unlocked_level", 1);
	UserDefault::getInstance()->deleteValueForKey("Back_ground_music_ID");
	UserDefault::getInstance()->deleteValueForKey("is_music_playing");
	UserDefault::getInstance()->flush();
	//CCLOG("Level %d has been saved after winning.", 0);
	UserDefault::getInstance()->destroyInstance();
}

void MenuScene::increaseBackgroundMusicVolume(float volumeDelta) {
	//CCLOG("volume + %f", volumeDelta);
	float newVolume = AudioEngine::getVolume(musicID) + volumeDelta;
	AudioEngine::setVolume(musicID, newVolume);
}

void MenuScene::menuExitCallback(cocos2d::Ref* pSender)
{
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
	sprite->setPosition(Vec2(visibleSize.width / 10 * 2,
		visibleSize.height / 10 * 2));
	this->addChild(sprite);
	sprite->runAction(RepeatForever::create(animate));
	sprite->setScale(0.2 * scaleS);

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
	sprite->setPosition(Vec2(visibleSize.width / 10 * 8,
		visibleSize.height / 10 * 8));
	this->addChild(sprite);
	sprite->runAction(RepeatForever::create(animate));
	sprite->setScale(0.2 * scaleS);
}

void MenuScene::createImageGura()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//auto background = Sprite::create("BackGround/backGround.png");
	auto image = Sprite::create("imageGura/GuraMeme (160).png");

	image->setPosition(Vec2(visibleSize.width / 10 * 8,
		visibleSize.height / 10 * 2));
	//image->setScale(2);
	auto rotateAction = RepeatForever::create(RotateBy::create(2.0f, -360)); // 360 độ trong 1 giây
	image->runAction(rotateAction);
	scaleS = (visibleSize.height / image->getContentSize().height);
	image->setScale(0.2 * scaleS);
	this->addChild(image, 0); // Đặt background ở layer thấp hơn để nó nằm dưới các phần tử khác của Scene
}