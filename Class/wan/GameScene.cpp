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

// ��ӭ���� ��ʼ������
bool GameScene::init()	{
	// �ȳ�ʼ�����࣬���ɹ�����false
	if( !Layer::init() )	{
		return false;
	}


	// ����plist��png
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PLIST_1);
    spriteSheet = SpriteBatchNode::create(ICON_1);
    this->addChild(spriteSheet);
	
	// ��ʼ������ֵ
	mapLBX = (GAME_SCREEN_WIDTH - SPRITE_WIDTH * COLS - (COLS - 1) * BOADER_WIDTH) / 2;
	mapLBY = (GAME_SCREEN_HEIGHT - SPRITE_WIDTH * ROWS - (ROWS - 1) * BOADER_WIDTH) / 2;

	// ��ӱ���ͼƬ
	auto bg = Sprite::create(SCENE_BG1);
	bg->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	this->addChild(bg, -1);

	
	// ��ӷ��ذ�ť
	auto back_item = MenuItemImage::create(
                                           BTN_BACK_1A,
                                           BTN_BACK_1B,
										   CC_CALLBACK_1(GameScene::menuBackCallback, this));
	back_item->setPosition(Vec2(GAME_SCREEN_WIDTH - back_item->getContentSize().width / 2, back_item->getContentSize().height / 2));

	auto menu = Menu::create(back_item, NULL);
    menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	
	// ����ttf����
	TTFConfig config("fonts/haibaoti.ttf",30);

	// ��Ϸʱ�����ʾ
	auto labelTime = Label::createWithTTF(config,StringUtils::format("Time: %d ",m_time));
	labelTime -> setPosition(Vec2( labelTime->getContentSize().width , GAME_SCREEN_HEIGHT - labelTime->getContentSize().height ));
	labelTime -> setTag(TAG_3);
	this ->addChild(labelTime);

	// ������ʾ
	auto labelScore = Label::createWithTTF(config,StringUtils::format("Score: %d ",m_score) );
	labelScore -> setPosition(Vec2( GAME_SCREEN_WIDTH/2 , GAME_SCREEN_HEIGHT - labelScore->getContentSize().height*2.6 ));
	labelScore->setTag(TAG_1);
	this->addChild(labelScore);

	// ������Ϊ0
	if(SHARED_USERDEFAULT->getIntegerForKey(KEY_SCORE) == 0){
		SHARED_USERDEFAULT->setIntegerForKey(KEY_SCORE, 0);
	}

	// ��߷�
	auto labelHScore = Label::createWithTTF(config, "Highest: 0");
	labelHScore->setPosition(Vec2(
								  GAME_SCREEN_WIDTH - labelHScore->getContentSize().width, 
								  GAME_SCREEN_HEIGHT - labelHScore->getContentSize().height));
	labelHScore->setString(StringUtils::format("Highest: %d ", SHARED_USERDEFAULT->getIntegerForKey(KEY_SCORE)));
	this->addChild(labelHScore);

	// �����¼�����
    auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

// ���غ�������ת����ӭ����
void GameScene::menuBackCallback( Ref* pSender )	{
	auto scene = WelcomeScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

// ��ʼ����ͼ
void GameScene::initMap() {
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			this->createSprite(r, c);
		}
	}
}


// ��������
void GameScene::createSprite( int row , int col )	{
	
	// �ȴ���һ����˾
	SpriteShape* spr = SpriteShape::create(row, col);
	
	// �������䶯��
	Vec2 endPosition = positionOfItem(row, col);
	Vec2 startPosition = Vec2(endPosition.x, endPosition.y + GAME_SCREEN_HEIGHT / 2);
    spr->setPosition(startPosition);
	float speed = startPosition.y / (1.5 * GAME_SCREEN_HEIGHT );
    spr->runAction(MoveTo::create(speed, endPosition));

    spriteSheet -> addChild(spr);

	// ������Ӧλ�ã�������˾����
    map[row][col] = spr;
}

// �������У���ȡ����ֵ
Vec2 GameScene::positionOfItem(int row , int col)
{
	float x = mapLBX + (SPRITE_WIDTH + BOADER_WIDTH) * col + SPRITE_WIDTH / 2;
    float y = mapLBY + (SPRITE_WIDTH + BOADER_WIDTH) * row + SPRITE_WIDTH / 2;
    return Vec2(x, y);
}

