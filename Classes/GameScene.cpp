#include "GameScene.h"
#include "Player.h"
#include "LevelSelectScene.h"

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

    createBackGroundGame();

    // Triển khai Scene chơi game ở đây

    // Đăng ký sự kiện bàn phím với trình quản lý sự kiện
    //auto keyboardListener = EventListenerKeyboard::create();

    //// Gán các hàm xử lý sự kiện cho đối tượng EventListenerKeyboard
    //keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    //keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

    //// Đăng ký đối tượng EventListenerKeyboard với trình quản lý sự kiện của Layer
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    //this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateAction));

    createTileMap();

    //auto treasure = Sprite::create("res/khobau.jpg");
    //treasure->setScale(0.1);
    //treasure->setPosition(Vec2(700, 150));
    //auto bodyTL = PhysicsBody::createBox(treasure->getContentSize());
    //bodyTL->setGravityEnable(false);
    //bodyTL->setDynamic(false);
    //bodyTL->setContactTestBitmask(true);
    //bodyTL->setCollisionBitmask(0);
    //treasure->setPhysicsBody(bodyTL);
    //this->addChild(treasure);


    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    listener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::moveCamera));

    return true;
}


bool GameScene::createTileMap()
{
    //load tile map
    _tilemap = new TMXTiledMap();
    int count = 0;

    if (_tilemap->initWithTMXFile("TileMap/GuraAdventureMap1.tmx")) {
        CCLOG("TRUE");
        this->addChild(_tilemap);

        getGround();
        getWall();
        getCeiling();
        getBarrier();
        getLocaSpawn();
        getHideChest();
        getDoor();

        spawnPlayer(spawnX, spawnY);


        return true;

    }
    else {
        CCLOG("FALSE");
        return false;
    }
}

void GameScene::getGround()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("ground");

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
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::getWall()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("Wall Line");

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
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::getCeiling()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("Ceiling Line");

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
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::getBarrier()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("barrier");

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
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::getHideChest()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("hideChest");

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
            physicsBody->setTag(200);

            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite);
        }
    }
    else
    {
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::getDoor()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("door");

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
            physicsBody->setTag(100);

            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite);
        }
    }
    else
    {
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::getLocaSpawn()
{
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup("spawnPoint");

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
        CCLOG("khong the lay obj tu tiled");
    }
}

void GameScene::spawnPlayer(float spawnX, float spawnY)
{
    player = Player::create();
    player->setPosition(Vec2(spawnX, spawnY));
    this->addChild(player);
}

//k can
void GameScene::addSpriteToTileMap()
{
    //// Tạo một Scene mới
    //auto otherScene = Player::create();

    //// Thực hiện chuyển đổi Scene mà không xóa GameScene hiện tại
    //Director::getInstance()->pushScene(otherScene);



}

void GameScene::createBackGroundGame()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto background = Sprite::create("BackGround/backGround.png");
    auto background = Sprite::create("BackGround/shrimp.png");

    background->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    // Thiết lập kích thước của background bằng kích thước hiển thị của cửa sổ
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    this->addChild(background, -1); // Đặt background ở layer thấp hơn để nó nằm dưới các phần tử khác của Scene
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
//        //CCLOG("%d", force);
//
//    }
//}

bool GameScene::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

    //scoreLabel->setString("Score: " + std::to_string(score));
    //CCLOG("onContactBegin");

    if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
    {
        if (nodeA->getCollisionBitmask() == 100 || nodeB->getCollisionBitmask() == 100)
        {
            if (nodeA->getCollisionBitmask() == 201 || nodeB->getCollisionBitmask() == 201)
            {
                //player->isColliding = true;
                
            }
            if (nodeA->getCollisionBitmask() == 202 || nodeB->getCollisionBitmask() == 202)
            {
                /*auto physicsBody = player->getPhysicsBody();
                physicsBody->setVelocity(Vec2((-player->x / player->x), physicsBody->getVelocity().y));*/
                player->isColliding = true;
                CCLOG("begin");

                //player->stopAllActions();

                //player->isColliding = true;
            }
        }


    }


    return true;
}

bool GameScene::onContactSeparate(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

    //scoreLabel->setString("Score: " + std::to_string(score));
    //CCLOG("onContactBegin");

    if (nodeA->getNode() != nullptr && nodeB->getNode() != nullptr)
    {
        if (nodeA->getCollisionBitmask() == 100 || nodeB->getCollisionBitmask() == 100)
        {
            if (nodeA->getCollisionBitmask() == 202 || nodeB->getCollisionBitmask() == 202)
            {
                /*auto physicsBody = player->getPhysicsBody();
                physicsBody->setVelocity(Vec2((-player->getPositionX() / player->getPositionX()), physicsBody->getVelocity().y));*/
                //player->isColliding = false;

            }
        }
    }

    return true;
}


void GameScene::pauseGame()
{
    Director::getInstance()->pause();

}

void GameScene::backToSelectLevelScene() 
{
    auto selectLevel = LevelSelectScene::createScene();
    Director::getInstance()->replaceScene(selectLevel);
}

void GameScene::moveCamera(float dt)
{
    // Lấy tọa độ của nhân vật
    auto playerPosition = player->getPosition();

    //// Tính toán vị trí mới cho camera
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //auto cameraPosition = Vec2(std::max(playerPosition.x, origin.x + visibleSize.width / 2), std::max(playerPosition.y, origin.y + visibleSize.height / 2));

    //// Đặt lại vị trí cho camera
    //Director::getInstance()->getRunningScene()->setPosition(cameraPosition);

    Vec2 cameraPosition = Vec2(-playerPosition.x + visibleSize.width - visibleSize.width/2,
        -playerPosition.y + visibleSize.height - visibleSize.height/2);
    Director::getInstance()->getRunningScene()->setPosition(cameraPosition);

    
}
