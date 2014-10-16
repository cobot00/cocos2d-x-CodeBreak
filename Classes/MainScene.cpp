#include "MainScene.h"
#include "StartScene.h"
#include "SendScoreScene.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

#define DIGIT_LENGTH 4
#define CODE_NONE 0
#define CODE_BLOW 1
#define CODE_HIT 2

Scene* MainScene::createScene()
{
    auto scene = Scene::create();

    auto layer = MainScene::create();

    scene->addChild(layer);

    return scene;
}

bool MainScene::init()
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
                                           CC_CALLBACK_1(MainScene::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = LabelTTF::create("Guess Number", "Arial", 32);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    // 4桁の数字の生成
    initializeDigit();

    // タッチモードを設定する
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);

    // ボタンの追加
    appendButton(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // 数値入力欄の追加
    appendInput(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 100));

    return true;
}

void MainScene::initializeDigit()
{
    srand(time(0));
    // 乱数を安定させるための空回し
    for (int i = 0; i < 5; i++) CCRANDOM_0_1();

    for (int i = 0; i < DIGIT_LENGTH; i++) {
        _digit4[i] = static_cast<int>(CCRANDOM_0_1() * 10);
    }
}

std::string MainScene::intToString(int p[])
{
    std::string str;
    for (int i = 0; i < DIGIT_LENGTH; i++) {
        str += std::to_string(p[i]);
    }
    return str;
}

void MainScene::menuCloseCallback(Ref* pSender)
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

void MainScene::appendButton(Vec2 point)
{
    auto decideLbl = LabelTTF::create("決定", "Arial", 24);
    auto decideBtn = ControlButton::create(decideLbl, Scale9Sprite::create("Images/CyanSquare.png"));
    decideBtn->setPreferredSize(Size(150, 40));
    decideBtn->setPosition(point.x, point.y - 50);
    decideBtn->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(MainScene::onTapDecideButton),
        Control::EventType::TOUCH_UP_INSIDE);

    addChild(decideBtn);

    auto resetLbl = LabelTTF::create("諦める", "Arial", 24);
    auto resetBtn = ControlButton::create(resetLbl, Scale9Sprite::create("extensions/orange_edit.png"));
    resetBtn->setPreferredSize(Size(150, 40));
    resetBtn->setPosition(point.x, point.y - 150);
    resetBtn->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(MainScene::onTapResetButton),
        Control::EventType::TOUCH_UP_INSIDE);

    addChild(resetBtn);
}

void MainScene::onTapDecideButton(Ref* sender, Control::EventType controlEvent)
{
    const char *number = _numberInput->getText();
    if (strlen(number) < DIGIT_LENGTH) {
        MessageBox("4桁の数字を入力してください","Warning");
        return;
    }

    codeBreak(number);

    if (_tryCount == 5) {
        MessageBox("GAME OVER","Warning");
        auto scene = StartScene::createScene();
        auto tran = TransitionFade::create(2, scene);
        Director::getInstance()->replaceScene(tran);
    }
}

void MainScene::codeBreak(const char *number)
{
    int result[DIGIT_LENGTH] = {CODE_NONE, CODE_NONE, CODE_NONE, CODE_NONE};
    int hit = 0;
    int blow = 0;

    std::string str;
    for (int i = 0; i < DIGIT_LENGTH; i++) {
        str = number[i];
        int num = std::stoi(str);
        if (num == _digit4[i]) {
            result[i] = CODE_HIT;
            hit += 1;
        }
    }

    for (int i = 0; i < DIGIT_LENGTH; i++) {
        if (result[i] == CODE_HIT) {
            continue;
        }
        for (int j = 0; j < DIGIT_LENGTH; j++) {
            str = number[j];
            int num = std::stoi(str);
            if (result[j] == CODE_NONE && num == _digit4[i]) {
                result[j] = CODE_BLOW;
                blow += 1;
                break;
            }
        }

    }

    _tryCount++;

    if (hit == DIGIT_LENGTH) {
        MessageBox("正解です！！","Congratulation");
        int score = (512 / _tryCount) + static_cast<int>(CCRANDOM_0_1() * 50);
        auto scene = SendScoreScene::createScene(score);
        auto tran = TransitionFade::create(2, scene);
        Director::getInstance()->replaceScene(tran);
    }

    updateResult(hit, blow, _tryCount);
}

void MainScene::updateResult(int hit, int blow, int tryCount)
{
    std::string hitAndBlow = "HIT:";
    hitAndBlow += std::to_string(hit);
    hitAndBlow += " BLOW:";
    hitAndBlow += std::to_string(blow);
    hitAndBlow += " (";
    hitAndBlow += std::to_string(tryCount);
    hitAndBlow += "回)";
    _result->setString(hitAndBlow.c_str());
}

void MainScene::onTapResetButton(Ref* sender, Control::EventType controlEvent)
{
    MessageBox("スタート画面に戻ります","Give Up!!");
    auto scene = StartScene::createScene();
    auto tran = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(tran);
}

void MainScene::appendInput(Vec2 point){
    _numberInput = EditBox::create(Size(200, 50), Scale9Sprite::create("switch-mask.png"));
    _numberInput->setPosition(point);
    _numberInput->setFontColor(ccBLUE);
    _numberInput->setPlaceHolder("4桁の数字");
    _numberInput->setMaxLength(4);
    _numberInput->setReturnType(EditBox::KeyboardReturnType::DONE);
    _numberInput->setInputMode(EditBox::InputMode::DECIMAL);
    addChild(_numberInput);

    auto answer = LabelTTF::create(intToString(_digit4).c_str(), "Arial", 32);
    answer->setPosition(Vec2(point.x + 200, point.y));
    this->addChild(answer, 1);

    _result = LabelTTF::create("", "Arial", 32);
    _result->setPosition(Vec2(point.x, point.y - 60));
    this->addChild(_result, 1);
}
