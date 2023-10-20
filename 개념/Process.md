## Process

- process: 프로그램을 실행하면 프로세스가 됨
- program counter: 다음에 실행할 명령어를 지정
- stack: temporary 데이터 보유
- data: 전역, 동적 변수 포함
- heap: 실행 시간 동안 동적으로 할당된 메모리를 포함

### process state

- new: process is being created
- running: instructions are being executed
- waiting: process is waiting for some event to occur (I/O)
- ready: process is waiting to be assigned to a processor
- terminated: process has finished execution

### Process Control Block (PCB)

PCB: Program을 process로 만들어주는 것

프로세스마다 1대 1로 대응

pcb안의 내용들은 systemcall을 사용해야만 접근 가능

os만 update 가능

- process state: 프로세스의 상태를 저장
- program counter: cpu가 다음으로 실행할 기계어를 가리키는 값
- cpu registers: cpu의 범용 레지스터, 연산 레지스터 등에 있던 값
- cpu scheduling information: 우선순위, 실행시간, cpu 점유시간 저장
- memory information: 프로세스 내부에서 사용하는 메모리의 위치, 용량, 페이지 테이블 등의 정보를 가짐
- accounting information
- I/O status information

### Process Scheduling

목적: maximize CPU utilization

- Job Queue
    - 메모리 할당을 위해 대기하는 프로세스들의 집합
    - long term scheduler
- Ready Queue
    - 이미 실행 중인 CPU 할당을 위해 대기하는 프로세스들의 집합
    - short term scheduler
- Device Queue: 특정 입출력 장치에 입출력 요청을 보낼때 필요한 큐로 해당 장치에 대해 요건이 있는 프로세스들이 해당 큐에 들어감

## Schedulers

CPU에서 실행되던것이 중단되는 경우

- 프로세스가 입출력 요청을 하여 입출력 큐에 넣어지기
- 프로세스가 새로운 자식 프로세스를 생성하고 자식 프로세스의 종료를 기다리는 동안 waiting queue로
- 프로세스가 인터럽트(timer event) 발생을 기다림
- 인터럽트 또는 할당된 시간 간격이 만료되어 프로세스가 코어에서 강제로 제거되어 ready queue에 돌아가기 가능

### Short-term scheduler

- ready 상태의 process중 어떤 것을 가지고 올 것인가
- 자주 실행 됨
- 스케줄링 작업이 제일 잦고 빠르게 진행됨

### Long-term scheduler

- 동시에 실행하는 프로세스의 수를 조절하는 스케줄러
- 프로세스를 실행 가능 상태로 변경하는 작업 수행
- 자주 실행 안됨 (하나의 process가 시스템을 떠날때만 발동)
- I/O bound랑 CPU-bound 잘 섞어서 사용
- I/O bound: 연산보다는 입력 작업이 많은거
- CPU-bound: 연산 많은거

### Medium-term scheduler

- 메모리에 올려진 프로세스의 수를 **swapping**을 통해 줄임
    
    (= degree of multiprogramming 낮춤)
    
- 목적: 메모리 효율을 증대시켜 동시 실행 프로세스 수를 늘리는 것

### Context Switch

cpu가 다른 프로세스로 전환할 때, 시스템은 이전 프로세스의 상태를 저장하고 context switch를 통해 새 프로세스에 대해 저장된 상태를 로드해야 합니다.

인터럽트나 트랩이 발생하면 기존의 프로세스는 잠깐 중단되고 다른 프로세스를 실행하는데 이때 현재의 프로세스 상태를 저장, 복원해야 한다. 이때 context switching을 사용

복잡한 OS와 PCB를 갖추고 있으면 → context switch가 길어진다

context switching이 발생하는 동안에는 CPU는 작업을 수행할 수 없음 

→ context switching이 자주 발생하면 CPU 효율 저하

### Process Creation 과정

자원할당(PCB 생성)

pid 배정

process간은 무조건 tree 구조

fork() system call

- 새로운 프로세스 생성
- 자신의 코드와 데이터들을 복제해서 넘겨줌
- 자식 process는 자신의 pid 할당 받음

exec() system call

- process 메모리를 새로운 프로그램으로 덮는 역할
- 원래 부모 process로부터 복제되었던 프로그램 코드는 새로운 코드로 덮어져 사라짐

wait() system call

- 부모 process가 실행을 마친 뒤 자식 process가 종료될때까지 기다릴때 사용
- 부모 process는 wait 상태가 되어 wait queue에서 대기

