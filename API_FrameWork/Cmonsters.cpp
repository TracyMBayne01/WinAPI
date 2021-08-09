#include "framework.h"
#include "Cmonsters.h"
#include "Cplayer.h"
#include "bulletManager.h"
/*
=================================================
	Find monster class ctrl+F4 ("name!")
=================================================
	monstername					imagename
1.		Csnaby			/		��
2.		Cslime			/		������
3.		Cmushman		/		�ӽ���
4	Cmushman_mushroom!	/		�ӽ���_�ӽ���		
5.		Cfairy			/		��
6.		Cflime			/		��
7.	Cboss_slime			/		����������
8.	Csemiboss_slime		/		�غ���������
9.	Cboss_flime			/		�����ö���
10. Cboss_mushmam		/		�����ӽ���
11. Cmushmam_mushroom_G	/		�ӽ���_����ӽ���
12. Cmushmam_mushroom2_P/		�ӽ���_����ӽ���
13. Cmushmam_mushroom3_B/		�ӽ���_�Ķ��ӽ���
14.
*/
//////////////////////////////////////////////////////////////
//////////////////////Csnaby!	��!//////////////////////////
//////////////////////////////////////////////////////////////
Csnaby::Csnaby()
{
	IMAGE->addFrameImage("��", "images/monsters/snaby-snaby.bmp", 144, 624, 3, 13, true);
}

Csnaby::~Csnaby()
{
}

HRESULT Csnaby::init()
{
	return S_OK;
}

void Csnaby::release()
{
}

void Csnaby::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Csnaby::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}		
	}
	//TCHAR str[128];
	//_stprintf_s(str, "������ ���� : %d", _vMonster.size());
	//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	//RectangleMake(getMemDC(), fackpy);
}

void Csnaby::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("��");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.width = newMonster.img->getFrameWidth()/3 * 2;
	newMonster.height = newMonster.img->getFrameHeight();
	newMonster.rc = RectMake(x+ newMonster.img->getFrameWidth()/6, y, newMonster.width, newMonster.height);
	newMonster.speed = 2;
	newMonster.hp = 40;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI*3/2;
	newMonster.range = 280;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Csnaby::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount >= 100) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		if (_viMonster->patternCount < 110) {
			_viMonster->x += cosf(_viMonster->angle) * 1;
			_viMonster->y -= sinf(_viMonster->angle) * 1;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y, _viMonster->width, _viMonster->height);
			if (_viMonster->patternCount % 30 == 0) {
				bm->getTriBulInstance()->fire(
					_viMonster->x + _viMonster->img->getFrameWidth()/2,
					_viMonster->y + _viMonster->img->getFrameHeight()/2,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		if (_viMonster->patternCount < 40) {
			_viMonster->x += cosf(_viMonster->angle)*_viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y, _viMonster->width, _viMonster->height);
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha-=2;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Csnaby::checkAngle()
{
	if (_viMonster->angle > PI_8 && _viMonster->angle <= PI_8 * 3) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTUP;
	}
	else if (_viMonster->angle > PI_8 * 3 && _viMonster->angle <= PI_8 * 5) {
		_viMonster->movestate = MONSTERMOVESTATE::UP;
	}
	else if (_viMonster->angle > PI_8 * 5 && _viMonster->angle <= PI_8 * 7) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTUP;
	}
	else if (_viMonster->angle > PI_8 * 7 && _viMonster->angle <= PI_8 * 9) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFT;
	}
	else if (_viMonster->angle > PI_8 * 9 && _viMonster->angle <= PI_8 * 11) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTDOWN;
	}
	else if (_viMonster->angle > PI_8 * 11 && _viMonster->angle <= PI_8 * 13) {
		_viMonster->movestate = MONSTERMOVESTATE::DOWN;
	}
	else if (_viMonster->angle > PI_8 * 13 && _viMonster->angle <= PI_8 * 15) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTDOWN;
	}
	else {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHT;
	}
}

void Csnaby::giveFrame()
{
	int maxFrameX;
	_viMonster->framecount++;
	//8����� ���¿� ���� Y������ ������
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
	case MONSTERACTIVE::FINDING:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 11;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 9;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 8;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 10;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 8; //����Ʈ
			break;
		}
		maxFrameX = 1;
		break;

	case MONSTERACTIVE::ATTACK:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 7;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 5;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 4;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 6;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 4; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::RNDMOVE:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 3;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 1;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 0;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 2;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 0; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::DEATH:
		_viMonster->frameY = 12;
		maxFrameX = 0;
		break;
	}

	//X������ ������
	int currentX = _viMonster->frameX;
	if (_viMonster->framecount % 12 == 0) {
		currentX++;
		if (currentX > maxFrameX) {
			currentX = 0;
		}
		_viMonster->frameX = currentX;
	}
}

void Csnaby::deathCheck()
{
	if (_viMonster->activestate != MONSTERACTIVE::DEATH && _viMonster->hp <= 0) {
		_viMonster->activestate = MONSTERACTIVE::DEATH;
		_viMonster->patternCount = 0;
	}
}

void Csnaby::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 6, iter->y, iter->width, iter->height);
	if (stun) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Csnaby::stuncheck()
{
	if (_viMonster->isStun && _viMonster->activestate != MONSTERACTIVE::DEATH) {
		_viMonster->stunCount++;
		if (_viMonster->stunCount > 20) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
			_viMonster->stunCount = 0;
			_viMonster->isStun = false;
		}
	}
}

void Csnaby::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::ATTACK;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->x, _viMonster->y, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� �������� ����.
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		_viMonster->angle = RND->getInt(360) * PI / 180;
	}
}
//////////////////////////////////////////////////////////////
//////////////////////Cslime!	������!////////////////////////
//////////////////////////////////////////////////////////////
Cslime::Cslime()
{
	IMAGE->addFrameImage("������", "images/monsters/slime.bmp", 180, 780, 3, 13, true);
}

Cslime::~Cslime()
{
}

HRESULT Cslime::init()
{
	return S_OK;
}

void Cslime::release()
{
}

void Cslime::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cslime::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "hp : %d", _viMonster->hp);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cslime::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("������");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.width = newMonster.img->getFrameWidth() / 3 * 2;
	newMonster.height = newMonster.img->getFrameHeight();
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() / 6, y + newMonster.img->getFrameWidth() / 4, newMonster.width, newMonster.height - newMonster.img->getFrameWidth() / 4);
	newMonster.speed = 2;
	newMonster.hp = 49;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 350;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cslime::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount >30 && _viMonster->patternCount < 60) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y + _viMonster->img->getFrameWidth() / 4, _viMonster->width, _viMonster->height - _viMonster->img->getFrameWidth() / 4);
		}
		if (_viMonster->patternCount >= 150) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		if (_viMonster->patternCount < 35) {
			if (_viMonster->patternCount % 30 == 0) {
				bm->getTriBulInstance()->fire(
					_viMonster->x + _viMonster->img->getFrameWidth() / 2,
					_viMonster->y + _viMonster->img->getFrameHeight() / 2,
					_viMonster->angle, 10);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount < 110) {
			if (_viMonster->patternCount == 15) {
				_viMonster->y -= 25;
			}
			if (_viMonster->patternCount == 30) {
				_viMonster->y += 25;
			}
			if (_viMonster->patternCount == 45) {
				_viMonster->y -= 25;
			}
			if (_viMonster->patternCount == 60) {
				_viMonster->y += 25;
			}
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y + _viMonster->img->getFrameWidth() / 4, _viMonster->width, _viMonster->height - _viMonster->img->getFrameWidth() / 4);
			if (_viMonster->patternCount % 97 == 0) {
				bm->getCirBulInstance()->fire(
					_viMonster->x + _viMonster->img->getFrameWidth() / 2,
					_viMonster->y + _viMonster->img->getFrameHeight() / 2,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		if (_viMonster->patternCount < 35) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y + _viMonster->img->getFrameWidth() / 4, _viMonster->width, _viMonster->height - _viMonster->img->getFrameWidth() / 4);
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 2;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cslime::checkAngle()
{
	if (_viMonster->angle > PI_8 && _viMonster->angle <= PI_8 * 3) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTUP;
	}
	else if (_viMonster->angle > PI_8 * 3 && _viMonster->angle <= PI_8 * 5) {
		_viMonster->movestate = MONSTERMOVESTATE::UP;
	}
	else if (_viMonster->angle > PI_8 * 5 && _viMonster->angle <= PI_8 * 7) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTUP;
	}
	else if (_viMonster->angle > PI_8 * 7 && _viMonster->angle <= PI_8 * 9) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFT;
	}
	else if (_viMonster->angle > PI_8 * 9 && _viMonster->angle <= PI_8 * 11) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTDOWN;
	}
	else if (_viMonster->angle > PI_8 * 11 && _viMonster->angle <= PI_8 * 13) {
		_viMonster->movestate = MONSTERMOVESTATE::DOWN;
	}
	else if (_viMonster->angle > PI_8 * 13 && _viMonster->angle <= PI_8 * 15) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTDOWN;
	}
	else {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHT;
	}
}

