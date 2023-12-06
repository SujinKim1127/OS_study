### Virtual Memory

- physical memory에서 user logical memory를 분리한 것
- 프로그램의 일부만 메모리에 있으면 된다
- 프로세스를 실행할 때 실행에 필요한 일부만 메모리에 로드하고 나머지는 디스크에 두는 것
- Logical address space는 physical address space보다 훨씬 클 수 있다
- 여러 프로세스에서 address space 공유 가능
- 프로세스 생성 속도 향상
- 더 많은 프로그램을 동시에 실행 가능
- 프로세스를 load하거나 swap 하는데 필요한 I/O 감소

> virtual memory를 지원한다 → 실행할 때 program 일부만 올린다 → process 생성이 빨라진다
> 

### Demand Paging

- 필요할때 page를 메모리에 올리는 것
- CPU가 요청할때, 프로세스의 데이터를 RAM에 올리는 것
- 프로세스의 모든 데이터를 메모리에 올리진 않음
- valid-invalid bit 사용

장점

- 현재 필요한 페이지만 메모리에 적재 → 메모리 사용량 감소
- 물리적 메모리 용량의 제약에서 벗어나기 가능
- 프로세스 전체를 메모리에 올리지 않기 때문에 입출력 오버헤드 감소

### valid-invalid bit

- page table에서 해당 page가 메모리에 있는지 나타냄
- invalid bit: 물리적 메모리에 없음
- 모든 page entry가 invalid로 초기화되어있음
- 주소 변환 시 bit가 invalid → page fault 발생

### page fault

CPU가 프로그램을 실행하면서 필요한 메시지가 물리적 메모리에 없는 경우에 발생 → swap 영역(가상 메모리)에서 페이지를 찾아서 physcial 메모리에 로드

<img width="547" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/9abc4484-9615-4eb8-9e31-4e42b1e7b2cb">

과정

1. OS는 내부 테이블 확인
    - invalid reference → 중단 (page fault)
    - just not in memory
2. (필요한 page를 찾고, physical memory에서) 빈 프레임 찾기
3. 디스크 작업을 통해 페이지를 프레임으로 swap
4. 테이블을 재설정하여 지금 메모리에 페이지 표시
    - validation bit를 V로 설정
5. page fault의 원인이 된 instruction 재시작

> 1. invalid 페이지에 접근하면 MMU가 trap을 발생하여 OS에 알림
2. OS는 CPU의 동작을 잠시 멈춤
3. OS는 요구된 페이지를 가상 메모리에서 찾음
4. 해당 페이지를 물리적 메모리의 비어있는 프레임에 로드
5. 페이지 테이블을 최신화
6. 중단되었던 CPU 다시 시작

⇒ physical 메모리에 비어있는 프레임이 존재하지 않을 경우 어떻게 할 것인가? 
: page replacement 알고리즘 사용하여 page fault를 최소화하자
> 

왜 page fault를 최소화해야하는가?

→ 오버헤드가 발생하니까 많이 발생할 수록 성능이 저하됨

### Page Replacement 알고리즘
<img width="527" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/d614278f-6a71-4cea-8dae-17454a88f69b">

현재 자신이 차지하고 있는 프레임을 지금 당장 실행해야 할 페이지에게 넘겨줄Victim Frame을 찾는 과정

- frame: 물리 메모리를 일정한 크기로 나눈 블록
- page: 가상 메모리를 일정한 크기로 나눈 블록

기본 알고리즘 순서

1. 디스크에서 원하는 페이지의 위치 찾기
2. free frame 찾기
    - free frame 있으면 사용하기
    - 없으면 , victim frame 찾기 위해 page replacement 알고리즘 사용
3. 원하는 페이지를 사용 가능한 프레임으로 가져와서 page와 frame 테이블 업데이트
4. 트랩의 원인이 된 instruction을 다시 시작하여 프로세스 계속 진행

### FIFO

가장 먼저 올라온 페이지를 우선적으로 내보내는 선입선출 알고리즘

