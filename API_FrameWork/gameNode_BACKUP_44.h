#pragma once
#include "image.h"
//Z레이어설정
#define ZFLOORMAP	0
#define ZCOLMAP		1
#define ZEFFECT1	2
#define ZCOL1		3
#define ZCOL2		4
#define ZUNIT		5
#define ZCOL3		6
#define ZCOL4		7
#define ZEFFECT2	8
#define ZABOVEMAP	9

//최대레이어
#define ZMAXLAYER	10

//색상정의
#define MINT		RGB(88, 245, 206)

<<<<<<< HEAD
static image* _backBuffer = IMAGE->addImage("backBuffer", WINSIZEX, WINSIZEY);
static image* _DOLBuffer = IMAGE->addImage("DOLBuffer", WINSIZEX*2, WINSIZEY*2);	//팀프용 DC
=======
static image* _backBuffer = IMAGE->addImage("backBuffer", WINSIZEX+300, WINSIZEY+300);
>>>>>>> �깉濡쒗깭�뼱�굹�옄釉뚮옖移�
class gameNode
{
private:

	//image* _backBuffer;//백버퍼
	//void setBackBuffer();//백버프셋팅
	HDC _hdc;
	bool _managerInit;
	
public:
	gameNode();
	virtual ~gameNode();

	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update(); //계산하는곳
	virtual void render(/*HDC hdc*/);


	image* getBackBuffer()const { return _backBuffer; }
	HDC getHDC()const { return _hdc; }
	HDC getMemDC()const { return _backBuffer->getMemDC(); }

	HDC getDolDC()const { return _DOLBuffer->getMemDC(); }		//팀프용 DC

	LRESULT MainProc(HWND hWnd, UINT imessage, WPARAM wParam, LPARAM lParam);

};

