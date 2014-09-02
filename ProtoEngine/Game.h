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
#include "Scene.h"

class Timer;
class Win32EventHander;
class Game;

class InputHandlerInterface
{
public:
    virtual void bindImple(Game* game){} 
    virtual bool isValid(){return true;}
    virtual void pause(){}
    virtual void restore(){}
    virtual void resize(int newWidth, int newHeight){} 
    virtual void onLButtonDown(int x, int y){}
    virtual void onLButtonUp(int x, int y){}
    virtual void onRButtonDown(int x, int y){}
    virtual void onRButtonUp(int x, int y){}
    virtual void onMouseMove(int x, int y){}
};

class Win32EventHander : public InputHandlerInterface
{
public:
    Win32EventHander();
    virtual void bindImple(Game* game);
    virtual bool isValid();
    virtual void pause();
    virtual void restore(); 
    virtual void resize(int newWidth, int newHeight);

    virtual void onLButtonDown(int x, int y);
    virtual void onLButtonUp(int x, int y);
    virtual void onRButtonDown(int x, int y);
    virtual void onRButtonUp(int x, int y);
    virtual void onMouseMove(int x, int y);

protected:
    Game* mGame;
    int mLastMouseX;
    int mLastMouseY;
    bool mIsLButtonDown;
    bool mIsRButtonDown;
};

class Game
{
public:
    Game();
    ~Game();

    bool isInitialized(){return mInitialized;}
    bool init();
    bool exit();
    int32 runWin32();
    RenderCore* getRenderCore();

protected:
    friend class Win32EventHander;
    void onPause();
    void onRestore();
    void onResize(float width, float height);

protected:
    void step(float delta);
    void driveRenderCore();
    void calcFrameTime();

protected:
    // Game state
    bool mInitialized;
    bool mGamePaused;
    bool mRenderCorePaused;

    Ticker mTicker;
    Win32EventHander mEventHandler;
    RenderWindow* mGameWindow;
    RenderCore* mRenderCore;
    Scene* mScene;

    // Stat variables
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