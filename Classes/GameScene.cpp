#include "GameScene.h"
#include "Player.h"
#include "LevelSelectScene.h"
#include "MenuScene.h"
#include "MenuLayer.h"

USING_NS_CC;

Scene* GameScene::createPhysicsWorld() {
    auto GameScene = GameScene::create();
    return GameScene;
}

bool GameScene::init() {
    if (!Scene::initWithPhysics()) {
        return false;
    }

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    this->getPhysicsWorld()->setGravity(Vec2(0, -2000));


    createTileMap(); //error

    createUiMenu();
    //createButtonHand(); //error

    //Thêm event listener cho sự kiện chạm
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    listener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //this->schedule(CC_SCHEDULE_SELECTOR(GameScene::update), 0.0f);
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::playerShowItem), 0.0f);



    return true;
}

int GameScene::loadLevel()
{
    return UserDefault::getInstance()->getIntegerForKey("select_level", 1);
}

void GameScene::winLevel(Ref* sender, int level) {
    if (player->checkAction())
    {
        UserDefault::getInstance()->setIntegerForKey("current_level", level);
        UserDefault::getInstance()->setBoolForKey("next_level_button_state", true);

        UserDefault::getInstance()->flush();
        CCLOG("Level %d has been saved after winning.", level);
        UserDefault::getInstance()->destroyInstance();
        auto levelSelectScene = LevelSelectScene::createScene();
        Director::getInstance()->replaceScene(levelSelectScene);
    }
}


bool GameScene::createTileMap()
{    
    //load tile map
    _tilemap = new TMXTiledMap();
    int count = 0;

    if (_tilemap->initWithTMXFile("TileMap/Map" + std::to_string(loadLevel()) + "Tester.tmx"))
    {
        getScaleSizeInTileMap();
        CalculateNewSizeTile();
        //scaleS = _tilemap->getMapSize().height / (_tilemap->getTileSize().height * (Director::getInstance()->getVisibleSize().height / _tilemap->getContentSize().height));

        _tilemap->setScale(Director::getInstance()->getVisibleSize().width / _tilemap->getContentSize().width,
            Director::getInstance()->getVisibleSize().height / _tilemap->getContentSize().height);
        this->addChild(_tilemap, 1);

        getLocaSpawn();
        spawnPlayer(spawnX, spawnY);

        getGround();
        getWall();
        getCeiling();
        getLadder();
        getBarrier();

        getHideItem();
        getAllQuest();
        getDoor();

        getTrap();
        getSwitch();

        addBackGroundToTilemap();




        return true;

    }
    else {
        return false;
    }
}

void GameScene::CalculateNewSizeTile()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    float tilemapH = (visibleSize.height / _tilemap->getTileSize().height) * _tilemap->getTileSize().height;
    float tileH = tilemapH / _tilemap->getMapSize().height;
    float sizeFullScene = (visibleSize.height / player->getSizePlayer()) / _tilemap->getTileSize().height;
    scaleS = (tileH / player->getSizePlayer());
    CCLOG("%f", tilemapH);
}

void GameScene::addBackGroundToTilemap()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto tileSize = _tilemap->getTileSize();
    //auto mapSize = _tilemap->getMapSize();

    //auto backGroundSize = Size(tileSize.width * mapSize.width, tileSize.height * mapSize.height);

    auto backGround = Sprite::create("BackGround/backGroundMap1.png");
    backGround->setContentSize(visibleSize);
    backGround->setAnchorPoint(Vec2(0, 0));
    this->addChild(backGround, 0);
}

