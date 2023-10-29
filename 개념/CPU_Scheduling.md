### CPU Scheduler

short-term scheduler = CPU scheduler

non-preemptive

- running → waiting
- terminates

preemptive

- running → ready
- waiting → ready

### Dispatcher

한 프로세스를 멈추고 다른 프로세스를 실행하는데까지 걸리는 시간

- context switching
- switching to user mode
- 사용자 프로그램을 적절한 주소에서 다시 시작

### Dispatch Latency

context switiching 하는데 걸리는 overhead

dispatcher가 자주 호출되면 그만큼 system performance가 떨어짐

⇒ user가 쓸 시간을 cpu가 사용하기 때문

### Scheduling Criteria

maximize 해야하는 것(비율)

- **cpu utilization**: 최대한 cpu가 바쁘도록
- **throughput**: 단위 시간당 실행되는 프로세스의 수

minimize 해야하는 것

- **turnaround time**: 프로세스가 들어가서 끝날때까지 걸린 시간
- **waiting time**: ready queue에서 기다린 시간의 합
- **response time**: 프로세스가 처음으로 cpu를 할당받기까지 걸린 시간

### FCFS

- ready queue에 도착한 순서대로 CPU 할당
- 한번 실행되면 그 프로세스가 종료되어야 다음 프로세스 실행 가능

### SJF

- ready queue에 있는 프로세스 중에 실행 시간이 가장 짧은 작업부터 CPU 할당
- 수행시간이 긴 프로세스는 뒤로 밀려나는 기아 현상 발생 → **Aging**으로 해결

### SRTF

- preemptive SJF
- 새로운 프로세스가 도착했을때, 현재 남아있는 작업 중 가장 빨리 끝나는 작업부터 CPU 할당
- 기아 현상 존재

### PS

- priority scheduling
- 우선 순위가 제일 높은 프로세스에게 CPU 할당

### RR
- 한 프로세스가 할당받은 시간 동안 작업하다가 작업을 완료하지 못하면 ready queue의 맨 뒤로 가서 자기 차례를 기다리는 방식


## Multilevel Queue Scheduling

특성에 따라 작업을 나누고 다른 알고리즘 적용

Foreground(interactive)-RR

Background(batch)-FCFS

장점

- ready queue 에서 하나 선택하는데 걸리는 시간 단축 가능
- 하나의 ready queue에서 하나의 프로세스를 선택하는데 걸리는 overhead 감소 가능
- 우선순위 명확하게 구분 가능 (독립적으로)
    
    ⇒ single 보다는 grouping해서 스케줄링하기 때문
    

단점

- priority scheduling이 가지는 단점과 동일
- ready queue 별로 봤을때는 priority scheduling 이기 때문
- 맨 마지막 프로세스는 위에 있는 프로세스들이 다 실행되기 전까지 실행 못함
    
    ⇒ starvation 발생
    
    (queue와 queue 사이를 이동 못하니까)
    

### Multilevel Feedback Queue
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/8c1136f7-81ae-42db-9a9a-3469301eb400)

처음 들어올때 8로 들어가고 quantum 8에서 못끝내면 qunatum 16으로 (CPU burst가 모자라기 때문)

### Multiprocessor Scheduling

cpu scheduling이 single processor 보다 complex

based on homogeneous multiprocessors

- Asymmetric multiprocessing(비대칭)
    - system data structure에 접근하여 분배
    - 코어 각각 하는일이 다름
- Symmetric multiprocessing (SMP)
    - 어느 processor에 넣어도 performance가 같다
    - 모든 코어들이 다 요청을 받는 것

Load Balancing attempts to keep workload evenly distributed

Load Balancing 해결 방법

- Push migration
    - 모여 있으면 이동 시킴
    - 주기적으로 각 core에 ready queue가 몇 개씩 있는지 검사
- Pull migration
    - 비어있으면 많이 있는 core꺼 가져오기
    - 각 core마다 자기 ready queue를 모니터링하는 processor 생성
    
    ⇒ idle processor가 non idle processor의 process pull
    

### Processor Affinity

: 멀티 코어 프로세서로 동작하는 컴퓨터 시스템에서 Process Scheduling을 할때 특정 CPU에서 실행되도록 제한하는 scheduler의 고려사항

프로세스의 프로세서 이주 정도

가급적 전에 사용하던 코어 사용하기

NUMA 구조 → 프로세서 친화성 좀 더 심도 있게 고려하기
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/cd876428-8152-4335-a3bc-f63225f0fe75)


### Multicore Processors

- 각각의 core마다 여러개의 thread를 실행할 수 있다

### Real-Time CPU Scheduling

Soft real-time systems

- 어느 정도의 error 허용 → quality 떨어짐
- context switching 적게, deadline meet이 중요

Hard real-time systems

- 반드시 deadline 지켜야 됨
- 융통성이 없다!

### Rate Monotonic Scheduling

- period 짧으면, rate 높아짐, priority도 높아짐
- 문제점
    - 요청이 오는 이벤트 주기를 파악하기 어려움
    - CPU 할당받고 수행해서 종료될때까지 걸리는 시간이 정확하지 않음

![image](https://github.com/SujinKim1127/OS_study/assets/58413633/bbc4ee08-731a-4f91-a732-ba2d4930502b)

### Missed Deadlines with Rate Monotonic Scheduling

Rate monotonic 스케줄은 정적 우선순위를 이용하는 경우가 최적
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/1481876b-c617-4579-8e0f-cb7e52241e27)


### Earliest Deadline First Scheduling (EDF)

- deadline 급한거 먼저 실행 (선점형)
- 이론적으로 100% CPU를 사용할 수 있지만,
- 실제로는 context switching 때문에 100%로 CPU를 사용하는 것은 불가능하다

### Algorithm Evaluation

Evaluation Methods

- deterministic evaluation
- queueing models
- simulations
- implementation