void Cslime::giveFrame()
{
	int maxFrameX;
	_viMonster->framecount++;
	//8����� ���¿� ���� Y������ ������
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
	case MONSTERACTIVE::FINDING:
	case MONSTERACTIVE::RNDMOVE:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 3;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 1;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 0;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 2;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 0; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::ATTACK:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 11;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 9;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 8;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 10;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 8; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount == 75) {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
		}
		if (_viMonster->patternCount > 75) {
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				_viMonster->frameY = 7;
				break;
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
				_viMonster->frameY = 5;
				break;
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
				_viMonster->frameY = 4;
				break;
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				_viMonster->frameY = 6;
				break;
			case MONSTERMOVESTATE::NONE:
				_viMonster->frameY = 4; //����Ʈ
				break;
			}
		}
		maxFrameX = 2;
		break;

	//case MONSTERACTIVE::RNDMOVE:
	//	switch (_viMonster->movestate)
	//	{
	//	case MONSTERMOVESTATE::RIGHTUP:
	//	case MONSTERMOVESTATE::UP:
	//		_viMonster->frameY = 3;
	//		break;
	//	case MONSTERMOVESTATE::LEFTUP:
	//	case MONSTERMOVESTATE::LEFT:
	//		_viMonster->frameY = 1;
	//		break;
	//	case MONSTERMOVESTATE::LEFTDOWN:
	//	case MONSTERMOVESTATE::DOWN:
	//		_viMonster->frameY = 0;
	//		break;
	//	case MONSTERMOVESTATE::RIGHTDOWN:
	//	case MONSTERMOVESTATE::RIGHT:
	//		_viMonster->frameY = 2;
	//		break;
	//	case MONSTERMOVESTATE::NONE:
	//		_viMonster->frameY = 0; //����Ʈ
	//		break;
	//	}
	//	maxFrameX = 2;
	//	break;

	case MONSTERACTIVE::DEATH:
		_viMonster->frameY = 12;
		maxFrameX = 2;
		break;
	}

	//X������ ������
	int currentX = _viMonster->frameX;
	if (_viMonster->framecount % 12 == 0) {
		currentX++;
		if (currentX > maxFrameX) {
			currentX = 0;
		}
		_viMonster->frameX = currentX;
	}
}

void Cslime::deathCheck()
{
	if (_viMonster->activestate != MONSTERACTIVE::DEATH && _viMonster->hp <= 0) {
		_viMonster->activestate = MONSTERACTIVE::DEATH;
		_viMonster->patternCount = 0;
	}
}

void Cslime::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 6, iter->y + iter->img->getFrameWidth() / 4, iter->width, iter->height - iter->img->getFrameWidth() / 4);
	if (stun) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cslime::stuncheck()
{
	if (_viMonster->isStun && _viMonster->activestate != MONSTERACTIVE::DEATH) {
		_viMonster->stunCount++;
		if (_viMonster->stunCount > 20) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
			_viMonster->stunCount = 0;
			_viMonster->isStun = false;
		}
	}
}

void Cslime::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		if(_viMonster->oldactivestate == MONSTERACTIVE::ATTACK) _viMonster->activestate = MONSTERACTIVE::ATTACK2;
		else _viMonster->activestate = MONSTERACTIVE::ATTACK;
		_viMonster->oldactivestate = MONSTERACTIVE::NONE;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->x, _viMonster->y, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� �������� ����.
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		_viMonster->angle = RND->getInt(360) * PI / 180;
	}
}
//////////////////////////////////////////////////////////////
//////////////////////Cmushman!	�ӽ���!///////////////////////
//////////////////////////////////////////////////////////////
Cmushman::Cmushman()
{
	IMAGE->addFrameImage("�ӽ���", "images/monsters/mushman-mushman.bmp", 144, 935, 3, 17, true);
}

Cmushman::~Cmushman()
{
}

HRESULT Cmushman::init()
{
	return S_OK;
}

void Cmushman::release()
{
}

void Cmushman::update(Cplayer* py, bulletManager* bm, Cmushman_mushroom* _mushroom)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		stuncheck();
		checkPlayerXY(py);
		move(bm, _mushroom);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cmushman::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
	}
	//TCHAR str[128];
	//_stprintf_s(str, "�÷��̾���ǥ : %.1f, %.1f", PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
}

void Cmushman::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�ӽ���");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.width = newMonster.img->getFrameWidth() / 3 * 2;
	newMonster.height = newMonster.img->getFrameHeight() * 3 / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() / 6, y, newMonster.width, newMonster.height);
	newMonster.speed = 2;
	newMonster.hp = 77;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 350;
	newMonster.stunCount = 0;
	newMonster.patternCount = 400;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cmushman::move(bulletManager* bm, Cmushman_mushroom* _mushroom)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount <= 600) {
			//���30�̻��̸� RNDMOVE�� ����
			if (_viMonster->patternCount % 100 > 0 && _viMonster->patternCount % 100 < 30) {
				_viMonster->frameX = 0;
				_viMonster->framecount = 0;
				_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
			}
			//����� �� ����Ȯ���ϰ� �� �ٲٱ�
			if (_viMonster->patternCount % 100 == 0)
				_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		else _viMonster->activestate = MONSTERACTIVE::FINDING;
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		if (_viMonster->patternCount < 120) {

		}
		else {
			_mushroom->addMonster(_viMonster->x, _viMonster->y);
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		//�����̴ٰ�
		if (_viMonster->patternCount % 100 < 30) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y + 2, _viMonster->width, _viMonster->height);
		}
		//���60�̻��̸� NONE���� ���ư���
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 2;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cmushman::checkAngle()
{
	if (_viMonster->angle > PI_8 && _viMonster->angle <= PI_8 * 3) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTUP;
	}
	else if (_viMonster->angle > PI_8 * 3 && _viMonster->angle <= PI_8 * 5) {
		_viMonster->movestate = MONSTERMOVESTATE::UP;
	}
	else if (_viMonster->angle > PI_8 * 5 && _viMonster->angle <= PI_8 * 7) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTUP;
	}
	else if (_viMonster->angle > PI_8 * 7 && _viMonster->angle <= PI_8 * 9) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFT;
	}
	else if (_viMonster->angle > PI_8 * 9 && _viMonster->angle <= PI_8 * 11) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTDOWN;
	}
	else if (_viMonster->angle > PI_8 * 11 && _viMonster->angle <= PI_8 * 13) {
		_viMonster->movestate = MONSTERMOVESTATE::DOWN;
	}
	else if (_viMonster->angle > PI_8 * 13 && _viMonster->angle <= PI_8 * 15) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTDOWN;
	}
	else {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHT;
	}
}

void Cmushman::giveFrame()
{
	int maxFrameX;
	_viMonster->framecount++;
	//8����� ���¿� ���� Y������ ������
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
	case MONSTERACTIVE::FINDING:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 9;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 7;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 6;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 8;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 6; //����Ʈ
			break;
		}
		maxFrameX = 1;
		break;

	case MONSTERACTIVE::ATTACK:
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::RNDMOVE:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 3;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 1;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 0;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 2;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 0; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::DEATH:
		_viMonster->frameY = 10;
		maxFrameX = 0;
		break;
	}

	//X������ ������1(������ �ƴҶ�)
	if (_viMonster->activestate != MONSTERACTIVE::ATTACK) {
		int currentX = _viMonster->frameX;
		if (_viMonster->framecount % 12 == 0) {
			currentX++;
			if (currentX > maxFrameX) {
				currentX = 0;
			}
			_viMonster->frameX = currentX;
		}
	}
	//x������ ������2(������ ��)
	else {
		if (_viMonster->framecount < 12) {
			_viMonster->frameX = 0;
			_viMonster->frameY = 4;
		}
		else if (_viMonster->framecount < 24) {
			_viMonster->frameX = 1;
			_viMonster->frameY = 4;
		}
		else if (_viMonster->framecount < 36) {
			_viMonster->frameX = 2;
			_viMonster->frameY = 4;
		}
		else if (_viMonster->framecount < 48) {
			_viMonster->frameX = 0;
			_viMonster->frameY = 5;
		}
		else if (_viMonster->framecount < 60) {
			_viMonster->frameX = 2;
			_viMonster->frameY = 4;
		}
		else if (_viMonster->framecount < 72) {
			_viMonster->frameX = 0;
			_viMonster->frameY = 5;
		}
		else if (_viMonster->framecount < 84) {
			_viMonster->frameX = 2;
			_viMonster->frameY = 4;
		}
		else if (_viMonster->framecount < 96) {
			_viMonster->frameX = 0;
			_viMonster->frameY = 5;
		}
		else if (_viMonster->framecount < 108) {
			_viMonster->frameX = 1;
			_viMonster->frameY = 5;
		}
		else {
			_viMonster->frameX = 2;
			_viMonster->frameY = 5;
		}
	}
}

void Cmushman::deathCheck()
{
	if (_viMonster->activestate != MONSTERACTIVE::DEATH && _viMonster->hp <= 0) {
		_viMonster->activestate = MONSTERACTIVE::DEATH;
		_viMonster->patternCount = 0;
	}
}

