#pragma once
#include "game.h"
#include "Entity/BaseEntity.h"

class Item {
public:
	Item();
	virtual ~Item();
	virtual int onPickedup(BaseEntity*);
	virtual int onUsing(BaseEntity*);
	virtual int onDropped(BaseEntity*);
};
