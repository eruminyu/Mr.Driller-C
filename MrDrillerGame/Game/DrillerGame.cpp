#include "DrillerGame.h"
#include <Level/StartLevel.h>
#include <Level/DrillerLevel.h>
#include <Level/PauseLevel.h>
#include <cassert>


DrillerGame::DrillerGame()
{
	levelList.emplace_back(std::make_shared<StartLevel>());
	levelList.emplace_back(std::make_shared<DrillerLevel>());
	levelList.emplace_back(std::make_shared<PauseLevel>());

	assert(levelList.size() == static_cast<size_t>(GameState::Count));

	// 최초 화면 세팅
	ChangeState(GameState::StartMenu);

}


void DrillerGame::StartGame()
{
	RestartGame();
}

void DrillerGame::RestartGame()
{
	levelList[static_cast<size_t>(GameState::GamePlay)] =
		std::make_shared<DrillerLevel>();
	ChangeState(GameState::GamePlay);
}

void DrillerGame::PauseGame()
{
	if (state == GameState::GamePlay)
	{
		ChangeState(GameState::Pause);
	}
}

void DrillerGame::ResumeGame()
{
	if (state == GameState::Pause)
	{
		ChangeState(GameState::GamePlay);
	}
}

void DrillerGame::QuitStartGame()
{
	ChangeState(GameState::StartMenu);
}

void DrillerGame::ExitGame()
{
	Quit();
}

void DrillerGame::ChangeState(GameState newState)
{
	const size_t index = static_cast<size_t>(newState);

	if (newState == GameState::Count || index >= levelList.size())
	{
		return;
	}

	state = newState;
	mainLevel = levelList[index];

}

