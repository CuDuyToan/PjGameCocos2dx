#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Triển khai Scene chơi game ở đây
    CCLOG("Playing game...");
    createTileMap();

    return true;
}

void GameScene::createTileMap()
{
    CCLOG("create tile Map");

    /*cocos2d::TMXTiledMap* _tileMap = new TMXTiledMap();
    _tileMap->initWithTMXFile("TileMap/Map1New.tmx");
    this->addChild(_tileMap);*/

    // Tạo một đối tượng TMXTiledMap và tải tilemap từ file .tmx
    auto tilemap = TMXTiledMap::create("TileMap/Map1New.tmx");

    // Kiểm tra xem tilemap có tải thành công hay không
    if (tilemap) {
        // Thêm tilemap vào scene hoặc layer của bạn
        CCLOG("tile map da duoc tai");
        this->addChild(tilemap);
    }
    else {
        // Xử lý khi không tải được tilemap
        CCLOG("khong the tai tile map");
    }



}
