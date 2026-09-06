# Mr. Driller-C

Windows 콘솔 환경에서 C++20으로 제작한 드릴 액션 게임입니다. 직접 구현한 DLL 기반 `CraftEngine` 위에 블록 파괴와 낙하, 산소 관리, 카메라 스크롤, 자동 플레이를 구성했습니다.

> 학습과 포트폴리오 목적으로 제작한 비공식 개인 프로젝트입니다.
>
> ## 개발 보고서

프로젝트의 구현 과정과 문제 해결 기록은 아래 글에 정리했습니다.

- [Mr.Driller-C 개발 보고서](블로그-글-주소)

[최신 Windows x64 릴리즈 다운로드](https://github.com/eruminyu/Mr.Driller-C/releases/latest)

## 주요 구현

- `StartMenu → GamePlay → Pause` 상태 전환과 재시작 처리
- 플레이 깊이에 맞춰 행을 추가하는 절차적 블록 생성
- 같은 색으로 연결된 블록 탐색과 일괄 파괴
- 연결된 블록 그룹 단위의 낙하 및 플레이어 충돌 판정
- 깊이, 점수, 산소 게이지와 게임 클리어·오버 조건
- 월드 좌표와 화면 좌표를 분리한 세로 카메라 스크롤
- 위치와 남은 산소를 함께 탐색 상태로 사용하는 BFS 자동 플레이
- 엔진 DLL과 설정 파일을 실행 폴더로 복사하는 빌드 구성

## 조작법

| 키 | 동작 |
|---|---|
| 방향키 | 이동 또는 드릴 방향 전환 |
| `Z` | 바라보는 방향의 블록 파괴 / 메뉴 선택 |
| `Space` | 자동 플레이 켜기·끄기 |
| `Esc` | 플레이 중 일시정지 |
| `Enter` 또는 `Esc` | 일시정지 해제 |
| `R` | 일시정지 또는 결과 화면에서 재시작 |
| `Q` | 일시정지 화면에서 메인 메뉴로 이동 |

## 기술 스택

- C++20
- Win32 Console API
- Visual Studio C++ / MSVC `v145`
- x64 Debug·Release 구성

## 프로젝트 구조

```text
.
├─ CraftEngine/          # 입력, 액터, 레벨, 충돌, 콘솔 렌더링 DLL
├─ MrDrillerGame/
│  ├─ Actor/             # Player, Block, UIActor
│  ├─ Game/              # 게임 상태와 공용 설정
│  ├─ Level/             # 시작, 플레이, 일시정지 레벨
│  ├─ Utils/             # 게임용 타이머
│  └─ Main.cpp
├─ Config/               # 프레임 속도와 콘솔 크기 설정
└─ ConsoleGameProject.slnx
```

실행 흐름은 `Main.cpp → DrillerGame → 현재 Level → Actor/UI` 순서이며, 공용 입력과 렌더링은 `CraftEngine`이 담당합니다.

## 빌드 방법

Visual Studio의 **C++를 사용한 데스크톱 개발** 워크로드와 Windows SDK가 필요합니다.

1. `ConsoleGameProject.slnx`를 엽니다.
2. 플랫폼을 `x64`, 구성을 `Debug` 또는 `Release`로 선택합니다.
3. 솔루션을 빌드합니다.

Release 실행 파일은 다음 위치에 생성됩니다.

```text
Bin/x64/Release/MrDrillerGame/MrDrillerGame.exe
```

빌드가 완료되면 실행에 필요한 `CraftEngine.dll`과 `Config/Setting.txt`가 실행 파일 폴더에 자동으로 복사됩니다.

## 실행 및 릴리즈

[GitHub Releases](https://github.com/eruminyu/Mr.Driller-C/releases)에서 `MrDrillerGame-Windows-x64.zip`을 내려받아 압축을 해제한 뒤 `MrDrillerGame.exe`를 실행합니다.

릴리즈 패키지에는 다음 파일만 포함됩니다.

```text
MrDrillerGame.exe
CraftEngine.dll
Config/Setting.txt
README.md
```

지원 환경은 Windows x64입니다.
