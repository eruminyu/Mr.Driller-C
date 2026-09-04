#pragma once

#include <Actor/Actor.h>
#include <string>

class UIActor : public Craft::Actor
{
	TYPE_DECLARATIONS(UIActor, Actor)

public:
	UIActor();
	virtual void Draw() override;

private:
	void DrawText(const std::string& text, int x, int y, Craft::Color textColor) const;
	void DrawSection(const std::string& title, int x, int y) const;
};
