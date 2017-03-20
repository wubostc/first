//#include <cocos2d.h>

#include "AppDelegate.h"
#include"CCPHPMYSQL.h"

#include"wan\GameDefine.h"

USING_NS_CC;



AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("first", Rect(0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
#else
        glview = GLViewImpl::create("first");
#endif
        director->setOpenGLView(glview);
    }
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60.0f);

	//·Ö±æÂÊÊÊÅä 
	//auto screen_size = glview->getFrameSize();
	//auto design_size = Size(0, 0);

	//if (screen_size.height == 480) {
	//	design_size.setSize(480, 800);
	//}
	//else if (screen_size.height == 720 ) {
	//	design_size.setSize(720,1280);
	//}
	//else if (screen_size.height == 1920) {
	//	design_size.setSize(1080,1920);
	//}

	//glview->setDesignResolutionSize(design_size.width, design_size.height, ResolutionPolicy::NO_BORDER);
	
//    if (frameSize.height > mediumResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is larger than the height of small size.
//    else if (frameSize.height > smallResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is smaller than the height of medium size.
//    else
//    {        
//        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
//    }

    register_all_packages();

	

	cocos2d::Director::getInstance()->runWithScene(TransitionFade::create(1.0f, CCPHPMYSQL::createScene()));

	//cocos2d::Director::getInstance()->runWithScene(WelcomeScene::createScene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
