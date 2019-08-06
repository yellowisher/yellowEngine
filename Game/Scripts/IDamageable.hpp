#ifndef __H_IDAMAGEABLE__
#define __H_IDAMAGEABLE__

class IDamageable
{
public:
	IDamageable() {}
	virtual ~IDamageable() {}

	virtual void die() = 0;
	virtual void* getTransform() = 0;
	virtual int getBaseType() = 0;
	void takeDamage(int damage, IDamageable* attacker)
	{
		if ((hp -= modifyDamage(damage)) <= 0)
		{
			die();
		}
		else
		{
			onAttacked(attacker);
		}
	}
	virtual void onAttacked(IDamageable* attacker) {}

	void setHp(int hp) { this->hp = hp; }
	int getHp() { return hp; }

	int team;
	int hp;

protected:
	virtual int modifyDamage(int damage) { return damage; }
};

#endif