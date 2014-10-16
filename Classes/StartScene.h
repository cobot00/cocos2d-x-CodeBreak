#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class StartScene : public cocos2d::Layer
{
private:
    cocos2d::EventListenerTouchOneByOne *eventListener;

    // ボタン関連
    void appendButton(cocos2d::Vec2 point);
    void onTapPlayButton(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void onTapScoreButton(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(StartScene);
};

#endif // __START_SCENE_H__
