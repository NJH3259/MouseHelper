# MouseHelper

C++ 콘솔 기반 미로 추격 게임입니다. 플레이어는 마우스 커서로 고양이(Cat)를 집어서
옮길 수 있고, 쥐(Mouse)는 고양이를 피해 치즈(Cheese)까지 이동해야 합니다. 고양이에게
쥐가 잡히면 실패, 쥐가 치즈에 도달하면 클리어입니다.

## 기술 스택

- C++ / Visual Studio (`MouseHelper.slnx`, x64 플랫폼)
- Windows 콘솔 API(`Windows.h`, `COORD` 등) 기반 자체 렌더링
- 자체 제작 엔진 **CraftEngine** 위에서 동작하는 **Game** 프로젝트로 구성된
  2-프로젝트 솔루션

## 폴더 구조

```
CraftEngine/   엔진 코드 (Actor, Core, Engine, Input, Level, Math, Physics, Render, Util)
Game/          게임 로직 (Actor, Algorithm, GameManager, Level, Util, Main.cpp)
Includes/      엔진 export 헤더
Config/        Setting.txt
Assets/        맵/액터 텍스트 이미지 (Stage1.txt, Cat.txt, Mouse.txt, Cheese.txt)
```

## 빌드

Windows + Visual Studio에서 `MouseHelper.slnx`를 열고 x64 구성으로 빌드한 뒤
`Game` 프로젝트를 실행합니다.

## 조작법

- 마우스 왼쪽 버튼: 커서 위치의 고양이를 클릭&드래그로 집어서 이동
- `R`: 레벨 리셋
- `D`: 디버그 모드 토글
- `ESC`: 일시정지

## 현재 개발 상태

처음 기획한 게임의 기능 자체는 모두 구현하였습니다. 쥐와 고양이 액터는 게임 시작 시 A* 알고리즘으로
탐색하여 목표를 향해 이동하고, 그 중 고양이는 일정 시간 간격으로 경로를 갱신합니다.

A* 알고리즘을 통한 경로 탐색은 액터의 중심 픽셀인 피봇을 지정하여 해당 위치를 기준으로 경로를
탐색하며, 경로 탐색 시 그리드는 n x m 크기의 액터가 벽과 겹치는 것처럼 보이지 않게 하기 위해
액터 사이즈에 비례하여 확장시키도록 하였습니다.

고양이 액터는 마우스 클릭으로 들고 클릭에서 땜으로써 내려놓을 수 있는데, 내려놓은 위치가
벽 혹은 그리드 바깥인 경우, 피봇을 기준으로 n x n 영역의 사각형을 확장시키며 영역 내에서
가장 가까운 땅 픽셀을 찾고 피봇이 해당 픽셀로 가도록 이동 벡터를 계산하여 위치를 변경시킵니다.

총 4개의 스테이지를 만들어 두었으며, 각 스테이지 별로 마우스 클릭을 통해 고양이를 이동시킬 수 있는
횟수가 제한되어 있습니다.
