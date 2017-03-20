#include "CCPHPMYSQL.h"
#include "User.h"
#include "Connector.h"
#include "Utils.h"
#include "JSON.h"
#include "wan/WelcomeScene.h"
#include "Regex.h"
#include "wan/GameDefine.h"

using namespace cocos2d;
using namespace cocos2d::ui;

CCPHPMYSQL::CCPHPMYSQL() {
}


CCPHPMYSQL::~CCPHPMYSQL() {
}


Scene *CCPHPMYSQL::createScene() {
	auto scene = Scene::create();
	auto layer = CCPHPMYSQL::create();
	scene->addChild(layer);
	return scene;
}

bool CCPHPMYSQL::init() {

	if (!cocos2d::Layer::init()) {
		return false;
	}

	visible_size = Director::getInstance()->getVisibleSize();

	status = Label::create();
	status->setTextColor(Color4B::ORANGE);
	this->addChild(status);
	status->setPosition(Vec2(visible_size.width / 2, visible_size.height * 0.9));
	status->setString("---");
	status->setScale(2.0f);
	status->setTag(E_TAG::LAB_STATUS);

	tf_uid = ui::TextField::create("your name or phone", "fonts/Marker Felt.ttf", 30);
	tf_uid->addEventListener(CC_CALLBACK_2(CCPHPMYSQL::getTextContent, this));
	tf_uid->setTouchEnabled(true);
	tf_uid->setPosition(Vec2(
							 visible_size.width / 2,
							 visible_size.height * 0.7)
							 );
	this->addChild(tf_uid);
	tf_uid->setMaxLengthEnabled(true);
	tf_uid->setMaxLength(11);
	tf_uid->setTag(E_TAG::TXT_UID);
	tf_uid->setContentSize(Size(100, 35));
	

	tf_pwd = ui::TextField::create("your password", "fonts/Marker Felt.ttf", 30);
	tf_pwd->addEventListener(CC_CALLBACK_2(CCPHPMYSQL::getTextContent, this));
	tf_pwd->setTouchEnabled(true);
	tf_pwd->setPosition(Vec2(
							 visible_size.width/2,
							 tf_uid->getPositionY() - tf_uid->getContentSize().height - 20)
							 );
	this->addChild(tf_pwd);
	tf_pwd->setMaxLength(18);
	tf_pwd->setMaxLengthEnabled(true);
	tf_pwd->setTag(E_TAG::TXT_PWD);
	tf_uid->setContentSize(Size(100, 35));


	Button* btn_signOn = Button::create("btn_A1.png",
										"btn_A2.png");
	btn_signOn->setPosition(Vec2(
								 visible_size.width / 2 - btn_signOn->getContentSize().width / 2,
								 tf_pwd->getPositionY() - btn_signOn->getContentSize().height - 20));
	btn_signOn->setPressedActionEnabled(true);
	btn_signOn->addTouchEventListener(CC_CALLBACK_2(CCPHPMYSQL::onHttpRequestCompleted, this));
	btn_signOn->setTitleText("sing on");
	this->addChild(btn_signOn);
	btn_signOn->setZoomScale(0.1f);
	btn_signOn->setTag(BTN_SIGN_ON);

	Button* btn_login = Button::create("btn_A1.png",
									   "btn_A2.png");
	btn_login->setPosition(Vec2(
							    visible_size.width / 2 + btn_login->getContentSize().width / 2,
								tf_pwd->getPositionY() - btn_login->getContentSize().height - 20));
	btn_login->setPressedActionEnabled(true);
	btn_login->addTouchEventListener(CC_CALLBACK_2(CCPHPMYSQL::onHttpRequestCompleted, this));
	btn_login->setTitleText("login");
	this->addChild(btn_login);
	btn_login->setZoomScale(0.1f);
	btn_login->setTag(BTN_LOGIN);

	return true;
}


