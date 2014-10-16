#include "SendScoreScene.h"
#include "StartScene.h"
#include "network/HttpClient.h"
#include "editor-support/spine/Json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;

Scene* SendScoreScene::createScene(int score)
{
    auto scene = Scene::create();
    auto layer = SendScoreScene::create();
    layer->setScore(score);
    scene->addChild(layer);

    return scene;
}

bool SendScoreScene::init(){
    if ( !Layer::init() ){
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(SendScoreScene::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto label = LabelTTF::create("Send Score", "Arial", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    // タッチモードを設定する
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);

    // ボタンの追加
    appendButton(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // 数値入力欄の追加
    appendInput(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));

    return true;
}


void SendScoreScene::menuCloseCallback(Ref* pSender)
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

void SendScoreScene::setScore(int score)
{
    _score = score;
    if (_score > 0) {
        _scoreInput->setText(std::to_string(_score).c_str());
        _scoreInput->setVisible(false);

        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        std::string caption = "SCORE:";
        caption += std::to_string(_score);
        auto socreLbl = LabelTTF::create(caption, "Arial", 32);
        socreLbl->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(socreLbl, 1);
    }
}

void SendScoreScene::appendButton(Vec2 point){
    auto sendLbl = LabelTTF::create("送信", "Arial", 24);
    auto sendBtn = ControlButton::create(sendLbl, Scale9Sprite::create("Images/CyanSquare.png"));
    sendBtn->setPreferredSize(Size(185, 40));
    sendBtn->setPosition(point.x, point.y - 100);
    sendBtn->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(SendScoreScene::onTapSendButton),
        Control::EventType::TOUCH_UP_INSIDE);

    addChild(sendBtn);

    auto returnLbl = LabelTTF::create("スタートに戻る", "Arial", 24);
    auto returnBtn = ControlButton::create(returnLbl, Scale9Sprite::create("extensions/orange_edit.png"));
    returnBtn->setPreferredSize(Size(185, 40));
    returnBtn->setPosition(point.x, point.y - 170);
    returnBtn->addTargetWithActionForControlEvents(
        this,
        cccontrol_selector(SendScoreScene::onTapReturnButton),
        Control::EventType::TOUCH_UP_INSIDE);

    addChild(returnBtn);
}

void SendScoreScene::onTapSendButton(Ref* sender, Control::EventType controlEvent){
    httpRequest();
}

void SendScoreScene::onTapReturnButton(Ref* sender, Control::EventType controlEvent)
{
    auto scene = StartScene::createScene();
    auto tran = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(tran);
}

void SendScoreScene::appendInput(Vec2 point){
    _scoreInput = EditBox::create(Size(150, 50), Scale9Sprite::create("switch-mask.png"));
    _scoreInput->setPosition(point);
    _scoreInput->setFontColor(ccBLUE);
    _scoreInput->setPlaceHolder("Score:");
    _scoreInput->setMaxLength(4);
    _scoreInput->setReturnType(EditBox::KeyboardReturnType::DONE);
    _scoreInput->setInputMode(EditBox::InputMode::DECIMAL);
    if (_score > 0) {
        _scoreInput->setText(std::to_string(_score).c_str());
    }
    addChild(_scoreInput);

    _userIdInput = EditBox::create(Size(200, 50), Scale9Sprite::create("switch-mask.png"));
    _userIdInput->setPosition(Vec2(point.x, point.y+60));
    _userIdInput->setFontColor(ccBLUE);
    _userIdInput->setPlaceHolder("UserId:");
    _userIdInput->setMaxLength(10);
    _userIdInput->setReturnType(EditBox::KeyboardReturnType::DONE);
    _userIdInput->setInputMode(EditBox::InputMode::ANY);
    addChild(_userIdInput);
}

void SendScoreScene::httpRequest()
{
    auto request = new HttpRequest();

    request->setUrl("http://app-redis-practice.herokuapp.com/ranking");
    request->setRequestType(HttpRequest::Type::POST);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    //request->setResponseCallback(this, httpresponse_selector(SendScoreScene::onHttpRequestCallBack));

    // JSONパラメーター
    std::string data = "{\"score\":";
    data += _scoreInput->getText();
    data += ", \"user_id\":\"";
    data += _userIdInput->getText();
    data += "\"}";

    const char* buffer = data.c_str();
    request->setRequestData(buffer, strlen(buffer));
    network::HttpClient::getInstance()->send(request);
    request->release();

    auto scene = StartScene::createScene();
    auto tran = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(tran);
}

void SendScoreScene::onHttpRequestCallBack(HttpClient* sender, HttpResponse* response)
{
    std::vector<char>* data = response->getResponseData();
    std::string result(data->begin(), data->end());

    Json* json = Json_create(result.c_str());

    std::string val = Json_getString(json, "response", "");
    MessageBox("Score Send","HTTP Response");

    auto scene = StartScene::createScene();
    auto tran = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(tran);
}
