#ifndef __SEND_SCORE_SCENE_H__
#define __SEND_SCORE_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

class SendScoreScene : public cocos2d::Layer
{
private:
    cocos2d::EventListenerTouchOneByOne *eventListener;

    // ボタン関連
    void appendButton(cocos2d::Vec2 point);
    void onTapSendButton(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);

    // 入力欄
    cocos2d::extension::EditBox* _userIdInput;
    cocos2d::extension::EditBox* _scoreInput;
    void appendInput(cocos2d::Vec2 point);
    int _score = 0;

public:
    static cocos2d::Scene* createScene(int score);

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(SendScoreScene);

    void setScore(int score);

    // HTTP
    void httpRequest();
    void onHttpRequestCallBack(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);
};

#endif // __SEND_SCORE_SCENE_H__
