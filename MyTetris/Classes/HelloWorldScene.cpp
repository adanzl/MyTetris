#include "cocos2d.h"
#include "HelloWorldScene.h"
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
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item top-right conner.
        pCloseItem->setPosition(ccp(size.width - 50, size.height - 50));


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
        btn_left->setPosition(ccp(size.width/5, 100));
        CCMenuItemImage *btn_right = CCMenuItemImage::create("images/right1.png", "images/right2.png", 
            this, menu_selector(HelloWorld::moveRight));
        btn_right->setPosition(ccp(size.width*2/5, 100));
        CCMenuItemImage *btn_change = CCMenuItemImage::create("images/c1.png", "images/c2.png", 
            this, menu_selector(HelloWorld::changeShape));
        btn_change->setPosition(ccp(size.width*3/5, 100));

        CCMenuItemImage *btn_down = CCMenuItemImage::create("images/orange.jpg", "images/green.jpg", 
            this, menu_selector(HelloWorld::moveDown));
        btn_down->setPosition(ccp(size.width*4/5, 100));


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
	if(d-((int)d / m_nSquareSize)*m_nSquareSize >0.000000001){
		liveDown(m_nSquareSize/FRAME_RATE);
		return;
	}
    //check
    checkAndRemove();
	return;

}

int HelloWorld::checkAndRemove(){

    //�ϰ����
    int i,j;
    int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
    int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;
    NodeMap liveSharpArray = m_pLiveSquare->getNodeMap();
    if(barrierCheck(liveSharpArray, offsetX, offsetY + 1)){
        //�����л�
        int n=0;
        for(i=0; i<_N_SQUARE; i++){
            for(j=0; j<_N_SQUARE; j++){
                if(liveSharpArray[i][j]){
                    m_squareMap[offsetY-_N_SQUARE+i][offsetX-2+j] = m_liveNodes[n++];
                }
            }
        }
        delete m_pLiveSquare;
        m_pLiveSquare = NULL;

        //������
        bool bRemove;
        for(j=0; j<_N_SQUARE; j++){
            bRemove = true;
            for(i=0; i<_N_WIDTH_BLOCK; i++){
                if(!m_squareMap[offsetY+j-(_N_SQUARE-1)][i]){
                    bRemove = false;
                    break;
                }
            }
            if(bRemove){
                //
                for(i=0; i<_N_WIDTH_BLOCK; i++){
                    removeChild(m_squareMap[offsetY+j-(_N_SQUARE-1)][i]);
                    m_squareMap[offsetY+j-(_N_SQUARE-1)][i] = NULL;
                }
                for(int x=0; x<_N_WIDTH_BLOCK; x++){
                    for(int y=offsetY-1; y>0; y--){
                        CCSprite* p = m_squareMap[y][x];
                        if(p){
                            CCPoint cP = p->getPosition();
                            p->setPosition(ccp(cP.x, cP.y - m_nSquareSize));
                        }
                        m_squareMap[y][x] = m_squareMap[y-1][x];
                    }
                }
                offsetY++;
            }
        }

        return 1;
    }

    liveDown(m_nSquareSize/FRAME_RATE);
    
    return 0;
}

int HelloWorld::barrierCheck(NodeMap sharpArray, int offx, int offy){
	int i, j;
	for(i=0; i<_N_SQUARE; i++){
		for(j=_N_SQUARE-1; j>=0; j--){
			if(sharpArray[j][i]){
				int y = offy - ((_N_SQUARE-1)-j);
                int x = offx - _N_SQUARE/2 + i;
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

    int i=5;
    while(i-- && !checkAndRemove()){
	    double d = m_livePoint.y - m_reference.y;
	    liveDown(d-((int)d / m_nSquareSize)*m_nSquareSize + m_nSquareSize);
    }
}

void HelloWorld::liveDown(double distance){
	for(int i=0; i<_N_SQUARE; i++){
		CCSprite* node = m_liveNodes[i];
		CCPoint cP = node->getPosition();
		node->setPosition(ccp(cP.x, cP.y - distance));
	}
	m_livePoint.y -= distance;
}

void HelloWorld::moveLeft(CCObject* pSender){
	NodeMap sharpArray = m_pLiveSquare->getNodeMap();
	int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
	int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;

    if(barrierCheck(sharpArray, offsetX-1, offsetY)){
        return;
    }

	for(int i=0; i<_N_SQUARE; i++){
		CCSprite* node = m_liveNodes[i];
		CCPoint cP = node->getPosition();
		node->setPosition(ccp(cP.x - m_nSquareSize, cP.y));
	}
	m_livePoint.x -= m_nSquareSize;
}

void HelloWorld::moveRight(CCObject* pSender){
	NodeMap sharpArray = m_pLiveSquare->getNodeMap();
	int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
	int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;

    if(barrierCheck(sharpArray, offsetX+1, offsetY)){
        return;
    }

	for(int i=0; i<_N_SQUARE; i++){
		CCSprite* node = m_liveNodes[i];
		CCPoint cP = node->getPosition();
		node->setPosition(ccp(cP.x + m_nSquareSize, cP.y));
	}
	m_livePoint.x += m_nSquareSize;
}

void HelloWorld::changeShape(CCObject* pSender){
	int offsetX = abs(m_livePoint.x - m_reference.x)/m_nSquareSize;
	int offsetY = abs(m_livePoint.y - m_reference.y)/m_nSquareSize;
	int ret = barrierCheck(m_pLiveSquare->getNextNodeMap(), offsetX, offsetY);

	if(ret == _OUT_OF_AREA_BARRIER || ret == _OUT_OF_AREA_DOWN){
		return;
	}

	if(ret == _OUT_OF_AREA_NO){
		m_pLiveSquare->ChangeSharp();
	}
	

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
	CCSprite *node = CCSprite::create(pic);
	double scale = m_nSquareSize/node->getContentSize().width;

	CCPoint anchorP = ccp(0, 0);
	node->setScale(scale);
	node->setAnchorPoint(anchorP);
	m_liveNodes[0] = node;
	addChild(node);
	node = CCSprite::create(pic);
	node->setScale(scale);
	node->setAnchorPoint(anchorP);
	m_liveNodes[1] = node;
	addChild(node);
	node = CCSprite::create(pic);
	node->setScale(scale);
	node->setAnchorPoint(anchorP);
	m_liveNodes[2] = node;
	addChild(node);
	node = CCSprite::create(pic);
	node->setScale(scale);
	node->setAnchorPoint(anchorP);
	m_liveNodes[3] = node;
	addChild(node);

	updateSquarePos(square, midPoint);
	m_livePoint = midPoint;
	return square;
}