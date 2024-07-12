#include "PlayGameScene.h"

USING_NS_CC;

Scene* PlayGameScene::createPhysicsWorld() {
    auto GameScene = PlayGameScene::create();
    return GameScene;
}

bool PlayGameScene::init() {
    if (!Scene::initWithPhysics()) {
        return false;
    }

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setGravity(Vec2(0, -2000));


    addTileMapToTileMaps();



    //Thêm event listener cho sự kiện chạm
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayGameScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(PlayGameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //contact listener
    auto ContactListener = EventListenerPhysicsContact::create();
    ContactListener->onContactBegin = CC_CALLBACK_1(PlayGameScene::onContactBegin, this);
    ContactListener->onContactSeparate = CC_CALLBACK_1(PlayGameScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ContactListener, this);

    //key listener
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(PlayGameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    return true;
}

void PlayGameScene::testCreateTileMap()
{
    // Tạo và thêm các TMXTiledMap vào vector
    for (int i = 1; i <= 3; ++i) {
        std::string filename = cocos2d::StringUtils::format("TileMap/level%d.tmx", i);
        auto tilemap = cocos2d::TMXTiledMap::create(filename);
        tileMaps.push_back(tilemap);
        this->addChild(tilemap);
        tilemap->setPosition(cocos2d::Vec2(tilemap->getContentSize().width * (i - 1), 0));
    }
}

bool PlayGameScene::addTileMapToTileMaps()
{
    for (int i = 1; i <= 3; ++i) {
        std::string filePath = cocos2d::StringUtils::format("TileMap/level2Scene%d.tmx", i);
        CCLOG("filepath %s ", filePath.c_str());
        cocos2d::TMXTiledMap* tilemap = cocos2d::TMXTiledMap::create(filePath);
        if (tilemap)
        {
            tileMaps.push_back(tilemap);

            createTileMap(tilemap, i);
            //tilemap->setPosition(cocos2d::Vec2(tilemap->getContentSize().width * (i - 1), 0));
        }
        else
        {
            delete tilemap;
        }
    }
    return true;
}

bool PlayGameScene::createTileMap(cocos2d::TMXTiledMap* _tilemap, int _scene)
{
    //load tile map
    //_tilemap = new TMXTiledMap();
    float scaleMap = 1;
    //_tilemap->setVisible(false);


    if (_tilemap)
    {
        // Lấy kích thước của các tile
        tileSize = _tilemap->getTileSize();

        speed = tileSize.width * 6;
        speedList.push_back(speed);

        this->addChild(_tilemap);

        float ratioWidth = Director::getInstance()->getVisibleSize().width / _tilemap->getContentSize().width;
        float ratioHeight = Director::getInstance()->getVisibleSize().height / _tilemap->getContentSize().height;

        _tilemap->setAnchorPoint(Vec2(0.5, 0.5));

        _tilemap->setPosition((Director::getInstance()->getVisibleSize().width / 2) + (Director::getInstance()->getVisibleSize().width) * (_scene - 1),
            Director::getInstance()->getVisibleSize().height / 2);

        /*if (_scene == 1)
        {
            _tilemap->setPosition(Director::getInstance()->getVisibleSize().width / 4,
                Director::getInstance()->getVisibleSize().height / 4);
        }else if (_scene == 2)
        {
            _tilemap->setPosition(Director::getInstance()->getVisibleSize().width / 4 * 3,
                Director::getInstance()->getVisibleSize().height / 4);
        }
        else if (_scene == 3)
        {
            _tilemap->setPosition(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 4 * 3);
        }*/

        /*_tilemap->setPosition(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2);*/

        getGround(_tilemap);
        getHiddenPassageAndAddSprite(_tilemap);
        getItem(_tilemap);
        getBarrier(_tilemap);
        getNextInTilemap(_tilemap);

        //add in level2scene1
        getNotification(_tilemap);
        getSuggest(_tilemap);
        getGateBarrier(_tilemap);
        getPuzzle(_tilemap);

        //level2scene2
        getGate(_tilemap);


        getSpawnPoint(_tilemap);
        //spawn
        spawnPlayer(_tilemap);

        //tutorial
        getTutorial(_tilemap);

        
        // set kích thước map bằng với kích thước màn hình 

        if (ratioWidth <= ratioHeight)
        {
            _tilemap->setScale(ratioWidth);
        }
        else if (ratioWidth > ratioHeight)
        {
            _tilemap->setScale(ratioHeight);
        }

        return true;

    }
    else {
        return false;
    }

}

void PlayGameScene::changeScene(int _scene)
{
    int num = 0;
    scene = _scene;

    for (TMXTiledMap* tilemap : tileMaps)
    {
        if (num+1 == _scene)
        {
            tilemap->setPosition((Director::getInstance()->getVisibleSize().width / 2),
                Director::getInstance()->getVisibleSize().height / 2);
        }
        else if (num + 1 != _scene)
        {
            tilemap->setPosition((Director::getInstance()->getVisibleSize().width / 2) + Director::getInstance()->getVisibleSize().width,
                Director::getInstance()->getVisibleSize().height / 2);
        }

        num++;
    }
}

//spawn player
void PlayGameScene::getSpawnPoint(cocos2d::TMXTiledMap* _tilemap)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // Lấy toạ độ gốc của toạ độ trực quan
    auto origin = Director::getInstance()->getVisibleOrigin();
    std::string obj = "spawn point";
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

            //CCLOG(objectProperties["name"].asString().c_str());
            if (objectProperties["name"].asString() == "spawn point")
            {
                // Lấy ra tọa độ trái và phải từ obj

                spawnX = objectProperties["x"].asFloat();
                spawnY = objectProperties["y"].asFloat();

            }
            else if (objectProperties["name"].asString() == "move to")
            {
                moveToX = objectProperties["x"].asFloat();
                moveToY = objectProperties["y"].asFloat();
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void PlayGameScene::spawnPlayer(cocos2d::TMXTiledMap* _tilemap)
{
    player = Player::create();
    player->setAnchorPoint(Vec2(0, 0));
    player->setPosition(Vec2(spawnX, spawnY));
    player->setScale((tileSize.height / player->getSizePlayer().height)*2.5);
    //CCLOG("%f", scaleSizeInMap);
    player->setName("player");
    playerList.push_back(player);
    _tilemap->addChild(player, 9999);

    if (moveToX != 0 || moveToY != 0)
    {
        playerMoveTo(moveToX);
    }
}

void PlayGameScene::playerMoveTo(float x)
{
    CCLOG("player move");

    float deltaX = x - playerList[scene -1]->getPositionX();

    if (playerList[scene - 1]->checkAction() /* && !stopFlag */)
    {
        if (deltaX < 0)
        {
            playerList[scene - 1]->setMoveL();
        }
        else
        {
            playerList[scene - 1]->setMoveR();
        }

        float distanceX = fabs(deltaX);
        float duration = distanceX / (speedList[scene-1]);

        playerList[scene - 1]->stopAllActions();
        //this->runAction(MoveTo::create(duration, Vec2(touchPointInNode.x, this->getPositionY())));
        /*if (!isColliding)
        {*/
        playerList[scene - 1]->runAction(Sequence::create(
            MoveTo::create(duration, Vec2(x, playerList[scene - 1]->getPositionY())),
            //physicsBody->setVelocity(Vec2(x, physicsBody->getVelocity().y)),
            CallFunc::create([this]() {
                playerList[scene - 1]->setMoveIdle();
                }),
            nullptr
        ));
    }
    else
    {
        CCLOG("cant move");
    }
}


//get obj layer in tile map and add sprite


//void PlayGameScene::getListTutorial()
//{
//    std::string obj = "list tutorial";
//    // Lấy layer chứa các đối tượng từ Tiled Map
//    auto objectLayer = _tilemap->getObjectGroup(obj);
//
//    if (objectLayer) {
//        // Lấy danh sách các đối tượng từ object layer
//        ValueVector objects = objectLayer->getObjects();
//
//        // Duyệt qua từng đối tượng và gán physics body
//        for (const auto& object : objects) {
//            ValueMap objectProperties = object.asValueMap();
//
//            float x = objectProperties["x"].asFloat();
//            float y = objectProperties["y"].asFloat();
//            float width = objectProperties["width"].asFloat();
//            float height = objectProperties["height"].asFloat();
//
//            // Lấy ra tọa độ trái và phải từ obj
//
//            // Tạo sprite và physics body
//            auto sprite = Sprite::create();
//            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
//            physicsBody->setDynamic(false);
//            physicsBody->setGravityEnable(false);
//            physicsBody->setContactTestBitmask(true);
//            physicsBody->setCollisionBitmask(0);
//            sprite->setPhysicsBody(physicsBody);
//            // Đặt vị trí cho sprite và thêm vào scene
//            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
//            _tilemap->addChild(sprite, 2);
//
//        }
//    }
//    else
//    {
//        CCLOG("khong the lay %s tu tiled", obj.c_str());
//    }
//}

void PlayGameScene::getGround(cocos2d::TMXTiledMap* _tilemap)
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

            // Lấy ra tọa độ trái và phải từ obj

            // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(200);
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

void PlayGameScene::getHiddenPassageAndAddSprite(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "hidden passage";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            //physicbox
            // Lấy ra tọa độ trái và phải từ obj
            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();

            //sprite
            std::string group = objectProperties["group"].asString();
            int spriteID = objectProperties["ID"].asInt();
            std::string tileset = objectProperties["tileset"].asString();

            //role
            std::string role = objectProperties["role"].asString();

            // Tạo sprite và physics body
            auto sprite = Sprite::create();
            if (sprite)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);


                sprite->setName("group : " + group);
                sprite->setContentSize(Size(width, height));
                sprite->setPhysicsBody(physicsBody);

                // Đặt vị trí cho sprite và thêm vào scenes
                sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));

                // Đặt vị trí cho sprite và thêm vào scene
                
                if (role != "switch")
                {
                    sprite->setTexture("inGame/tileSet/"+ tileset +"/  (" + std::to_string(spriteID + 1) + ").png");
                    //physicsBody->setPositionOffset(Vec2(0,0));
                    sprite->setPosition(Vec2(x + width / 2, y + height / 2));
                    physicsBody->setCollisionBitmask(200);
                }
                else if (role == "switch")
                {
                    physicsBody->setName(group);
                    physicsBody->setCollisionBitmask(201);
                    //sprite->setPosition(Vec2(x + width / 2, y + height / 2));
                }

                _tilemap->addChild(sprite, 100);

            }
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void PlayGameScene::getBarrier(cocos2d::TMXTiledMap* _tilemap)
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

            // Lấy ra tọa độ trái và phải từ obj

            // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(200);
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


