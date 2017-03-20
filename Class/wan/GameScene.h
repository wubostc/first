#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <cocos2d.h>
#include "GameDefine.h"

USING_NS_CC;

class SpriteShape;

class GameScene : public Layer
{
public:

    /****** 一些初始化 ******/
	GameScene();
	static Scene* createScene();
	virtual bool init();  
	// 初始化地图
	void initMap();
	// 创建精灵
	void createSprite( int row , int col );
	CREATE_FUNC(GameScene);
	virtual void onEnterTransitionDidFinish();
	//virtual void cleanup();

	void onExitTransitionDidStart();

	/***** 一些按钮 ******/
	// 返回欢迎界面函数
	void menuBackCallback( Ref* pSender );
	

	
	/***** 一些工具函数 *****/
	// 得到对应行列精灵的坐标值
	Vec2 positionOfItem( int row , int col );
	// 更新函数，每帧执行
	void update( float t );
	// 根据触摸的点位置，返回是地图中哪个精灵
	SpriteShape* spriteOfPoint(Point *point);
	// 倒计时的实现
	void myClock( float t );
	// 游戏结束
	void gameOver( float t );



	/***** 检查移除填补相关 *****/
	// 检测是否有可消除精灵
	void checkAndRemoveSprite();
	// 标记可以移除的精灵
	void markRemove( SpriteShape* spr );
	// 移除精灵
	void removeSprite();
	
	// 精灵的爆炸移除
	void explodeSprite( SpriteShape* spr );
	// 精灵的横向消除
	void explodeSpecialH(Point point);
	// 精灵的纵向消除

    void explodeSpecialV(Point point);
	// 对移除的精灵进行的操作
	void actionEndCallback(Node *node);
	// 纵向检查
	void getColChain(SpriteShape *spr, std::list<SpriteShape *> &chainList);
	// 横向检查
	void getRowChain(SpriteShape *spr, std::list<SpriteShape *> &chainList);
	// 填补空缺
	void fillSprite();



	/***** 触摸事件 *****/
	bool onTouchBegan(Touch *touch, Event *unused);
    void onTouchMoved(Touch *touch, Event *unused);
	// 交换精灵
	void swapSprite();

private:

	/***** 全局重要变量 *****/
	// 地图二维数组
	SpriteShape* map[ROWS][COLS];
	// 源精灵，目标精灵
	SpriteShape* staSprite;
	SpriteShape* endSprite;
	// 时间
	int m_time;
	// 分数
	int m_score;



	/***** 工具变量 *****/
	// 绘制一批图片
	SpriteBatchNode* spriteSheet;
	// 绘图的最左下角坐标位置
	float mapLBX,mapLBY;
	
private:

	/***** 标志变量 ****/
	// 标志 是否在执行动作
	bool isAction;
	// 标志 是否有空缺需要填补
    bool isFillSprite;
	// 标志 是否可以触摸
	bool isTouchEna;
	// 标志 产生的四消精灵是否为横向
	bool isRow;

};

#endif // __GAME_SCENE_H__