void GameScene::getGround()
{
    std::string obj = "ground";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(201);
            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 2);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getWall()
{
    std::string obj = "wall";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 1.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(202);
            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 2);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getCeiling()
{
    std::string obj = "ceilling";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(203);
            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 2);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getBarrier()
{
    std::string obj = "barrier";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        CCLOG("barrier");
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            std::string name = objectProperties["name"].asString();
            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(250);
            physicsBody->setName(name);
            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 2);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getAllQuest()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string obj = "questList";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            std::string needSprite = objectProperties["needSprite"].asString();
            std::string name = objectProperties["name"].asString();
            std::string request = objectProperties["request"].asString();
            std::string reward = objectProperties["reward"].asString();
            std::string requestPath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + request + ".png";
            std::string spritePath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + needSprite + ".png";


            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            Sprite* sprite = Sprite::create(spritePath);
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(0);
            physicsBody->setName(name.c_str());

            auto chat = Sprite::create("inGame/chat icon/chatIcon.png");
            chat->setScale(0.05 * (Director::getInstance()->getVisibleSize().height / chat->getContentSize().height));
            chat->setVisible(false);
            chat->setName("request");

            auto itemRequest = Sprite::create(requestPath);
            itemRequest->setPosition(Vec2(chat->getContentSize().width * 0.5, chat->getContentSize().height * 0.6));
            itemRequest->setScale( 0.75 * (chat->getContentSize().height / itemRequest->getContentSize().height));
            chat->addChild(itemRequest);

            sprite->addChild(chat, 100);
            chat->setPosition(Vec2(sprite->getContentSize().width/2, sprite->getContentSize().height * 2));

            //tao nut "thao tac voi quest"
            auto ButtonNormal = Sprite::create("inGame/button/buttonInQuest.png");
            //ButtonNormal->setAnchorPoint(Vec2(0.5, 0.5));
            auto ButtonSelected = Sprite::create("inGame/button/buttonInQuest.png");
            //ButtonSelected->setAnchorPoint(Vec2(0.5, 0.5));
            ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                ButtonNormal->getContentSize().height * 1.1));
            auto buttonGetItem = MenuItemSprite::create(
                ButtonNormal,
                ButtonSelected,
                std::bind(&GameScene::requestItemForNodeContact, this, std::placeholders::_1, request, reward, sprite));
            buttonGetItem->setScale(0.075 * (visibleSize.height / buttonGetItem->getContentSize().height));
            buttonGetItem->setVisible(false);
            buttonGetItem->setTag(578);
            buttonGetItem->setName("button");
            /*auto buttonInQuest = Sprite::create("inGame/button/buttonInQuest.png");
            buttonInQuest->setPosition(buttonGetItem->getPosition().x + buttonGetItem->getContentSize().width * 0.4,
                buttonGetItem->getPosition().y + buttonGetItem->getContentSize().width * 0.4);
            buttonInQuest->setScale((buttonGetItem->getContentSize().height / buttonInQuest->getContentSize().height)*1.5);
            buttonGetItem->addChild(buttonInQuest, -1);*/

            //tao menu va them cac nut
            auto menu = Menu::create(buttonGetItem, nullptr);
            menu->setPosition(Vec2(x + width/2, y + height * 2));
            menu->setName("menu " + name);

            _tilemap->addChild(menu, 100);

            sprite->setPhysicsBody(physicsBody);
            //sprite->setName(name.c_str());
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 100);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getDoor()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    std::string obj = "door";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            std::string name = objectProperties["name"].asString();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            // Lấy ra tọa độ trái và phải từ physics body

            //tao nut "thao tac voi quest"
            auto ButtonNormal = Sprite::create("inGame/button/win level.png");
            auto ButtonSelected = Sprite::create("inGame/button/win level.png");
            ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                ButtonNormal->getContentSize().height * 1.1));
            auto buttonGetItem = MenuItemSprite::create(
                ButtonNormal,
                ButtonSelected,
                std::bind(&GameScene::winLevel, this, std::placeholders::_1, loadLevel()));
            buttonGetItem->setScale(0.05 * (visibleSize.height / ButtonNormal->getContentSize().height));
            buttonGetItem->setVisible(false);
            buttonGetItem->setTag(578);
            buttonGetItem->setName("button");

            //tao menu va them cac nut
            auto menu = Menu::create(buttonGetItem, nullptr);
            menu->setPosition(Vec2(x + width / 2, y + height * 2));
            menu->setName("menu " + name);

            _tilemap->addChild(menu, 100);

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(0);
            physicsBody->setName(name);

            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            sprite->setName("sprite " + name);
            _tilemap->addChild(sprite, 2);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getHideItem()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::string obj = "hideItem";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            std::string name = objectProperties["name"].asString();
            int layer = objectProperties["layer"].asInt();
            std::string spritePath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + name + ".png";

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create(spritePath.c_str());
            if (sprite)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(0);
                physicsBody->setName(name.c_str());

                //tao nut "thao tac voi quest"
                auto ButtonNormal = Sprite::create("inGame/button/takeItem.png");
                auto ButtonSelected = Sprite::create("inGame/button/takeItem.png");
                ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                    ButtonNormal->getContentSize().height * 1.1));
                auto buttonGetItem = MenuItemSprite::create(
                    ButtonNormal,
                    ButtonSelected,
                    std::bind(&GameScene::getItemInNodeContact, this, std::placeholders::_1, name));
                buttonGetItem->setScale(0.05 * (visibleSize.height / ButtonNormal->getContentSize().height));
                buttonGetItem->setVisible(false);
                buttonGetItem->setTag(578);
                buttonGetItem->setName("button");

                //tao menu va them cac nut
                auto menu = Menu::create(buttonGetItem, nullptr);
                menu->setPosition(Vec2(x + width / 2, y + height * 2));
                menu->setName("menu " + name);

                _tilemap->addChild(menu, 100);

                sprite->setOpacity(255);
                sprite->setContentSize(Size(width, height));
                sprite->setPhysicsBody(physicsBody);
                //sprite->setName(name.c_str());
                // Đặt vị trí cho sprite và thêm vào scenes
                sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));
                sprite->setName("sprite " + name);
                _tilemap->addChild(sprite, layer);
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getLadder()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string obj = "ladder";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            std::string name = objectProperties["name"].asString();
            std::string location = objectProperties["location"].asString();
            int layer = objectProperties["layer"].asInt();
            std::string spritePath = "inGame/item sprite/" + location + ".png";
            listOfLadder.push_back(new Rect(x, y, width, height));
            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            if (sprite)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(0);
                physicsBody->setName(name + ", location " + location);

                //tao nut "thao tac voi quest"
                std::string pathButton;
                if (location == "bottom")
                {
                    pathButton = "inGame/button/Up.png";
                }
                else if (location == "top")
                {
                    pathButton = "inGame/button/Down.png";
                }
                if (location != "middle")
                {
                    auto ButtonNormal = Sprite::create(pathButton);
                    auto ButtonSelected = Sprite::create(pathButton);
                    ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                        ButtonNormal->getContentSize().height * 1.1));
                    buttonMove = MenuItemSprite::create(
                        ButtonNormal,
                        ButtonSelected,
                        std::bind(&GameScene::useLadder, this, std::placeholders::_1, name, location, width, height));
                    buttonMove->setScale(0.025 * (visibleSize.height / ButtonNormal->getContentSize().height));
                    buttonMove->setVisible(false);
                    buttonMove->setName("button");

                    //tao menu va them cac nut
                    auto menu = Menu::create(buttonMove, nullptr);
                    menu->setPosition(Vec2(x + width / 2, y + height * 2.5));
                    menu->setName("menu " + name + ", location " + location);

                    _tilemap->addChild(menu, 100);

                    //sprite->setOpacity(255);
                    sprite->setContentSize(Size(width, height));
                    sprite->setPhysicsBody(physicsBody);
                    //sprite->setName(name.c_str());
                    // Đặt vị trí cho sprite và thêm vào scenes
                    sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));
                    sprite->setName("sprite " + name + ", location " + location);
                    _tilemap->addChild(sprite, layer);
                }
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getTrap()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string obj = "trap";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            std::string name = objectProperties["name"].asString();
            int layer = objectProperties["layer"].asInt();
            std::string spritePath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + name + ".png";
            bool addSprite = objectProperties["sprite"].asBool();
            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            
            auto sprite = Sprite::create();
            if (addSprite)
            {
                CCLOG("add sprite path %s", spritePath.c_str());
                sprite = Sprite::create(spritePath);
            }
            if (sprite)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(250);
                physicsBody->setName(name);
                
                //tao nut "thao tac voi quest"
                std::string pathButton;

                //sprite->setOpacity(255);
                //sprite->setContentSize(Size(width, height));
                sprite->setPhysicsBody(physicsBody);
                //sprite->setName(name.c_str());
                // Đặt vị trí cho sprite và thêm vào scenes
                sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));
                sprite->setName("sprite " + name);
                _tilemap->addChild(sprite, layer);
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getTrapByName(std::string trapName)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string obj = "trap";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            std::string name = objectProperties["name"].asString();
            int layer = objectProperties["layer"].asInt();
            std::string spritePath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + name + ".png";
            bool addSprite = objectProperties["sprite"].asBool();
            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body

            auto sprite = Sprite::create();
            if (addSprite)
            {
                CCLOG("add sprite path %s", spritePath.c_str());
                sprite = Sprite::create(spritePath);
            }
            if (sprite && name == trapName)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(210);
                physicsBody->setName(name);

                //tao nut "thao tac voi quest"
                std::string pathButton;

                //sprite->setOpacity(255);
                sprite->setContentSize(Size(width, height));
                sprite->setPhysicsBody(physicsBody);
                //sprite->setName(name.c_str());
                // Đặt vị trí cho sprite và thêm vào scenes
                sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));
                sprite->setName("sprite " + name);
                _tilemap->addChild(sprite, layer);
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getSwitch()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string obj = "switch";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            std::string name = objectProperties["name"].asString();
            std::string controlObjects = objectProperties["control objects"].asString();
            int layer = objectProperties["layer"].asInt();
            bool  status = objectProperties["turn on"].asBool();
            std::string spritePath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + name + "/" + std::to_string(status) + ".png";
            listOfLadder.push_back(new Rect(x, y, width, height));
            // Lấy ra tọa độ trái và phải từ physics body

            CCLOG("switch path %s", spritePath.c_str());

        // Tạo sprite và physics body
            auto sprite = Sprite::create(spritePath);
            if (sprite)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(0);
                physicsBody->setName(name);

                //tao nut "thao tac voi quest"
                auto ButtonNormal = Sprite::create("inGame/button/takeItem.png");
                auto ButtonSelected = Sprite::create("inGame/button/takeItem.png");
                ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                    ButtonNormal->getContentSize().height * 1.1));
                buttonMove = MenuItemSprite::create(
                    ButtonNormal,
                    ButtonSelected,
                    std::bind(&GameScene::turnTrap, this, std::placeholders::_1, controlObjects, name, status, sprite));
                buttonMove->setScale(0.025 * (visibleSize.height / ButtonNormal->getContentSize().height));
                buttonMove->setVisible(false);
                buttonMove->setName("button");

                //tao menu va them cac nut
                auto menu = Menu::create(buttonMove, nullptr);
                menu->setPosition(Vec2(x + width / 2, y + height * 2.5));
                menu->setName("menu " + name);

                _tilemap->addChild(menu, 100);

                //sprite->setOpacity(255);
                sprite->setContentSize(Size(width, height));
                sprite->setPhysicsBody(physicsBody);
                //sprite->setName(name.c_str());
                // Đặt vị trí cho sprite và thêm vào scenes
                sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));
                sprite->setName("sprite " + name);
                _tilemap->addChild(sprite, layer);
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getQuestList()
{
    std::string obj = "questList";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            int IDquest = objectProperties["IDQuest"].asInt();

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(0);
            physicsBody->setTag(IDquest);

            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 2);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::visibleNextLevel(float dt)
{
    for (int i = 0; i < 5; i++)
    {
        CCLOG("item in player %s", player->items[i].c_str());
        if (player->items[i] == "next level")
        {
            auto button = _tilemap->getChildByName("menu door")->getChildByName("button");
            if (button)
            {
                button->setVisible(true);
            }
            return;
        }
    }
}