void Cmushman::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 6, iter->y + 2, iter->width, iter->height);
	if (stun) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cmushman::stuncheck()
{
	if (_viMonster->isStun && _viMonster->activestate != MONSTERACTIVE::DEATH) {
		_viMonster->stunCount++;
		if (_viMonster->stunCount > 20) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
			_viMonster->stunCount = 0;
			_viMonster->patternCount = 400;
			_viMonster->isStun = false;
		}
	}
}

void Cmushman::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		//��ǥ ������ �� ����(��Ÿ� ����� ��������)
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		float tempangle = UTIL::getAngle(_viMonster->x, _viMonster->y, _viMonster->targetX, _viMonster->targetY) - PI;
		if (tempangle < 0) tempangle += PI2;
		if (tempangle > PI2) tempangle -= PI2;
		_viMonster->angle = (tempangle);
	}
	else {
		//��Ÿ����� �ƴ϶�� �ٰ�����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->x, _viMonster->y, _viMonster->targetX, _viMonster->targetY);
	}
	//��� Ư���ϰ�, NONE�϶� FIND�� �ֱ������� ����.
	//���� ����ī��Ʈ�� ���̳Ŀ����� �ٽ� NONE���� ���� ATTACK���� ���� ������.
	if (_viMonster->patternCount < 600)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->activestate = MONSTERACTIVE::NONE;
	}
	else {
		_viMonster->activestate = MONSTERACTIVE::ATTACK;
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
	}
}
//////////////////////////////////////////////////////////////
//////////Cmushman_mushroom!	�ӽ���_�ӽ���!/////////////////
//////////////////////////////////////////////////////////////
Cmushman_mushroom::Cmushman_mushroom()
{
	IMAGE->addFrameImage("�ӽ���_�ӽ���", "images/monsters/mushman_mushroom.bmp", 144, 220, 3, 4, true);
}

Cmushman_mushroom::~Cmushman_mushroom()
{
}

HRESULT Cmushman_mushroom::init()
{
	return S_OK;
}

void Cmushman_mushroom::release()
{
}

void Cmushman_mushroom::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		_viMonster->x = _viMonster->neverchangeX;
		_viMonster->y = _viMonster->neverchangeY;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() * 3 / 8, _viMonster->y + _viMonster->img->getFrameHeight() * 3 / 4, _viMonster->width, _viMonster->height);
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cmushman_mushroom::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
	}
	//TCHAR str[128];
	//_stprintf_s(str, "�÷��̾���ǥ : %.1f, %.1f", PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
}

void Cmushman_mushroom::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�ӽ���_�ӽ���");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.neverchangeX = x;
	newMonster.neverchangeY = y;
	newMonster.width = newMonster.img->getFrameWidth() / 4 + 1;
	newMonster.height = newMonster.img->getFrameHeight() / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() * 3 / 8, y + newMonster.img->getFrameHeight() * 3 / 4, newMonster.width, newMonster.height);
	newMonster.speed = 2;
	newMonster.hp = 36;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1500;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cmushman_mushroom::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount % 70 == 0) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		bm->getHomBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			_viMonster->angle, 0);
		_viMonster->activestate = MONSTERACTIVE::NONE;
		break;
	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount == 60) {
			int snum = RND->getInt(2);
			if (snum == 0) {
				bm->getCirBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right- _viMonster->rc.left)/2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
			else {
				bm->getPsnBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
			_viMonster->afterDeath = true;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 2;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cmushman_mushroom::checkAngle()
{
}

void Cmushman_mushroom::giveFrame()
{
	int maxFrameX;
	_viMonster->framecount++;
	//8����� ���¿� ���� Y������ ������
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
	case MONSTERACTIVE::FINDING:
	case MONSTERACTIVE::ATTACK:
	case MONSTERACTIVE::RNDMOVE:
		_viMonster->frameY = 0;
		maxFrameX = 2;
		break;
	case MONSTERACTIVE::ATTACK2:
		_viMonster->frameY = 3;
		maxFrameX = 2;
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->frameY = 0;
		maxFrameX = 0;
		break;
	}

	//X������ ������1(������ �ƴҶ�)
	if (_viMonster->activestate != MONSTERACTIVE::ATTACK2) {
		int currentX = _viMonster->frameX;
		if (_viMonster->framecount % 12 == 0) {
			currentX++;
			if (currentX > maxFrameX) {
				currentX = 0;
			}
			_viMonster->frameX = currentX;
		}
	}
	//X������ ������2(���߰���)
	else {
		int currentX = _viMonster->frameX;
		if (_viMonster->framecount % 15 == 0) {
			currentX++;
			if (currentX > maxFrameX) {
				currentX = 0;
			}
			_viMonster->frameX = currentX;
		}
	}
}

void Cmushman_mushroom::deathCheck()
{
	if (_viMonster->activestate != MONSTERACTIVE::DEATH && _viMonster->hp <= 0) {
		_viMonster->activestate = MONSTERACTIVE::DEATH;
		_viMonster->patternCount = 0;
	}
}

void Cmushman_mushroom::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() * 3 / 8, iter->y + iter->img->getFrameHeight() * 3 / 4, iter->width, iter->height);
	//������ ������ ���°ɷ�..
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cmushman_mushroom::stuncheck()
{
}

void Cmushman_mushroom::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//��ǥ ������ �� ����
	_viMonster->targetX = PLAYER->getPlayerAddress().x;
	_viMonster->targetY = PLAYER->getPlayerAddress().y;
	_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
		_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
		_viMonster->targetX, _viMonster->targetY);
	if (_viMonster->patternCount >= 350) {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::ATTACK2;
	}
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->activestate = MONSTERACTIVE::ATTACK;
	}
}
//////////////////////////////////////////////////////////////
////////////////Cfairy!				��!///////////////////
//////////////////////////////////////////////////////////////
Cfairy::Cfairy()
{
	IMAGE->addFrameImage("��", "images/monsters/fairy-forest.bmp", 168, 930, 3, 10, true);
}

Cfairy::~Cfairy()
{
}

HRESULT Cfairy::init()
{
	return S_OK;
}

void Cfairy::release()
{
}

void Cfairy::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cfairy::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "����������X : %d", _viMonster->frameX);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cfairy::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("��");
	newMonster.frameX = 1;
	newMonster.frameY = 0;
	newMonster.framecount = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.width = newMonster.img->getFrameWidth() / 3;
	newMonster.height = newMonster.img->getFrameHeight() * 3 / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() / 3, y + newMonster.img->getFrameHeight() / 5, newMonster.width, newMonster.height);
	newMonster.speed = 1;
	newMonster.hp = 77;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI *3 / 2;
	newMonster.range = 450;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.attackNum = 0;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cfairy::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount % 100 == 30)
		{
			_viMonster->framecount = 0;
			_viMonster->frameX = 1;
			_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		}
		if (_viMonster->patternCount > 150) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		if (_viMonster->patternCount < 35) {
			if (_viMonster->patternCount % 30 == 0) {
				bm->getWidBulInstance()->fire(
					_viMonster->rc.left +(_viMonster->rc.right- _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom- _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 1;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount < 35) {
			if (_viMonster->patternCount % 30 == 0) {
				bm->getRtnBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2 + 20,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2 + 20,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 1;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
		}	
		break;
	case MONSTERACTIVE::RNDMOVE:
		if (_viMonster->patternCount < 60) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 3, _viMonster->y + _viMonster->img->getFrameHeight() / 5, _viMonster->width, _viMonster->height);
		}
		else {
			_viMonster->frameX = 1;
			_viMonster->framecount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 2;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cfairy::checkAngle()
{
	if (_viMonster->angle > PI_8 && _viMonster->angle <= PI_8 * 3) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTUP;
	}
	else if (_viMonster->angle > PI_8 * 3 && _viMonster->angle <= PI_8 * 5) {
		_viMonster->movestate = MONSTERMOVESTATE::UP;
	}
	else if (_viMonster->angle > PI_8 * 5 && _viMonster->angle <= PI_8 * 7) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTUP;
	}
	else if (_viMonster->angle > PI_8 * 7 && _viMonster->angle <= PI_8 * 9) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFT;
	}
	else if (_viMonster->angle > PI_8 * 9 && _viMonster->angle <= PI_8 * 11) {
		_viMonster->movestate = MONSTERMOVESTATE::LEFTDOWN;
	}
	else if (_viMonster->angle > PI_8 * 11 && _viMonster->angle <= PI_8 * 13) {
		_viMonster->movestate = MONSTERMOVESTATE::DOWN;
	}
	else if (_viMonster->angle > PI_8 * 13 && _viMonster->angle <= PI_8 * 15) {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHTDOWN;
	}
	else {
		_viMonster->movestate = MONSTERMOVESTATE::RIGHT;
	}
}

