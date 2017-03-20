#include "GameScene.h"
#include "SpriteShape.h"
#include "WelcomeScene.h"
#include "GameOverScene.h"

using namespace CocosDenshion;

GameScene::GameScene()
: spriteSheet(NULL)
, isFillSprite(false)
, isAction(true)
, isTouchEna(true)
, staSprite(NULL)
, endSprite(NULL)
, m_time(10)
, m_score(0)
{
}

Scene* GameScene::createScene()	{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
	return scene;
}

// 欢迎界面 初始化函数
bool GameScene::init()	{
	// 先初始化父类，不成功返回false
	if( !Layer::init() )	{
		return false;
	}


	// 加载plist和png
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_1);
    spriteSheet = SpriteBatchNode::create(ICON_1);
    this->addChild(spriteSheet);
	
	// 初始化坐标值
	mapLBX = (GAME_SCREEN_WIDTH - SPRITE_WIDTH * COLS - (COLS - 1) * BOADER_WIDTH) / 2;
	mapLBY = (GAME_SCREEN_HEIGHT - SPRITE_WIDTH * ROWS - (ROWS - 1) * BOADER_WIDTH) / 2;

	// 添加背景图片
	auto bg = Sprite::create(SCENE_BG1);
	bg->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	this->addChild(bg, -1);

	
	// 添加返回按钮
	auto back_item = MenuItemImage::create(
                                           BTN_BACK_1A,
                                           BTN_BACK_1B,
										   CC_CALLBACK_1(GameScene::menuBackCallback, this));
	back_item->setPosition(Vec2(GAME_SCREEN_WIDTH - back_item->getContentSize().width / 2, back_item->getContentSize().height / 2));

	auto menu = Menu::create(back_item, NULL);
    menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	
	// 加载ttf字体
	TTFConfig config("fonts/haibaoti.ttf",30);

	// 游戏时间的显示
	auto labelTime = Label::createWithTTF(config,StringUtils::format("Time: %d ",m_time));
	labelTime -> setPosition(Vec2( labelTime->getContentSize().width , GAME_SCREEN_HEIGHT - labelTime->getContentSize().height ));
	labelTime -> setTag(TAG_3);
	this ->addChild(labelTime);

	// 分数显示
	auto labelScore = Label::createWithTTF(config,StringUtils::format("Score: %d ",m_score) );
	labelScore -> setPosition(Vec2( GAME_SCREEN_WIDTH/2 , GAME_SCREEN_HEIGHT - labelScore->getContentSize().height*2.6 ));
	labelScore->setTag(TAG_1);
	this->addChild(labelScore);

	// 不存在为0
	if(SHARED_USERDEFAULT->getIntegerForKey(KEY_SCORE) == 0){
		SHARED_USERDEFAULT->setIntegerForKey(KEY_SCORE, 0);
	}

	// 最高分
	auto labelHScore = Label::createWithTTF(config, "Highest: 0");
	labelHScore->setPosition(Vec2(
								  GAME_SCREEN_WIDTH - labelHScore->getContentSize().width, 
								  GAME_SCREEN_HEIGHT - labelHScore->getContentSize().height));
	labelHScore->setString(StringUtils::format("Highest: %d ", SHARED_USERDEFAULT->getIntegerForKey(KEY_SCORE)));
	this->addChild(labelHScore);

	// 触摸事件处理
    auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

// 返回函数，跳转到欢迎界面
void GameScene::menuBackCallback( Ref* pSender )	{
	auto scene = WelcomeScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

// 初始化地图
void GameScene::initMap() {
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			this->createSprite(r, c);
		}
	}
}


// 创建精灵
void GameScene::createSprite( int row , int col )	{
	
	// 先创建一个寿司
	SpriteShape* spr = SpriteShape::create(row, col);
	
	// 创建下落动画
	Vec2 endPosition = positionOfItem(row, col);
	Vec2 startPosition = Vec2(endPosition.x, endPosition.y + GAME_SCREEN_HEIGHT / 2);
    spr->setPosition(startPosition);
	float speed = startPosition.y / (1.5 * GAME_SCREEN_HEIGHT );
    spr->runAction(MoveTo::create(speed, endPosition));

    spriteSheet -> addChild(spr);

	// 数组相应位置，置上寿司对象
    map[row][col] = spr;
}

