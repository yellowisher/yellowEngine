#include "Arrow.hpp"
#include "ArrowShooter.hpp"


COMPONENT_IMPL(ArrowShooter)

void ArrowShooter::initialize(Unit* unit)
{
	_unit = unit;
}


void ArrowShooter::fireArrow(Transform* target)
{
	static Model* arrowPrefab = Model::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Model\\other\\Arrow_A.fbx");
	static Material* arrowMaterial = Material::create("C:\\Users\\yApy\\Desktop\\Game\\Asset\\Materials\\arrow.yem");

	GameObject* arrowObject = arrowPrefab->instantiate("arrow", arrowMaterial);
	Arrow* arrow = arrowObject->addComponent<Arrow>();
	arrow->transform->setPosition(transform->getWorldPosition());
	//arrow->transform->setRotation(transform->getWorldRotation());
	arrow->transform->setScale(0.03f, 0.03f, 0.03f);
	arrow->initialize(target, _unit->damage, _unit);
}
