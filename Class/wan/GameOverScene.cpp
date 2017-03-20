#include "GameOverScene.h"
#include "GameScene.h"
#include "WelcomeScene.h"
#include "../Connector.h"
#include "../User.h"
#include "../JSON.h"
using namespace CocosDenshion;

Scene* GameOver::createScene() {
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);
	return scene;
}

bool GameOver::init() {
	if (!Layer::init()) {
		return false;
	}

	TTFConfig config("fonts/yuehei.otf", 48);

	// 添加背景图片
	auto sprite = Sprite::create("scene_end.png");
	sprite->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	this->addChild(sprite);
	sprite->setTag(E_TAG::BG);

	// 添加重新开始按钮
	auto restartItem = MenuItemImage::create(
		"btn_restart01.png",
		"btn_restart02.png",
		CC_CALLBACK_1(GameOver::menuRestartCallback, this));

	restartItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 6));

	// 添加返回主菜单按钮
	auto backItem = MenuItemImage::create(
		"btn_back01.png",
		"btn_back02.png",
		CC_CALLBACK_1(GameOver::menuBackCallback, this));

	backItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, backItem->getContentSize().height / 2));


	// 添加退出游戏按钮
	auto exitItem = MenuItemImage::create(
		"btn_exit01.png",
		"btn_exit02.png",
		CC_CALLBACK_1(GameOver::menuExitCallback, this));
	exitItem->setPosition(Vec2(GAME_SCREEN_WIDTH - exitItem->getContentSize().width / 2, exitItem->getContentSize().height / 2));

	auto menu = Menu::create(restartItem, backItem, exitItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	// 分数
	auto labelScore = Label::createWithTTF(config, "  0  ");
	labelScore->setPosition(Point(GAME_SCREEN_WIDTH / 2.0f, GAME_SCREEN_HEIGHT / 1.4f));
	labelScore->setTag(E_TAG::SC);
	this->addChild(labelScore);
	if (SHARED_USERDEFAULT->getIntegerForKey(KEY_SCORE) == 0) {
		SHARED_USERDEFAULT->setIntegerForKey(KEY_SCORE, 0);
	}

	// http 显示最高分
	auto highest = Label::createWithTTF(config, "---");
	highest->setPosition(Point(GAME_SCREEN_WIDTH * 0.95f, GAME_SCREEN_HEIGHT * 0.5f));
	highest->setTag(E_TAG::HIGHEST);
	highest->setAnchorPoint(Vec2(1.f, 0.5f));
	highest->setColor(Color3B::RED);
	this->addChild(highest);

	// http 昵称
	auto nickname = Label::create();
	nickname->setString("---");
	nickname->setPosition(Point(GAME_SCREEN_WIDTH * 0.05f, GAME_SCREEN_HEIGHT * 0.5f));
	nickname->setAnchorPoint(Vec2(0.f, 0.5f));
	nickname->setTag(E_TAG::NICKNAME);
	nickname->setScale(2.f);
	nickname->setColor(Color3B::RED);
	this->addChild(nickname);


	return true;
}

void GameOver::menuRestartCallback(Ref *pSender) {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionPageTurn::create(1.0f, scene, false));
}

void GameOver::menuBackCallback(Ref *pSender) {
	auto scene = WelcomeScene::createScene();
	Director::getInstance()->replaceScene(TransitionPageTurn::create(1.0f, scene, false));
}



void GameOver::menuExitCallback(Ref *pSender) {
	Director::getInstance()->end();
}

void GameOver::setScore(int sc) {
	dynamic_cast<Label *>(this->getChildByTag(E_TAG::SC))->setString(StringUtils::format(" %u ", sc));

	auto orgi = SHARED_USERDEFAULT->getIntegerForKey(KEY_SCORE);

	if (sc > orgi) {

		// upload scores to the server
		uploadScore(sc);

		SHARED_USERDEFAULT->setIntegerForKey(KEY_SCORE, sc);

		SHARED_AUDIO->playEffect(MUSIC_WIN, false);

		// 新纪录
		auto newRecord = Sprite::create("sprite_newRecord.png");
		newRecord->setPosition(Point(GAME_SCREEN_WIDTH / 3.05f, GAME_SCREEN_HEIGHT / 1.22f));
		newRecord->setScale(10.0f);// start size
		newRecord->runAction(ScaleTo::create(1.2f, 1.0));

		this->addChild(newRecord);
	}
	else {
		SHARED_AUDIO->playEffect(MUSIC_FAI, false);
	}


}


void GameOver::uploadScore(size_t score) {
	//  upload
	MyConnector::getInstance()->upScore(score);
}


void GameOver::showHighestScore(float dt) {
	// download
	if (MyConnector::getInstance()->getHighestScoreFromServer(
															  SHARED_USERINFO->getInfo(User::UID), 
															  SHARED_USERINFO->getInfo(User::PWD))) {
		// there is an error
		if (!JSON::isJSON()) {
			dynamic_cast<Label *>(this->getChildByTag(E_TAG::HIGHEST))->setString(StringUtils::toString(JSON::getString()));
		}
		
		// good job
		auto score = JSON::getScore();
		dynamic_cast<Label *>(this->getChildByTag(E_TAG::HIGHEST))->setString(StringUtils::toString(score));
		auto nickname = JSON::getNickname();
		dynamic_cast<Label *>(this->getChildByTag(E_TAG::NICKNAME))->setString(nickname);

	}
}

// show the socre and nickname after the `TransitionDidFinish`
void GameOver::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();

	scheduleOnce(CC_SCHEDULE_SELECTOR(GameOver::showHighestScore), 1.5f);
}