// ���º�����ÿ֡��ִ��
void GameScene::update(float t)
{
	if (m_time == 0) {
		return;
	}

	 // ����Ƿ���ִ�ж���
	if ( isAction ) {
        // ����Ϊfalse
        isAction = false;
		// ɨ��һ�����о��飬����û�п���������
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

	// ������������ƶ��У����Ӵ����¼�
	isTouchEna = !isAction;

	// ���û�ж���ִ��    
    if (!isAction) {
		// �Ƿ��о�����Ҫ�
		if ( isFillSprite ) {
            //��ըЧ��������ŵ�������˾����ǿ�����
            fillSprite();
            isFillSprite = false;
		}
		else
		{
			checkAndRemoveSprite();
		}
    }

	// �����仯
	Label *labelScore = (Label *)this->getChildByTag(TAG_1);
	labelScore -> setString( StringUtils::format("Score: %d ",m_score));
}

// ����Ƿ��о�������Ƴ�
void GameScene::checkAndRemoveSprite()
{
	SpriteShape *spr;
    // �趨��˾�ĺ��Ӽ�飬֮ǰ�����о������ú��Ӽ�飬����μ��Ҫ��֮ǰ���еļ�鶼���ܺ���
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
			// �����λ��û�о���
			if( !spr )	{
				continue;
			}
			// ����þ�����Ҫ���Ƴ�
			if( spr -> getIsNeedRemove() )	{
				continue;
			}
			// ����þ����������ɵľ���
			if ( spr -> getIgnoreCheck() ) {
				continue;
			}

			// ������ͬ����List
			std::list< SpriteShape *> colChainList;
			getColChain( spr , colChainList );
			// ������ͬ����List
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
			// ��־ �Ƿ���Ҫ�趨���Ӽ��ľ���
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
			// ��������ɵ��������4��, ȡ���һ���仯Ϊ���⾫��
			if (!isSetedIgnoreCheck && longerList.size() > 3) {
				spr -> setIgnoreCheck(true);
				spr -> setIsNeedRemove(false);
				spr->setDisplayMode( isRow ? DISPLAY_MODE_HORIZONTAL :DISPLAY_MODE_VERTICAL );
			}
		}
	}

	// ��������˵ľ���
	removeSprite();
}

// ��ǿ����Ƴ��ľ���
void GameScene::markRemove( SpriteShape* spr )	{

	// ����Ѿ������Ҫ�Ƴ����Ͳ���Ҫ�ٱ��
	if ( spr -> getIsNeedRemove()) {
        return;
    }
	// ����þ��鱻���ӣ�����Ҫ���
	if ( spr -> getIgnoreCheck() ) {
        return;
    }
   
	// �ȱ���Լ�
    spr -> setIsNeedRemove(true);
    // �����Ҫ��ǵľ����Ƿ�Ϊ �������⾫��
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

// �Ƴ�����
void GameScene::removeSprite()
{
	// ��һ���Ƴ��Ķ���
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

// ����ı�ը�Ƴ�
void GameScene::explodeSprite( SpriteShape* spr )	{
	
	SHARED_AUDIO->playEffect(MUSIC_EXPLOAD, false);

	float time = 0.2f;
	// ����Ķ���
    spr->runAction(Sequence::create(
                                      ScaleTo::create(time, 0.0),
									  CallFuncN::create(CC_CALLBACK_1(GameScene::actionEndCallback, this)),
                                      NULL));

	// ��ըЧ�� ԲȦ�Ķ���
    auto circleSprite = Sprite::create("circle.png");
	addChild(circleSprite, TAG_1);
	circleSprite->setPosition(spr->getPosition());
	circleSprite->setScale(0);// start size
    circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
                                             NULL));
}

