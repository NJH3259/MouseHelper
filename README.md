# MouseHelper

Windows 콘솔 화면에 더블 버퍼링으로 그려지는 2D 게임. 자체 제작한 **CraftEngine** 위에서 동작하며, 플레이어는 마우스로 고양이(Cat)를 드래그해 쥐(Mouse)를 잡는다. 고양이가 놓여난 상태에서는 A* 알고리즘으로 쥐를 향한 경로를 탐색해 스스로 움직인다.

## 구조

- `CraftEngine/` — 렌더링(`ScreenBuffer`, `Renderer`), 입력(`Input`), 물리(`CollisionSystem`), 레벨/액터 관리 등을 담당하는 자체 엔진
- `Includes/` — CraftEngine의 공개 헤더
- `Game/` — MouseHelper 게임 로직
  - `Actor/` — `Player`, `Cat`, `Mouse`, `Cheese` 등 게임 액터
  - `Algorithm/` — `AStar`, `Node`, `GridExpander` (경로 탐색)
  - `Level/` — 스테이지 정의
  - `GameManager/` — 게임 상태(타이틀/플레이/일시정지) 전환
- `Assets/`, `Config/` — 게임 리소스 및 설정 파일

## 빌드

Windows + Visual Studio 환경에서 `MouseHelper.slnx`를 열어 x64 구성으로 빌드한다. `CraftEngine`을 DLL로 빌드한 뒤 `Game`이 이를 참조한다.

## 조작

마우스로 고양이를 클릭해 원하는 위치로 드래그하면, 고양이가 그 위치를 기준으로 A* 경로를 계산해 쥐를 추적한다.
