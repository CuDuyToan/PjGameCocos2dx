#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include "cocos2d.h"

class puzzle : public cocos2d::Layer {
public:

    virtual bool init() {
        if (!Layer::init()) {
            return false;
        }

        isPieceBeingMoved = false;

        createDestination("ButtonLevel/DefaultUnlockLevel.png", cocos2d::Vec2(200, 200));
        createDestination("ButtonLevel/DefaultUnlockLevel.png", cocos2d::Vec2(400, 200));

        // Tạo các mảnh ghép và thêm vào layer
        auto piece1 = createPuzzlePiece("ButtonLevel/UnlockLevel.png", cocos2d::Vec2(100, 100), cocos2d::Vec2(200, 200));
        this->addChild(piece1);
        puzzlePieces.push_back(piece1);

        // Tiếp tục thêm các mảnh ghép khác tương tự
        auto piece2 = createPuzzlePiece("ButtonLevel/UnlockLevel.png", cocos2d::Vec2(150, 100), cocos2d::Vec2(400, 200));
        this->addChild(piece2);
        puzzlePieces.push_back(piece2);


        this->scheduleUpdate();

        return true;
    }

    void update(float dt) override {
        if (isPuzzleSolved()) {
            //Director::getInstance()->popScene();
        }
    }

    bool isPuzzleSolved() const {
        for (const auto& piece : puzzlePieces) {
            auto correctPos = static_cast<cocos2d::Vec2*>(piece->getUserData());
            if (!piece->getPosition().equals(*correctPos)) {
                return false;
            }
        }
        return true;
    }

    virtual ~puzzle() {
        for (auto piece : puzzlePieces) {
            delete static_cast<cocos2d::Vec2*>(piece->getUserData());
        }
    }

    CREATE_FUNC(puzzle);

private:
    std::vector<cocos2d::Sprite*> puzzlePieces;

    bool isPieceBeingMoved; // Biến cờ để kiểm tra trạng thái di chuyển

    cocos2d::Sprite* createPuzzlePiece(const std::string& filename, const cocos2d::Vec2& startPos, const cocos2d::Vec2& correctPos) {
        auto piece = cocos2d::Sprite::create(filename);
        piece->setPosition(startPos);

        piece->setUserData(new cocos2d::Vec2(correctPos));

        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = [this, piece](cocos2d::Touch* touch, cocos2d::Event* event) {
            if (isPieceBeingMoved) {
                return false;
            }
            if (piece->getBoundingBox().containsPoint(touch->getLocation())) {
                isPieceBeingMoved = true;
                return true;
            }
            return false;
        };

        listener->onTouchMoved = [piece](cocos2d::Touch* touch, cocos2d::Event* event) {
            piece->setPosition(touch->getLocation());
        };

        listener->onTouchEnded = [this, piece, correctPos](cocos2d::Touch* touch, cocos2d::Event* event) {
            isPieceBeingMoved = false;
            if (piece->getPosition().getDistance(correctPos) < 50) {
                piece->setPosition(correctPos);
            }
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, piece);

        return piece;
    }

    void createDestination(const std::string& filename, const cocos2d::Vec2& position) {
        auto destination = cocos2d::Sprite::create(filename);
        destination->setPosition(position);
        this->addChild(destination);
    }

};

#endif // __PUZZLE_H__
