#pragma once

#include "Scene.h"

class GoosePlayground : public Scene
{
public:
	GoosePlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

};
