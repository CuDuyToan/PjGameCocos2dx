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

    createTileMap();

    createUiMenu();
    createButtonHand();

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

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::update), 0.0f);

    return true;
}


bool GameScene::createTileMap()
{
    //load tile map
    _tilemap = new TMXTiledMap();
    int count = 0;

    if (_tilemap->initWithTMXFile("TileMap/maptest.tmx")) 
    {
        this->addChild(_tilemap);

        getGround();
        getWall();
        getCeiling();
        //getBarrier();
        getHideItem();
        getLocaSpawn();
        getAllQuest();
        getDoor();

        spawnPlayer(spawnX, spawnY);


        return true;

    }
    else {
        return false;
    }
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
            _tilemap->addChild(sprite);
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
            _tilemap->addChild(sprite);
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
            _tilemap->addChild(sprite);
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
            _tilemap->addChild(sprite);
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
            auto homeNormal = Sprite::create("inGame/handIcon.png");
            auto homeSelected = Sprite::create("inGame/handIcon.png");
            homeSelected->setScale(1.1);
            auto Home = MenuItemSprite::create(
                homeNormal,
                homeSelected,
                std::bind(&GameScene::getItemInNodeContact, this, std::placeholders::_1, name));
            Home->setScale(0.05);
            Home->setVisible(false);
            //Home->setTag(578);
            Home->setName(name.c_str());

            //tao menu va them cac nut
            auto menu = Menu::create(Home, nullptr);
            menu->setPosition(Vec2(x + width / 2, y + height * 2));
            menu->setTag(578);
            menu->setName(name.c_str());

            _tilemap->addChild(menu, 100);

            sprite->setPhysicsBody(physicsBody);
            //sprite->setName(name.c_str());
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, -1);
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
            _tilemap->addChild(sprite);
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
            auto Home = MenuItemImage::create(
                "SquareButton/Home Square Button.png",
                "SquareButton/Home col_Square Button.png",
                std::bind(&GameScene::getItemInNodeContact, this, std::placeholders::_1, name));
            Home->setScale(0.1);
            Home->setVisible(false);
            Home->setTag(578);
            Home->setName(name.c_str());

            //tao menu va them cac nut
            auto menu = Menu::create(Home, nullptr);
            menu->setPosition(Vec2(x+width/2,y+height*2));
            menu->setTag(578);
            menu->setName(name.c_str());

            _tilemap->addChild(menu,100);

            sprite->setPhysicsBody(physicsBody);
            //sprite->setName(name.c_str());
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite,-1);
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
            _tilemap->addChild(sprite);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void GameScene::getChildOfTileMapWithName(std::string name)
{
    for (const auto& child : _tilemap->getChildren())
    {
        if (child->getName() == name.c_str())
        {
            for (const auto& childOfChild : child->getChildren())
            {
                if (childOfChild->getName() == name.c_str())
                {
                    if (!childOfChild->isVisible())
                    {
                        CCLOG("da vao5");
                        childOfChild->setVisible(true);
                    }
                    else
                    {
                        childOfChild->setVisible(false);
                    }
                }
            }
            
            
        }
        //std::string name = child->getName();
        //Vec2 position = child->getPosition();
        //CCLOG("Child Name: %s, Position: (%f, %f)", name.c_str(), position.x, position.y);
    }
}

void GameScene::getItemInNodeContact(Ref* sender, const std::string& message)
{
    CCLOG("%s", message.c_str());
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

    if (true)
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
        float duration = distanceX / 500.0f;

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

    return true;
}

void GameScene::getLocaSpawn()
{
    std::string obj = "spawnPoint";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            spawnX = objectProperties["x"].asFloat();
            spawnY = objectProperties["y"].asFloat();

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
    player = Player::create();
    player->setAnchorPoint(Vec2(0 , 0));
    player->setPosition(Vec2(spawnX, spawnY));
    this->addChild(player);
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
                getChildOfTileMapWithName(nodeA->getName().c_str());
            }
            else
            {
                getChildOfTileMapWithName(nodeB->getName().c_str());
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
                getChildOfTileMapWithName(nodeA->getName().c_str());
            }
            else
            {
                getChildOfTileMapWithName(nodeB->getName().c_str());
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
    Home->setScale(0.3);
    Home->setPosition(Vec2(visibleSize.width/2 - Home->getContentSize().width / 10 * 2,
        visibleSize.height/2 - Home->getContentSize().width / 10 * 2));
    //tao menu va them cac nut
    auto menu = Menu::create(Home,nullptr);

    addChild(menu);
}

void GameScene::createButtonHand()
{
    HandButton = MenuItemImage::create(
        "LargeButton/Exit Button.png",
        "LargeButton/Exit  col_Button.png",
        CC_CALLBACK_1(GameScene::goToHome, this));
    HandButton->setContentSize(HandButton->getContentSize()*0.02);
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