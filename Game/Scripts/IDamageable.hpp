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
		if ((_hp -= modifyDamage(damage)) <= 0)
		{
			die();
		}
	}

	void setHp(int hp) { _hp = hp; }
	int getHp() { return _hp; }

	int baseType;
	int team;

protected:
	virtual int modifyDamage(int damage) = 0;

private:
	int _hp;
};

#endif