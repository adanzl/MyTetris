#include "cocos2d.h"
#include "Square.h"

NodeMap CSquare::allocMap(){
    NodeMap p = new int[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            p[i][j]=0;
    return p;
}

CSquare::CSquare(void): m_nIndexState(0), m_nState(1)
{
	srand( (unsigned)time(NULL));
	m_type = SQUARETYPE(rand()%(sizeof(SQUARETYPE)+1));
    //m_type = TYPE_S;
    NodeMap nodeMap = NULL;
	switch (m_type)
	{
	case TYPE_T:
        m_nState = 4;
        m_NodeMap = new NodeMap[m_nState];
        nodeMap = m_NodeMap[0] = allocMap();
		nodeMap[1][1] = 1;
		nodeMap[2][1] = 1;
		nodeMap[3][1] = 1;
		nodeMap[2][2] = 1;
		nodeMap = m_NodeMap[1] = allocMap();
		nodeMap[2][1] = 1;
		nodeMap[2][2] = 1;
		nodeMap[2][3] = 1;
		nodeMap[3][2] = 1;
		nodeMap = m_NodeMap[2] = allocMap();
		nodeMap[1][2] = 1;
		nodeMap[2][1] = 1;
		nodeMap[3][2] = 1;
		nodeMap[2][2] = 1;
		nodeMap = m_NodeMap[3] = allocMap();
		nodeMap[2][2] = 1;
		nodeMap[3][1] = 1;
		nodeMap[3][2] = 1;
		nodeMap[3][3] = 1;
		break;
	case TYPE_L:
        m_nState = 4;
        m_NodeMap = new NodeMap[m_nState];
        nodeMap = m_NodeMap[0] = allocMap();
		nodeMap[1][1] = 1;
		nodeMap[2][1] = 1;
		nodeMap[3][1] = 1;
		nodeMap[3][2] = 1;
		nodeMap = m_NodeMap[1] = allocMap();
		nodeMap[2][1] = 1;
		nodeMap[2][2] = 1;
		nodeMap[2][3] = 1;
		nodeMap[3][1] = 1;
		nodeMap = m_NodeMap[2] = allocMap();
		nodeMap[1][1] = 1;
		nodeMap[1][2] = 1;
		nodeMap[2][2] = 1;
		nodeMap[3][2] = 1;
		nodeMap = m_NodeMap[3] = allocMap();
		nodeMap[2][3] = 1;
		nodeMap[3][1] = 1;
		nodeMap[3][2] = 1;
		nodeMap[3][3] = 1;
        
		break;
	case TYPE_I:
        m_nState = 2;
        m_NodeMap = new NodeMap[m_nState];
        //m_NodeMap[0] = alloc();
        nodeMap = m_NodeMap[0] = allocMap();
		nodeMap[0][1] = 1;
		nodeMap[1][1] = 1;
		nodeMap[2][1] = 1;
		nodeMap[3][1] = 1;
		nodeMap = m_NodeMap[1] = allocMap();
		nodeMap[1][0] = 1;
		nodeMap[1][1] = 1;
		nodeMap[1][2] = 1;
		nodeMap[1][3] = 1;
		break;
	case TYPE_O:
        m_nState = 1;
        m_NodeMap = new NodeMap[m_nState];
        nodeMap = m_NodeMap[0] = allocMap();
		nodeMap[2][2] = 1;
		nodeMap[2][1] = 1;
		nodeMap[3][1] = 1;
		nodeMap[3][2] = 1;
		break;
	case TYPE_S:
        m_nState = 2;
        m_NodeMap = new NodeMap[m_nState];
        nodeMap = m_NodeMap[0] = allocMap();
		nodeMap[1][1] = 1;
		nodeMap[2][1] = 1;
		nodeMap[3][2] = 1;
		nodeMap[2][2] = 1;
        nodeMap = m_NodeMap[1] = allocMap();
		nodeMap[3][1] = 1;
		nodeMap[2][2] = 1;
		nodeMap[3][2] = 1;
		nodeMap[3][3] = 1;
        
		break;
	default:
		break;
	}
}

CSquare::~CSquare(void)
{
    for(int i=0; i<m_nState; i++){
        delete[] m_NodeMap[i];
    }
}


void CSquare::ChangeSharp(){

    m_nIndexState = (m_nIndexState+1)%m_nState;
    
}