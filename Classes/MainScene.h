#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

class MainScene : public cocos2d::Layer
{
private:
    cocos2d::EventListenerTouchOneByOne *eventListener;

    // 4桁の数字
    int _digit4[4];
    void initializeDigit();
    std::string intToString(int*);
    int _tryCount = 0;

    // コードブレイク
    void codeBreak(const char *number);
    void updateResult(int hit, int blow, int tryCount);

    // ボタン関連
    void appendButton(cocos2d::Vec2 point);
    void onTapDecideButton(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void onTapResetButton(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);

    // 入力欄
    cocos2d::extension::EditBox* _numberInput;
    void appendInput(cocos2d::Vec2 point);
    cocos2d::LabelTTF* _result;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(MainScene);
};

#endif // __MAIN_SCENE_H__
