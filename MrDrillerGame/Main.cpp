#include <Engine/Engine.h>
#include <Game/DrillerGame.h>
#include <windows.h>

int main()
{
	SetConsoleTitleA("Mr. Driller-C");

	DrillerGame game;
	game.Run();

}