void GameScene::turnTrap(Ref* sender, const std::string& controlObjects, const std::string& name, bool& status, Sprite* sprite)
{
    status = !status; 
    std::string newSpritePath = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + name + "/" + std::to_string(status) + ".png";
    sprite->setTexture(newSpritePath);

    auto trapSprite = _tilemap->getChildByName("sprite " + controlObjects);
    if (!trapSprite)
    {
        CCLOG("get trap");
        getTrapByName(controlObjects);
        return;
    }
    for (int i = 0; i < 2; i++)
    {
        trapSprite = _tilemap->getChildByName("sprite " + controlObjects);
        //CCLOG("turn trap %s");
        if (trapSprite)
        {
            i = 0;
            CCLOG("remove trap");
            trapSprite->removeFromParentAndCleanup(true);
        }
    }
}

void GameScene::useLadder(Ref* sender, const std::string& nameLadder, const std::string& location, float width, float height)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float x = 0;
    float y = 0;
    float XAxisVariable = visibleSize.width / _tilemap->getContentSize().width;
    float YAxisVariable = visibleSize.height / _tilemap->getContentSize().height;
    if (location == "bottom")
    {
        x = _tilemap->getChildByName("sprite " + nameLadder + ", location " + "top")->getPositionX() * XAxisVariable;
        y = (_tilemap->getChildByName("sprite " + nameLadder + ", location " + "top")->getPositionY() + height*1.5) * YAxisVariable;
        CCLOG("x= %f , y= %f", x, y);
    }
    else if (location == "top")
    {
        x = _tilemap->getChildByName("sprite " + nameLadder + ", location " + "bottom")->getPositionX() * XAxisVariable;
        y = (_tilemap->getChildByName("sprite " + nameLadder + ", location " + "bottom")->getPositionY() + height * 0.5) * YAxisVariable;
        CCLOG("x= %f , y= %f", x, y);
    }

    player->setPosition(Vec2(x, y));
}

