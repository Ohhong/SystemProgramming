# Project1

## 1. 프로젝트 문제 및 목표
이 프로그램은 앞으로 구현하게 될 SIC/XE머신을 구현하기 위한 전 단계로서 어셈블러, 링크, 로더들을 실행하게 될 셸(shell)과 컴파일을 통해서 만들어진 object코드가 적재 되고 실행될 메모리공간과 mnemonic (ADD, COMP, FLOAT, etc …)을 opcode값으로 변환하 는 OPCODE 테이블과 관련 명령어들을 구현하는 프로그램입니다.

## 2. 요구사항
### 2.1 프로젝트 목표 설정
- 이 프로그램을 실행시키면 아래와 같이 unix shell과 유사한 입력 프롬프트상태가 된다.  
sicsim>  
- 이 상태에서 아래에 있는 명령어들을 입력할 때, 그에 해당되는 기능을 수행하여야 한다.  
- 구현해야 할 사항들
① 셸 ( sicsim> )  
② 셸 관련 명령어들 (help, dir, quit, history)  
③ 메모리공간 (1MB의 메모리를 할당해서 사용)  
④ 메모리공간 관련 명령어들 (dump, edit, fill, reset)  
⑤ opcode 테이블 (HashTable로 만들어야 함)  
⑥ opcode 관련 명령어들 (opcode, opcodelist)  

### 2.2 합성
본 프로젝트는 어셈블러, 링크, 로더들을 실행하게 될 셸(shell)과 컴파일을 통해서 만 들어진 object코드가 적재되고 실행될 메모리공간과 mnemonic(ADD, COMP, FLOAT, etc…) 을 opcode값으로 변환하는 OPCODE 테이블과 관련 명령어들을 구현하는 것이다. 따라서 완성된 프로그램이 수행해야 하는 각각의 기능을 구현하기 위해 필요한 자료구조와 알 고리즘을 구상한 후, 전체적인 프로그램을 설계한다.  

### 2.3 제작
#### 1) Shell 관련 명령어  
① sicsim> help  
  - 아래와 같이 Shell에서 실행 가능한 모든 명령어들의 리스트를 화면에 출력 해준다.  
h[elp]  
d[ir]  
q[uit]  
hi[story]  
du[mp] [start, end]  
e[dit] address, value  
f[ill] start, end, value  
reset  
opcode mnemonic  
opcodelist  

② sicsim> d[ir]  
- 현재 디렉터리에 있는 파일들을 출력한다.  
- system call을 이용하여 구현하지 않도록 해야 합니다.  
즉, system(..) 이나 exec(…) 와 같은 함수는 사용을 금합니다.  
- dirent.h, sys/stat.h를 참조합니다.  
- .과 ..는 포함되어도, 되어있지 않아도 관계없습니다.  
- Dir의 결과를 출력할 때 실행 파일은 파일 이름 옆에 "\*"표시를, 디렉터리는 '/'표시를 해야 합니다.  
ex) sicsim> dir  
Desktop/ Mail/ Work/  
a.out\* dead.letter Ingabi/ mail/  

③ sicsim> q[uit]  
- sicsim 을 종료한다.  

④ sicsim> hi[story]  
- 아래와 같이 현재까지 사용한 명령어들을 순서대로 번호와 함께 보여준다. 가장 최근 사용한 명령어가 리스트의 하단에 오도록 한다.  
ex) sicsim> history  
  1 dump  
  2 dump 14, 37  
  3 e 14, E3  
  …중간 생략…  
  908 reset  
  909 d  
  910 history  
  
수행한 모든 명령어는 History 에 계속 추가되어야 합니다. 만약 history가 비어 있다면 아무것도 출력하지 않고 다시 입력 프롬프트로 돌아옵니다.
(일반적으로 history를 친다면 그것도 명령어로 가정하므로, 빈 경우는 없습니다.)
잘못된 명령어를 입력하는 경우, 수행하지 않고 history에도 남기지 않습니다
History 명령은 아래의 그림과 같이 반드시 linked list의 형태로 구현이 되어야 합니다.

