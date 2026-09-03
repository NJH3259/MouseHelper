# MouseHelper

C++ 콘솔 기반 미로 추격 게임입니다. 플레이어는 마우스 커서로 고양이(Cat)를 집어서
옮길 수 있고, 쥐(Mouse)는 고양이를 피해 치즈(Cheese)까지 이동해야 합니다. 고양이에게
잡히면 실패, 치즈에 도달하면 클리어입니다.

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

## 현재 개발 상태 (6.AStar 브랜치)

액터 크기를 고려해 그리드를 확장하는 `GridExpander`와 A* 길찾기
(`Game/Algorithm/AStar.cpp`)를 구현하는 중이며, 아직 고양이/쥐의 실제 이동
로직에는 연결되지 않은 작업 진행 단계입니다.