// ����ĺ�������
void GameScene::explodeSpecialH(Point point)
{
	                    
    SimpleAudioEngine::getInstance()->playEffect(MUSIC_EXPLOAD_ONE,false);  

	// ��������Ӧ�ı���
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

// �������������
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

// ���Ƴ��ľ�����еĲ���
void GameScene::actionEndCallback(Node *node)	{
	SpriteShape *spr = (SpriteShape *)node;
	map[spr->getRow()][spr->getCol()] = NULL;
	spr -> removeFromParent();
}

// ������
void GameScene::getColChain(SpriteShape *spr, std::list<SpriteShape *> &chainList)	{
	// ��ӵ�һ�����飨�Լ���
    chainList.push_back(spr);
    
	auto curr_row = spr->getRow();

	// �������
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
    
	// ���Ҳ���
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

// ������
void GameScene::getRowChain(SpriteShape *spr, std::list<SpriteShape *> &chainList)	{
	// �Ƚ���һ����������ȥ
	chainList.push_back(spr);
    
	auto curr_col = spr->getCol();

	// ���ϲ���
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
    
	// ���²���
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

// ���ȱλ��
void GameScene::fillSprite()	{

	// ���侫���ܸ����������������ļ��㣩
	int sum = 0;
	 // �����ƶ������־
    isAction = true;

    int *colEmptyInfo = (int *)malloc(sizeof(int) * COLS);
    memset((void *)colEmptyInfo, 0, sizeof(int) * COLS);
    
    // �����ڵľ��齵������
	SpriteShape *spr = NULL;
    for (int c = 0; c < COLS; c++) {
        int removedSpriteOfCol = 0;
        // �Ե�����
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
        
        // ��¼��Ӧ�����Ƴ�������
        colEmptyInfo[c] = removedSpriteOfCol;
		sum+=removedSpriteOfCol;
    }
    
    // �½��µľ��飬������
    for (int c = 0; c < COLS; ++c ) {
        for (int r = ROWS - colEmptyInfo[c]; r < ROWS ; ++r ) {
            createSprite(r,c);
        }
    }
    
	m_score += sum*30;
    free(colEmptyInfo);
}

// ��ʼ����
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

// �������ƶ��ķ���
void GameScene::onTouchMoved(Touch *touch, Event *unused)	{
	// ���û�г�ʼ���� ���� �����¼������У�ֱ�ӷ���
	if (!staSprite || !isTouchEna) {
        return;
    }
    
	// ��ȡ ��ʼ���� ������
    int row = staSprite->getRow();
    int col = staSprite->getCol();
    
	// ��ȡ�ƶ����� �� ��λ��
    auto location = touch->getLocation();
    auto halfSpriteWidth = SPRITE_WIDTH / 2;
    auto halfSpriteHeight = SPRITE_WIDTH / 2;
    
    auto  upRect = Rect(staSprite->getPositionX() - halfSpriteWidth,
                        staSprite->getPositionY() + halfSpriteHeight,
                        SPRITE_WIDTH,
                        SPRITE_WIDTH);
    
	// �ж��������ĸ������ƶ���
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
    
    // ���򣬲���һ����Ч���ƶ�
}

// ���ݴ����ĵ�λ�ã������ǵ�ͼ���ĸ�����
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

// ��������
void GameScene::swapSprite()	{
	// �ƶ��У��������ٴδ�����ִ�ж�������Ϊtrue
    isAction = true;
    isTouchEna = false;

	// ��ʼ���� �� ��ֹ���� ������Ϊ��
	if (!staSprite || !endSprite) {
        return;
    }
    
	Vec2 posOfSrc = staSprite->getPosition();
	Vec2 posOfDest = endSprite->getPosition();

    
    // �������н���λ��
	map[ staSprite -> getRow() ][staSprite -> getCol() ] = endSprite;
	map[ endSprite -> getRow() ][endSprite -> getCol() ] = staSprite;

    int tmpRow = staSprite->getRow();
    int tmpCol = staSprite->getCol();
    staSprite->setRow(endSprite->getRow());
    staSprite->setCol(endSprite->getCol());
    endSprite->setRow(tmpRow);
    endSprite->setCol(tmpCol);
    
    // ����Ƿ�������
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
        // ����ܹ����������������ƶ��������ƶ�������
        staSprite->runAction(MoveTo::create(time, posOfDest));
        endSprite->runAction(MoveTo::create(time, posOfSrc));
        return;
    }
    
    // �������������ƶ���ȥ��Ҫ����
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
	// ʱ��仯
	--m_time;

	// ����labelʱ��
	dynamic_cast<Label *>(this->getChildByTag(TAG_3))->setString(StringUtils::format("Time: %d ", m_time));

	// ���ʱ���0����ôִ����Ϸ������������������Ϸ��������
	if (m_time == 0) {
		// fixed:2016/2/19
		unschedule(CC_SCHEDULE_SELECTOR(GameScene::myClock));
		
		// ��Ϸ���� ����
		auto gmov = Sprite::create("pic_gameover.png");
		gmov->setPosition(Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT * 1.5));
		this->addChild(gmov);

		auto action = MoveTo::create(3.0f, Point(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));

		gmov->runAction(action);

		// ��ת����Ϸ��������
		scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::gameOver), 3.5f);
	}
}

// ��Ϸ��������
void GameScene::gameOver(float dt) {
	auto scene = Scene::create();
	auto layer = GameOver::create();

	// ���ݵ�ǰ��Ϸ��õķ���
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
