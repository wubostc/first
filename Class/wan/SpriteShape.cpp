#include "SpriteShape.h"

SpriteShape::SpriteShape()
: m_col(0)
, m_row(0)
, m_imgIndex(0)
, m_isNeedRemove(false)
{
}

SpriteShape *SpriteShape::create(int row, int col)
{
	SpriteShape *spr = new SpriteShape();
	spr->m_row = row;
	spr->m_col = col;
	//spr->m_imgIndex = rand() % TOTAL_SPRITE; 
	spr->m_imgIndex = random(1, TOTAL_SPRITE); 
    spr->initWithSpriteFrameName(spriteNormal[spr->m_imgIndex]);
	spr->autorelease();
	return spr;
}

// ÉèÖÃ¾«Áé×´Ì¬
void SpriteShape::setDisplayMode(DisplayMode mode)
{
	m_displayMode = mode;
    
    SpriteFrame *frame;
    switch (mode) {
        case DISPLAY_MODE_VERTICAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sprVertical[m_imgIndex]);
            break;
        case DISPLAY_MODE_HORIZONTAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sprHorizontal[m_imgIndex]);
            break;
        default:
            return;
    }
    //setDisplayFrame(frame);
	setSpriteFrame(frame);
}