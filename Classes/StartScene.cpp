#include "StartScene.h"
#include "MainScene.h"
#include "SendScoreScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* StartScene::createScene()
{
    auto scene = Scene::create();

    auto layer = StartScene::create();

    scene->addChild(layer);

    return scene;
}

bool StartScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(StartScene::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = LabelTTF::create("Start Game", "Arial", 32);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    // タッチモードを設定する
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);

    // ボタンの追加
    appendButton(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    return true;
}


void StartScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void StartScene::appendButton(Vec2 point)
{
    auto playButton = ControlButton::create(Scale9Sprite::create("btn-play-normal.png"));
    playButton->setAdjustBackgroundImage(false);
    playButton->setPosition(point.x, point.y);
    playButton->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(StartScene::onTapPlayButton),
        Control::EventType::TOUCH_UP_INSIDE);

    addChild(playButton);

    auto scoreButton = ControlButton::create(Scale9Sprite::create("Images/SendScoreButton.png"));
    scoreButton->setAdjustBackgroundImage(false);
    scoreButton->setPosition(point.x, point.y - 100);
    scoreButton->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(StartScene::onTapScoreButton),
        Control::EventType::TOUCH_UP_INSIDE);

    addChild(scoreButton);
}

void StartScene::onTapPlayButton(Ref* sender, Control::EventType controlEvent)
{
    auto scene = MainScene::createScene();
    auto tran = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(tran);
}

void StartScene::onTapScoreButton(Ref* sender, Control::EventType controlEvent)
{
    auto scene = SendScoreScene::createScene(0);
    auto tran = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(tran);
}
