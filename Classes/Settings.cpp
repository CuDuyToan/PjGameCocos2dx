#include "Settings.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

bool Settings::init() {
    if (!Layer::init())
    {
        return false;
    }

    //this->setVisible(false);

    //createSettingsButton();


    return true;
}

void Settings::createSettingsButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto settingsButton = ui::Button::create(
        "ButtonLevel/DefaultSettings.png", 
        "ButtonLevel/Settings.png");
    //settingsButton->setScale(0.2 * (visibleSize.height / settingsButton->getContentSize().height));
    settingsButton->setPosition(Vec2(origin.x + visibleSize.width - settingsButton->getContentSize().width / 2,
        origin.y + visibleSize.height - settingsButton->getContentSize().height / 2));
 
    settingsButton->addClickEventListener([=](Ref* sender) {
        createButtons();
        });
    this->addChild(settingsButton);
}

void Settings::createButtons()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createPanel();
    
    // Create "Chơi Lại" button
    auto replayButton = MenuItemImage::create(
        "UICompleteGame/DefaultRepeat.png",
        "UICompleteGame/Repeat.png",
        [](Ref* sender) {
            
        });
    replayButton->setScale(0.2);
    replayButton->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - 20));

    // Create "Next Level" button
    auto nextLevelButton = MenuItemImage::create(
        "UICompleteGame/DefaultNextLevel.png",
        "UICompleteGame/NextLevel.png",
        [](Ref* sender) {
            
        });
    nextLevelButton->setScale(0.2);
    nextLevelButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 20));

    auto backLevelButton = MenuItemImage::create(
        "UICompleteGame/DefaultListLevel.png",
        "UICompleteGame/ListLevel.png",
        [](Ref* sender) {
            
        });
    backLevelButton->setScale(0.2);
    backLevelButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - 20));


    // Create menu and add buttons
    auto menu = Menu::create(replayButton, nextLevelButton, backLevelButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void Settings::createPanel()
{   
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto panel = Sprite::create("UICompleteGame/PanelMedium.png");
    panel->setScale(0.2f);
    panel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(panel);

    /*auto label = Label::createWithSystemFont("Level Complete", "fonts / Marker Felt.ttf", 150);
    label->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - label->getContentSize().height));
    panel->addChild(label);*/
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 150, GlyphCollection::DYNAMIC);
    ttfConfig.bold = true;
    auto label = Label::createWithTTF(ttfConfig, "Level Complete");
    label->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - label->getContentSize().height));
    panel->addChild(label);
}