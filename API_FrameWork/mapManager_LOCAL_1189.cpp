#include "framework.h"
#include "mapManager.h"
#include"monsterManager.h"
HRESULT mapManager::init()
{
	/*_Cmap1 = dynamic_cast<Cmap*>(SCENE->addScene("_Cmap1", new Cmap));
	_Cmap2 = dynamic_cast<Cmap2*>(SCENE->addScene("_Cmap2", new Cmap2));
	_Cmap3 = dynamic_cast<Cmap3*>(SCENE->addScene("_Cmap3", new Cmap3));
	_Cmap4 = dynamic_cast<Cmap4*>(SCENE->addScene("_Cmap4", new Cmap4));
	_Cmap5 = dynamic_cast<Cmap5*>(SCENE->addScene("_Cmap5", new Cmap5));
	_Cmap6 = dynamic_cast<Cmap6*>(SCENE->addScene("_Cmap6", new Cmap6));
	_Cmap7 = dynamic_cast<Cmap7*>(SCENE->addScene("_Cmap7", new Cmap7));
	_Cmap8 = dynamic_cast<Cmap8*>(SCENE->addScene("_Cmap8", new Cmap8));
	_Cmap9 = dynamic_cast<Cmap9*>(SCENE->addScene("_Cmap9", new Cmap9));*/
	_Cmap10 = dynamic_cast<Cmap10*>(SCENE->addScene("_Cmap10", new Cmap10));

	_chestMap = dynamic_cast<chestMap*>(SCENE->addScene("_chestMap", new chestMap));
	_shopMap = dynamic_cast<shopMap*>(SCENE->addScene("_shopMap", new shopMap));
	_statueMap = dynamic_cast<statueMap*>(SCENE->addScene("_statueMap", new statueMap));
	_moruMap = dynamic_cast<moruMap*>(SCENE->addScene("_moruMap", new moruMap));
	_stage1_Boss = dynamic_cast<stage1_Boss*>(SCENE->addScene("	_stage1_Boss", new 	stage1_Boss));

	

	
	_Cmap10->setMonstermemoryLink(mm);
	_chestMap->setMonstermemoryLink(mm);
	_shopMap->setMonstermemoryLink(mm);
	_statueMap->setMonstermemoryLink(mm);
	_moruMap->setMonstermemoryLink(mm);
	_stage1_Boss->setMonstermemoryLink(mm);
	
	while (remainRoom > 2 )
	{
		makeclear();
		makestage1((MAXSIZE - 1) / 2,(MAXSIZE+1)/2);
		mapSize = 12 - remainRoom;
		setstatueRoom();
		setchestRoom();
		setMORURoom();
		setShopRoom();
		setNormal();
		setBossRoom();
	}


	
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind != MAPKIND::NONE)
			{
				_mStage1.insert(pair<string, motherMap*>(stage1[i][k].sceneKey, stage1[i][k]._motherMap));
				if (stage1[i][k].mapkind == MAPKIND::START)
				{
					//���� �ٽõ��鼭 ���� �־���
					currentMap = stage1[i][k].sceneKey;
					currentIndex.x = i;
					currentIndex.y = k;
					SCENE->changeScene(currentMap);
				}
			}
		}
	}


	return S_OK;
}

void mapManager::release()
{
	SCENE->release();
}