//void GameScene::setColorLayer()
//{
//    for (int i = 1; i < 3; i++)
//    {
//        std::string nameLayer = "tree" + std::to_string(i);
//        CCLOG("name layer [%s]", nameLayer.c_str());
//        auto layer = _tilemap->getLayer(nameLayer.c_str());
//        if (layer) {
//            ValueMap properties = layer->getProperties();
//            for (const auto& prop : properties) {
//                std::string propName = prop.first;
//                CCLOG("nameProperty %s", propName.c_str());
//                Value propValue = prop.second;
//                if (propValue.getType() == Value::Type::STRING) {
//                    CCLOG("Property %s: %s", propName.c_str(), propValue.asString().c_str());
//                }
//                else if (propValue.getType() == Value::Type::INTEGER) {
//                    CCLOG("Property %s: %d", propName.c_str(), propValue.asInt());
//                }
//                else if (propValue.getType() == Value::Type::FLOAT) {
//                    CCLOG("Property %s: %f", propName.c_str(), propValue.asFloat());
//                } // Thêm các kiểu dữ liệu khác nếu cần
//            }
//        }
//        else {
//            CCLOG("Layer not found");
//        }
//
//    }
//}
//
//Color3B GameScene::colorFromHex(const std::string& hexString) {
//    int r, g, b;
//    sscanf(hexString.c_str(), "#%02x%02x%02x", &r, &g, &b);
//    return Color3B(r, g, b);
//}