void Cfairy::giveFrame()
{
	int maxFrameX;
	_viMonster->framecount++;
	//8����� ���¿� ���� Y������ ������
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
	case MONSTERACTIVE::FINDING:
	case MONSTERACTIVE::RNDMOVE:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 3;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 1;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 0;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 2;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 0; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::ATTACK:
	case MONSTERACTIVE::ATTACK2:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 7;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 5;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 4;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 6;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 4; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;

	case MONSTERACTIVE::DEATH:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 9;
			_viMonster->frameX = 1;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 8;
			_viMonster->frameX = 2;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 8;
			_viMonster->frameX = 1;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 9;
			_viMonster->frameX = 0;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 8; //����Ʈ
			_viMonster->frameX = 1; //����Ʈ
			break;
		}
		maxFrameX = 0;
		break;
	}

	int currentX = _viMonster->frameX;
	//���������϶� x������ ����
	if (_viMonster->activestate == MONSTERACTIVE::NONE||_viMonster->activestate == MONSTERACTIVE::FINDING) {
		_viMonster->frameX = 1;
	}
	//�׾��� �� �ѹ��� ����
	else if (_viMonster->activestate == MONSTERACTIVE::DEATH){
		if (_viMonster->framecount > 12) {
			_viMonster->frameY = 8;
			_viMonster->frameX = 0;
		}
	}
	//�����̰ų� �������� �� X������ ������
	else {
		if (_viMonster->framecount % 12 == 0) {
			currentX++;
			if (currentX > maxFrameX) {
				currentX = 0;
			}
			_viMonster->frameX = currentX;
		}
	}
}

void Cfairy::deathCheck()
{
	if (_viMonster->activestate != MONSTERACTIVE::DEATH && _viMonster->hp <= 0) {
		_viMonster->activestate = MONSTERACTIVE::DEATH;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
	}
}

void Cfairy::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 3, iter->y + iter->img->getFrameHeight() / 5, iter->width, iter->height);
	if (stun) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cfairy::stuncheck()
{
	if (_viMonster->isStun && _viMonster->activestate != MONSTERACTIVE::DEATH) {
		_viMonster->stunCount++;
		if (_viMonster->stunCount > 20) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
			_viMonster->stunCount = 0;
			_viMonster->isStun = false;
		}
	}
}

void Cfairy::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		if (_viMonster->oldactivestate == MONSTERACTIVE::ATTACK && _viMonster->attackNum >= 2) {
			_viMonster->activestate = MONSTERACTIVE::ATTACK2;
			_viMonster->attackNum = 0;
		}
		else {
			_viMonster->activestate = MONSTERACTIVE::ATTACK;
			_viMonster->attackNum++;
		}
		_viMonster->oldactivestate = MONSTERACTIVE::NONE;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->x, _viMonster->y, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� �������� ����.
	else {
		_viMonster->frameX = 1;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		_viMonster->angle = RND->getInt(360) * PI / 180;
	}
}
//////////////////////////////////////////////////////////////
////////////////Cflime!				��!///////////////////////
//////////////////////////////////////////////////////////////
Cflime::Cflime()
{
	IMAGE->addFrameImage("��", "images/monsters/flime-flime.bmp", 240, 549, 3, 9, true);
}

Cflime::~Cflime()
{
}

HRESULT Cflime::init()
{
	return S_OK;
}

void Cflime::release()
{
}

void Cflime::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		_viMonster->x = _viMonster->neverchangeX;
		_viMonster->y = _viMonster->neverchangeY;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() / 4, _viMonster->width, _viMonster->height);;
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cflime::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "����ī��Ʈ : %d", _viMonster->stunCount);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cflime::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("��");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.neverchangeX = x;
	newMonster.neverchangeY = y;
	newMonster.width = newMonster.img->getFrameWidth() / 2;
	newMonster.height = newMonster.img->getFrameHeight() * 3 / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() / 4, y + newMonster.img->getFrameHeight() / 4, newMonster.width, newMonster.height);
	newMonster.speed = 0;
	newMonster.hp = 57;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 450;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.attackNum = 0;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cflime::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount > 140) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		if (_viMonster->patternCount < 280) {
			if (_viMonster->patternCount == 55) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					_viMonster->angle, 0);
			}
			else if (_viMonster->patternCount == 85) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					_viMonster->angle, 0);
			}
			else if (_viMonster->patternCount == 115) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount < 48) {
		}
		else if (_viMonster->patternCount == 48) {
			bm->getPsnBulInstance()->fire(
				_viMonster->x + _viMonster->img->getFrameWidth() / 2,
				_viMonster->y + _viMonster->img->getFrameHeight() / 2,
				_viMonster->angle, 0);
		}
		else if (_viMonster->patternCount < 270) {
			//��� ������ ����������� ���
		}
		else {
			//���ݻ��� ����
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 2;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cflime::giveFrame()
{
	int maxFrameX;
	_viMonster->framecount++;
	//8����� ���¿� ���� Y������ ������
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
	case MONSTERACTIVE::FINDING:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 3;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 1;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 0;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 2;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 0; //����Ʈ
			break;
		}
		maxFrameX = 1;
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::ATTACK:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 7;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 5;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 4;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 6;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 4; //����Ʈ
			break;
		}
		maxFrameX = 2;
		break;
	case MONSTERACTIVE::ATTACK2:
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			_viMonster->frameY = 3;
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			_viMonster->frameY = 1;
			break;
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			_viMonster->frameY = 0;
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			_viMonster->frameY = 2;
			break;
		case MONSTERMOVESTATE::NONE:
			_viMonster->frameY = 0; //����Ʈ
			break;
		}
		maxFrameX = 1;
		break;

	case MONSTERACTIVE::DEATH:
		_viMonster->frameY = 8;
		maxFrameX = 0;
		break;
	}

	int currentX = _viMonster->frameX;
	//�����Ѹ��ȱ� ����. ������ �����ӿ��� �����ֱ�.
	if (_viMonster->activestate == MONSTERACTIVE::ATTACK)
	{
		if (_viMonster->patternCount < 250) {
			if (_viMonster->framecount % 15 == 0) {
				if (currentX < maxFrameX) {
					currentX++;
					_viMonster->frameX = currentX;
				}
				else {
					_viMonster->framecount = 0;
				}
			}
		}
		else {
			//�����Ӱ���
			if (_viMonster->framecount % 15 == 0) {
				currentX--;
				_viMonster->frameX = currentX;
				if (currentX < 0) {
					_viMonster->frameX = 0;
				}
			}
		}
	}
	else if (_viMonster->activestate == MONSTERACTIVE::ATTACK2)
	{
		//����2 ���°� ���ڸ��� ������� ���
		if (_viMonster->patternCount < 40) {
			_viMonster->frameX = 2;
		}
		else if (_viMonster->patternCount < 60) {
			_viMonster->frameX = 1;
		}
		else{
			if (_viMonster->framecount % 15 == 0) {
				currentX++;
				if (currentX > maxFrameX) {
					currentX = 0;
				}
				_viMonster->frameX = currentX;
			}
		}
	}
	//NONE�̰ų� DEATH�϶� ����������.
	else {
		if (_viMonster->framecount % 15 == 0) {
			currentX++;
			if (currentX > maxFrameX) {
				currentX = 0;
			}
			_viMonster->frameX = currentX;
		}
	}
}

void Cflime::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 4, iter->y + iter->img->getFrameHeight() / 4, iter->width, iter->height);
	if (stun) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cflime::stuncheck()
{
	if (_viMonster->isStun && _viMonster->activestate!= MONSTERACTIVE::DEATH) {
		_viMonster->stunCount++;
		if (_viMonster->stunCount > 20) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
			_viMonster->stunCount = 0;
			_viMonster->isStun = false;
		}
	}
}

void Cflime::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		if (_viMonster->oldactivestate == MONSTERACTIVE::ATTACK && _viMonster->attackNum >= 2) {
			_viMonster->activestate = MONSTERACTIVE::ATTACK2;
			_viMonster->attackNum = 0;
		}
		else {
			_viMonster->activestate = MONSTERACTIVE::ATTACK;
			_viMonster->attackNum++;
		}
		_viMonster->oldactivestate = MONSTERACTIVE::NONE;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->x, _viMonster->y, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� NONE ����.
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::NONE;
		_viMonster->angle = DEGREE(270);
	}
}
//////////////////////////////////////////////////////////////
//////////Cboss_slime!		����������!///////////////////////
//////////////////////////////////////////////////////////////
Cboss_slime::Cboss_slime()
{
	IMAGE->addFrameImage("����������", "images/monsters/boss_KingSlime.bmp", 576*2, 2496*2, 3, 13, true);
}

Cboss_slime::~Cboss_slime()
{
}

HRESULT Cboss_slime::init()
{
	return S_OK;
}

void Cboss_slime::release()
{
}

