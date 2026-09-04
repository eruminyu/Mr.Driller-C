#pragma once
#include <Level/Level.h>

class StartLevel : public Craft::Level
{
public:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	int selectedMenuIndex = 0;
};