void PlayGameScene::getNextInTilemap(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "next";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
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

            std::string operationMethod = objectProperties["operation method"].asString();

            // Lấy ra tọa độ trái và phải từ obj

            // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(202);
            physicsBody->setName(operationMethod);
            sprite->setPhysicsBody(physicsBody);
            sprite->setName("sprite : " + operationMethod);
            
            if (operationMethod == "button")
            {
                //tao nut "thao tac voi quest"
                auto ButtonNormal = Sprite::create("inGame/button/takeItem.png");
                auto ButtonSelected = Sprite::create("inGame/button/takeItem.png");
                ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                    ButtonNormal->getContentSize().height * 1.1));
                auto buttonGetItem = MenuItemSprite::create(
                    ButtonNormal,
                    ButtonSelected,
                    std::bind(&PlayGameScene::nextButtonHandle, this, std::placeholders::_1));
                buttonGetItem->setScale((tileSize.height / buttonGetItem->getContentSize().height) * 2);
                buttonGetItem->setTag(578);
                buttonGetItem->setName("button");

                //tao menu va them cac nut
                auto menu = Menu::create(buttonGetItem, nullptr);
                menu->setVisible(false);
                menu->setPosition(Vec2(width / 2, height * 1.5));
                menu->setName("menu " + name);

                sprite->addChild(menu, 100);
            }


            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));
            _tilemap->addChild(sprite, 999);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void PlayGameScene::next()
{
    auto menuScene = MenuScene::create();
    Director::getInstance()->replaceScene(menuScene);

    //if (level > loadHighScoreLevel())
    //{
    //    UserDefault::getInstance()->setIntegerForKey("current_level", level);
    //    UserDefault::getInstance()->setBoolForKey("next_level_button_state", true);

    //    UserDefault::getInstance()->flush();
    //    //CCLOG("Level %d has been saved after winning.", level);
    //    UserDefault::getInstance()->destroyInstance();
    //}
}