void Cboss_slime::update(Cplayer* py, bulletManager* bm, Csemiboss_slime* ss)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		//stuncheck();
		checkPlayerXY(py);
		move(bm, ss);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cboss_slime::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->aniRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->ani);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "����������X : %d", _viMonster->frameX);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cboss_slime::addMonster(float centerx, float centery)
{

	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("����������");
	newMonster.ani = ANIMATION->addNoneKeyAnimation("����������", 0, 2, 8, true, true);
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.width = newMonster.img->getFrameWidth() / 2;
	newMonster.height = newMonster.img->getFrameHeight() / 3;
	newMonster.x = centerx - newMonster.img->getFrameWidth() / 2 ;
	newMonster.y = centery - newMonster.img->getFrameHeight() * 3 / 4;
	newMonster.rc = RectMake(newMonster.x + newMonster.img->getFrameWidth() / 4, newMonster.y + newMonster.img->getFrameHeight() * 2 / 3, newMonster.width, newMonster.height);
	newMonster.speed = 2;
	newMonster.hp = 220;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1000;
	newMonster.framecount = 0;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.attackNum = 2;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cboss_slime::move(bulletManager* bm, Csemiboss_slime* ss)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount > 30 && _viMonster->patternCount < 60) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
		}
		if (_viMonster->patternCount >= 120) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
		_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
		if (_viMonster->patternCount < 35) {
			if (_viMonster->patternCount % 30 == 0) {
				bm->getCirBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right- _viMonster->rc.left)/2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 30);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount < 110) {
			if (_viMonster->patternCount == 15) {
				_viMonster->y -= 100;
			}
			if (_viMonster->patternCount == 30) {
				_viMonster->y += 100;
			}
			if (_viMonster->patternCount == 45) {
				_viMonster->y -= 100;
			}
			if (_viMonster->patternCount == 60) {
				_viMonster->y += 100;
			}
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
			if (_viMonster->patternCount % 100 == 0) {
				bm->getSlmBos1Bullnstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		if (_viMonster->patternCount < 35) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			ss->addMonster(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2 - 30,
				_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2);
			ss->addMonster(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2 + 30,
				_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2);
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cboss_slime::giveFrame()
{
	if (_viMonster->framecount == 0) {
		//8����� ���¿� ���� �ִϸ��̼� ������
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 9, 11, 8, true, true);
				break;
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 3, 5, 8, true, true);
				break;
			case MONSTERMOVESTATE::NONE:
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 0, 2, 8, true, true);
				break;
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 6, 8, 8, true, true);
				break;
			}
			//maxFrameX = 2;
			break;

		case MONSTERACTIVE::ATTACK:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 33, 35, 5, false, false);
				break;											
			case MONSTERMOVESTATE::LEFTUP:						
			case MONSTERMOVESTATE::LEFT:						
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 27, 29, 5, false, false);
				break;												
			case MONSTERMOVESTATE::NONE:							
			case MONSTERMOVESTATE::LEFTDOWN:						
			case MONSTERMOVESTATE::DOWN:							
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 24, 26, 5, false, false);
				break;											
			case MONSTERMOVESTATE::RIGHTDOWN:					
			case MONSTERMOVESTATE::RIGHT:						
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 30, 32, 5, false, false);
				break;
			}
			//maxFrameX = 2;
			break;

		case MONSTERACTIVE::DEATH:
			_viMonster->frameY = 12;
			_viMonster->frameX = 0;
			break;
		}
	}

	if (_viMonster->activestate == MONSTERACTIVE::ATTACK2 && _viMonster->framecount == 75) {
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 21, 23, 5, false, false);
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 15, 17, 5, false, false);
			break;
		case MONSTERMOVESTATE::NONE:
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 12, 14, 5, false, false);
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "����������", 18, 20, 5, false, false);
			break;
		}
	}

	//������ī��Ʈ����(���º�ȭ �˸��ܿ� ��� ����)
	_viMonster->framecount++;

}

void Cboss_slime::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 4, iter->y + iter->img->getFrameHeight() * 2 / 3, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cboss_slime::stuncheck(){}

void Cboss_slime::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		if (_viMonster->attackNum == 3 && _viMonster->oldactivestate == MONSTERACTIVE::ATTACK) {
			_viMonster->attackNum = 0;
			_viMonster->activestate = MONSTERACTIVE::ATTACK2;
		}
		else {
			_viMonster->attackNum++;
			_viMonster->activestate = MONSTERACTIVE::ATTACK;
		}
		_viMonster->oldactivestate = MONSTERACTIVE::NONE;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� �������� ����.
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		_viMonster->angle = RND->getInt(360) * PI / 180;
	}
}
//////////////////////////////////////////////////////////////
//////////Csemiboss_slime!		�غ���������!//////////////////
//////////////////////////////////////////////////////////////
Csemiboss_slime::Csemiboss_slime()
{
	IMAGE->addFrameImage("�غ���������", "images/monsters/boss_KingSlime.bmp", 576, 2496, 3, 13, true);
}

Csemiboss_slime::~Csemiboss_slime()
{
}

HRESULT Csemiboss_slime::init()
{
	return S_OK;
}

void Csemiboss_slime::release()
{
}

void Csemiboss_slime::update(Cplayer* py, bulletManager* bm, Cslime* slm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		//stuncheck();
		checkPlayerXY(py);
		move(bm, slm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Csemiboss_slime::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->aniRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->ani);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "����������X : %d", _viMonster->frameX);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Csemiboss_slime::addMonster(float centerx, float centery)
{

	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�غ���������");
	newMonster.ani = ANIMATION->addNoneKeyAnimation("�غ���������", 0, 2, 8, true, true);
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.width = newMonster.img->getFrameWidth() / 2;
	newMonster.height = newMonster.img->getFrameHeight() / 3;
	newMonster.x = centerx - newMonster.img->getFrameWidth() / 2;
	newMonster.y = centery - newMonster.img->getFrameHeight() * 3 / 4;
	newMonster.rc = RectMake(newMonster.x + newMonster.img->getFrameWidth() / 4, newMonster.y + newMonster.img->getFrameHeight() * 2 / 3, newMonster.width, newMonster.height);
	newMonster.speed = 2;
	newMonster.hp = 70;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1000;
	newMonster.framecount = 0;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.attackNum = 0;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::ATTACK;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Csemiboss_slime::move(bulletManager* bm, Cslime* slm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount > 30 && _viMonster->patternCount < 60) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
		}
		if (_viMonster->patternCount >= 100) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
		_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
		if (_viMonster->patternCount < 35) {
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		if (_viMonster->patternCount < 110) {
			if (_viMonster->patternCount == 15) {
				_viMonster->y -= 100;
			}
			if (_viMonster->patternCount == 30) {
				_viMonster->y += 100;
			}
			if (_viMonster->patternCount == 45) {
				_viMonster->y -= 100;
			}
			if (_viMonster->patternCount == 60) {
				_viMonster->y += 100;
			}
			if (_viMonster->patternCount == 75) {
				_viMonster->y -= 100;
			}
			if (_viMonster->patternCount == 90) {
				_viMonster->y += 100;
			}
			if (_viMonster->patternCount > 75 && _viMonster->patternCount < 100) {
				_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed*2;
				_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed*2;
			}
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
			if (_viMonster->patternCount % 100 == 0) {
				//��ǥ ������ �� ����
				_viMonster->targetX = PLAYER->getPlayerAddress().x;
				_viMonster->targetY = PLAYER->getPlayerAddress().y;
				_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2, _viMonster->targetX, _viMonster->targetY);
				bm->getCirBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		if (_viMonster->patternCount < 35) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 4, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 3, _viMonster->width, _viMonster->height);
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			slm->addMonster(_viMonster->rc.left+(_viMonster->rc.right- _viMonster->rc.left)/2 - 30, 
				_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2-10);
			slm->addMonster(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2 + 30,
				_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2+10);
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Csemiboss_slime::giveFrame()
{
	if (_viMonster->framecount == 0) {
		//8����� ���¿� ���� �ִϸ��̼� ������
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 9, 11, 8, true, true);
				break;
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 3, 5, 8, true, true);
				break;
			case MONSTERMOVESTATE::NONE:
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 0, 2, 8, true, true);
				break;
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 6, 8, 8, true, true);
				break;
			}
			//maxFrameX = 2;
			break;

		case MONSTERACTIVE::ATTACK:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 33, 35, 5, false, false);
				break;
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 27, 29, 5, false, false);
				break;
			case MONSTERMOVESTATE::NONE:
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 24, 26, 5, false, false);
				break;
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 30, 32, 5, false, false);
				break;
			}
			//maxFrameX = 2;
			break;

		case MONSTERACTIVE::DEATH:
			_viMonster->frameY = 12;
			_viMonster->frameX = 0;
			break;
		}
	}

	if (_viMonster->activestate == MONSTERACTIVE::ATTACK2 && _viMonster->framecount == 75) {
		switch (_viMonster->movestate)
		{
		case MONSTERMOVESTATE::RIGHTUP:
		case MONSTERMOVESTATE::UP:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 21, 23, 5, false, false);
			break;
		case MONSTERMOVESTATE::LEFTUP:
		case MONSTERMOVESTATE::LEFT:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 15, 17, 5, false, false);
			break;
		case MONSTERMOVESTATE::NONE:
		case MONSTERMOVESTATE::LEFTDOWN:
		case MONSTERMOVESTATE::DOWN:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 12, 14, 5, false, false);
			break;
		case MONSTERMOVESTATE::RIGHTDOWN:
		case MONSTERMOVESTATE::RIGHT:
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�غ���������", 18, 20, 5, false, false);
			break;
		}
	}

	//������ī��Ʈ����(���º�ȭ �˸��ܿ� ��� ����)
	_viMonster->framecount++;

}

void Csemiboss_slime::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 4, iter->y + iter->img->getFrameHeight() * 2 / 3, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Csemiboss_slime::stuncheck() {}

