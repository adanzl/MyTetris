#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#define N_COLOR 5

const char* g_color[N_COLOR] = {
	"images/yellow.jpg",
	"images/orange.jpg",
	"images/purple.jpg",
	"images/red.jpg",
	"images/green.jpg",	
};

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		// Get window size and place the label upper. 
		CCSize size = CCDirector::sharedDirector()->getWinSize();

        m_nSquareSize = size.width / 24;

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "images/CloseNormal.png",
            "images/CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
		pCloseItem->setScale(size.height / pCloseItem->getContentSize().height / 14);
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item top-right conner.
        pCloseItem->setPosition(ccp(size.width - 50, size.height - 50));

        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.wav", true);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        CC_BREAK_IF(! pLabel);
        
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("images/HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

		CCSprite* pSquareArea = CCSprite::create("images/p4.jpg");
		CC_BREAK_IF(!pSquareArea);
		CCSize areaSize = pSquareArea->getContentSize();
		double mainAreaX = (int)(size.width/2/m_nSquareSize)*m_nSquareSize;
		double mainAreaY = (int)(size.height/2/m_nSquareSize+1)*m_nSquareSize;
        pSquareArea->setPosition(ccp(mainAreaX, mainAreaY));
		double scaleX = _N_WIDTH_BLOCK*m_nSquareSize/areaSize.width;
		double scaleY = _N_HEIGTH_BLOCK*m_nSquareSize/areaSize.height;
		pSquareArea->setScaleX(scaleX);
		pSquareArea->setScaleY(scaleY);
        this->addChild(pSquareArea);
		m_pLiveSquare = NULL;
		m_nFrame = 0;
		for(int i=0; i<_N_HEIGTH_BLOCK; i++){
			for(int j=0; j<_N_WIDTH_BLOCK; j++){
				m_squareMap[i][j]=NULL;
            }
        }
		m_start = ccp(mainAreaX, mainAreaY + _N_HEIGTH_BLOCK*m_nSquareSize/2);
		//
		m_reference = ccp(mainAreaX - _N_WIDTH_BLOCK*m_nSquareSize/2, 
			mainAreaY + _N_HEIGTH_BLOCK*m_nSquareSize/2);

		
        CCMenuItemImage *btn_left = CCMenuItemImage::create("images/left1.png", "images/left2.png", 
            this, menu_selector(HelloWorld::moveLeft));
        btn_left->setPosition(ccp(size.width/5, size.height/10));
		btn_left->setScale(size.height / btn_left->getContentSize().height / 14);

        CCMenuItemImage *btn_right = CCMenuItemImage::create("images/right1.png", "images/right2.png", 
            this, menu_selector(HelloWorld::moveRight));
        btn_right->setPosition(ccp(size.width*2/5, size.height/10));
		btn_right->setScale(size.height / btn_right->getContentSize().height / 14);
        CCMenuItemImage *btn_change = CCMenuItemImage::create("images/c1.png", "images/c2.png", 
            this, menu_selector(HelloWorld::changeShape));
        btn_change->setPosition(ccp(size.width*3/5, size.height/10));
		btn_change->setScale(size.height / btn_change->getContentSize().height / 14);

        CCMenuItemImage *btn_down = CCMenuItemImage::create("images/orange.jpg", "images/green.jpg", 
            this, menu_selector(HelloWorld::moveDown));
        btn_down->setPosition(ccp(size.width*4/5, size.height/10));
		btn_down->setScale(size.height / btn_down->getContentSize().height / 14);


        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, btn_left, btn_right, btn_change, 
            btn_down, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::update(float time){
	if(!m_pLiveSquare){
		m_pLiveSquare = createSquare(m_start);
		//m_pLiveSquare->setPosition(ccp(0, m_limitSize.height/2*m_scale));
		m_nFrame = 0;
		return;
	}

	double d = m_livePoint.y - m_reference.y;
	m_nFrame++;
	if(m_nFrame%(int)FRAME_RATE == 0){
		m_nFrame = 0;
		liveMove(_DIRECTION_DWON, m_nSquareSize);
	} else {
		//check
		checkAndRemove();
	}

}

int HelloWorld::checkAndRemove(){

    //’œ∞≠ºÏ≤‚
    int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
    int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;
    NodeMap liveSharpArray = m_pLiveSquare->getNodeMap();
    if(barrierCheck(liveSharpArray, offsetX, offsetY + 1)){
        //æÿ’Û«–ªª
        int n=0;
        for(int i=0; i<_N_SQUARE; i++){
            for(int j=0; j<_N_SQUARE; j++){
                if(liveSharpArray[i][j]){
                    m_squareMap[offsetY-_N_SQUARE+i][offsetX-2+j] = m_liveNodes[n++];
                }
            }
        }
        delete m_pLiveSquare;
        m_pLiveSquare = NULL;

        //œ˚øÈºÏ≤‚
        bool bRemove;
		int nRemoved = 0;
        for(int j=0; j<_N_SQUARE; j++){
            bRemove = true;
			int cy = offsetY - j + nRemoved - 1;
            for(int i=0; i<_N_WIDTH_BLOCK; i++){
                if(!m_squareMap[cy][i]){
                    bRemove = false;
                    break;
                }
            }
            if(bRemove){
                //
                for(int i=0; i<_N_WIDTH_BLOCK; i++){
                    removeChild(m_squareMap[cy][i]);
                    m_squareMap[cy][i] = NULL;
                }
				for(int y=cy; y>0; y--){
					for(int x=0; x<_N_WIDTH_BLOCK; x++){
                        CCSprite* p = m_squareMap[y-1][x];
                        if(p){
                            CCPoint cP = p->getPosition();
                            p->setPosition(ccp(cP.x, cP.y - m_nSquareSize));
                        }
                        m_squareMap[y][x] = m_squareMap[y-1][x];
                    }
                }
                nRemoved++;
            }
        }

        return 1;
    }

    //liveDown(m_nSquareSize);
    
    return 0;
}

int HelloWorld::barrierCheck(NodeMap sharpArray, int offx, int offy){
	int i, j;
	for(i=0; i<_N_SQUARE; i++){
		for(j=_N_SQUARE-1; j>=0; j--){
			if(sharpArray[j][i]){
				int y = offy - ((_N_SQUARE-1)-j);
                int x = offx - _N_SQUARE/2 + i;
				if(!y)
					break;
				if(y > _N_HEIGTH_BLOCK){
					return _OUT_OF_AREA_DOWN;
				} else if(x < 0){
					return _OUT_OF_AREA_LEFT;
				} else if(x >= _N_WIDTH_BLOCK){
					return _OUT_OF_AREA_RIGHT;
				} else if(m_squareMap[y-1][i+offx-2]){
					return _OUT_OF_AREA_BARRIER;
				}
				break;
			}
		}
	}
	return _OUT_OF_AREA_NO;
}

void HelloWorld::moveDown(CCObject* pSender){

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("blip1.wav");
	m_nFrame = 0;
    int i=5;
    while(i-- && !checkAndRemove()){
	    liveMove(_DIRECTION_DWON, m_nSquareSize);
    }
}

void HelloWorld::liveMove(int direction, double distance){
    if(direction == _DIRECTION_DWON){
        for(int i=0; i<_N_SQUARE; i++){
            CCSprite* node = m_liveNodes[i];
            CCPoint cP = node->getPosition();
            node->setPosition(ccp(cP.x, cP.y - distance));
        }
        m_livePoint.y -= distance;
    } else if(direction == _DIRECTION_LEFT){
        for(int i=0; i<_N_SQUARE; i++){
            CCSprite* node = m_liveNodes[i];
            CCPoint cP = node->getPosition();
            node->setPosition(ccp(cP.x - m_nSquareSize, cP.y));
        }
        m_livePoint.x -= m_nSquareSize;
    } else if(direction == _DIRECTION_RIGHT){
        for(int i=0; i<_N_SQUARE; i++){
            CCSprite* node = m_liveNodes[i];
            CCPoint cP = node->getPosition();
            node->setPosition(ccp(cP.x + m_nSquareSize, cP.y));
        }
        m_livePoint.x += m_nSquareSize;
    }
}

void HelloWorld::moveLeft(CCObject* pSender){

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("blip1.wav");
	NodeMap sharpArray = m_pLiveSquare->getNodeMap();
	int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
	int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;

    if(barrierCheck(sharpArray, offsetX-1, offsetY)){
        return;
    }

	liveMove(_DIRECTION_LEFT, m_nSquareSize);
}

void HelloWorld::moveRight(CCObject* pSender){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("blip1.wav");
	NodeMap sharpArray = m_pLiveSquare->getNodeMap();
	int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
	int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;

    if(barrierCheck(sharpArray, offsetX+1, offsetY)){
        return;
    }

	liveMove(_DIRECTION_RIGHT, m_nSquareSize);
}

void HelloWorld::changeShape(CCObject* pSender){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("blip1.wav");
	int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
	int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;
    NodeMap nextMap = m_pLiveSquare->getNextNodeMap();
	int ret = barrierCheck(nextMap, offsetX, offsetY);

	if(ret == _OUT_OF_AREA_BARRIER || ret == _OUT_OF_AREA_DOWN){
		return;
	}
    if(ret != _OUT_OF_AREA_NO){
        int ret1;
        switch (ret){
        case _OUT_OF_AREA_LEFT:
            ret1 = barrierCheck(nextMap, offsetX+1, offsetY);
            if(ret1 == _OUT_OF_AREA_LEFT){
                ret1 = barrierCheck(nextMap, offsetX+2, offsetY);
                if(ret1 == _OUT_OF_AREA_NO){
                    liveMove(_DIRECTION_RIGHT, m_nSquareSize*2);
                } else {
                    return;
                }
            } else if(ret1 == _OUT_OF_AREA_NO){
                liveMove(_DIRECTION_RIGHT, m_nSquareSize);
            } else {
                return;
            }
            break;
        case _OUT_OF_AREA_RIGHT:
            ret1 = barrierCheck(nextMap, offsetX-1, offsetY);
            if(ret1 == _OUT_OF_AREA_LEFT){
                ret1 = barrierCheck(nextMap, offsetX-2, offsetY);
                if(ret1 == _OUT_OF_AREA_NO){
                    liveMove(_DIRECTION_LEFT, m_nSquareSize*2);
                } else {
                    return;
                }
            } else if(ret1 == _OUT_OF_AREA_NO){
                liveMove(_DIRECTION_LEFT, m_nSquareSize);
            } else {
                return;
            }
            break;
        default:
            break;
        }
    }

    m_pLiveSquare->ChangeSharp();
	updateSquarePos(m_pLiveSquare, m_livePoint);
}

void HelloWorld::updateSquarePos(CSquare* square, CCPoint midPoint){
	int n=0;
	NodeMap nodeMap = square->getNodeMap();

	for (int i=0; i<_N_SQUARE; i++){
		for(int j=0; j<_N_SQUARE; j++){
			if(nodeMap[i][j]){
				CCSprite* node = m_liveNodes[n++];
				double x = (j-_N_SQUARE/2) * m_nSquareSize + midPoint.x;
				double y = (_N_SQUARE-1-i) * m_nSquareSize + midPoint.y;
				node->setPosition(ccp(x, y));
			}
		}
	}
}

CSquare* HelloWorld::createSquare(CCPoint midPoint){

	CSquare* square = new CSquare();
	srand((unsigned)time(NULL));
	const char* pic = g_color[rand()%N_COLOR];
	m_liveNodes[0] = CCSprite::create(pic);
	double scale = m_nSquareSize/m_liveNodes[0]->getContentSize().width;

	CCPoint anchorP = ccp(0, 0);
	m_liveNodes[0]->setScale(scale);
	m_liveNodes[0]->setAnchorPoint(anchorP);
	addChild(m_liveNodes[0]);

	m_liveNodes[1] = CCSprite::create(pic);
	m_liveNodes[1]->setScale(scale);
	m_liveNodes[1]->setAnchorPoint(anchorP);
	addChild(m_liveNodes[1]);

	m_liveNodes[2] = CCSprite::create(pic);
	m_liveNodes[2]->setScale(scale);
	m_liveNodes[2]->setAnchorPoint(anchorP);
	addChild(m_liveNodes[2]);

	m_liveNodes[3] = CCSprite::create(pic);
	m_liveNodes[3]->setScale(scale);
	m_liveNodes[3]->setAnchorPoint(anchorP);
	addChild(m_liveNodes[3]);

	updateSquarePos(square, midPoint);
	m_livePoint = midPoint;
	return square;
}