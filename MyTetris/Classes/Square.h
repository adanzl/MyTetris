#ifndef __H_SQUARE__
#define __H_SQUARE__

#define _N_SQUARE 4

enum SQUARETYPE{
	TYPE_T = 0, TYPE_L, TYPE_I, TYPE_O, TYPE_S
};

typedef int(*NodeMap)[_N_SQUARE];

class CSquare
{
public:
	CSquare(void);
	virtual ~CSquare(void);

	static NodeMap allocMap();
    void ChangeSharp();
	NodeMap getNodeMap(){return m_NodeMap[m_nIndexState];};
	NodeMap getNextNodeMap(){return m_NodeMap[(m_nIndexState+1)%m_nState];};

private:
	NodeMap* m_NodeMap;
    int m_nState;
    int m_nIndexState;
	SQUARETYPE m_type;
};
#endif

