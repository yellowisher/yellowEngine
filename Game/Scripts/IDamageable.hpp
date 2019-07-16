#ifndef __H_IDAMAGEABLE__
#define __H_IDAMAGEABLE__

class IDamageable
{
public:
	IDamageable() {}
	virtual ~IDamageable() {}

	virtual void die() = 0;
	virtual void* getTransform() = 0;
	void takeDamage(int damage)
	{
		if ((hp -= modifyDamage(damage)) <= 0)
		{
			die();
		}
	}

	void setHp(int hp) { this->hp = hp; }
	int getHp() { return hp; }

	int baseType;
	int team;
	int hp;

protected:
	virtual int modifyDamage(int damage) { return damage; }
};

#endif