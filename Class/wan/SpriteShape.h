#ifndef  _Inkmoo_SpriteShape_h_
#define  _Inkmoo_SpriteShape_h_

#include "cocos2d.h"
#include "GameDefine.h"

USING_NS_CC;

class SpriteShape : public Sprite
{
public:
	/***** һЩ��ʼ�� *****/
	SpriteShape();
	static SpriteShape* create( int row , int col );
    
	/***** ���ߺ��� *****/
	// �趨����״̬
	void setDisplayMode(DisplayMode mode);
	

	/***** �趨���� *****/
	CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int, m_col, Col);
    CC_SYNTHESIZE(int, m_imgIndex, ImgIndex);
	CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
	CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck);
    CC_SYNTHESIZE_READONLY(DisplayMode, m_displayMode, DisplayMode);
    
};


#endif