void PlayGameScene::nextButtonHandle(Ref* sender)
{
    next();
}

void PlayGameScene::getNextScene(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "next scene";
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

            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

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

            _tilemap->addChild(sprite, 9999);
        }
    }
}

//level
int PlayGameScene::loadHighScoreLevel()
{
    return UserDefault::getInstance()->getIntegerForKey("current_level", 1);
}

//item , switch ...
void PlayGameScene::getItem(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "item";
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
            int IDInTileMap = objectProperties["id"].asInt();
            int spriteID = objectProperties["ID"].asInt();
            std::string tileset = objectProperties["tileset"].asString();
            int tutorialNumber = objectProperties["tutorial"].asInt();


            // Lấy ra tọa độ trái và phải từ physics body

        // Tạo sprite và physics body
            //CCLOG("%s", sprit);]

            Sprite* sprite = Sprite::create("inGame/tileSet/" + tileset + "/  (" + std::to_string(spriteID + 1) + ").png");
            if (sprite)
            {
                auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setGravityEnable(false);
                physicsBody->setContactTestBitmask(true);
                physicsBody->setCollisionBitmask(0);
                physicsBody->setTag(IDInTileMap);
                physicsBody->setName(name);

                //tao nut "thao tac voi quest"
                auto ButtonNormal = Sprite::create("inGame/button/takeItem.png");
                auto ButtonSelected = Sprite::create("inGame/button/takeItem.png");
                ButtonSelected->setContentSize(Size(ButtonNormal->getContentSize().width * 1.1,
                    ButtonNormal->getContentSize().height * 1.1));
                auto buttonGetItem = MenuItemSprite::create(
                    ButtonNormal,
                    ButtonSelected,
                    std::bind(&PlayGameScene::itemButtonHandle, this, std::placeholders::_1, sprite));
                buttonGetItem->setScale((tileSize.height / buttonGetItem->getContentSize().height) * 2);
                buttonGetItem->setTag(578);
                buttonGetItem->setName("button");

                //tao menu va them cac nut
                auto menu = Menu::create(buttonGetItem, nullptr);
                menu->setVisible(false);
                menu->setPosition(Vec2(width / 2 , height * 1.5));
                menu->setName("menu " + name + "-" + std::to_string(IDInTileMap));

                sprite->addChild(menu, 100);

                if (tutorialNumber != 0)
                {
                    Tutorial* tutorial = Tutorial::create();
                    tutorial->setScale((tileSize.width / tutorial->getSpriteSize().width) * 3);
                    tutorial->setPosition(Vec2(width / 2, height * 2.5));
                    tutorial->setName("tutorial animation");
                    tutorial->setTag(tutorialNumber);
                    tutorial->setVisible(false);
                    sprite->addChild(tutorial, 9999);
                }


                sprite->setOpacity(255);
                sprite->setContentSize(Size(width, height));
                sprite->setPhysicsBody(physicsBody);
                //sprite->setName(name.c_str());
                // Đặt vị trí cho sprite và thêm vào scenes
                sprite->setPosition(Vec2(Vec2(x + width / 2, y + height / 2)));
                sprite->setName("sprite " + name + "-" + std::to_string(IDInTileMap));
                _tilemap->addChild(sprite, layer);
            }

        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void  PlayGameScene::setVisibleButtonNode(std::string name, int ID, bool TF)
{
    CCLOG("set visible");
    auto sprite = tileMaps[scene - 1]->getChildByName("sprite " + name + "-" + std::to_string(ID));
    if (sprite)
    {
        auto menu = sprite->getChildByName("menu " + name + "-" + std::to_string(ID));

        if (menu)
        {
            menu->setVisible(TF);
            if (sprite->getChildByName("tutorial animation"))
            {
                // Lên lịch gọi hàm để thiết lập visible sau một khoảng thời gian

                /*this->scheduleOnce([sprite , TF](float dt) {
                    if (sprite)
                    {
                        sprite->getChildByName("tutorial animation")->setVisible(TF);
                    }
                    }, 0.25, "set_visible_key");*/
                
            }
        }
        else
        {
            CCLOG("menu not available");
        }
    }
    else
    {
        CCLOG("sprite not available");
    }
}

//on-off  open-close door,trap,...
void PlayGameScene::openHiddenPassage(std::string group)
{
    CCLOG("open hidden passage [%s]", group.c_str());
    auto children = tileMaps[scene - 1]->getChildren();

    for (auto child : children) {
        if (child->getName() == "group : " + group) {
            CCLOG("remove child");
            child->removeFromParentAndCleanup(true);
        }
    }
}


//set button for item, quest, switch...
void PlayGameScene::itemButtonHandle(Ref* sender, Sprite* sprite)
{
    CCLOG("take item");

    if (tutorial)
    {
        if (sprite->getChildByName("tutorial animation")->getTag() == tutorialNum)
        {
            if (sprite->getChildByName("tutorial animation"))
            {
                sprite->getChildByName("tutorial animation")->setVisible(false);
                completeTutorialNum(sprite->getChildByName("tutorial animation")->getTag(), 1);
                sprite->removeFromParentAndCleanup(true);
            }
        }
        //return;
    }
}

//tutorial
void PlayGameScene::getTutorial(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "list tutorial";
    // Lấy layer chứa các đối tượng từ Tiled Map
    auto objectLayer = _tilemap->getObjectGroup(obj);

    if (objectLayer) {
        tutorial = true;
        // Lấy danh sách các đối tượng từ object layer
        ValueVector objects = objectLayer->getObjects();

        // Duyệt qua từng đối tượng và gán physics body
        for (const auto& object : objects) {
            ValueMap objectProperties = object.asValueMap();

            float x = objectProperties["x"].asFloat();
            float y = objectProperties["y"].asFloat();
            float width = objectProperties["width"].asFloat();
            float height = objectProperties["height"].asFloat();
            int tutorialOrder = objectProperties["number"].asInt();
            std::string name = objectProperties["name"].asString();
            if (tutorialOrder > tutorialMax)
            {
                tutorialMax = tutorialOrder;
            }
            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

            // Tạo sprite và physics body
            auto sprite = Sprite::create();
            auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(1.0f, 0.0f, 0.0f));
            physicsBody->setDynamic(false);
            physicsBody->setGravityEnable(false);
            physicsBody->setContactTestBitmask(true);
            physicsBody->setCollisionBitmask(0);
            physicsBody->setName(name);
            physicsBody->setTag(tutorialOrder);
            sprite->setPhysicsBody(physicsBody);
            // Đặt vị trí cho sprite và thêm vào scene
            sprite->setPosition(Vec2(x + width / 2, y + height / 2));

            Tutorial* tutorial = Tutorial::create();
            tutorial->setScale((tileSize.width / tutorial->getSpriteSize().width) * 3);
            tutorial->setPosition(Vec2(-width/2, height/2));
            tutorial->setName("tutorial animation");
            sprite->addChild(tutorial, 9999);

            sprite->setVisible(false);
            sprite->setName("tutorial sprite");
            sprite->setTag(tutorialOrder);


            _tilemap->addChild(sprite, 9999);
        }
    }
    else
    {
        CCLOG("khong the lay %s tu tiled", obj.c_str());
    }
}