void GameScene::getItemInTileMapWithName(std::string name)
{
    CCLOG("%s", name.c_str());
    auto nameItem = _tilemap->getChildByName("menu " + name);

    if (name == "door")
    {
        CCLOG("[menu door s %s]", nameItem->getName().c_str());
        auto button = nameItem->getChildByName("button");
        if (button)
        {
            if (!button->isVisible()) {
                this->schedule(CC_SCHEDULE_SELECTOR(GameScene::visibleNextLevel), 0.0f);
            }
            else
            {
                button->setVisible(false);
                this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::visibleNextLevel));
            }
        }
        return;
    }else if (nameItem) {
        CCLOG("[%s]", nameItem->getName().c_str());
        auto button = nameItem->getChildByName("button");
        if (button)
        {
            if (!button->isVisible()) {
                button->setVisible(true);
            }
            else
            {
                button->setVisible(false);
            }
        }
    }
}

void GameScene::getItemInNodeContact(Ref* sender, const std::string& name)
{
    if (player->checkAction())
    {
        CCLOG("name item %s", name.c_str());
        for (int i = 0; i < 5; ++i)
        {
            if (player->items[i] == "")
            {
                player->items[i] = name.c_str();
                //_tilemap->getChildByName("menu "+ name)->removeFromParent();
                auto menu = _tilemap->getChildByName("menu " + name);
                auto sprite = _tilemap->getChildByName("sprite " + name);
                if (menu)
                {
                    menu->removeFromParentAndCleanup(true);
                }
                if (sprite)
                {
                    sprite->removeFromParentAndCleanup(true);
                }
                break;
            }
            else
            {
                if (i == 4)
                {
                    CCLOG("inventory full");
                }
            }
        }

    }
}