void Csemiboss_slime::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		if (_viMonster->oldactivestate == MONSTERACTIVE::ATTACK) {
			_viMonster->activestate = MONSTERACTIVE::ATTACK2;
		}
		else {
			_viMonster->activestate = MONSTERACTIVE::ATTACK;
		}
		_viMonster->oldactivestate = MONSTERACTIVE::NONE;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� �������� ����.
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		_viMonster->angle = RND->getInt(360) * PI / 180;
	}
}
//////////////////////////////////////////////////////////////
//////////Cboss_flime!			�����ö���!////////////////////
//////////////////////////////////////////////////////////////
Cboss_flime::Cboss_flime()
{
	IMAGE->addFrameImage("�����ö���", "images/monsters/flime-bossFlime.bmp", 750, 1710, 3, 9, true);
}

Cboss_flime::~Cboss_flime()
{
}

HRESULT Cboss_flime::init()
{
	return S_OK;
}

void Cboss_flime::release()
{
}

void Cboss_flime::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		//stuncheck();
		_viMonster->x = _viMonster->neverchangeX;
		_viMonster->y = _viMonster->neverchangeY;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y + _viMonster->img->getFrameHeight() * 2 / 7, _viMonster->width, _viMonster->height);
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cboss_flime::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->aniRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->ani);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "hp : %d", _viMonster->hp);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cboss_flime::addMonster(float centerx, float centery)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�����ö���");
	newMonster.ani = ANIMATION->addNoneKeyAnimation("�����ö���", 1, 2, 3, true, true);
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.width = newMonster.img->getFrameWidth() * 2 / 3;
	newMonster.height = newMonster.img->getFrameHeight() * 2 / 3;
	newMonster.x = centerx - newMonster.img->getFrameWidth() / 2;
	newMonster.y = centery - newMonster.img->getFrameHeight() * 2 / 3;
	newMonster.neverchangeX = newMonster.x;
	newMonster.neverchangeY = newMonster.y;
	newMonster.rc = RectMake(newMonster.x + newMonster.img->getFrameWidth() / 6, newMonster.y + newMonster.img->getFrameHeight() * 2 / 7, newMonster.width, newMonster.height);
	newMonster.speed = 0;
	newMonster.hp = 450;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1000;
	newMonster.framecount = 0;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.attackNum = 0;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cboss_flime::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount > 100) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		//���� �������� ���ÿ� �߻� (������ 3��)
		if (_viMonster->patternCount < 280) {
			if (_viMonster->patternCount == 55) {
				bm->getPsnBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
				bm->getCirBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left)/2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
			else if (_viMonster->patternCount == 85) {
				bm->getCirBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle + 0.3, 0);
			}
			else if (_viMonster->patternCount == 115) {
				bm->getCirBulInstance()->fire(
					_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
					_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
					_viMonster->angle, 0);
			}
		}
		else if (_viMonster->patternCount >= 280 && _viMonster->patternCount < 350) {
			//�޽�
		}
		else {
			_viMonster->frameX = 0;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		//�����ϳ�. ���ð��� �Ǵ� �����ٱ�.
		if (_viMonster->attackNum == 1) {
			//���ð�����
			if (_viMonster->patternCount == 55) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					0, 0);
			}
			else if (_viMonster->patternCount == 155) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					0, 0);
			}
			else if (_viMonster->patternCount == 255) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					0, 0);
			}
			else if (_viMonster->patternCount < 355) {
				//�޽�
			}
			else {
				//���ݻ��� ����
				_viMonster->frameX = 0;
				_viMonster->framecount = 0;
				_viMonster->patternCount = 0;
				_viMonster->activestate = MONSTERACTIVE::NONE;
				_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
			}
		}
		else {
			//���� �ٱ��
			if (_viMonster->patternCount == 55) {
				bm->getnidBulInstance()->fire(
					PLAYER->getPlayerAddress().x,
					PLAYER->getPlayerAddress().y,
					0, 0);
			}
			else if (_viMonster->patternCount < 355) {
				//�޽�
			}
			else {
				//���ݻ��� ����
				_viMonster->frameX = 0;
				_viMonster->framecount = 0;
				_viMonster->patternCount = 0;
				_viMonster->activestate = MONSTERACTIVE::NONE;
				_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
			}
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cboss_flime::giveFrame()
{
	if (_viMonster->framecount == 0) {
		//8����� ���¿� ���� �ִϸ��̼� ������
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
			case MONSTERMOVESTATE::NONE:
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ö���", 1, 2, 3, true, true);
				break;
			}
			break;

		case MONSTERACTIVE::DEATH:
			_viMonster->frameY = 8;
			_viMonster->frameX = 0;
			break;
		}
	}

	//ATTACK1 : �������������϶� �պκи� ������ ����
	if (_viMonster->activestate == MONSTERACTIVE::ATTACK) {
		if (_viMonster->framecount == 0) {
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ö���", 2, 0, 4, true, true);
		}
		else if(_viMonster->framecount == 200){
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ö���", 1, 2, 3, true, true);
		}
	}
	//ATTACK2 : �������������϶� �պκи� ������ ����
	if (_viMonster->activestate == MONSTERACTIVE::ATTACK2) {
		if (_viMonster->framecount == 0) {
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ö���", 12, 14, 3, false, false);
		}
		else if (_viMonster->framecount == 300) {
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ö���", 14, 12, 3, false, false);
		}
	}

	//������ī��Ʈ����(���º�ȭ �˸��ܿ� ��� ����)
	_viMonster->framecount++;

}

void Cboss_flime::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 6, iter->y + iter->img->getFrameHeight() * 2 / 7, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cboss_flime::stuncheck()
{
}

void Cboss_flime::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//�÷��̾��� ��ǥ�� Ȯ���� ��Ÿ����� ���Դٸ� ���ݻ��µ���.
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	if (distance < _viMonster->range)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		if (_viMonster->oldactivestate == MONSTERACTIVE::ATTACK) {
			_viMonster->activestate = MONSTERACTIVE::ATTACK2;
			_viMonster->attackNum++;
			if (_viMonster->attackNum == 3) _viMonster->attackNum = 1;
		}
		else {
			_viMonster->activestate = MONSTERACTIVE::ATTACK;
		}
		_viMonster->oldactivestate = MONSTERACTIVE::NONE;
		//��ǥ ������ �� ����
		_viMonster->targetX = PLAYER->getPlayerAddress().x;
		_viMonster->targetY = PLAYER->getPlayerAddress().y;
		_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2, _viMonster->targetX, _viMonster->targetY);
	}
	//��Ÿ����� �ƴ϶�� �������� ����.
	else {
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->patternCount = 0;
		_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
		_viMonster->angle = RND->getInt(360) * PI / 180;
	}
}
//////////////////////////////////////////////////////////////
//////////Cboss_mushmam!			�����ӽ���!////////////////
//////////////////////////////////////////////////////////////
Cboss_mushmam::Cboss_mushmam()
{
	IMAGE->addFrameImage("�����ӽ���", "images/monsters/mushmam-mushmam.bmp", 432, 2475, 3, 15, true);
}

Cboss_mushmam::~Cboss_mushmam()
{
}

HRESULT Cboss_mushmam::init()
{
	return S_OK;
}

void Cboss_mushmam::release()
{
}

void Cboss_mushmam::update(Cplayer* py, bulletManager* bm, Cmushmam_mushroom_G* mrG, Cmushmam_mushroom_P* mrP, Cmushmam_mushroom_B* mrB, Cmushman_mushroom* smr)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		//stuncheck();
		checkPlayerXY(py);
		move(bm, mrG, mrP, mrB, smr);
		checkAngle();
		giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cboss_mushmam::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->aniRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->ani);
			//TCHAR str[128];
			//_stprintf_s(str, "hp : %d", _viMonster->hp);
			//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
	}
}

