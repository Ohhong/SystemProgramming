# Project2

## 1. 프로젝트 문제 및 목표
프로젝트 #1에서 구현한 셀(shell)에 assemble 기능을 추가하는 프로그램입니다. SIC/XE의 assembly program source 파일을 입력 받아서 object파일을 생성하고, 어셈블리 과정 중 생성된 symbol table과 결과물인 object 파일을 볼 수 있는 기능을 제공해야 합니다. 교재의 2.2까지 설명된 SIC/XE 어셈블러의 기능을 구현함을 원칙으로 한다.

## 2. 요구사항
### 2.1 프로젝트 목표 설정
- 이미 제출한 프로젝트#1에 아래의 기능들을 추가해야 합니다.  
- 구현해야 할 사항들 (다음 페이지에 보다 자세한 설명이 나옵니다.)  
① Shell 관련 명령어들 (help, type)  
② assembler (assemble)  
③ assemble 관련 명령어 (symbol)  

### 2.2 합성
프로젝트 #1에서 구현한 셀(shell)에 assemble 기능을 추가하는 프로그램을 작성하는 프로젝트로, SIC/XE machine의 assembly program source 파일을 입력 받아서 object파일을 생성하고, 어셈블리 과정 중 생성된 symbol table과 결과물인 object 파일을 볼 수 있는 기능을 제공해야 한다. 이와 같은 기능을 제공하는 프로그램을 작성하기 위해 필요한 자료구조와 알고리즘을 구상하여 전체적인 프로그램을 설계한다.  

### 2.3 제작
#### 1) Shell 관련 명령어  
① sicsim> help - 아래와 같이 Shell에서 실행 가능한 모든 명령어들의 리스트를 화면에 출력해준다.  
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;h[elp]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;d[ir]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;q[uit]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;hi[story]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;du[mp] [start, end]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;e[dit] address, value  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;f[ill] start, end, value  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;reset  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;opcode mnemonic  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;opcodelist  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;assemble filename  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;type filename  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;symbol  
  
② sicsim> type filename  
- filename에 해당하는 파일을 현재 디렉터리에서 읽어서 화면에 출력한다.  
- 현재 디렉터리에 해당 파일이 존재하지 않으면 에러 메시지를 출력한다.  
- 시스템 콜을 사용하지 않는다.  
  ex) sicsim> type example.obj  
  HCOPY 00100000107A  
  T00101E150C10364820610810334C0000454F46000003000000  
  E001000  
  
#### 2) SIC/XE 어셈블러 명령  
① sicsim> assemble filename  
- filename에 해당하는 소스 파일을 읽어서 object파일과 리스팅 파일을 만든다.  
- 소스 파일의 확장자는 .asm 입니다.  
- 리스팅 파일의 파일명은 소스 파일과 동일하고 확장자는 .lst 입니다.  
- object 파일의 파일명은 소스 파일과 동일하고 확장자는 .obj 입니다.  
- 소스파일에 에러가 존재할 경우, 리스팅 파일과 object파일을 생성하지 않고 에러 내용을 화면에 출력합니다. 에러 발생시 바로 명령이 종료됩니다. 에러의 내용은 디버깅을 위해서 어떤 라인에서 에러가 발생했는지 출력한다.  
  ex) sicsim> assemble example.asm  
  &nbsp;&nbsp;&nbsp;&nbsp;i. 파일명을 2_5.lst 로 하는 리스팅 파일과 2_5.obj 로 하는 object파일이 만들어집니다.  
  
  ex) sicsim> assemble example.asm  
  &nbsp;&nbsp;&nbsp;&nbsp;5 COPY START 1000 COPY FILE…  
  &nbsp;&nbsp;&nbsp;&nbsp;10 FIRST STL RETADR SAVE RETURN ADDRESS  
  &nbsp;&nbsp;&nbsp;&nbsp;…………  
  &nbsp;&nbsp;&nbsp;&nbsp;255 END FIRST  
  
  ex) sicsim> assemble example.asm  
  &nbsp;&nbsp;&nbsp;&nbsp;output file : [example.lst], [example.obj]  
  
  ex) sicsim> type example.lst  
  &nbsp;&nbsp;&nbsp;&nbsp;5 0000 COPY START 0  
  &nbsp;&nbsp;&nbsp;&nbsp;10 0000 FIRST STL RETADR 17202D  
  &nbsp;&nbsp;&nbsp;&nbsp;12 0003 LDB #LENGTH 69202D  
  &nbsp;&nbsp;&nbsp;&nbsp;…………  
  &nbsp;&nbsp;&nbsp;&nbsp;255 END FIRST  
  
  ex) sicsim> type example.obj  
  &nbsp;&nbsp;&nbsp;&nbsp;HCOPY 000000001077  
  &nbsp;&nbsp;&nbsp;&nbsp;T0000001D17202D69202D……………………………………  
  &nbsp;&nbsp;&nbsp;&nbsp;T00001D………  
  &nbsp;&nbsp;&nbsp;&nbsp;…..  
  &nbsp;&nbsp;&nbsp;&nbsp;E000000  
  
② sicsim> symbol
- assemble 과정 중에 생성된 symbol table을 화면에 출력합니다. Symbol table은 각자 설계를 하고, 출력은 아래와 같이 합니다.  
- 가장 최근에 assemble 한 파일의 symbol table을 출력합니다.  
- symbol의 출력은 symbol을 기준으로 내림차순으로 정렬이 되어야 합니다.  
  ex) sicsim> assemble example.asm  
  &nbsp;&nbsp;&nbsp;&nbsp;output file : [example.lst], [example.obj]  
  
  ex) sicsim> symbol  
  &nbsp;&nbsp;&nbsp;&nbsp;(\t)RETADR(\t)0030 
  &nbsp;&nbsp;&nbsp;&nbsp;=>실제 출력 될시에는 RETADR 0030
- 즉 하나의 symbol당 한 line을 차지하고, 탭+Symbol+탭+주소값+\n을 의미합니다. 맨마지막 줄에는 \n(엔터)를 빼주세요.
