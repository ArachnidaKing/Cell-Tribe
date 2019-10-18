#pragma once
#include "../game.h"

constexpr int ENTITY_BEYOND = -4;
constexpr int ENTITY_EXIST = -3;
constexpr int ENTITY_DEAD = -2;
constexpr int ITEM_USED = 2;
constexpr int ENTITY_ATTACKED = 3;


class MapController;
class EntityLiving;
class SLObject;

class Entity {
public:
	Entity();
	virtual ~Entity();
	virtual int update() = 0;
	virtual int print() const = 0;
	bool getDeath() const;
	bool getDisplay() const;
	double getHealth() const;
	Point getPoint() const;
	virtual int inRange(const Point&) const;
	virtual int inRange(const Point&, const double&) const;
	virtual bool canBeAttacked() const;
	virtual int beAttacked(EntityLiving*, const double&);
	virtual bool canBeUsed() const;
	virtual int beUsed(EntityLiving*);
	virtual int respawn(MapController*);
	virtual int spwan(MapController*);
	virtual bool isPlayer() const;
protected:
	double health;
	void setPoint(const Point&);
	void setPoint(const double&, const double&);
	void setDeath();
	void setMapController(MapController*, const bool&);
	virtual void display(const bool&);
	MapController* getMapController() const;
	SLObject* slObject;
private:
	Point point;
	MapController* mapController;
	bool death, show, showObject;
};