// 根据行列，获取坐标值
Vec2 GameScene::positionOfItem(int row , int col)
{
	float x = mapLBX + (SPRITE_WIDTH + BOADER_WIDTH) * col + SPRITE_WIDTH / 2;
    float y = mapLBY + (SPRITE_WIDTH + BOADER_WIDTH) * row + SPRITE_WIDTH / 2;
    return Vec2(x, y);
}

// 更新函数，每帧都执行
void GameScene::update(float t)
{
	if (m_time == 0) {
		return;
	}

	 // 检测是否在执行动作
	if ( isAction ) {
        // 设置为false
        isAction = false;
		// 扫描一遍所有精灵，看有没有可以消除的
		for( int r = 0 ; r < ROWS ; ++r )	{
			for( int c = 0  ; c < COLS ; ++c )	{
				SpriteShape* spr = map[r][c];
				if (spr && spr->getNumberOfRunningActions() > 0) {
					isAction = true;
					break;
				}
			}
		}
	}

	// 如果精灵正在移动中，忽视触摸事件
	isTouchEna = !isAction;

	// 如果没有动作执行    
    if (!isAction) {
		// 是否有精灵需要填补
		if ( isFillSprite ) {
            //爆炸效果结束后才掉落新寿司，增强打击感
            fillSprite();
            isFillSprite = false;
		}
		else
		{
			checkAndRemoveSprite();
		}
    }

	// 分数变化
	Label *labelScore = (Label *)this->getChildByTag(TAG_1);
	labelScore -> setString( StringUtils::format("Score: %d ",m_score));
}

// 检测是否有精灵可以移除
void GameScene::checkAndRemoveSprite()
{
	SpriteShape *spr;
    // 设定寿司的忽视检查，之前可能有精灵设置忽视检查，但这次检查要将之前所有的检查都不能忽视
	for( int r = 0 ; r < ROWS ; ++r )	{
		for( int c = 0 ; c < COLS ; ++c )	{
			spr = map[r][c];
			if( !spr )	{
				continue;
			}
			spr -> setIgnoreCheck(false);
		}
	}


	for( int r = 0 ; r < ROWS ; ++r )	{
		for( int c = 0 ; c < COLS ; ++c )	{
			spr = map[r][c];
			// 如果该位置没有精灵
			if( !spr )	{
				continue;
			}
			// 如果该精灵需要被移除
			if( spr -> getIsNeedRemove() )	{
				continue;
			}
			// 如果该精灵是新生成的精灵
			if ( spr -> getIgnoreCheck() ) {
				continue;
			}

			// 纵向相同精灵List
			std::list< SpriteShape *> colChainList;
			getColChain( spr , colChainList );
			// 横向相同精灵List
			std::list< SpriteShape *> rowChainList;
			getRowChain( spr , rowChainList );

			std::list< SpriteShape *> longerList;
			if( colChainList.size() >= rowChainList.size() )	{
				if( colChainList.size() < 3 )	{
					continue;
				}
				longerList = colChainList;
				isRow = false;
			}
			else if( rowChainList.size() > colChainList.size() )	{
				if( rowChainList.size() < 3 )	{
					continue;
				}
				longerList = rowChainList;
				isRow = true;
			}

			std::list<SpriteShape *>::iterator itList;
			// 标志 是否需要设定忽视检查的精灵
			bool isSetedIgnoreCheck = false;

			for( itList = longerList.begin() ; itList != longerList.end() ; ++itList ) {
				spr = ( SpriteShape * )* itList;
				if( !spr )	{
					continue;
				}

				if( longerList.size() > 3 )	{
					if ( spr == staSprite || spr == endSprite ) {
						isSetedIgnoreCheck = true;
						spr->setIgnoreCheck(true);
						spr->setIsNeedRemove(false);
						spr->setDisplayMode( isRow ? DISPLAY_MODE_HORIZONTAL :DISPLAY_MODE_VERTICAL );
						continue;
					}
				}
				markRemove( spr );
			}   
			// 如何是自由掉落产生的4消, 取最后一个变化为特殊精灵
			if (!isSetedIgnoreCheck && longerList.size() > 3) {
				spr -> setIgnoreCheck(true);
				spr -> setIsNeedRemove(false);
				spr->setDisplayMode( isRow ? DISPLAY_MODE_HORIZONTAL :DISPLAY_MODE_VERTICAL );
			}
		}
	}

	// 消除标记了的精灵
	removeSprite();
}

