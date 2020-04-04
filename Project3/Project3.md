# Project3

## 1. 프로젝트 문제 및 목표
프로젝트 #1, #2 에서 구현한 셀(shell)에 linking과 loading 기능을 추가하는 프로그램입니다. 프로젝트 #2 에서 구현된 assemble 명령을 통해서 생성된 object 파일을 link시켜 메모리에 올리는 일을 수행합니다.

## 2. 요구사항
### 2.1 프로젝트 목표 설정
- 프로젝트#2 에 아래의 기능들을 추가해야 합니다.
- 구현해야 할 사항들
① 주소 지정 명령어 ( progaddr )
② Linking Loader ( loader )
③ 프로그램 실행 ( run )
④ debug 명령어 ( bp )  

### 2.2 합성
본 프로젝트 #2 에서 구현한 셀(shell)에 linking과 loading 기능을 추가하는 프로그램으로, 프로젝트 #2 에서 구현된 assemble 명령을 통해서 생성된 object 파일을 link시켜 메모리에 올리는 일을 수행한다. 주소 지정 명령어, Linking Loader, 프로그램 실행 명령어, debug 명령어 등을 구현해야 하는데 이를 위해 필요한 자료구조 및 알고리즘을 구상하여 전체 프로그램을 설계한다.

### 2.3 제작
#### 1) 주소 지정 명령어
sicsim> progaddr [address]
- loader 또는 run 명령어를 수행할 때 시작하는 주소를 지정합니다.
- sicsim이 시작되면 default로 progaddr는 0x00 주소로 지정됩니다.  
  ex) sicsim> progaddr 4000  
  program address를 0x4000 주소로 지정합니다.  
  loader 또는 run 명령어의 수행이 0x4000 부터 시작합니다.  
  
#### 2) Linking Loader
sicsim> loader [object filename1] [object filename2] […]  
- filename1, filename2, … 에 해당하는 object 파일을 읽어서 linking 작업을 수행 후, 가상 메모리(1M)에 그 결과를 기록합니다.  
- 파일 개수는 3개까지만 고려합니다.  
- Loader 실행이 성공적이면, load map을 화면에 출력합니다.  
- 에러가 존재할 경우, 에러 내용이 화면에 출력됩니다.  
  ex) sicsim> loader proga.obj progb.obj progc.obj  
- proga.obj, progb.obj, progc.obj 를 가지고서 가상 메모리에 그 결과를 기록하고 load map을 화면에 출력합니다.  
  ex) sicsim> progaddr 4000  
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sicsim> loader proga.obj progb.obj progc.obj  
  
#### 3) 프로그램 실행
sicsim> run
- loader 명령어의 수행으로 메모리에 load된 프로그램을 실행합니다.  
- progaddr 명령어로 지정한 주소부터 실행됩니다.  
- 실행 결과로써 register 상태를 화면에 출력합니다. 출력되는 register는 A, X, L, PC, B, S, T 입니다.  
- Breakpoint까지 실행되고 Breakpoint가 없으면 프로그램 끝까지 실행됩니다. ( 명령어 bp 참조 )  

#### 4) debug 명령어
sicsim> bp [address]
- sicsim에 breakpoint를 지정합니다. Breakpoint는 bp 명령어를 통해서 프로그램 라인 길이만큼 지정할 수 있습니다.  
- run을 수행하면 breakpoint까지 프로그램이 실행되고 프로그램이 정지합니다. 다음 번 run의 실행은 정지된 breakpoint부터 시작하고 그 다음 breakpoint까지 진행됩니다. Breakpoint가 없으면 프로그램 끝까지 실행됩니다.  
  ex) sicsim> bp clear  
- sicsim에 존재하는 breakpoint를 전부 삭제합니다.  
  ex) sicsim> bp  
- sicsim에 존재하는 breakpoint를 전부 화면에 출력합니다.  