void CCPHPMYSQL::onHttpRequestCompleted(Ref *sender, Widget::TouchEventType type) {

	// event
	if (type == Widget::TouchEventType::ENDED) {

		std::string uid(tf_uid->getString());
		std::string pwd(tf_pwd->getString());
		
		// regex
		if (Regex::verifyUID(uid.c_str()) == Regex::UNKNOW) {
			char *p = nullptr;
			dynamic_cast<Label *>(this->getChildByTag(E_TAG::LAB_STATUS))->setString("username incorret");
#if (DEBUG_FIRST == 1)
			CCLOG("username incorrect");
#endif
			return;
		}
		// regex
		if (Regex::verifyPWD(pwd.c_str()) == Regex::UNKNOW) {
			dynamic_cast<Label *>(this->getChildByTag(E_TAG::LAB_STATUS))->setString("password incorrect");
#if (DEBUG_FIRST == 1)
			CCLOG("password incorrect");
#endif
			return;
		}

		// http post
		bool f = false;
		auto tag = dynamic_cast<Button *>(sender)->getTag();
		if (tag == BTN_SIGN_ON) {
			f = MyConnector::getInstance()->signOn(uid, pwd);
		}
		else if (tag == BTN_LOGIN) {
			f = MyConnector::getInstance()->login(uid, pwd);
		}

		// ok
		if (f) {
			
			// Oh, it has an error...
			if (!JSON::isJSON()) {
				dynamic_cast<Label *>(this->getChildByTag(E_TAG::LAB_STATUS))->setString(JSON::getString());
				return;
			}

			// That's good
			int code = JSON::getCode();
			if (code == CODE_SUCCESS) {

				// verify both
				if (FileQ::read(KEY_USERINFO_UID) != uid) {
					SHARED_USERDEFAULT->setIntegerForKey(KEY_SCORE, 0);
				}
				SHARED_USERINFO->setInfo(uid, TYPE_INFO::UID);
				SHARED_USERINFO->setInfo(pwd, TYPE_INFO::PWD);
				FileQ::wirte(uid.c_str(), pwd.c_str());
				Director::getInstance()->replaceScene(TransitionFlipAngular::create(1.f, WelcomeScene::createScene()));
				return;
			}
			else if (code == CODE_FAILED) {
				auto lable_status = dynamic_cast<Label *>(this->getChildByTag(E_TAG::LAB_STATUS));
				std::string msg = JSON::getMessage();
				if (!msg.empty()) {
					lable_status->setString(JSON::getMessage());
#if (DEBUG_FIRST == 1)
					CCLOG("%s%s", MSG_FAILED.c_str(), JSON::getMessage());
#endif
				}
				return;
			}
			else {
				auto lable_status = dynamic_cast<Label *>(this->getChildByTag(E_TAG::LAB_STATUS));
				lable_status->setString(JSON::getMessage());
#if (DEBUG_FIRST == 1)
				CCLOG("%s%d", MSG_FATAL.c_str(), code);
#endif
				return;
			}
		}
		
		else {
			dynamic_cast<Label *>(this->getChildByTag(E_TAG::LAB_STATUS))->setString(MSG_FATAL + "please check your network");
			return;
		}
	}
}

void CCPHPMYSQL::getTextContent(Ref *sender, ui::TextField::EventType type) {
	
	//if (type == ui::TextField::EventType::DETACH_WITH_IME) {
	//	auto str = dynamic_cast<ui::TextField *>(sender)->getString();
	//}

}

void CCPHPMYSQL::onEnter() {
	Layer::onEnter();

	// setting the string before the application starts
	dynamic_cast<TextField *>(this->getChildByTag(E_TAG::TXT_UID))->setString(FileQ::read(KEY_USERINFO_UID));
	dynamic_cast<TextField *>(this->getChildByTag(E_TAG::TXT_PWD))->setString(FileQ::read(KEY_USERINFO_PWD));
}

