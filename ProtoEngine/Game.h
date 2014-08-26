#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <sstream>

#include "reference.h"
#include "typedefs.h"
#include "singleton.h"
#include "RenderCore.h"
#include "Ticker.h"
#include "RenderWindow.h"

class Timer;
class Win32EventHander;
class Game;
class EventHandlerInterface
{
public:
    virtual void bindImple(Game* game){} 
    virtual void pause(){}
    virtual void restore(){}
    virtual void resize(int newWidth, int newHeight){} 
};

class TestHandler :  public EventHandlerInterface
{
public:
    TestHandler();
    virtual void bindImple(Game* game); 
    virtual void pause();
    virtual void restore();
    virtual void resize(int newWidth, int newHeight);

public:
    std::wstring mDebugInfo;
    bool bStarted;
};

class Win32EventHander : public EventHandlerInterface
{
public:
    Win32EventHander();
    virtual void bindImple(Game* game);
    virtual void pause();
    virtual void restore(); 
    virtual void resize(int newWidth, int newHeight);

protected:
    Game* mGame;
    RenderCore* mRenderCore;
};

class Game
{
public:
    Game();
    ~Game();

    bool isInitialized(){return mInitialized;}
    bool init();
    bool exit();
    // A windows msg pump
    int32 runWin32();
    RenderCore* getRenderCore();

protected:
    void onPause();
    void onRestore();

protected:
    void step(float delta);
    void driveRenderCore();
    void calcFrameTime();

protected:
    bool mInitialized;
    Ticker mTicker;
    Win32EventHander mEventHandler;
    TestHandler mTestHandler;
    RenderWindow* mMainWindow;
    RenderCore* mRenderCore;

    // Stat variables
protected:
    bool mEnableStat;
    Timer* mGameTimer;
    Timer* mRenderTimer;
    Timer* mInitTimer; 
    std::wostringstream mFrameDebugInfo; 
    std::wstring mRenderCoreDebugInfo;
    float mFrameTimeSum;
    float mGameTimeSum; 
    float mRenderTimeSum;
    float mStatInterval; // Gives average stat every 300ms
    uint32 mFrameCounter;
};

#endif