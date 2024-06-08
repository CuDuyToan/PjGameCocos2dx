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

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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


void GameScene::winLevel(int level) {

    UserDefault::getInstance()->setIntegerForKey("current_level", level);
    UserDefault::getInstance()->setBoolForKey("next_level_button_state", true);

    UserDefault::getInstance()->flush();
    CCLOG("Level %d has been saved after winning.", level);
    UserDefault::getInstance()->destroyInstance();
    auto levelSelectScene = LevelSelectScene::createScene();
    Director::getInstance()->replaceScene(levelSelectScene);
}


bool GameScene::createTileMap()
{    
    //load tile map
    _tilemap = new TMXTiledMap();
    int count = 0;

    if (_tilemap->initWithTMXFile("TileMap/Map2Tester.tmx"))
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

        //getBarrier();

        getHideItem();
        getAllQuest();
        getDoor();
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
    scaleS = (tileH / player->getSizePlayer()) * scaleSizeInMap;
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
            physicsBody->setCollisionBitmask(204);
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
            std::string name = objectProperties["name"].asString();
            std::string request = objectProperties["request"].asString();
            std::string reward = objectProperties["reward"].asString();//

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(0);
            physicsBody->setName(name.c_str());

            //tao nut "thao tac voi quest"
            auto ButtonNormal = Sprite::create("inGame/button/handIcon.png");
            auto ButtonSelected = Sprite::create("inGame/button/handIcon.png");
            ButtonNormal->setScale(0.5);
            ButtonSelected->setScale(0.6);
            auto buttonGetItem = MenuItemSprite::create(
                ButtonNormal,
                ButtonSelected,
                std::bind(&GameScene::requestItemForNodeContact, this, std::placeholders::_1, request, reward));
            buttonGetItem->setScale(0.1);
            buttonGetItem->setVisible(false);
            buttonGetItem->setTag(578);
            buttonGetItem->setName("button");
            buttonGetItem->setAnchorPoint(Vec2(0.5, 0));

            //tao menu va them cac nut
            auto menu = Menu::create(buttonGetItem, nullptr);
            menu->setPosition(Vec2(x + width / 2, y + height * 2));
            menu->setName("menu " + name);

            _tilemap->addChild(menu, 100);

            sprite->setPhysicsBody(physicsBody);
            //sprite->setName(name.c_str());
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

void GameScene::getDoor()
{
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
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(0);
            physicsBody->setTag(000);

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
            std::string spritePath = "inGame/item sprite/" + name + ".png";

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
                auto ButtonNormal = Sprite::create("inGame/button/handIcon.png");
                auto ButtonSelected = Sprite::create("inGame/button/handIcon.png");
                ButtonSelected->setScale(1.1);
                auto buttonGetItem = MenuItemSprite::create(
                    ButtonNormal,
                    ButtonSelected,
                    std::bind(&GameScene::getItemInNodeContact, this, std::placeholders::_1, name));
                buttonGetItem->setScale(0.1);
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
    auto nameItem = _tilemap->getChildByName("menu " + name);
    if (nameItem) {
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

void GameScene::requestItemForNodeContact(Ref* sender, const std::string& request, const std::string& reward)
{
    if (player->checkAction())
    {

        bool found = false;
        CCLOG("requestItem");
        for (int i = 0; i < 5; i++)
        {
            if (player->items[i] == reward && reward == "next level")
            {
                winLevel(3);
                return;
            }


        }
        for (int i = 0; i < 5; i++)
        {
            player->setWork();
            if (player->items[i] == request.c_str()) {
                //std::string nameChild = request.c_str();
                player->items[i] = "";
                //auto childRemove = this->getChildByName(nameChild);
                found = true;
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
                break;
            }
            else
            {

            }
        }

        if (!found)
        {

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

    if (player->checkAction())
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
        float duration = distanceX / ((player->getSizePlayer()*3) * (scaleS * 1.25));

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
    player->setScale(scaleS);
    this->addChild(player, 10);
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

bool GameScene::onContactSeparate(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

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

void GameScene::playerShowItem(float dt)
{
    sortItemInventory();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto spriteSize = _tilemap->getTileSize();

    // Tọa độ X ban đầu (bắt đầu từ bên phải màn hình)
    float startX = visibleSize.width - spriteSize.width / 2;

    // Tọa độ Y cố định (giữa màn hình)
    float startY = visibleSize.height / 20;

    for (int i = 0; i < 5; i++)
    {
        std::string name = "id item sprite " + std::to_string(i);
        //CCLOG("name sprite view %s", name.c_str());
        if (player->items[i] != "" && player->items[i] != "next level")
        {
            std::string pathItem = "inGame/item sprite/" + player->items[i] + ".png";
            auto spriteItem = Sprite::create(pathItem);

            if (spriteItem)
            {
                // Đặt vị trí cho Sprite từ phải sang trái
                float posX = startX - i * spriteItem->getContentSize().width;
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

void GameScene::createButtonHand()
{
    HandButton = MenuItemImage::create(
        "LargeButton/Exit Button.png",
        "LargeButton/Exit  col_Button.png",
        CC_CALLBACK_1(GameScene::goToHome, this));
    HandButton->setContentSize(HandButton->getContentSize() * 0.02);
    HandButton->setPosition(Vec2(player->getPosition().x + 2, player->getPosition().y));
    HandButton->setTag(0);
    //this->addChild(HandButton);
}

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