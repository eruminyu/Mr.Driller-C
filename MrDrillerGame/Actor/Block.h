#pragma once
#include <Actor/Actor.h>




class Block : public Craft::Actor

{
	TYPE_DECLARATIONS(Block, Actor)

public:
	enum class SetColor
	{
		Red, 
		Blue, 
		Green, 
		Yellow, 
		Dust,
		Oxygen,
		Count
	};

	bool IsColorBlock() const
	{
		return blockColor == SetColor::Red
			|| blockColor == SetColor::Blue
			|| blockColor == SetColor::Green
			|| blockColor == SetColor::Yellow;
	}

	Block(const Craft::Vector2& position, SetColor color);
	virtual void Draw() override;

public:
	SetColor GetColor() const
	{
		return blockColor;
	}
	


private:
	static Craft::Color ChangeColor(SetColor color);

	
	SetColor blockColor;




};

