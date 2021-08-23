#include "framework.h"
#include "Inventory.h"

HRESULT Inventory::init()
{
	_isInvenON = false;
	_isDebug = false;

	this->imageInit();

	for (int i = 0; i < 15; i++)
	{
		_InvenSlot[i] = RectMake(1067 + (i % 5) * (_Bright_button_image->getWidth() + 2),
			256 + (i / 5) * (_Bright_button_image->getHeight() + 5),
			_Bright_button_image->getWidth(), _Bright_button_image->getHeight());
	}

	_vectItemData = ITEM->copyItemVect();

	return S_OK;
}

void Inventory::release() {}

void Inventory::update()
{

	if (INPUT->isOnceKeyDown('I'))
	{
		_isInvenON = !_isInvenON;
	}

	if (INPUT->isOnceKeyDown('K'))
	{
		randnum = RND->getFromInTo(0, _vectItemData.size() - 1);
		InventoryDataPushBack(_vectItemData[randnum].item_name, _vectItemData[randnum].equipHP, _vectItemData[randnum].equipMP);
	}

	if (_isInvenON)
	{
		for (int i = 0; i < 15; i++)
		{
			if (PtInRect(&_InvenSlot[i], { m_ptMouse.x,m_ptMouse.y }))
			{
				//클릭햇을때 아이템 타입에 따라 상호작용
				if (INPUT->isOnceKeyDown(VK_LBUTTON) && i < _vectInventory.size())
				{
					for (auto& j : _vectItemData)
					{
						if (j.item_name == _vectInventory[i].item_name)
						{
							if (!_vectInventory[i].isEquip)
							{//미착용중 아이템
								if (PLAYERDATA->PartsEquipCheck(j))
								{//착용가능
									PLAYERDATA->setEquip(j, _vectInventory[i].equipHP, _vectInventory[i].equipMP);
									_vectInventory[i].isEquip = true;
								}
							}
							else if (_vectInventory[i].isEquip)
							{//착용해제
								PLAYERDATA->takeOffEquip(j, _vectInventory[i].equipHP, _vectInventory[i].equipMP);
								_vectInventory[i].isEquip = false;
							}
						}
					}
				}
				else if (INPUT->isOnceKeyDown(VK_RBUTTON) && i < _vectInventory.size())
				{
					if (!_vectInventory[i].isEquip)
					{
						_iterInventory = _vectInventory.begin() + i;
						_vectInventory.erase(_iterInventory);
					}
				}
			}
		}
	}
}

void Inventory::render(HDC hdc)
{
	if (_isInvenON)
	{
		//레이아웃 출력
		ZORDER->UIRender(_inventory_layout, ZUIFIRST, 0, 1033, 195);

		for (int i = 0; i < 15; i++)
		{
			if (PtInRect(&_InvenSlot[i], { m_ptMouse.x,m_ptMouse.y }))
			{
				//밝게하기 출력
				ZORDER->UIAlphaRender(_Bright_button_image, ZUIFIRST, 3, _InvenSlot[i].left, _InvenSlot[i].top, 50);
			}

			//현재아이템 인벤토리에 출력하는 부분
			if (i < _vectInventory.size())
			{
				for (auto& j : _vectItemData)
				{
					if (j.item_name == _vectInventory[i].item_name)
					{
						InventoryInfoRender(j,i);
						ZORDER->UIRender(j.item_image, ZUIFIRST, 1, _InvenSlot[i].left+11, _InvenSlot[i].top+11);
					}
				}
				if (_vectInventory[i].isEquip) {
					ZORDER->UIRender(_Equip_Mark_image, ZUIFIRST, 2, _InvenSlot[i].left, _InvenSlot[i].top);
				}
			}
		}

		//스탯정보출력
		showInvenUi();
	}

	//현재악세서리 출력부분
	int accindex = 0;
	for (auto& i : _vectAccesory)
	{
		ZORDER->UIAlphaRender(i, ZUIFIRST, 0, WINSIZEX / 2 - (i->getWidth() / 2) * (_vectAccesory.size()) + accindex * i->getWidth(), 699, PLAYERDATA->getUIAlpha());
		accindex++;
	}

}

void Inventory::imageInit()
{
	IMAGE->addImage("인벤토리정보상자", "images/UI/인벤토리설명.bmp", 349, 224, false);
	_inventory_layout = IMAGE->addImage("인벤토리레이아웃", "images/UI/인벤layout2.bmp", 240 * 1.3, 296 * 1.3, 1);
	_Bright_button_image = IMAGE->addImage("버튼밝게", "images/UI/버튼밝게.bmp", 38 * 1.3, 38 * 1.3, 1);
	_Equip_Mark_image = IMAGE->addImage("장착표시", "images/UI/장착틀1.bmp", 38 * 1.3, 38 * 1.3, 1);
}

