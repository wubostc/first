#ifndef __CCPHPMYSQL_H__
#define __CCPHPMYSQL_H__
#include <cocos2d.h>
#include <ui/CocosGUI.h>



class CCPHPMYSQL : public cocos2d::Layer
{
public:
	typedef enum {
		BTN_SIGN_ON = 1000,
		BTN_LOGIN,
		TXT_UID,
		TXT_PWD,
		LAB_STATUS
	} E_TAG;
public:
	CCPHPMYSQL();
	~CCPHPMYSQL();

	static cocos2d::Scene *createScene();

	bool init();

	CREATE_FUNC(CCPHPMYSQL);

	void onHttpRequestCompleted(Ref *sender,cocos2d::ui::Widget::TouchEventType type);

	void getTextContent(cocos2d::Ref *, cocos2d::ui::TextField::EventType);

	void onEnter();

private:
	cocos2d::Label *status;
	cocos2d::Size visible_size;

	cocos2d::ui::TextField *tf_uid;
	cocos2d::ui::TextField *tf_pwd;
};

#endif //__CCPHPMYSQL_H__