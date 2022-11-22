#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "../Components/SceneComponent.h"

class drawableObject
{
public:
	virtual void draw() = 0;
	virtual void setGui() {};
	bool Editing;

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void update() {};

	static SceneComponent* scene;
};

#endif