void mapManager::update()
{
	currentMonNum = 0;
	doorstate(mm->getSnaby());
	doorstate(mm->getSlime());
	doorstate(mm->getMushman());
	doorstate(mm->getMushman_Mushroom());
	doorstate(mm->getFairy());
	doorstate(mm->getFlime());
	doorstate(mm->getBoss_Slime());
	doorstate(mm->getSemiBoss_Slime());
	doorstate(mm->getBoss_Flime());
	doorstate(mm->getBoss_Mushmam());
	doorstate(mm->getBoss_Mushroom_G());
	doorstate(mm->getBoss_Mushroom_P());
	doorstate(mm->getBoss_Mushroom_B());
	doorstate(mm->getYggdrasil_Bomb());
	doorstate(mm->getYggdrasil());
	if (currentMonNum != 0)
	{
		checkright= false;
		checkleft = false;
		checkleft = false;
		checkbottom = false;
		stage1[currentIndex.x][currentIndex.y].isClear = true;
	}
	else
	{
		UseableDoor();
	}
	if (checkright)
	{
		SCENE->changeScene(stage1[currentIndex.x + 1][currentIndex.y].sceneKey);
		currentIndex.x = currentIndex.x + 1;
		currentIndex.y = currentIndex.y;
		PLAYER->getPlayerAddress().x = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[0].Door.right + 20;
		PLAYER->getPlayerAddress().y = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[0].Door.bottom - (stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[0].Door.bottom - stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[0].Door.top)/2;
		currentMap = stage1[currentIndex.x][currentIndex.y].sceneKey;

	}
	if (checkleft)
	{
		SCENE->changeScene(stage1[currentIndex.x - 1][currentIndex.y].sceneKey);
		currentIndex.x = currentIndex.x - 1;
		currentIndex.y = currentIndex.y;
		PLAYER->getPlayerAddress().x = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[2].Door.left - 20;
		PLAYER->getPlayerAddress().y = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[2].Door.bottom - (stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[2].Door.bottom - stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[2].Door.top) / 2;

		currentMap = stage1[currentIndex.x][currentIndex.y].sceneKey;
	}
	if (checkbottom)
	{
		SCENE->changeScene(stage1[currentIndex.x][currentIndex.y+1].sceneKey);
		currentIndex.x = currentIndex.x;
		currentIndex.y = currentIndex.y + 1;
		PLAYER->getPlayerAddress().x = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[1].Door.right - (stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[1].Door.right - stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[1].Door.left) / 2;
		PLAYER->getPlayerAddress().y = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[1].Door.bottom + 20;

		currentMap = stage1[currentIndex.x][currentIndex.y].sceneKey;
	}
	if (checktop)
	{
		SCENE->changeScene(stage1[currentIndex.x][currentIndex.y-1].sceneKey);
		currentIndex.x = currentIndex.x;
		currentIndex.y = currentIndex.y - 1;
		PLAYER->getPlayerAddress().x = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[3].Door.right - (stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[3].Door.right - stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[3].Door.left) / 2;
		PLAYER->getPlayerAddress().y = stage1[currentIndex.x][currentIndex.y]._motherMap->getDungeonDoor()[3].Door.top -20;

		currentMap = stage1[currentIndex.x][currentIndex.y].sceneKey;
	}
	SCENE->update();
}

void mapManager::render()
{
	
	SCENE->render();
	for (size_t i = 0; i < MAXSIZE; i++)
	{
		for (size_t j = 0; j < MAXSIZE; j++)
		{
			string str;
			//str = to_string((int)stage1[i][j].mapkind);
			//ZORDER->ZorderTextOut(str, ZMAXLAYER, 100 + 20 * i, 100 + 20 * j, RGB(0, 0, 0));

			str = stage1[i][j].sceneKey;
			ZORDER->ZorderTextOut(str, ZMAXLAYER, 300 + 50 * i, 100 + 20 * j, RGB(0, 0, 0));
		}
	}
	string str;
	str = to_string((int)currentIndex.x);
	ZORDER->ZorderTextOut(str, ZMAXLAYER, 100 + 20 , 300 , RGB(0, 0, 0));
	string str2;
	str2 = to_string((int)currentIndex.y);
	ZORDER->ZorderTextOut(str2, ZMAXLAYER, 100 + 40 , 300 , RGB(0, 0, 0));

	str = currentMap;
	ZORDER->ZorderTextOut(str, ZMAXLAYER, WINSIZEX/2, WINSIZEY/2, RGB(0, 0, 0));
}



void mapManager::doorstate(monster* monster)
{
	vector<tagMonster>& vMonster = monster->getVMonster();
	vector<tagMonster>::iterator iter = vMonster.begin();
	for (iter; iter != vMonster.end(); ++iter)
	{
		currentMonNum += monster->getVMonster().size();
	}
}

