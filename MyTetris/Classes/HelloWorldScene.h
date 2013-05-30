#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Square.h"
#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#define _N_WIDTH_BLOCK 16
#define _N_HEIGTH_BLOCK 25
#define FRAME_RATE 30.0

#define _OUT_OF_AREA_BARRIER   -1
#define _OUT_OF_AREA_NO		    0
#define _OUT_OF_AREA_LEFT		1
#define _OUT_OF_AREA_RIGHT		2
#define _OUT_OF_AREA_DOWN		3

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	virtual void update(float time);

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

private:
	CSquare*        m_pLiveSquare;
	CCSprite* 		m_pNodes[_N_HEIGTH_BLOCK][_N_WIDTH_BLOCK];
	CCPoint			m_start;//top-mid
	CCPoint			m_reference;//left-top
	CCPoint			m_livePoint;//mid-bottom
	CCSprite*		m_liveNodes[4];
	CCPoint			m_liveNodePos[4];
    int             m_nSquareSize;
	int				m_nFrame;
	CCSprite*       m_squareMap[_N_HEIGTH_BLOCK][_N_WIDTH_BLOCK];

	void moveDown(CCObject* pSender);
	void moveLeft(CCObject* pSender);
	void moveRight(CCObject* pSender);
	void changeShape(CCObject* pSender);

	CSquare* createSquare(CCPoint);
	void updateSquarePos(CSquare*, CCPoint);
	void liveDown(double distance);
	int barrierCheck(NodeMap sharpArray, int offx, int offy);
    int checkAndRemove();
};

#endif  // __HELLOWORLD_SCENE_H__