void Inventory::InventoryDataPushBack(string item_name, int equipHP, int equipMP)
{
	for (auto& i : _vectItemData)
	{
		if (i.item_name == item_name) {
			if (i.itemType != ITEMTYPE::accesory) {
				InventoryData temp = { item_name,0,equipHP,equipMP };
				_vectInventory.push_back(temp);
			}
			else {
				_vectAccesory.push_back(i.item_image);
				PLAYERDATA->setEquip(_vectItemData[randnum], _vectItemData[randnum].equipHP, _vectItemData[randnum].equipMP);
			}
		}
	}
}


bool Inventory::inventoryEmptyCheck()
{
	if (_vectInventory.size() < 15) return true;
	else return false;
}

void Inventory::showInvenUi()
{
	//플레이어정보 출력
	ZORDER->UITextOut(to_string(PLAYERDATA->getMaxHP()), ZUISECOND, 1165, 424, RGB(0, 0, 0));
	ZORDER->UITextOut(to_string(PLAYERDATA->getMaxMP()), ZUISECOND, 1280, 424, RGB(0, 0, 0));
	ZORDER->UITextOut(to_string(PLAYERDATA->getData().presentAtk), ZUISECOND, 1165-7, 424+23, RGB(0, 0, 0));
	ZORDER->UITextOut(to_string(PLAYERDATA->getData().AtkSpeed), ZUISECOND, 1280, 424 + 23, RGB(0, 0, 0));
	ZORDER->UITextOut(to_string(PLAYERDATA->getData().presentSkillPower), ZUISECOND, 1165-7, 424 + 23*2, RGB(0, 0, 0));
	ZORDER->UITextOut(to_string(PLAYERDATA->getData().equipSkillCoolTime), ZUISECOND, 1280, 424 + 23 * 2, RGB(0, 0, 0));
	TCHAR str[128];
	_stprintf_s(str, "%.1f", PLAYERDATA->getData().presentSpeed);
	string speed(str);
	ZORDER->UITextOut(speed, ZUISECOND, 1165-6, 424 + 23*3, RGB(0, 0, 0));
	ZORDER->UITextOut(to_string(PLAYERDATA->getData().Critical), ZUISECOND, 1280, 424 + 23 * 3, RGB(0, 0, 0));
	//인벤아이템수, 골드
	ZORDER->UITextOut(to_string(_vectInventory.size()), ZUISECOND, 1074, 541, RGB(120, 112, 98));
	ZORDER->UITextOut(to_string(PLAYERDATA->getGold()), ZUISECOND, 1273, 538, RGB(0, 0, 0));
}

void Inventory::InventoryInfoRender(ITEMDATA itemdata, int index)
{
	//장비 설명
	if (PtInRect(&_InvenSlot[index], m_ptMouse))
	{
	infoRc = RectMake(_InvenSlot[index] .left- IMAGE->findImage("인벤토리정보상자")->getWidth(), _InvenSlot[index].top,
		IMAGE->findImage("인벤토리정보상자")->getWidth(),
		IMAGE->findImage("인벤토리정보상자")->getHeight());

		RECT txtRc = RectMake(infoRc.left + 15, infoRc.top + 15, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30);
		string str = itemdata.item_name; /*아이템 이름*/
		ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
			CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY견고딕")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		//txtRc = RectMake(infoRc.left + 15, infoRc.top + 15 + 40, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30 - 40);
		//str = itemdata.item_image; /*아이템 이미지*/
		//ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
		//	CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		//		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY견고딕")),
		//	RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		//ZORDER->UIRender(itemdata.item_image, ZUITHIRD, 3, infoRc.left+10, infoRc.top+20);

		ZORDER->UIStretchRender(itemdata.item_image, ZUITHIRD, 3, infoRc.left +41, infoRc.top +72, 2);
		
		txtRc = RectMake(infoRc.left +97 , infoRc.top +53 , RecWidth(infoRc) - 30, RecHeight(infoRc) - 30 );
		str = itemdata.item_stat_Info; /*아이템 능력치*/
		ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
			CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY견고딕")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		txtRc = RectMake(infoRc.left + 19, infoRc.top + 110, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30);
		str = itemdata.item_info; /*아이템 설명*/
		ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
			CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY견고딕")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		ZORDER->UIAlphaRender(IMAGE->findImage("인벤토리정보상자"), ZUITHIRD, 0,
				_InvenSlot[index].left - IMAGE->findImage("인벤토리정보상자")->getWidth(),
			_InvenSlot[index].top, 150);
	}
}