void GameScene::requestItemForNodeContact(Ref* sender, const std::string& request, const std::string& reward, Sprite* sprite)
{
    if (player->checkAction())
    {
        bool found = false;
        CCLOG("requestItem"); 
        for (int i = 0; i < 5; i++)
        {
            player->setWork();
            if (player->items[i] == request.c_str()) {

                found = true;

                this->scheduleOnce([=](float dt) mutable {
                    sprite->removeFromParentAndCleanup(true);

                    //std::string nameChild = request.c_str();
                    player->items[i] = "";
                    //auto childRemove = this->getChildByName(nameChild);
                    CCLOG("delete %s", request.c_str());
                    CCLOG("delete check [%s]", player->items[i].c_str());

                    for (int j = i; j < 5; j++)
                    {
                        if (player->items[j] == "")
                        {
                            player->items[4] = reward;
                            CCLOG("take [%s]", reward.c_str());
                            break;
                        }
                    }
                }, 1.0f, "finish quest");
                break;
            }
            else
            {

            }
        }

        if (!found)
        {
            this->scheduleOnce([=](float dt) mutable {
                sprite->getChildByName("request")->setVisible(true);
                this->scheduleOnce([=](float dt) mutable {
                    sprite->getChildByName("request")->setVisible(false);
                    }, 2.0f, "unvisible request");
                }, 1.0f, "visible request");
        }

    }
}

void GameScene::sortItemInventory()
{
    for (int i = 0; i < 5; ++i)
    {
        if (player->items[i] == "")
        {
            for (int j = i; j < 5; ++j)
            {
                if (player->items[j] != "")
                {
                    player->items[i] = player->items[j];
                    player->items[j] = "";
                    break;
                }
            }
        }
    }
}

void GameScene::appearHandButton()
{

}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto touchPoint = touch->getLocationInView();

    Vec2 location = touch->getLocation();
    return true;

}

bool GameScene::onTouchEnded(Touch* touch, Event* event)
{
    CCLOG("move");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto touchPoint = touch->getLocationInView();

    auto touchPointInNode = this->convertToNodeSpace(touchPoint);

    CCLOG("%f %f", touchPoint.x, touchPoint.y);

    // Kiểm tra nếu touch nằm trong bounding box của player
    //if (Home->getBoundingBox().containsPoint(touchPointInNode))
    //{
    //    CCLOG("player");
    //    // Tạo một MainMenu Scene mới hoặc lấy ra Scene đã tồn tại
    //    auto mainMenuScene = MenuScene::createScene();

    //    // Thay thế Scene hiện tại bằng MainMenu Scene
    //    Director::getInstance()->replaceScene(mainMenuScene);
    //    return false; // Ngăn chặn sự kiện touch tiếp tục được xử lý
    //}

    float deltaX = touchPointInNode.x - player->getPositionX();

    if (player->checkAction() && !stopFlag)
    {
        if (deltaX < 0)
        {
            player->setMoveL();
        }
        else
        {
            player->setMoveR();
        }

        float distanceX = fabs(deltaX);
        float duration = distanceX / ((player->getSizePlayer()*3) * (scaleS * 1.5));

        player->stopAllActions();
        //this->runAction(MoveTo::create(duration, Vec2(touchPointInNode.x, this->getPositionY())));
        /*if (!isColliding)
        {*/
        player->runAction(Sequence::create(
            MoveTo::create(duration, Vec2(touchPointInNode.x, player->getPositionY())),
            //physicsBody->setVelocity(Vec2(x, physicsBody->getVelocity().y)),
            CallFunc::create([this]() {
                player->setMoveIdle();
                }),
            nullptr
        ));
    }
    else
    {
        CCLOG("cant move");
    }

    return true;
}

void GameScene::getLocaSpawn()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // Lấy toạ độ gốc của toạ độ trực quan
    auto origin = Director::getInstance()->getVisibleOrigin();
    std::string obj = "spawnPoint";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);
    float XAxisVariable = visibleSize.width / _tilemap->getContentSize().width;
    float YAxisVariable = visibleSize.height / _tilemap->getContentSize().height;

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            spawnX = objectProperties["x"].asFloat() * XAxisVariable;
            spawnY = objectProperties["y"].asFloat() * YAxisVariable;

            // Lấy ra tọa độ trái và phải từ obj
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::spawnPlayer(float spawnX, float spawnY)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    player = Player::create();
    player->setAnchorPoint(Vec2(0, 0));
    player->setPosition(Vec2(spawnX, spawnY));
    player->setScale(scaleS * scaleSizeInMap);
    CCLOG("%f", scaleSizeInMap);
    this->addChild(player, 9999);
}

