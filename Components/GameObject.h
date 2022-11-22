#pragma once
#include "../Headers/FLKBehaviour.h"
#include <iostream>;


class GameObject : public FLKBehaviour
{
public:
	void FLKBehaviour::Draw(){}
	void FLKBehaviour::OnStart(){}
	void FLKBehaviour::OnUpdate(){}
	virtual void Start();

public:
	char NameObject[128] = "New Object";
};