#pragma once
#include "gameNode.h"
#define INVINTIME 8
#define STUNTIME 30

class Cplayer;
class bulletManager;

enum class MONSTERMOVESTATE
{
	NONE,
	RIGHTUP,
	UP,
	LEFTUP,
	LEFT,
	LEFTDOWN,
	DOWN,
	RIGHTDOWN,
	RIGHT
};
enum class MONSTERACTIVE
{
	NONE,
	FINDING,
	RNDMOVE,
	ATTACK,
	ATTACK2,
	ATTACK3,
	DEATH,
};
struct tagMonster
{
	image* img;
	animation* ani;
	int frameX, frameY;
	int framecount;
	RECT rc;
	RECT footRc;
	RECT bossRc[2];
	float x, y;
	int width;
	int height;
	float speed;
	int hp;
	int attackNum;	//20210805추가 - 공격횟수카운트필요한 몬스터를 위함.
	bool isHit;		//20210805추가 - 맞았을때 자동 추격위함
	float neverchangeX, neverchangeY;	//20210808추가 - 고정형 보스의 피격시 움직임을 위함.
	bool isStun;
	bool isDeath;
	bool afterDeath;
	bool findPlayer;
	float targetX;
	float targetY;
	float angle;
	float range;
	int stunCount;
	int patternCount;
	int deathalpha;
	bool isLeft;		//20210809 - 보스패턴구현
	bool isNextPhase;	//20210809 - 보스패턴구현
	bool oldPhase;		//20210809 - 보스패턴구현
	bool isInvincible = false;			//20210809 - 피격시 짧은 무적상태추가
	int invinCount = 0;
	MONSTERACTIVE activestate;
	MONSTERACTIVE oldactivestate;
	MONSTERMOVESTATE movestate;
};
class monster : public gameNode
{
protected:
	vector<tagMonster> _vMonster;
	vector<tagMonster>::iterator _viMonster;
	bool _isDebug;
public:
	monster();
	~monster();

	HRESULT init();
	void release();
	void update(Cplayer* py, bulletManager* bm);
	void render();

	virtual void addMonster(float x, float y);
	virtual void move(bulletManager* bm);
	virtual void checkAngle();
	virtual void giveFrame();
	virtual void deathCheck();
	virtual void knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun = false);
	virtual void stuncheck();
	virtual void checkPlayerXY(Cplayer* py);
	virtual void goOrStay(float speed);
	virtual void makeCollisionRect();

	virtual void checkInvincibility();

	vector<tagMonster>& getVMonster() { return _vMonster; }
	void setIsDebug(bool isDebug) { _isDebug = isDebug; }
};