// 标记可以移除的精灵
void GameScene::markRemove( SpriteShape* spr )	{

	// 如果已经标记了要移除，就不需要再标记
	if ( spr -> getIsNeedRemove()) {
        return;
    }
	// 如果该精灵被忽视，不需要标记
	if ( spr -> getIgnoreCheck() ) {
        return;
    }
   
	// 先标记自己
    spr -> setIsNeedRemove(true);
    // 检查需要标记的精灵是否为 四消特殊精灵
    if ( spr ->getDisplayMode() == DISPLAY_MODE_VERTICAL) {
        for ( int r = 0; r < ROWS ; ++r ) {
			SpriteShape* tmp = map[r][spr->getCol()];
            if ( !tmp || tmp == spr ) {
                continue;
            }
            
            if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
                tmp->setIsNeedRemove(true);
            } else {
                markRemove(tmp);
            }
        }
    } else if ( spr -> getDisplayMode() == DISPLAY_MODE_HORIZONTAL) {
        for (int c = 0; c < COLS ; ++c ) {
            SpriteShape *tmp = map[ spr -> getRow()][c];
            if (!tmp || tmp == spr) {
                continue;
            }
            
            if (tmp->getDisplayMode() == DISPLAY_MODE_NORMAL) {
                tmp->setIsNeedRemove(true);
            } else {
                markRemove(tmp);
            }
        }
    }
}

// 移除精灵
void GameScene::removeSprite()
{
	// 做一套移除的动作
    isAction = true;
    
	for( int r = 0 ; r < ROWS ; ++r )	{
		for( int c = 0 ; c < COLS ; ++c )	{
			SpriteShape* spr = map[r][c];
			if( !spr )	{
				continue;
			}

			if( spr -> getIsNeedRemove() )	{
				isFillSprite = true;

				if( spr -> getDisplayMode() == DISPLAY_MODE_HORIZONTAL)
				{
					explodeSpecialH( spr -> getPosition() );
				}
				else if ( spr -> getDisplayMode() == DISPLAY_MODE_VERTICAL)
				{
					explodeSpecialV( spr -> getPosition() );
				}
				explodeSprite( spr );
			}
		}
	}
}

// 精灵的爆炸移除
void GameScene::explodeSprite( SpriteShape* spr )	{
	
	SHARED_AUDIO->playEffect(MUSIC_EXPLOAD, false);

	float time = 0.2f;
	// 精灵的动作
    spr->runAction(Sequence::create(
                                      ScaleTo::create(time, 0.0),
									  CallFuncN::create(CC_CALLBACK_1(GameScene::actionEndCallback, this)),
                                      NULL));

	// 爆炸效果 圆圈的动作
    auto circleSprite = Sprite::create("circle.png");
	addChild(circleSprite, TAG_1);
	circleSprite->setPosition(spr->getPosition());
	circleSprite->setScale(0);// start size
    circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
                                             NULL));
}

// 精灵的横向消除
void GameScene::explodeSpecialH(Point point)
{
	                    
    SimpleAudioEngine::getInstance()->playEffect(MUSIC_EXPLOAD_ONE,false);  

	// 先设置相应的变量
    float scaleX = 4.0f;
    float scaleY = 0.7f;
    float time = 0.3f;
    Point startPosition = point;
    float speed = 0.6f;
    
    auto colorSpriteRight = Sprite::create("colorHRight.png");
	addChild(colorSpriteRight, TAG_1);
	Point endPosition1 = Point(point.x - GAME_SCREEN_WIDTH, point.y);
    colorSpriteRight->setPosition(startPosition);
    colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition1),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
                                             NULL));
    
    auto colorSpriteLeft = Sprite::create("colorHLeft.png");
	addChild(colorSpriteLeft, TAG_1);
    Point endPosition2 = Point(point.x + GAME_SCREEN_WIDTH, point.y);
    colorSpriteLeft->setPosition(startPosition);
    colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition2),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
                                             NULL));
    
	
}

