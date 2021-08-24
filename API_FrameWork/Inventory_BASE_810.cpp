#include "framework.h"
#include "Inventory.h"

HRESULT Inventory::init()
{
	IMAGE->addImage("�κ��丮��������", "images/object/inventoryinfobox.bmp", WINSIZEX * 3 / 5, 150, false);
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
				//Ŭ�������� ������ Ÿ�Կ� ���� ��ȣ�ۿ�
				if (INPUT->isOnceKeyDown(VK_LBUTTON) && i < _vectInventory.size())
				{
					for (auto& j : _vectItemData)
					{
						if (j.item_name == _vectInventory[i].item_name)
						{
							if (!_vectInventory[i].isEquip)
							{//�������� ������
								if (PLAYERDATA->PartsEquipCheck(j))
								{//���밡��
									PLAYERDATA->setEquip(j, _vectInventory[i].equipHP, _vectInventory[i].equipMP);
									_vectInventory[i].isEquip = true;
								}
							}
							else if (_vectInventory[i].isEquip)
							{//��������
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
		//���̾ƿ� ���
		ZORDER->UIRender(_inventory_layout, ZUIFIRST, 0, 1033, 195);

		for (int i = 0; i < 15; i++)
		{
			if (PtInRect(&_InvenSlot[i], { m_ptMouse.x,m_ptMouse.y }))
			{
				//����ϱ� ���
				ZORDER->UIAlphaRender(_Bright_button_image, ZUIFIRST, 3, _InvenSlot[i].left, _InvenSlot[i].top, 50);
			}

			//��������� �κ��丮�� ����ϴ� �κ�
			if (i < _vectInventory.size())
			{
				if (_vectInventory[i].isEquip) {
					ZORDER->UIRender(_Equip_Mark_image, ZUIFIRST, 2, _InvenSlot[i].left, _InvenSlot[i].top);
				}
				for (auto& j : _vectItemData)
				{
					if (j.item_name == _vectInventory[i].item_name)
					{
						ZORDER->UIRender(j.item_image, ZUIFIRST, 1, _InvenSlot[i].left+11, _InvenSlot[i].top+11);
						InventoryInfoRender(i);
					}
				}
			}
		}
	}

	//����Ǽ����� ��ºκ�
	int accindex = 0;
	for (auto& i : _vectAccesory)
	{
		ZORDER->UIAlphaRender(i, ZUIFIRST, 0, WINSIZEX / 2 - (i->getWidth() / 2) * (_vectAccesory.size()) + accindex * i->getWidth(), 699, PLAYERDATA->getUIAlpha());
		accindex++;
	}

	
}

void Inventory::imageInit()
{
	_inventory_layout = IMAGE->addImage("�κ��丮���̾ƿ�", "images/UI/�κ�layout2.bmp", 240 * 1.3, 296 * 1.3, 1);
	_Bright_button_image = IMAGE->addImage("��ư���", "images/UI/��ư���.bmp", 38 * 1.3, 38 * 1.3, 1);
	_Equip_Mark_image = IMAGE->addImage("����ǥ��", "images/UI/����Ʋ1.bmp", 38 * 1.3, 38 * 1.3, 1);
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

void Inventory::InventoryInfoRender(int array)
{
	//��� ����
	if (PtInRect(&_InvenSlot[array], m_ptMouse))
	{
		isclicked = true;
	}
	else
	{
		isclicked = false;
	}
	if (isclicked) {
	infoRc = RectMake(600,240,
		IMAGE->findImage("�κ��丮��������")->getWidth(),
		IMAGE->findImage("�κ��丮��������")->getHeight());
		RECT txtRc = RectMake(infoRc.left + 15, infoRc.top + 15, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30);
		string str = _vectItemData[array].item_name; /*������ �̸�*/
		ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
			CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY�߰���")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		//txtRc = RectMake(infoRc.left + 15, infoRc.top + 15 + 40, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30 - 40);
		//str = _vectItemData[array].item_image; /*������ �̹���*/
		//ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
		//	CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		//		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY�߰���")),
		//	RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		txtRc = RectMake(infoRc.left + 15, infoRc.top + 15 + 40, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30 - 120);
		str = _vectItemData[array].item_stat_Info; /*������ ����*/
		ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
			CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY�߰���")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		txtRc = RectMake(infoRc.left + 15, infoRc.top + 15 + 40, RecWidth(infoRc) - 30, RecHeight(infoRc) - 30 - 160);
		str = _vectItemData[array].item_info; /*������ ����*/
		ZORDER->UIDrawText(str, ZUITHIRD, txtRc,
			CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY�߰���")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

	}
}