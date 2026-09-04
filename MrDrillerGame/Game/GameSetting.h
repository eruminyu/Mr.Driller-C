#pragma once

// 한 개의 논리 셀을 콘솔에서 3x3 문자로 표시한다.
constexpr int cellWidth = 3;
constexpr int cellHeight = 3;
constexpr int gridWidth = 10;
constexpr int visibleGridRows = 9;
constexpr int targetDepth = 100;
// 첫 블록 행(y=7) 바로 위에 착지한 위치(y=6)를 깊이 0m로 사용한다.
constexpr int depthOriginY = 6;
// 깊이 0m부터 목표 200m 좌표까지 모두 포함한다.
constexpr int worldHeight = depthOriginY + targetDepth + 1;

// 플레이 영역 너비
constexpr int playAreaWidth = gridWidth * cellWidth;
// UI 영역 너비
constexpr int uiAreaWidth = 20;
// 플레이/UI 구분선
constexpr int dividerX = playAreaWidth + 1;

// 전체 화면 너비
constexpr int screenWidth = playAreaWidth + uiAreaWidth + 3;
// 전체 화면 높이
constexpr int screenHeight = 30;
// 오른쪽 경계선
constexpr int uiRightBorder = screenWidth - 1;

// 플레이어 위쪽 경계
constexpr int playAreaUpBorder = 0;

constexpr int fieldLeft = 1;
constexpr int fieldTop = 1;
