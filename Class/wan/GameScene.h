#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <cocos2d.h>
#include "GameDefine.h"

USING_NS_CC;

class SpriteShape;

class GameScene : public Layer
{
public:

    /****** һЩ��ʼ�� ******/
	GameScene();
	static Scene* createScene();
	virtual bool init();  
	// ��ʼ����ͼ
	void initMap();
	// ��������
	void createSprite( int row , int col );
	CREATE_FUNC(GameScene);
	virtual void onEnterTransitionDidFinish();
	//virtual void cleanup();

	void onExitTransitionDidStart();

	/***** һЩ��ť ******/
	// ���ػ�ӭ���溯��
	void menuBackCallback( Ref* pSender );
	

	
	/***** һЩ���ߺ��� *****/
	// �õ���Ӧ���о��������ֵ
	Vec2 positionOfItem( int row , int col );
	// ���º�����ÿִ֡��
	void update( float t );
	// ���ݴ����ĵ�λ�ã������ǵ�ͼ���ĸ�����
	SpriteShape* spriteOfPoint(Point *point);
	// ����ʱ��ʵ��
	void myClock( float t );
	// ��Ϸ����
	void gameOver( float t );



	/***** ����Ƴ����� *****/
	// ����Ƿ��п���������
	void checkAndRemoveSprite();
	// ��ǿ����Ƴ��ľ���
	void markRemove( SpriteShape* spr );
	// �Ƴ�����
	void removeSprite();
	
	// ����ı�ը�Ƴ�
	void explodeSprite( SpriteShape* spr );
	// ����ĺ�������
	void explodeSpecialH(Point point);
	// �������������

    void explodeSpecialV(Point point);
	// ���Ƴ��ľ�����еĲ���
	void actionEndCallback(Node *node);
	// ������
	void getColChain(SpriteShape *spr, std::list<SpriteShape *> &chainList);
	// ������
	void getRowChain(SpriteShape *spr, std::list<SpriteShape *> &chainList);
	// ���ȱ
	void fillSprite();



	/***** �����¼� *****/
	bool onTouchBegan(Touch *touch, Event *unused);
    void onTouchMoved(Touch *touch, Event *unused);
	// ��������
	void swapSprite();

private:

	/***** ȫ����Ҫ���� *****/
	// ��ͼ��ά����
	SpriteShape* map[ROWS][COLS];
	// Դ���飬Ŀ�꾫��
	SpriteShape* staSprite;
	SpriteShape* endSprite;
	// ʱ��
	int m_time;
	// ����
	int m_score;



	/***** ���߱��� *****/
	// ����һ��ͼƬ
	SpriteBatchNode* spriteSheet;
	// ��ͼ�������½�����λ��
	float mapLBX,mapLBY;
	
private:

	/***** ��־���� ****/
	// ��־ �Ƿ���ִ�ж���
	bool isAction;
	// ��־ �Ƿ��п�ȱ��Ҫ�
    bool isFillSprite;
	// ��־ �Ƿ���Դ���
	bool isTouchEna;
	// ��־ ���������������Ƿ�Ϊ����
	bool isRow;

};

#endif // __GAME_SCENE_H__