void PlayGameScene::visibleTutorialNum(float dt)
{

    tutorialNum++;
    if (tutorial)
    {
        auto children = tileMaps[scene - 1]->getChildren();

        for (auto childSprite : children)
        {
            if (childSprite->getName() == "tutorial sprite" && childSprite->getTag() == tutorialNum)
            {
                childSprite->setVisible(true);
            }
        }
    }
}

void PlayGameScene::completeTutorialNum(int number, float delay)
{
    CCLOG("complete %d tutorial", number);

    this->scheduleOnce(CC_SCHEDULE_SELECTOR(PlayGameScene::visibleTutorialNum), delay);

}

//notification
void PlayGameScene::getNotification(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "notification";
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
            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

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

            _tilemap->addChild(sprite, 9999);
        }
    }
}


//suggest
void PlayGameScene::getSuggest(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "suggest";
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

            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

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

            _tilemap->addChild(sprite, 9999);
        }
    }
}


//gate
void PlayGameScene::getGateBarrier(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "gate barrier";
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

            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

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

            _tilemap->addChild(sprite, 9999);
        }
    }
}

//puzzle
void PlayGameScene::getPuzzle(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "puzzle";
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

            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

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

            _tilemap->addChild(sprite, 9999);
        }
    }
}


//gate
void PlayGameScene::getGate(cocos2d::TMXTiledMap* _tilemap)
{
    std::string obj = "puzzle";
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

            //bool TF = objectProperties["TF"].asBool();

            // Lấy ra tọa độ trái và phải từ obj

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

            _tilemap->addChild(sprite, 9999);
        }
    }
}


