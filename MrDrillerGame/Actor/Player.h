#pragma once

#include <Actor/Actor.h>
#include <Utils/Timer.h>

class Player : public Craft::Actor
{
	TYPE_DECLARATIONS(Player, Actor)


public:
	Player();
	virtual void Draw() override;
	const Craft::Vector2& GetLookDirection() const { return playerLook; }
	void SetLookDirection(const Craft::Vector2& direction) { playerLook = direction; }



private:
	virtual void Tick(float deltaTime) override;
	Timer fallTimer{ 0.2f };

	Craft::Vector2 playerLook{ 0, 0 };



};