### Process Creation Program in Linux
<img width="657" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/56857635-aa6d-464e-b7c3-21589bda8603">

### Process Termination

exit() system call 호출

- process가 가지고 있던 resource들 풀어줌
- 부모 process에게 자신의 상태를 전달

부모 process가 자식 process를 종료시킬 수 있는 상황

1. 자식 process가 과도하게 resource를 많이 사용할때
2. 자식 process의 작업이 더 이상 필요 없을때
3. 부모 process가 작업을 종료했고, 해당 운영체제가 orphan process를 허용하지 않을때

Orphan Process: 부모 프로세스가 먼저 실행 종료된 프로세스

Zombie Process: 자식 프로세스는 종료되었는데 부모 프로세스가 wait()을 호출하지 않은 경우

⇒ 부모가 wait() 호출하면 zombie process 제거 됨

## Inter Process Communication - IPC

시스템에서 실행중인 다른 프로세스에 영향을 주고받지 못하는 프로세스는 independent

주고 받으면 cooperating

process가 cooperating 해야하는 이유

- information sharing
- computation speed up
- modularity

IPC 방법
<img width="655" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/26e7be78-27ba-4daa-bc36-eebb2718e9e0">

1. shared memory: 공용 메모리 공간 사용
2. message passing: 프로세스간 메시지 교환
3. pipe, named pipe
4. signal
5. socket

### Shared Memory

- 공유 메모리를 생성해 공유 메모리에 공유할 데이터를 송수신하는 방법
- 공유 메모리는 유저 영역에 할당 됨
- 핵심 이슈: 사용자 프로세스가 공유 메모리에 액세스할 때 작업을 동기화할 수 있는 메커니즘을 제공하는 것

unbounded-buffer - 실질적으로 버퍼크기에 제약X

bounded-buffer - 고정된 버퍼 크기

- producer
    
    버퍼 공간 하나를 아낄 수 있다 (메모리가 작을 때 좋다)
    
    ```jsx
    item next_produced; 
    while (true) {
    		/* produce an item in next produced */ 
    		while (((in + 1) % BUFFER_SIZE) == out); 
    		/* do nothing */ 
    		buffer[in] = next_produced;
    		in = (in + 1) % BUFFER_SIZE;
    }
    ```
    
- consumer
    
    쓰기 전에만 안읽으면 됨
    
    ```jsx
    item next_consumed;
    while (true) {
    		while (in == out)
    		/* do nothing */ 
    		next_consumed = buffer[out];
    		out = (out + 1) % BUFFER_SIZE;
    		/* consume the item in next consumed */
    }; 
    ```
    

### Message Passing

- `send(message)`와 `receive(message)` 기능이 반드시 primitive로 있어야 함
- process P와 Q가 통신하려면 둘 사이에 communication link 반드시 존재해야 함
- Logical
    - Direct or indirect
    - Synchronous or asynchronous
    - Automatic or explict buffering

### Direct Communication

송수신자 명시하기

- `send(P, message)` - send a message **to** process P
- `receive(Q, message)` - Receive a message **from** process Q

링크는 OS가 만들어줌

### Indirect Communication

mailbox 사용

- `send(A, message)` - send a message **to mailbox A**
- `receive(A, message)` - receive a message **from mailbox A**

```
Q. process P1, P2, P3가 mailbox A를 나눠쓴다. 
P1이 메세지를 mailbox에 보내고 P2와 P3가 받았다. 
누가 메세지를 받는가?
```

A. 3가지 방법

- 메일박스링크가 최대 2개로만 연결되게하기
- 한번에 하나의 프로세스만 `receive()` 실행할 수 있도록 하기
- 시스템이 임의로 선택하도록

### Synchronization

Synchronous (동기화)

- Blocking send: 메시지가 수신될 때까지 sender가 block됨
- Blocking receive: 메시지를 사용할 수 있을 때까지 receiver가 block됨

Asynchronous(비동기화)

- Non-blocking send - sender가 메세지를 보내고 하던 작업 계속 진행
- Non-blocking receive - receiver가 유효한 메시지나 null을 받음


### Pipes

두 프로세스가 통신할 수 있게 하는 전달자로서 동작

- ordinary pipes - 해당 파이프를 생성한 프로세스 외부에서 액세스 불가능
- named pipes - parent-child 관계 없이 접근 가능

### Ordinary pipes

단방향 통신임

양방향 통신하고 싶으면 파이프 2개 사용하기

<img width="598" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/fb7e8a41-2c4e-494c-b732-169f6de78916">


[출처]: https://jooona.tistory.com/6
