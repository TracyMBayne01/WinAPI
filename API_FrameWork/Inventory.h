#pragma once
#include "singleton.h"

struct ITEMDATA;
enum ITEMTYPE;

struct InventoryData {
	string item_name;
	bool isEquip;
	int equipHP;
	int equipMP;
};


class Inventory : public Singleton<Inventory>
{
	bool _isDebug;
	bool _isInvenON;
public:
	void setIsDebug(bool isDebug) { _isDebug = isDebug; }
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

private:
	void imageInit();
	image* _inventory_layout;
	image* _Bright_button_image;
	RECT _InvenSlot[15];

	vector<ITEMDATA> _vectItemData;
	//vector<ITEMDATA>::iterator _iterItemData;

	vector<InventoryData> _vectInventory;
	vector<InventoryData>::iterator _iterInventory;

public:
	void InventoryDataPushBack(string item_name,int equipHP = 0,int equipMP = 0);
	void InventoryItemRender();
	void itemErase();
};