//event
bool PlayGameScene::onTouchBegan(Touch* touch, Event* event)
{
    CCLOG("onTouchBegan");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto touchPoint = touch->getLocationInView();
    auto touchPointInNode = tileMaps[scene - 1]->convertToNodeSpace(touchPoint);
    Vec2 location = touch->getLocation();

    // Lấy tất cả các đối tượng vật lý tại vị trí chạm
    auto physicsWorld = Director::getInstance()->getRunningScene()->getPhysicsWorld();
    auto point = Point(location.x, location.y);
    auto arr = physicsWorld->getShapes(point);

    for (auto& obj : arr)
    {
        // Lấy physics body
        auto physicsBody = obj->getBody();
        if (physicsBody)
        {
            // Lấy node liên kết với physics body
            auto node = physicsBody->getNode();
            if (node->getName() == "tutorial sprite" && node->getTag() == tutorialNum && tutorialNum != 0)
            {
                node->setVisible(false);

                //node->getChildByName("tutorial sprite")->getChildByName("tutorial animation")->setVisible(false);
                if (node->getTag() == tutorialMax)
                {
                    tutorial = false;
                }
                playerMoveTo(touchPointInNode.x);
                /*auto sprite = node->getChildByName("tutorial animation");
                sprite->setVisible(false);*/
                //completeTutorialNum(tutorialNum, 1);
            }
            else
            {
                CCLOG("khong phu hop");
            }
        }
    }

    return true;

}

