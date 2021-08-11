#pragma once
#include "singleton.h"

class bulletManager;

//////////////////////////////////////////////////

enum class PLACE {
	ROOM,
	DUNGEON
};
enum class STATE {
	IDLE,
	WALK,
	RUN,
	DASH,
	ATTSTAFF,
	STAFFCHARGE,
	TALK,
	KNOCKBACK,
	DIE
};
enum class WEAPONTYPE{
	EMPTY,
	SWORD,
	BOW,
	STAFF
};
struct tagAttackBox {
	RECT rc;
	bool isHit;
};
struct tagDamegeFont {
	int damage;
	bool isActivate;
};
enum DIRECTION
{
	UPLEFT=0,
	UP,
	UPRIGHT,
	RIGHT,
	DOWNRIGHT,
	DOWN,
	DOWNLEFT,
	LEFT
};
struct tagInputDirection {
	bool isRight;
	bool isDown;
	bool isLeft;
	bool isUp;
};
struct Player
{
	RECT playerRect;
	PLACE _place;
	WEAPONTYPE weapon;
	tagAttackBox AttackBox;
	float x, y;
	int HealthPoint;
	int ManaPoint;
	int isHit;
	int isDashHit;
};

struct DashEffect {
	image* dashEffect;
	int x, y;
	int dashFrameX;
	DIRECTION direction;
	int dashAlpha;
};

/////////////////////////////////////////////////////////

class Cplayer : public Singleton<Cplayer>
{
private:
	bool _isDebug;
	bool _isAutoRun;
	bool _frameswitching;
private:
	vector<tagDamegeFont*> _damageFont;
private:
	float _walkspeed;
	Player _player;
	STATE _state;
	tagInputDirection _inputDirection;
	DIRECTION _direction;
	DIRECTION _moveDirection;

	int _hitCount;
	float _knockBackAngle;
	int _knockBackTime;
	int _gracePeriod;
	int _knockBackIndex;

	int _count;
	int _index;

	int _dashCount;
	int _dashIndex;
	float _dashAngle;

	int _attCount;
	int _attIndex;
	float _attAngle;

private:

	image* _walk_img;
	image* _run_img;
	image* _dash_img;
	image* _attStaff_img;
	image* _knockBack_img;
	vector<DashEffect> _vectDashEffect;
	vector<DashEffect>::iterator _iterDashEffect;

private:

	bulletManager* _Cbullet;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void imageInit();

	void inputCheck();
	void inputDirectionCheck();
	void stateCheck();
	void movePlayer();
	void setPlayerFrame();
	void angleCheckDirection(float angle);

	void pushbackDashEffect(int x,int y, int FrameX,DIRECTION direction);
	void renderDashEffecct(HDC hdc);

	void hitStateCheck();
	void hitPlayer(int bulletX, int bulletY);

	Player& getPlayerAddress() { return _player; }
	void setIsDebug(bool isDebug) { _isDebug = isDebug; }
	void setBulletManagerMemoryLink(bulletManager* BM) { _Cbullet = BM; }
	STATE& getSTATEAddress() { return _state; }
};