// 精灵的纵向消除
void GameScene::explodeSpecialV(Point point)
{
	SimpleAudioEngine::getInstance()->playEffect(MUSIC_EXPLOAD_ONE, false);

    float scaleY = 4.0f ;
    float scaleX = 0.7f ;
    float time = 0.3f;
    Point startPosition = point;
    float speed = 0.6f;

    auto colorSpriteDown = Sprite::create("colorVDown.png");
	addChild(colorSpriteDown, 10);
	Point endPosition1 = Point(point.x , point.y - GAME_SCREEN_HEIGHT);
    colorSpriteDown->setPosition(startPosition);
    colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition1),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
                                             NULL));
    
    auto colorSpriteUp = Sprite::create("colorVUp.png");
	addChild(colorSpriteUp, 10);
    Point endPosition2 = Point(point.x , point.y + GAME_SCREEN_HEIGHT);
    colorSpriteUp->setPosition(startPosition);
    colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition2),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
                                             NULL));

	
}

// 对移除的精灵进行的操作
void GameScene::actionEndCallback(Node *node)	{
	SpriteShape *spr = (SpriteShape *)node;
	map[spr->getRow()][spr->getCol()] = NULL;
	spr -> removeFromParent();
}

// 纵向检查
void GameScene::getColChain(SpriteShape *spr, std::list<SpriteShape *> &chainList)	{
	// 添加第一个精灵（自己）
    chainList.push_back(spr);
    
	auto curr_row = spr->getRow();

	// 向左查找
    int neighborCol = spr->getCol() - 1;
    while (neighborCol >= 0) {
		SpriteShape *neighborSprite = map[curr_row][neighborCol];
        if (neighborSprite
            && (neighborSprite->getImgIndex() == spr->getImgIndex())
            && !neighborSprite->getIsNeedRemove()) {
            chainList.push_back(neighborSprite);
            neighborCol--;
        } else {
            break;
        }
    }
    
	// 向右查找
    neighborCol = spr->getCol() + 1;
    while (neighborCol < COLS) {
		SpriteShape *neighborSprite = map[curr_row][neighborCol];
		if (neighborSprite
            && (neighborSprite->getImgIndex() == spr->getImgIndex())
            && !neighborSprite->getIsNeedRemove()) {
            chainList.push_back(neighborSprite);
            neighborCol++;
        } else {
            break;
        }
    }
}

// 横向检查
void GameScene::getRowChain(SpriteShape *spr, std::list<SpriteShape *> &chainList)	{
	// 先将第一个精灵加入进去
	chainList.push_back(spr);
    
	auto curr_col = spr->getCol();

	// 向上查找
    int neighborRow = spr->getRow() - 1;
    while (neighborRow >= 0) {
		SpriteShape *neighborSprite = map[neighborRow][curr_col];
        if (neighborSprite
            && (neighborSprite->getImgIndex() == spr->getImgIndex())
            && !neighborSprite->getIsNeedRemove()) {
            chainList.push_back(neighborSprite);
            neighborRow--;
        } else {
            break;
        }
    }
    
	// 向下查找
    neighborRow = spr->getRow() + 1;
    while (neighborRow < ROWS) {
		SpriteShape *neighborSprite = map[neighborRow][curr_col];
        if (neighborSprite
            && (neighborSprite->getImgIndex() == spr->getImgIndex())
            && !neighborSprite->getIsNeedRemove()) {
            chainList.push_back(neighborSprite);
            neighborRow++;
        } else {
            break;
        }
    }
}

// 填补空缺位置
void GameScene::fillSprite()	{

	// 下落精灵总个数（方便后面分数的计算）
	int sum = 0;
	 // 重置移动方向标志
    isAction = true;

    int *colEmptyInfo = (int *)malloc(sizeof(int) * COLS);
    memset((void *)colEmptyInfo, 0, sizeof(int) * COLS);
    
    // 将存在的精灵降落下来
	SpriteShape *spr = NULL;
    for (int c = 0; c < COLS; c++) {
        int removedSpriteOfCol = 0;
        // 自底向上
        for (int r = 0; r < ROWS; r++ ) {
            spr = map[r][c];
            if ( spr == NULL ) {
                ++removedSpriteOfCol;
            } else {
                if ( removedSpriteOfCol > 0) {
                    int newRow = r - removedSpriteOfCol;
                    map[newRow][c] = spr;
                    map[r][c] = NULL;
                  
                    Vec2 startPosition = spr->getPosition();
                    Vec2 endPosition = positionOfItem(newRow, c);
					float speed = (startPosition.y - endPosition.y) / GAME_SCREEN_HEIGHT*3;
                    spr->stopAllActions();
                    spr->runAction(CCMoveTo::create(speed, endPosition));
                    
                    spr->setRow(newRow);
                }
            }
        }
        
        // 记录相应列数移除的数量
        colEmptyInfo[c] = removedSpriteOfCol;
		sum+=removedSpriteOfCol;
    }
    
    // 新建新的精灵，并降落
    for (int c = 0; c < COLS; ++c ) {
        for (int r = ROWS - colEmptyInfo[c]; r < ROWS ; ++r ) {
            createSprite(r,c);
        }
    }
    
	m_score += sum*30;
    free(colEmptyInfo);
}

