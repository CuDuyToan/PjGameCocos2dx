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
    CCLOG("Game scene 12121");

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    createBackGroundGame();

    // Triển khai Scene chơi game ở đây

    // Đăng ký sự kiện bàn phím với trình quản lý sự kiện
    auto keyboardListener = EventListenerKeyboard::create();

    // Gán các hàm xử lý sự kiện cho đối tượng EventListenerKeyboard
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

    // Đăng ký đối tượng EventListenerKeyboard với trình quản lý sự kiện của Layer
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateAction));

    createTileMap();

    player = Player::create();
    player->setPosition(Vec2(500, 500));
    addChild(player);

    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    listener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}



void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    // Xử lý sự kiện khi một phím được nhấn


    CCLOG("keyPress");
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_D:
        CCLOG("D");
        x = 200;
        player->setMoveR();
        break;
    case EventKeyboard::KeyCode::KEY_A:
        CCLOG("A");
        x = -200;
        player->setMoveL();
        break;
    case EventKeyboard::KeyCode::KEY_W:
        CCLOG("W");
        if (true)
        {
            y = 180;
            player->setMoveD();
        }
        break;
    case EventKeyboard::KeyCode::KEY_S:
        CCLOG("S");
        if (true)
        {
            y = -180;
            player->setMoveD();
        }
        break;
    case EventKeyboard::KeyCode::KEY_P:
        pauseGame();
        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        CCLOG("Space bar");
        if (player->checkJump() == false)
        {
            force = 450;
            player->Jump();
        }

        break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        backToSelectLevelScene();
        break;
    default:
        break;
    }
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    // Xử lý sự kiện khi một phím được thả ra
    CCLOG("keyReleased");
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_D:
        CCLOG("D");
        if (x == 200)
        {
            x = 0;
            player->setMoveIdle();
        }
        break;
    case EventKeyboard::KeyCode::KEY_A:
        CCLOG("A");
        if (x == -200)
        {
            x = 0;
            player->setMoveIdle();
        }
        break;
    case EventKeyboard::KeyCode::KEY_W:
        CCLOG("W");
        y = 0;
        //player->setMoveIdle();
        break;
    case EventKeyboard::KeyCode::KEY_S:
        CCLOG("S");
        y = 0;
        //player->setMoveIdle();
        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        CCLOG("Space bar");
        //jump

        break;

    default:
        break;
    }
}

bool GameScene::createTileMap()
{

    //load tile map
    _tilemap = new TMXTiledMap();
    int count = 0;

    if (_tilemap->initWithTMXFile("TileMap/Map1.tmx")) {
        CCLOG("TRUE");
        this->addChild(_tilemap);

        getGround();
        getWall();
        getCeiling();

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
    auto objectLayer = _tilemap->getObjectGroup("Ground Line");

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

void GameScene::updateAction(float dt) {

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // Lấy vật lý thể hiện của sprite
    auto physicsBody = player->getPhysicsBody();

    if (player->checkMovePlayer() == 1)
    {

    }

    if (!isCollidingWall) {
        physicsBody->setVelocity(Vec2(x, physicsBody->getVelocity().y));
    }
    if (player->checkJump() == true)
    {
        physicsBody->setVelocity(Vec2(physicsBody->getVelocity().x, physicsBody->getVelocity().y + force));
        force = force - (force * 3 / 4);
        if (force <= 10)
        {
            force = 0;
        }
        //CCLOG("%d", force);

    }
}

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
                //physicPlayer->applyForce(Vec2(0, 2000));
                player->Grounding();
                CCLOG("Grounding");
            }
            if (nodeA->getCollisionBitmask() == 202 || nodeB->getCollisionBitmask() == 202)
            {
                auto physicsBody = player->getPhysicsBody();
                physicsBody->setVelocity(Vec2((-x / x), physicsBody->getVelocity().y));
                isCollidingWall = true;
                CCLOG("begin");
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
                physicsBody->setVelocity(Vec2(-x/x, physicsBody->getVelocity().y));*/
                isCollidingWall = false;
                CCLOG("end");
            }
        }


    }

    return true;
}


void GameScene::pauseGame()
{
    Director::getInstance()->pause();

    /*this->unscheduleAllCallbacks();
    this->stopAllActions();
    player->stopAllActions();*/


}

void GameScene::backToSelectLevelScene() 
{
    auto selectLevel = LevelSelectScene::createScene();
    Director::getInstance()->replaceScene(selectLevel);
}