void GameScene::getScaleSizeInTileMap()
{
    std::string obj = "scale";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            scaleSizeInMap = objectProperties["scaleSize"].asFloat();
            if (objectProperties["scaleSize"].asFloat())
            {
               
            }

            // Lấy ra tọa độ trái và phải từ obj
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}
//void GameScene::updateAction(float dt) {
//
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//    // Lấy vật lý thể hiện của sprite
//    auto physicsBody = player->getPhysicsBody();
//
//    if (player->checkMovePlayer() == 1)
//    {
//
//    }
//
//    if (!player->isCollidingWall) {
//        physicsBody->setVelocity(Vec2(x, physicsBody->getVelocity().y));
//    }
//    if (player->checkJump() == true)
//    {
//        physicsBody->setVelocity(Vec2(physicsBody->getVelocity().x, physicsBody->getVelocity().y + force));
//        force = force - (force * 3 / 4);
//        if (force <= 10)
//        {
//            force = 0;
//        }
//
//    }
//}

bool GameScene::onContactBegin(PhysicsContact& contact) {

    //auto nodeA = contact.getShapeA()->getBody()->getNode();
    //auto nodeB = contact.getShapeB()->getBody()->getNode();

    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

    if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 250 ||
        nodeA->getCollisionBitmask() == 250 && nodeB->getCollisionBitmask() == 100)
    {
        stopFlag = true;
        player->stopAllActions();
        player->setMoveIdle();
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::playerMoveLR), 0.0f);

        if (nodeA->getName() == "barrier" || nodeB->getName() == "barrier")
        {
        }
    }

    if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 0 ||
        nodeA->getCollisionBitmask() == 0 && nodeB->getCollisionBitmask() == 100)
    {
        if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
        {
            if (nodeA->getCollisionBitmask() == 0)
            {
                CCLOG("button visible");
                getItemInTileMapWithName(nodeA->getName().c_str());
            }
            else
            {
                CCLOG("button visible");
                getItemInTileMapWithName(nodeB->getName().c_str());
            }

            if (nodeA->getTag() == 000 || nodeB->getTag() == 000)
            {



            }

            /*if (nodeA->getCollisionBitmask() == 0)
            {
                CCLOG("door");
            }

            if (nodeB->getCollisionBitmask() == 0)
            {
                CCLOG("door");
            }*/
        }
    }
    
    /*if (nodeA->getCollisionBitmask() == 66 && nodeB->getCollisionBitmask() == 100 ||
        nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 66)
    {
        if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
        {
            if (nodeA->getCollisionBitmask() == 66)
            {
                nodeA->getNode()->getChildByName("menu ladder1")->getChildByName("button")->setVisible(true);
            }
            else
            {
                nodeB->getNode()->getChildByName("menu ladder1")->getChildByName("button")->setVisible(true);
            }
            if (nodeA->getCollisionBitmask() == 0)
            {
                CCLOG("door");
            }
            if (nodeB->getCollisionBitmask() == 0)
            {
                CCLOG("door");
            }
        }
    }*/

    return true;
}

bool GameScene::onContactSeparate(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

    if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 250 ||
        nodeA->getCollisionBitmask() == 250 && nodeB->getCollisionBitmask() == 100)
    {

        stopFlag = false;
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::playerMoveLR));

        if (nodeA->getName() == "barrier" || nodeB->getName() == "barrier")
        {
        }
    }

    if (nodeA->getCollisionBitmask() == 100 && nodeB->getCollisionBitmask() == 0 ||
        nodeA->getCollisionBitmask() == 0 && nodeB->getCollisionBitmask() == 100)
    {
        if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
        {
            if (nodeA->getCollisionBitmask() == 0)
            {
                getItemInTileMapWithName(nodeA->getName().c_str());
            }
            else
            {
                getItemInTileMapWithName(nodeB->getName().c_str());
            }

            if (nodeA->getTag() == 000 || nodeB->getTag() == 000)
            {



            }

            if (nodeA->getCollisionBitmask() == 0)
            {
                CCLOG("door");
            }

            if (nodeB->getCollisionBitmask() == 0)
            {
                CCLOG("door");
            }
        }
    }

    return true;
}