void Cboss_mushmam::addMonster(float centerx, float centery)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�����ӽ���");
	newMonster.ani = ANIMATION->addNoneKeyAnimation("�����ӽ���", 18, 19, 3, true, true);
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.width = newMonster.img->getFrameWidth() * 2 / 3;
	newMonster.height = newMonster.img->getFrameHeight() * 3 / 4;
	newMonster.x = centerx - newMonster.img->getFrameWidth() / 2;
	newMonster.y = centery - newMonster.img->getFrameHeight() / 2;
	newMonster.rc = RectMake(newMonster.x + newMonster.img->getFrameWidth() / 6, newMonster.y + 2, newMonster.width, newMonster.height);
	newMonster.speed = 2;
	newMonster.hp = 450;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 340;
	newMonster.framecount = 0;
	newMonster.stunCount = 0;
	newMonster.patternCount = 300;
	newMonster.deathalpha = 255;
	newMonster.attackNum = 1;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cboss_mushmam::move(bulletManager* bm, Cmushmam_mushroom_G* mrG, Cmushmam_mushroom_P* mrP, Cmushmam_mushroom_B* mrB, Cmushman_mushroom* smr)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount < 550) {
			//���30�̻��̸� RNDMOVE�� ����
			if (_viMonster->patternCount % 100 > 0 && _viMonster->patternCount % 100 < 30) {
				_viMonster->frameX = 0;
				_viMonster->framecount = 0;
				_viMonster->activestate = MONSTERACTIVE::RNDMOVE;
			}
			//����� �� ����Ȯ���ϰ� �� �ٲٱ�
			if (_viMonster->patternCount % 100 == 0)
				_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		else _viMonster->activestate = MONSTERACTIVE::FINDING;
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		//�ɱ�
		if (_viMonster->patternCount < 150) {
		}
		else {
			if(color == 0) mrG->addMonster(_viMonster->x, _viMonster->y);
			else if (color == 1) mrP->addMonster(_viMonster->x, _viMonster->y);
			else mrB->addMonster(_viMonster->x, _viMonster->y);			
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::ATTACK2:
		//�Ѹ���
		if (_viMonster->patternCount < 120) {
		}
		else {
			smr->addMonster(_viMonster->x-20, _viMonster->y);
			smr->addMonster(_viMonster->x+120, _viMonster->y);
			smr->addMonster(_viMonster->x+60, _viMonster->y+70);
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
			_viMonster->oldactivestate = MONSTERACTIVE::ATTACK2;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;
	case MONSTERACTIVE::RNDMOVE:
		//�����̴ٰ�
		if (_viMonster->patternCount % 100 < 30) {
			_viMonster->x += cosf(_viMonster->angle) * _viMonster->speed;
			_viMonster->y -= sinf(_viMonster->angle) * _viMonster->speed;
			_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() / 6, _viMonster->y + 2, _viMonster->width, _viMonster->height);
		}
		//���30�̻��̸� NONE���� ���ư���
		else {
			_viMonster->framecount = 0;
			_viMonster->activestate = MONSTERACTIVE::NONE;
		}
		break;

	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cboss_mushmam::giveFrame()
{
	if (_viMonster->framecount == 0) {
		//8����� ���¿� ���� �ִϸ��̼� ������
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 27, 28, 3, false, true);
				break;
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 21, 22, 3, false, true);
				break;
			case MONSTERMOVESTATE::NONE:
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 18, 19, 3, false, true);
				break;
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 24, 25, 3, false, true);
				break;
			}
			break;
		case MONSTERACTIVE::RNDMOVE:
			switch (_viMonster->movestate)
			{
			case MONSTERMOVESTATE::RIGHTUP:
			case MONSTERMOVESTATE::UP:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 9, 11, 3, true, true);
				break;
			case MONSTERMOVESTATE::LEFTUP:
			case MONSTERMOVESTATE::LEFT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 3, 5, 3, true, true);
				break;
			case MONSTERMOVESTATE::NONE:
			case MONSTERMOVESTATE::LEFTDOWN:
			case MONSTERMOVESTATE::DOWN:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 0, 2, 3, true, true);
				break;
			case MONSTERMOVESTATE::RIGHTDOWN:
			case MONSTERMOVESTATE::RIGHT:
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", 6, 8, 3, true, true);
				break;
			}
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->frameY = 10;
			_viMonster->frameX = 0;
			break;
		}
	}


	//ATTACK1 : �ɱ�
	if (_viMonster->activestate == MONSTERACTIVE::ATTACK) {
		if (_viMonster->framecount == 0) {
			if (color == 0) {
				int arrAtk1[] = { 12, 13, 14, 15, 14, 15, 14, 15, 16, 17 };
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", arrAtk1, (int)sizeof(arrAtk1) / sizeof(int), 4, false);
			}
			else if (color == 1) {
				int arrAtk1[] = { 33, 34, 35, 36, 35, 36, 35, 36, 37, 38 };
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", arrAtk1, (int)sizeof(arrAtk1) / sizeof(int), 4, false);
			}
			else {
				int arrAtk1[] = { 39, 40, 41, 42, 41, 42, 41, 42, 43, 44 };
				ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", arrAtk1, (int)sizeof(arrAtk1) / sizeof(int), 4, false);
			}			
		}
	}
	//ATTACK2 : �Ѹ���
	if (_viMonster->activestate == MONSTERACTIVE::ATTACK2) {
		if (_viMonster->framecount == 0) {
			int arrAtk2[] = { 13, 13, 12, 23 };
			ANIMATION->changeNonKeyAnimation(_viMonster->ani, "�����ӽ���", arrAtk2, (int)sizeof(arrAtk2) / sizeof(int), 3, false);
		}
	}

	//������ī��Ʈ����(���º�ȭ �˸��ܿ� ��� ����)
	_viMonster->framecount++;

}

void Cboss_mushmam::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() / 6, iter->y + 2, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cboss_mushmam::stuncheck()
{
}

void Cboss_mushmam::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	float distance = UTIL::getDistance(_viMonster->x, _viMonster->y, PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	float angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
		_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
		PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);

	//�Ÿ��� �����ٸ�, �־����� ������ �������� ���ؼ� ����.
	if (distance < _viMonster->range){
		//�÷��̾ ���ϴ� ���� ����ϰ�, ��45������ �������� �̴´�. 180�� ���ؼ� ������ �����´�.
		_viMonster->angle = PI + DEGREE(RND->getFromInTo((int)(angle * 180 / PI) - 45, (int)(angle * 180 / PI) + 45));
		//����.
		while (_viMonster->angle < 0) _viMonster->angle += PI2;
		while (_viMonster->angle > PI2) _viMonster->angle -= PI2;
	}
	//�Ÿ��� �ִٸ�, ��Ÿ����� �ٰ�����.
	else {
		//�÷��̾ ���� �ٰ�����.
		_viMonster->angle = angle;
	}


	//��� Ư���ϰ�, NONE�϶� FIND�� �ֱ������� ����.
	//���� ����ī��Ʈ�� ���̳Ŀ����� �ٽ� NONE���� ���� ATTACK���� ���� ������.
	if (_viMonster->patternCount < 550)
	{
		_viMonster->frameX = 0;
		_viMonster->framecount = 0;
		_viMonster->activestate = MONSTERACTIVE::NONE;
	}
	else {
		if (_viMonster->attackNum == 2) {
			_viMonster->attackNum = 0;
			_viMonster->activestate = MONSTERACTIVE::ATTACK2;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
		}
		else {
			color = RND->getInt(3);		//0�ʷ�, 1����, 2�Ķ�
			_viMonster->attackNum++;
			_viMonster->activestate = MONSTERACTIVE::ATTACK;
			_viMonster->framecount = 0;
			_viMonster->patternCount = 0;
		}
	}
	
}
//////////////////////////////////////////////////////////////
///////Cmushmam_mushroom_G!		�ӽ���_����ӽ���!/////////////
//////////////////////////////////////////////////////////////
Cmushmam_mushroom_G::Cmushmam_mushroom_G()
{
	IMAGE->addFrameImage("�ӽ���_����ӽ���", "images/monsters/mushmam_mushroom1.bmp", 432, 660, 3, 4, true);
}

Cmushmam_mushroom_G::~Cmushmam_mushroom_G()
{
}

HRESULT Cmushmam_mushroom_G::init()
{
	return S_OK;
}

void Cmushmam_mushroom_G::release()
{
}

void Cmushmam_mushroom_G::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		_viMonster->x = _viMonster->neverchangeX;
		_viMonster->y = _viMonster->neverchangeY;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() * 3 / 8, _viMonster->y + _viMonster->img->getFrameHeight() * 3 / 4, _viMonster->width, _viMonster->height);
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		//giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cmushmam_mushroom_G::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
	}
	//TCHAR str[128];
	//_stprintf_s(str, "�÷��̾���ǥ : %.1f, %.1f", PLAYER->getPlayerAddress().x, PLAYER->getPlayerAddress().y);
	//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
}

void Cmushmam_mushroom_G::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�ӽ���_����ӽ���");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.neverchangeX = x;
	newMonster.neverchangeY = y;
	newMonster.width = newMonster.img->getFrameWidth() / 4 + 1;
	newMonster.height = newMonster.img->getFrameHeight() / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() * 3 / 8, y + newMonster.img->getFrameHeight() * 3 / 4, newMonster.width, newMonster.height);
	newMonster.speed = 0;
	newMonster.hp = 50;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1500;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cmushmam_mushroom_G::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount > 100) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		bm->getHomBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			DEGREE(45), 0);
		bm->getHomBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			DEGREE(135), 0);
		bm->getHomBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			DEGREE(225), 0);
		bm->getHomBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			DEGREE(315), 0);
		_viMonster->activestate = MONSTERACTIVE::NONE;
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cmushmam_mushroom_G::giveFrame()
{
}

void Cmushmam_mushroom_G::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() * 3 / 8, y + iter->img->getFrameHeight() * 3 / 4, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cmushmam_mushroom_G::stuncheck()
{
}

void Cmushmam_mushroom_G::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//��ǥ ������ �� ����
	_viMonster->targetX = PLAYER->getPlayerAddress().x;
	_viMonster->targetY = PLAYER->getPlayerAddress().y;
	_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
		_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
		_viMonster->targetX, _viMonster->targetY);

	_viMonster->patternCount = 0;
	_viMonster->activestate = MONSTERACTIVE::ATTACK;
}
//////////////////////////////////////////////////////////////
///////Cmushmam_mushroom_P!		�ӽ���_����ӽ���!/////////////
//////////////////////////////////////////////////////////////
Cmushmam_mushroom_P::Cmushmam_mushroom_P()
{
	IMAGE->addFrameImage("�ӽ���_����ӽ���", "images/monsters/mushmam_mushroom2.bmp", 432, 660, 3, 4, true);
}