// 开始触摸
bool GameScene::onTouchBegan(Touch *touch, Event *unused)	{
	// fixed:2016/2/19	
	if (m_time == 0) {
		return false;
	}
	
	staSprite = NULL;
	endSprite = NULL;

	if ( isTouchEna ) {
        auto location = touch->getLocation();
		staSprite = spriteOfPoint(&location);
    }
	return isTouchEna;
}

// 触摸后移动的方向
void GameScene::onTouchMoved(Touch *touch, Event *unused)	{
	// 如果没有初始精灵 或者 触摸事件不可行，直接返回
	if (!staSprite || !isTouchEna) {
        return;
    }
    
	// 获取 初始精灵 的行列
    int row = staSprite->getRow();
    int col = staSprite->getCol();
    
	// 获取移动到的 点 的位置
    auto location = touch->getLocation();
    auto halfSpriteWidth = SPRITE_WIDTH / 2;
    auto halfSpriteHeight = SPRITE_WIDTH / 2;
    
    auto  upRect = Rect(staSprite->getPositionX() - halfSpriteWidth,
                        staSprite->getPositionY() + halfSpriteHeight,
                        SPRITE_WIDTH,
                        SPRITE_WIDTH);
    
	// 判断是在向哪个方向移动，
    if (upRect.containsPoint(location)) {
        ++row;
        if ( row < ROWS ) {
            endSprite = map[row][col];
        }
        swapSprite();
        return;
    }
    
    auto  downRect = Rect(staSprite->getPositionX() - halfSpriteWidth,
                        staSprite->getPositionY() - (halfSpriteHeight * 3),
                        SPRITE_WIDTH,
                        SPRITE_WIDTH);
    
    if (downRect.containsPoint(location)) {
        --row;
        if ( row >= 0 ) {
            endSprite = map[row][col];
        }
        swapSprite();
        return;
    }
    
    auto  leftRect = Rect(staSprite->getPositionX() - (halfSpriteWidth * 3),
                          staSprite->getPositionY() - halfSpriteHeight,
                          SPRITE_WIDTH,
						  SPRITE_WIDTH);
    
    if (leftRect.containsPoint(location)) {
        --col;
        if ( col >= 0 ) {
            endSprite = map[row][col];
        }
        swapSprite();
        return;
    }
    
    auto  rightRect = Rect(staSprite->getPositionX() + halfSpriteWidth,
                          staSprite->getPositionY() - halfSpriteHeight,
                          SPRITE_WIDTH,
                          SPRITE_WIDTH);
    
    if (rightRect.containsPoint(location)) {
        ++col;
        if ( col < COLS ) {
			endSprite = map[row][col];
        }
        swapSprite();
        return;
    }
    
    // 否则，并非一个有效的移动
}

// 根据触摸的点位置，返回是地图中哪个精灵
SpriteShape *GameScene::spriteOfPoint(Point *point)
{
    SpriteShape *spr = NULL;
    Rect rect = Rect(0, 0, 0, 0);
	Size sz;
	sz.height=SPRITE_WIDTH;
	sz.width=SPRITE_WIDTH;

	for( int r = 0 ; r < ROWS ; ++r )	{
		for( int c = 0 ; c < COLS ; ++c )	{
			spr = map[r][c];
			if( spr )	{
				rect.origin.x = spr->getPositionX() - ( SPRITE_WIDTH / 2);
				rect.origin.y = spr->getPositionY() - ( SPRITE_WIDTH / 2);

				rect.size = sz;
				if (rect.containsPoint(*point)) {
					return spr;
				}
			}
		}
	}
    
    return NULL;
}

