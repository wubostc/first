#include "WelcomeScene.h"
#include "GameDefine.h"
#include "GameScene.h"

using namespace CocosDenshion;

Scene* WelcomeScene::createScene()	{
    auto scene = Scene::create();
    auto layer = WelcomeScene::create();
    scene->addChild(layer);
	return scene;
}

// 欢迎界面 初始化函数
bool WelcomeScene::init()	{
	// 先初始化父类，不成功返回false
	if (!Layer::init()) {
		return false;
	}

	// 添加背景图片
    auto bg = Sprite::create(SCENE_START);
	bg->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	this->addChild(bg);

	// 添加开始按钮
	auto start_item = MenuItemImage::create(
                                           BTN_START_1A,
                                           BTN_START_1B,
										   CC_CALLBACK_1(WelcomeScene::menuStartCallback, this));
	start_item->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6));
	auto menu = Menu::create(start_item, nullptr);
    menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void WelcomeScene::menuStartCallback(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionPageTurn::create(1.0f, GameScene::createScene(),false));
}

void WelcomeScene::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();  
	SHARED_AUDIO->playBackgroundMusic(MUSIC_BG1,true);
}


void WelcomeScene::onExitTransitionDidStart() {
	Layer::onExitTransitionDidStart();
	SHARED_AUDIO->stopBackgroundMusic(MUSIC_BG1);
}