Cmushmam_mushroom_P::~Cmushmam_mushroom_P()
{
}

HRESULT Cmushmam_mushroom_P::init()
{
	return S_OK;
}

void Cmushmam_mushroom_P::release()
{
}

void Cmushmam_mushroom_P::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		_viMonster->x = _viMonster->neverchangeX;
		_viMonster->y = _viMonster->neverchangeY;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() * 3 / 8, _viMonster->y + _viMonster->img->getFrameHeight() * 3 / 4, _viMonster->width, _viMonster->height);
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		//giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cmushmam_mushroom_P::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "hp : %d", _viMonster->hp);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cmushmam_mushroom_P::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�ӽ���_����ӽ���");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.neverchangeX = x;
	newMonster.neverchangeY = y;
	newMonster.width = newMonster.img->getFrameWidth() / 4 + 1;
	newMonster.height = newMonster.img->getFrameHeight() / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() * 3 / 8, y + newMonster.img->getFrameHeight() * 3 / 4, newMonster.width, newMonster.height);
	newMonster.speed = 0;
	newMonster.hp = 50;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1500;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cmushmam_mushroom_P::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount % 350 == 0) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		bm->getPsnBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			_viMonster->angle, 0);
		_viMonster->activestate = MONSTERACTIVE::NONE;
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cmushmam_mushroom_P::giveFrame()
{
}

void Cmushmam_mushroom_P::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() * 3 / 8, y + iter->img->getFrameHeight() * 3 / 4, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cmushmam_mushroom_P::stuncheck()
{
}

void Cmushmam_mushroom_P::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//��ǥ ������ �� ����
	_viMonster->targetX = PLAYER->getPlayerAddress().x;
	_viMonster->targetY = PLAYER->getPlayerAddress().y;
	_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
		_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
		_viMonster->targetX, _viMonster->targetY);

	_viMonster->patternCount = 0;
	_viMonster->activestate = MONSTERACTIVE::ATTACK;
}
//////////////////////////////////////////////////////////////
///////Cmushmam_mushroom_B!		�ӽ���_�Ķ��ӽ���!/////////////
//////////////////////////////////////////////////////////////
Cmushmam_mushroom_B::Cmushmam_mushroom_B()
{
	IMAGE->addFrameImage("�ӽ���_�Ķ��ӽ���", "images/monsters/mushmam_mushroom3.bmp", 432, 660, 3, 4, true);
}

Cmushmam_mushroom_B::~Cmushmam_mushroom_B()
{
}

HRESULT Cmushmam_mushroom_B::init()
{
	return S_OK;
}

void Cmushmam_mushroom_B::release()
{
}

void Cmushmam_mushroom_B::update(Cplayer* py, bulletManager* bm)
{
	//����
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		_viMonster->x = _viMonster->neverchangeX;
		_viMonster->y = _viMonster->neverchangeY;
		_viMonster->rc = RectMake(_viMonster->x + _viMonster->img->getFrameWidth() * 3 / 8, _viMonster->y + _viMonster->img->getFrameHeight() * 3 / 4, _viMonster->width, _viMonster->height);
		stuncheck();
		checkPlayerXY(py);
		move(bm);
		checkAngle();
		//giveFrame();
		deathCheck();
	}
	//�Ҹ�
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); )
	{
		if (_viMonster->afterDeath) {
			_viMonster = _vMonster.erase(_viMonster);
		}
		else {
			++_viMonster;
		}
	}
}

void Cmushmam_mushroom_B::render()
{
	for (_viMonster = _vMonster.begin(); _viMonster != _vMonster.end(); ++_viMonster)
	{
		switch (_viMonster->activestate)
		{
		case MONSTERACTIVE::NONE:
		case MONSTERACTIVE::FINDING:
		case MONSTERACTIVE::RNDMOVE:
		case MONSTERACTIVE::ATTACK:
		case MONSTERACTIVE::ATTACK2:
			if (_isDebug) RectangleMake(getMemDC(), _viMonster->rc);
			_viMonster->img->frameRender(getMemDC(), _viMonster->x, _viMonster->y, _viMonster->frameX, _viMonster->frameY);
			break;
		case MONSTERACTIVE::DEATH:
			_viMonster->img->alphaFrameRender(getMemDC(), _viMonster->x, _viMonster->y,
				_viMonster->frameX, _viMonster->frameY, _viMonster->deathalpha);
			break;
		}
		//TCHAR str[128];
		//_stprintf_s(str, "hp : %d", _viMonster->hp);
		//TextOut(getMemDC(), 100, 0, str, lstrlen(str));
	}
}

void Cmushmam_mushroom_B::addMonster(float x, float y)
{
	tagMonster newMonster;
	newMonster.img = IMAGE->findImage("�ӽ���_�Ķ��ӽ���");
	newMonster.frameX = 0;
	newMonster.frameY = 0;
	newMonster.x = x;
	newMonster.y = y;
	newMonster.neverchangeX = x;
	newMonster.neverchangeY = y;
	newMonster.width = newMonster.img->getFrameWidth() / 4 + 1;
	newMonster.height = newMonster.img->getFrameHeight() / 4;
	newMonster.rc = RectMake(x + newMonster.img->getFrameWidth() * 3 / 8, y + newMonster.img->getFrameHeight() * 3 / 4, newMonster.width, newMonster.height);
	newMonster.speed = 0;
	newMonster.hp = 50;
	newMonster.isStun = false;
	newMonster.isDeath = false;
	newMonster.findPlayer = false;
	newMonster.afterDeath = false;
	newMonster.targetX = 0;
	newMonster.targetY = 0;
	newMonster.angle = PI * 3 / 2;
	newMonster.range = 1500;
	newMonster.stunCount = 0;
	newMonster.patternCount = 0;
	newMonster.deathalpha = 255;
	newMonster.activestate = MONSTERACTIVE::NONE;
	newMonster.oldactivestate = MONSTERACTIVE::NONE;
	newMonster.movestate = MONSTERMOVESTATE::NONE;
	_vMonster.push_back(newMonster);
}

void Cmushmam_mushroom_B::move(bulletManager* bm)
{
	_viMonster->patternCount++;
	switch (_viMonster->activestate)
	{
	case MONSTERACTIVE::NONE:
		if (_viMonster->patternCount % 100 == 0) {
			_viMonster->activestate = MONSTERACTIVE::FINDING;
		}
		break;
	case MONSTERACTIVE::FINDING:
		break;
	case MONSTERACTIVE::ATTACK:
		bm->getCirBulInstance()->fire(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
			_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
			_viMonster->angle, 0);
		_viMonster->activestate = MONSTERACTIVE::NONE;
		break;
	case MONSTERACTIVE::RNDMOVE:
		break;
	case MONSTERACTIVE::DEATH:
		_viMonster->deathalpha -= 3;
		if (_viMonster->deathalpha < 0) _viMonster->deathalpha = 0;
		if (_viMonster->deathalpha == 0) {
			_viMonster->afterDeath = true;
		}
		break;
	}
}

void Cmushmam_mushroom_B::giveFrame()
{
}

void Cmushmam_mushroom_B::knockback(vector<tagMonster>::iterator iter, float x, float y, int damage, float knockbackRange, bool stun)
{
	if (iter->activestate == MONSTERACTIVE::DEATH) return;
	//�ܺ�ȣ����Լ�(���ݿ�������)
	//�������ݿ�
	iter->hp -= damage;
	//�Ѿ��� ���� �ٶ󺸴� �������� �˹�
	float centerx, centery;
	centerx = iter->rc.left + (iter->rc.right - iter->rc.left) / 2;
	centery = iter->rc.top + (iter->rc.bottom - iter->rc.top) / 2;
	float nbangle = UTIL::getAngle(x, y, centerx, centery);
	iter->x += cosf(nbangle) * knockbackRange;
	iter->y -= sinf(nbangle) * knockbackRange;
	iter->rc = RectMake(iter->x + iter->img->getFrameWidth() * 3 / 8, y + iter->img->getFrameHeight() * 3 / 4, iter->width, iter->height);
	//������ ���� ���ϸ��� �ʴ´�.
	if (stun = false) {
		iter->isStun = stun;
		iter->frameX = 0;
		iter->framecount = 0;
		iter->patternCount = 0;
		iter->activestate = MONSTERACTIVE::NONE;
	}
}

void Cmushmam_mushroom_B::stuncheck()
{
}

void Cmushmam_mushroom_B::checkPlayerXY(Cplayer* py)
{
	if (_viMonster->activestate != MONSTERACTIVE::FINDING) return;
	//��ǥ ������ �� ����
	_viMonster->targetX = PLAYER->getPlayerAddress().x;
	_viMonster->targetY = PLAYER->getPlayerAddress().y;
	_viMonster->angle = UTIL::getAngle(_viMonster->rc.left + (_viMonster->rc.right - _viMonster->rc.left) / 2,
		_viMonster->rc.top + (_viMonster->rc.bottom - _viMonster->rc.top) / 2,
		_viMonster->targetX, _viMonster->targetY);

	_viMonster->patternCount = 0;
	_viMonster->activestate = MONSTERACTIVE::ATTACK;
}
