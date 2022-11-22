#pragma once
class FLKBehaviour
{
public:
	virtual void OnStart() = 0;
	virtual void OnUpdate() = 0;
	virtual void Draw() = 0;
};