bool PlayGameScene::onTouchEnded(Touch* touch, Event* event)
{
    if (!tutorial)
    {
        //CCLOG("move");
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        auto touchPoint = touch->getLocationInView();

        auto touchPointInNode = tileMaps[scene-1]->convertToNodeSpace(touchPoint);

        playerMoveTo(touchPointInNode.x);
    }


    return true;
}

bool PlayGameScene::onContactBegin(PhysicsContact& contact) {

    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

    if ((nodeA->getCollisionBitmask() == 100 || nodeB->getCollisionBitmask() == 100)
        /*&& ((nodeA->getName() != nodeB->getName()) && (nodeA->getTag() != nodeB->getTag()))*/)
    {
        // hidden passage
        if (nodeA->getCollisionBitmask() == 201 || nodeB->getCollisionBitmask() == 201)
        {
            if (nodeA->getCollisionBitmask() == 201)
            {
                openHiddenPassage(nodeA->getName());
            }
            else if (nodeB->getCollisionBitmask() == 201)
            {
                openHiddenPassage(nodeB->getName());
            }
        }

        if (nodeA->getCollisionBitmask() == 202 || nodeB->getCollisionBitmask() == 202)
        {
            if (nodeA->getName() == "contact" || nodeB->getName() == "contact")
            {
                next();
            }
            else if (nodeA->getName() == "button" || nodeB->getName() == "button")
            {
                tileMaps[scene - 1]->getChildByName("sprite : button")->getChildByName("menu next level")->setVisible(true);
            }
        }

        // set visible button
        if (nodeA->getCollisionBitmask() == 0 || nodeB->getCollisionBitmask() == 0)
        {
            if (nodeA->getName() == "tutorial" || nodeB->getName() == "tutorial")
            {
                if (nodeA->getName() == "tutorial")
                {
                    nodeA->removeFromWorld();
                    completeTutorialNum(nodeA->getTag(), 1);
                }else if(nodeB->getName() == "tutorial")
                {
                    nodeB->removeFromWorld();
                    completeTutorialNum(nodeB->getTag(), 1);
                }
                
            }

            if (nodeA->getCollisionBitmask() == 0)
            {
                setVisibleButtonNode(nodeA->getName(), nodeA->getTag(), true);
            }
            else if(nodeB->getCollisionBitmask() == 0)
            {
                setVisibleButtonNode(nodeB->getName(), nodeB->getTag(), true);
            }
        }
    }


    return true;
}

bool PlayGameScene::onContactSeparate(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();

    if (nodeA->getName() == "player" || nodeB->getName() == "player")
    {
        // set visible button
        if (nodeA->getCollisionBitmask() == 0 || nodeB->getCollisionBitmask() == 0)
        {
            if (nodeA->getCollisionBitmask() == 0)
            {
                setVisibleButtonNode(nodeA->getName(), nodeA->getTag(), false);
            }
            else if (nodeB->getCollisionBitmask() == 0)
            {
                setVisibleButtonNode(nodeB->getName(), nodeB->getTag(), false);
            }
        }
    }


    return true;
}

void PlayGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    int _scene = scene;
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        scene += 1;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        scene -= 1;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_1:
            changeScene(1);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_2:
        changeScene(2);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_3:
        changeScene(3);
        break;
    default:
        break;
    }
}