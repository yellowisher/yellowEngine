#ifndef __H_IDAMAGEABLE__
#define __H_IDAMAGEABLE__

class IDamageable
{
public:
	IDamageable() {}
	virtual ~IDamageable() {}

	virtual void die() = 0;
	void takeDamage(int damage)
	{
		if ((_hp -= damage) <= 0)
		{
			die();
		}
	}

	void setHp(int hp) { _hp = hp; }
	int getHp() { return _hp; }
	int baseType;

private:
	int _hp;
};

#endif