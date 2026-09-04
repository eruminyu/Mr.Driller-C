#pragma once

#include <Engine/Engine.h>
#include <memory>
#include <vector>


enum class GameState
{
	// 게임 시작, 첫 화면
	StartMenu = 0,
	// 메인 게임 플레이 화면
	GamePlay,
	// 일시정지
	Pause,
	Count
};



class DrillerGame : public Craft::Engine
{
public:
	DrillerGame();

	// 게임 시작
	void StartGame();
	void RestartGame();
	// 일시 정지
	void PauseGame();
	// 일시정지에서 게임으로
	void ResumeGame();
	// 일시정지에서 게임 시작으로
	void QuitStartGame();
	// 게임 시작에서 프로그램 종료
	void ExitGame();


	void ChangeState(GameState newState);

private:
	std::vector<std::shared_ptr<Craft::Level>> levelList;

	// 게임의 최초 화면
	GameState state = GameState::StartMenu;


};