![image](https://github.com/SujinKim1127/OS_study/assets/58413633/c58e322b-cd0e-466d-9c67-ef5b3162324f)

page faults: 15

page frame을 더 늘릴 경우 page faults가 더 발생하는 현상 발생 → **Belady’s 이상현상** 발생

### Optimal

- 가장 먼 미래에 참조되는 page를 대체하는 방법
- 앞으로 사용하지 않을 아이를 쫓아내기
- 항상 최적의 결과를 가짐
- 미래의 참조를 모두 알고 있어야 함 → 실제로 사용하기 어려움

![image](https://github.com/SujinKim1127/OS_study/assets/58413633/866da5d2-69bb-41e9-bbba-b67275b482a9)

page faults: 9

### LRU

- 사용한지 오랜된 것 쫓아내는 방법

![image](https://github.com/SujinKim1127/OS_study/assets/58413633/b82e70e5-b6cc-4573-a5f7-43b8f29de3aa)

page faults: 12

### Second-Chance

<img width="550" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/c50560b5-a73f-445b-9ae4-a7f39e0a58ce">

- LRU approximation algorithms
- clock replacement
- reference bit가 0인 것을 찾을때까지 시계처럼 한 바퀴씩 포인터를 이동하다가 0인것을 찾으면 해당 page를 교체하는 방식

과정

- reference bit = 1 → 0 으로 change
- 한바퀴 돌고 왔는데 (Second chance) 여전히 0이면 해당 page를 교체

### Enhanced Second-Chance

- reference bit & modify bit 사용
- 4개의 class (reference, modify)
    - (0, 0): not recently, not modified: best page to replace 👍🏻
    - (0, 1): not recently, modified: 교체하기 전에 반드시 write
    - (1, 0): recently, not modified: 또 사용할 가능성 O
    - (1, 1): recently, modified: 또 사용할 가능성 O, 교체하기 전에 write 할 필요 O
    
    ⇒ (0,0) → (0,1) → (1,0) → (1,1) 순서로
    
- 포인터 2개가 필요 → 2 handed clock algorithm 이라고도 부름

## Allocation of Frames

### Global Replacement

- dynamic allocation
- replace 할 때 다른 프로세스에 할당된 frame 뺏어오기 가능
- 자신의 page fault rate 조절 불가능
- 좋은 처리량 (흔하게 사용)

### Local Replacement

- static allocation
- 자신에게 할당된 frame 내에서만 교체
- 알고리즘을 프로세스마다 독자적으로 운영하는 경우에 가능
- 쉬고 있는 메모리 사용 불가능 → 비효율적

### Thrashing

: 프로세스가 원활한 수행에 필요한 최소한의 page frame을 할당받지 못해서 실행보다 swapping 하는데 더 많은 시간을 소모하는 현상

발생 과정

1. page가 부족하여 page fault가 증가
2. swapping(I/O) 작업 증가 → CPU utilization 감소
3. OS는 Multiprogramming Degree를 높여야 한다고 판단 → 또 다른 프로세스 시스템에 추가
4. 프로세스당 할당된 page frame 감소 → page fault 증가
5. 프로세스는 swapping으로 인해 바쁨. 대부분의 시간에 CPU는 한가

발생 원인

- locality size의 총합 > 메모리 총 크기

### Working-Set Model

- thrashing의 해결 방안
- 가능한 최대 multiprogramming Degree를 유지하면서 Thrashing 방지

Working set: locality에 기반하여 프로세스가 일정 시간 동안 원활히 수행되기 위해 한꺼번에 메모리에 올라와있어야 하는 page들의 집합

### Page-Fault Frequency Scheme (PFF)

<img width="536" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/1f6237f9-1106-402d-9f94-72ac319ce850">

page fault의 상한값과 하한값을 두고, 

page fault rate가 상한값을 넘으면 frame을 더 할당하고

하한값보다 낮아지면 할당된 frame 수를 줄이는 방법

## Allocating Kernel Memory

- user 모드에서 수행중인 프로세스가 추가적인 메모리를 요구하면 커널이 관리하는 페이지 프레임 중에서 할당
- 커널 메모리는 별도의 메모리 풀에서 할당하는 정책 사용
    - 커널
        - 다양한 크기의 자료구조를 위해 메모리 할당 받음
        - 단편화에 의한 낭비 줄여야 함
        - 커널 코드 or 데이터는 페이징 X
    - 사용자 모드 프로세스
        - 페이지: 연속적일 필요 X
        - 물리 메모리에 직접 접근하는 장치는 물리적으로 연속적인 메모리 필요로 함

### Buddy System Allocator
<img width="512" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/cebb273c-9978-417c-908d-dd89bd5cce6c">

- 큰 버퍼들을 반복적으로 이등분하여 작은 버퍼들 생성
- 인접한 free 버퍼들을 합치는 과정 반복
- 버퍼가 나누어 질 때, 각각을 서로의 버디라고 부름
- 외부 단편화 없애기 위해 사용

### Slab Allocator

<img width="514" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/842190ca-64ae-4678-a857-dd7a682d31f6">

- cache에서 각 크기마다 Object를 가지고 있게 되어서 내부 단편화 없애기 가능
- Slab: 물리적으로 연속된 page들의 묶음
- 미리 할당하는 방법.
- Cache에 저장해두었다가, 사용되면 Used로 표시, 사용하지 않으면, Free로 Object에 표시
- 단편화에 의한 메모리 낭비 X
- 메모리 요청 빠르게 수행




<br/><br/><br/>


---

출처
> https://rebro.kr/179
> https://namoo-gamedev.tistory.com/28
> [https://velog.io/@jiseong/OS-가상메모리와-페이지폴트-gyq8tftz](https://velog.io/@jiseong/OS-%EA%B0%80%EC%83%81%EB%A9%94%EB%AA%A8%EB%A6%AC%EC%99%80-%ED%8E%98%EC%9D%B4%EC%A7%80%ED%8F%B4%ED%8A%B8-gyq8tftz)
> https://namoo-gamedev.tistory.com/28
> https://velog.io/@huttzza/OS-Ch10-Virtual-Memory-2
> [https://wpaud16.tistory.com/entry/운영체제-Demand-paging-요구-페이징-page-fault란](https://wpaud16.tistory.com/entry/%EC%9A%B4%EC%98%81%EC%B2%B4%EC%A0%9C-Demand-paging-%EC%9A%94%EA%B5%AC-%ED%8E%98%EC%9D%B4%EC%A7%95-page-fault%EB%9E%80)
> https://blog.naver.com/klp0712/220883220425
