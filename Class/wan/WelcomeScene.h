#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class WelcomeScene : public cocos2d::Layer
{
public:
	/***** 初始化函数 *****/
    static cocos2d::Scene* createScene();
	virtual bool init();  
	CREATE_FUNC(WelcomeScene);
	virtual void onEnterTransitionDidFinish();
	//virtual void cleanup();
	
	/***** 按钮函数 *****/
	void menuStartCallback(Ref* pSender);
	

	void onExitTransitionDidStart();
};

#endif // __WELCOME_SCENE_H__