#### 2) 메모리 관련 명령어
앞으로 구현하게 될 assembler, linker& loader를 통해서 만들어진 object 파일을 올려서 실행하게 될 Shell 내의 메모리 공간에 관련된 명령어들로써 이 Shell에
서는 사이즈가 1MByte(16 X 65536)인 가상의 메모리 공간을 구현하여야 한다.  
아래에 나와있는 숫자는 모두 16진수입니다.

① sicsim> du[mp] [start, end]  
- 할당되어 있는 메모리의 내용을 아래와 같은 형식으로 출력시켜 준다.
- 가장 왼쪽 칼럼은 출력하는 메모리 주소를 나타낸다. 주소는 5자리로 고정하고 16진수로 출력할 것. 16진수 표현 시 알파벳은 대문자로 표시한다.
- 가운데 칼럼은 메모리 내용을 16진수 형태로 보여준다. 역시 16진수 표현 시 알파벳은 대문자로 표시할 것.
- 가장 오른쪽 칼럼은 메모리 내용을 byte 별로 대응하는 ASCII code 형태로 보 여준다. ASCII code 로 출력해야 할 범위는 16진수로 20 ~ 7E 까지 이며 그 이 외의 값은 ‘.’으로 출력한다.
- 자세한 ASCII code 는 교재의 appendix 또는 인터넷을 참고할 것.

- dump  
기본적으로 10라인이 출력된다. (한 라인은 메모리의 16개 바이트로 구성)  
dump의 실행으로 출력된 마지막 address는 내부에 저장하고 있다.  
다시 dump를 실행시키면 마지막 ( address + 1 ) 번지부터 출력된다.  
dump 명령어가 처음 시작될 때는 0 번지부터 출력된다.  
dump가 출력할 시 boundary check를 하여 exception error 처리한다.  
- dump start  
start 번지부터 10라인(160개)을 출력.  
주소를 넘어간 경우 주소의 끝 (0xFFFFF)까지 출력.  
- dump start, end  
start부터 end번지까지의 내용을 출력.  
Start 주소가 end 주소보다 작은 값이 들어온 경우, 에러 처리.  
Ex> dump 37, 4  

② sicsim> e[dit] address, value
- 메모리의 address번지의 값을 value에 지정된 값으로 변경한다.  

③ sicsim> f[ill] start, end, value
- 메모리의 start번지부터 end번지까지의 값을 value에 지정된 값으로 변경한다.

④ sicsim> reset
- 메모리 전체를 전부 0으로 변경시킨다.

#### 3) OPCODE TABLE관련 명령어
- sic source코드를 어셈블러를 통해서 object코드로 변환시키기 위해서는 주어진 명령어(mnemonic)를 해당하는 opcode로 변환하는 작업이 필요합니다.(opcode.txt 파일이 있음.)
  18  ADD 3/4  
  58  ADDF  3/4  
  40  AND 2  
    …  
    …  
    …  
  DC  WD  3/4  

- sicsim프로그램을 실행시킬 때 opcode.txt 파일의 내용을 읽어 들여서 아래와 같은 모양의 Hash Table을 만들어야 합니다. Hash Table의 사이즈는 20 이며, Hash Function 등의 다른 사항들은 각자가 디자인 할 것.  

- Hash Table의 사이즈에 비해서 테이블에 넣어야 할 mnemonic의 개수가 훨씬 많기 때문에 충돌이 생기는 mnemonic들은 위의 그림과 같이 반드시 linked list의 형태로 구현이 되어야 합니다.  

① sicsim> opcode mnemonic  
- 명령어를 입력하면 해당하는 opcode를 출력한다.  
  sicsim> opcode ADD  
  opcode is 18  
  simsim> opcode LDB  
  opcode is 68  
  
② sicsim> opcodelist  
- opcode Hash Table의 내용을 아래와 같은 형식으로 출력합니다. (아래는 단지 참고사항이며 Hash Function에 따라서 나오는 값들의 순서는 다를 수 있습니다.)  
  0 : [ADD,18] -> [JEQ,30]  
  1 : [STS,7C] -> [LDS,6C] -> [JEQ,30]  
    …  
  19 : [LPS,D0]  