// 交换精灵
void GameScene::swapSprite()	{
	// 移动中，不允许再次触摸，执行动作设置为true
    isAction = true;
    isTouchEna = false;

	// 初始精灵 和 终止精灵 均不能为空
	if (!staSprite || !endSprite) {
        return;
    }
    
	Vec2 posOfSrc = staSprite->getPosition();
	Vec2 posOfDest = endSprite->getPosition();

    
    // 在数组中交换位置
	map[ staSprite -> getRow() ][staSprite -> getCol() ] = endSprite;
	map[ endSprite -> getRow() ][endSprite -> getCol() ] = staSprite;

    int tmpRow = staSprite->getRow();
    int tmpCol = staSprite->getCol();
    staSprite->setRow(endSprite->getRow());
    staSprite->setCol(endSprite->getCol());
    endSprite->setRow(tmpRow);
    endSprite->setCol(tmpCol);
    
    // 检查是否能消除
	std::list<SpriteShape *> colChainListOfFirst;
    getColChain(staSprite, colChainListOfFirst);
    
    std::list<SpriteShape *> rowChainListOfFirst;
    getRowChain(staSprite, rowChainListOfFirst);
    
    std::list<SpriteShape *> colChainListOfSecond;
    getColChain(endSprite, colChainListOfSecond);
    
    std::list<SpriteShape *> rowChainListOfSecond;
    getRowChain(endSprite, rowChainListOfSecond);
    

	float time = 0.2f;

    if (colChainListOfFirst.size() >= 3
        || rowChainListOfFirst.size() >= 3
        || colChainListOfSecond.size() >= 3
        || rowChainListOfSecond.size() >= 3) {
        // 如果能够消除，仅仅进行移动（不会移动回来）
        staSprite->runAction(MoveTo::create(time, posOfDest));
        endSprite->runAction(MoveTo::create(time, posOfSrc));
        return;
    }
    
    // 不能消除，则移动过去还要返回
	map[ staSprite -> getRow()][staSprite -> getCol() ] = endSprite;
	map[ endSprite -> getRow()][endSprite -> getCol() ] = staSprite;

    tmpRow = staSprite->getRow();
    tmpCol = staSprite->getCol();
    staSprite->setRow(endSprite->getRow());
    staSprite->setCol(endSprite->getCol());
    endSprite->setRow(tmpRow);
    endSprite->setCol(tmpCol);
    
    staSprite->runAction(Sequence::create(
                                      MoveTo::create(time, posOfDest),
                                      MoveTo::create(time, posOfSrc),
                                      NULL));
    endSprite->runAction(Sequence::create(
                                      MoveTo::create(time, posOfSrc),
                                      MoveTo::create(time, posOfDest),
                                      NULL));
}

void GameScene::myClock( float dt )
{
	// 时间变化
	--m_time;

	// 更新label时间
	dynamic_cast<Label *>(this->getChildByTag(TAG_3))->setString(StringUtils::format("Time: %d ", m_time));

	// 如果时间归0，那么执行游戏结束动画，并跳到游戏结束界面
	if (m_time == 0) {
		// fixed:2016/2/19
		unschedule(CC_SCHEDULE_SELECTOR(GameScene::myClock));
		
		// 游戏结束 动画
		auto gmov = Sprite::create("pic_gameover.png");
		gmov->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT * 1.5));
		this->addChild(gmov);

		auto action = MoveTo::create(3.0f, Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));

		gmov->runAction(action);

		// 跳转到游戏结束界面
		scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::gameOver), 3.5f);
	}
}

// 游戏结束函数
void GameScene::gameOver(float dt) {
	auto scene = Scene::create();
	auto layer = GameOver::create();

	// 传递当前游戏获得的分数
	layer->setScore(m_score);
	scene->addChild(layer);

	Director::getInstance()->replaceScene(TransitionFadeUp::create(1.0f, scene));
}

void GameScene::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	SHARED_AUDIO->playBackgroundMusic(MUSIC_BG2, true);
	initMap();
	scheduleUpdate();
	schedule(CC_SCHEDULE_SELECTOR(GameScene::myClock), 1.0f);
}

void GameScene::onExitTransitionDidStart() {
	Layer::onExitTransitionDidStart();
	SHARED_AUDIO->stopBackgroundMusic();
}