void mapManager::UseableDoor()
{
	if (stage1[currentIndex.x - 1][currentIndex.y].mapkind != MAPKIND::NONE)
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[0].isOpen = true;
	}
	else
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[0].isOpen = false;
	}

	if (stage1[currentIndex.x][currentIndex.y - 1].mapkind != MAPKIND::NONE)
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[1].isOpen = true;
	}
	else
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[1].isOpen = false;
	}
	if (stage1[currentIndex.x + 1][currentIndex.y].mapkind != MAPKIND::NONE)
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[2].isOpen = true;
	}
	else
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[2].isOpen = false;
	}
	if (stage1[currentIndex.x][currentIndex.y + 1].mapkind != MAPKIND::NONE)
	{

		_mStage1.find(currentMap)->second->getDungeonDoor()[3].isOpen = true;
	}
	else
	{
		_mStage1.find(currentMap)->second->getDungeonDoor()[3].isOpen = false;
	}

}
void mapManager::makestage1(int i, int k)
{
	if (remainRoom <= 0 ||
		i < 0 ||
		i >= MAXSIZE ||
		k < 0 ||
		k >= MAXSIZE ||
		stage1[i][k].mapkind != MAPKIND::NONE)
		return;

	remainRoom--;
	stage1[i][k].mapkind = MAPKIND::NORMAL;
	if (!(bool)(RND->getInt(4))) makestage1(i + 1, k);//r
	if (!(bool)(RND->getInt(4))) makestage1(i -1, k);//l
	if (!(bool)(RND->getInt(4))) makestage1(i, k-1);//u
	if (!(bool)(RND->getInt(4))) makestage1(i, k +1);//d

}

bool mapManager::setstatueRoom()
{
	int setstatue = mapSize - 6;
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind == MAPKIND::NORMAL)
			{
				setstatue--;
				if (RND->getInt(100) > 50 || setstatue == 0)
				{
					stage1[i][k] = { _statueMap,"_statueMap",STATUEROOM };
				return true;
				}
			}
		}
	}
	return false;
}
bool mapManager::setchestRoom()
{
	int setchest = mapSize-5;
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind == MAPKIND::NORMAL)
			{
				setchest--;
				if (RND->getInt(100) > 50 || setchest == 0)
				{
					stage1[i][k] = { _chestMap,"_chestMap",CHESTROOM };
				return true;
				}
			}
		}
	}
	return false;
}



bool mapManager::setMORURoom()
{
	int setMORU = mapSize-4;
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind == MAPKIND::NORMAL)
			{
				setMORU--;
				if (RND->getInt(100) > 50 || setMORU == 0)
				{
					stage1[i][k] = { _moruMap,"_moruMap",MORUROOM };
					return true;
				}
			}
		}
	}
	return false;
}

bool mapManager::setShopRoom()
{
	int setShop = mapSize-3;
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind == MAPKIND::NORMAL)
			{
				setShop--;
				if ( RND->getInt(100) >50 || setShop == 0)
				{
					stage1[i][k] = { _shopMap,"_shopMap",SHOP };
				return true;
				}
			}
		}
	}
	return false;
}
bool mapManager::setNormal()
{
	
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind == MAPKIND::NORMAL)
			{
				switch (RND->getInt(9))
				{
				case 0:
				{
					string temp;
					temp = "_Cmap" +to_string(i) + to_string(k);
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);

				}	break;
				case 1:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);					
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap2*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap2));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 2:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);					
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap3*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap3));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 3:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);				
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap4*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap4));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 4:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);				
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap5*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap5));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 5:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);					
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap6*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap6));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 6:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);				
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap7*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap7));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 7:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);				
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap8*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap8));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
				case 8:
				{
					string temp;
					temp = "_Cmap" + to_string(i) + to_string(k);				
					stage1[i][k].sceneKey = temp;
					stage1[i][k]._motherMap = dynamic_cast<Cmap9*>(SCENE->addScene(stage1[i][k].sceneKey, new Cmap9));
					stage1[i][k]._motherMap->setMonstermemoryLink(mm);
				}
					break;
			
				
				}
			}
		}
	}
	return true;
}




bool mapManager::setBossRoom()
{

	int setBoss = mapSize - 2;
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			if (stage1[i][k].mapkind == MAPKIND::NORMAL)
			{
				setBoss--;
				if ( setBoss == 1)
				{
					stage1[i][k] = { _stage1_Boss,"	_stage1_Boss",BOSSROOM };
					return true;
				}
			}
		}
	}
	return false;
}

//bool mapManager::setNextRoom()
//{
//
//}





void mapManager::makeclear() {
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int k = 0; k < MAXSIZE; k++)
		{
			stage1[i][k].mapkind = NONE;
			stage1[i][k].sceneKey = "";
			stage1[i][k]._motherMap = nullptr;
		}
	}
	remainRoom = 12;
	stage1[(MAXSIZE - 1) / 2][(MAXSIZE - 1) / 2] = { _Cmap10,"_Cmap10",START,true };
	remainRoom--;
}