void GameScene::playerMoveLR(float dt)
{
    direction = -direction;
    player->setPosition(Vec2(player->getPositionX() + direction, player->getPositionY()));

    player->getChildByName("danger")->setVisible(true);

    this->scheduleOnce([=](float dt) mutable {
        player->getChildByName("danger")->setVisible(false);
        }, 1.0f, "unvisible danger");
}

void GameScene::playerShowItem(float dt)
{
    if (player)
    {
        
        //CCLOG("%f, %f", player->getPosition().x, player->getPosition().y);
    }
    sortItemInventory();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto spriteSize = _tilemap->getTileSize();

    // Tọa độ X ban đầu (bắt đầu từ bên phải màn hình)
    float startX = visibleSize.width / 20;

    // Tọa độ Y cố định (giữa màn hình)
    float startY = visibleSize.height / 20 * 19;

    for (int i = 0; i < 5; i++)
    {
        std::string name = "id item sprite " + std::to_string(i);
        //CCLOG("name sprite view %s", name.c_str());
        if (player->items[i] != "" && player->items[i] != "next level")
        {
            std::string pathItem = "inGame/level" + std::to_string(loadLevel()) + "/sprite/" + player->items[i] + ".png";
            auto spriteItem = Sprite::create(pathItem);

            if (spriteItem)
            {
                // Đặt vị trí cho Sprite từ phải sang trái
                float posX = startX + i * spriteItem->getContentSize().width;
                spriteItem->setScale(0.1*(visibleSize.height / spriteItem->getContentSize().height));
                spriteItem->setPosition(Vec2(posX, startY));
                spriteItem->setName(name);
                //CCLOG("[%s]", spriteItem->getName().c_str());

                this->addChild(spriteItem, 100);

                //CCLOG("link sprite: %s", player->items[i].c_str());
            }
            //else
            //{
            //    auto child = this->getChildByName(name);
            //    if (child)
            //    {
            //        child->removeFromParent();
            //    }
            //}
        }
        else
        {
            auto removeChild = this->getChildByName(name);
            if (removeChild)
            {
                removeChild->setPosition(Vec2(50, 2000));
                removeChild->removeFromParentAndCleanup(true);
            }
        }
    }
}

void GameScene::pauseGame()
{
    Director::getInstance()->pause();

}

void GameScene::createUiMenu()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //tao nut "exit"
    auto Home = MenuItemImage::create(
        "SquareButton/Home Square Button.png",
        "SquareButton/Home col_Square Button.png",
        CC_CALLBACK_1(GameScene::goToHome, this));
    Home->setAnchorPoint(Vec2(1,1));
    Home->setScale(0.1 * (visibleSize.height / Home->getContentSize().height));
    Home->setPosition(Vec2(visibleSize.width - Home->getContentSize().width/10,
        visibleSize.height - Home->getContentSize().width/10));
    //tao menu va them cac nut
    auto menu = Menu::create(Home, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 100);
}

//void GameScene::createButtonHand()
//{
//    Handbutto = MenuItemImage::create(
//        "LargeButton/Exit Button.png",
//        "LargeButton/Exit  col_Button.png",
//        CC_CALLBACK_1(GameScene::goToHome, this));
//    HandButton->setContentSize(HandButton->getContentSize() * 0.02);
//    HandButton->setPosition(Vec2(player->getPosition().x + 2, player->getPosition().y));
//    HandButton->setTag(0);
//    //this->addChild(HandButton);
//}

void GameScene::goToHome(cocos2d::Ref* pSender)
{
    // Tạo một MainMenu Scene mới hoặc lấy ra Scene đã tồn tại
    auto mainMenuScene = MenuScene::createScene();

    // Thay thế Scene hiện tại bằng MainMenu Scene
    Director::getInstance()->replaceScene(mainMenuScene);
}

void GameScene::goToSelectLevelMenu()
{
    auto selectLevel = LevelSelectScene::createScene();
    Director::getInstance()->replaceScene(selectLevel);
}

void GameScene::visibleChatIcon(bool TF, std::string nameItem)
{
    
}