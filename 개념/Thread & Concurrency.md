### Thread

- 프로세스 내에서 실행되는 여러 흐름의 단위

### Multithreaded programming의 장점

1. Responsiveness 응답성
    - 최초의 응답이 올때까지 걸리는 시간이 빨라짐
2. Resource Sharing 자원 공유
    - process가 할당받은 resource를 thread가 공유하기 때문에 자원 공유 가능
    - OS는 process에만 자원 할당 해줌
3. Economy 경제성
    - thread는 light-weight process이기 때문에 OS가 하는일이 적음
    - system 개입이 별로 없다
4. Scalability 확장성
    - 한 프로세스를 여러 프로세서에서 수행 가능 → 효율적

### Multicore Programming

multicore programming challenges

- task dividing activities
- load balancing
- data splitting
    - cpu가 멀티 프로세서여도 메모리가 하나면 메모리에 access 할 때 bottle neck이 발생되므로 memory도 application이 task로 나뉘듯 data도 separate core에서 실행될 수 있도록 (module로) 나누어야 함
- minimizing data dependency
- testing and debugging

### Parallelism

- 프로세서가 여러개인 경우 멀티 스레드를 통해 병렬성을 높일 수 있다
    
    → 프로세스의 스레드들이 각각 다른 프로세서에서 병렬적으로 수행될 수 있기 때문
    
- CPU 개수에 비례
- Data parallelism
- Task parallelism

### Concurrency

- 프로세서가 하나인 경우엔 멀티 스레드를 통해 동시성 높이기 가능
- 실제로는 각각의 시간에 한 작업만 수행되지만, 병렬적으로 수행되는것처럼 보이는 것

### Concurrecny vs Parallelism
<img width="604" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/12068c53-bb08-4691-b6d0-b9661e3e1d84">

### Multithreading Models

- user threads
    - 커널 위에서 커널의 지원 없이 user level의 thread library가 관리
    - POSIX Pthreads, Windows threads, Java threads
- kernel threads
    - 커널이 지원하는 스레드
    - Windows, Solaris, Linux, Mac OS

### User threads와 Kernel threads의 관계

1. Many-to-One Model
    
    <img width="194" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/dbc7021f-7f57-4f70-819b-52074b5ace13">

    - 하나의 커널 스레드에 여러 유저 스레드를 연결하는 모델
    - many user-level threads mapped to single kernel thread
2. One-to-One Model
    
    <img width="572" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/00f4c0e4-72f0-408c-9be8-6e894113adcc">

    - 하나의 커널 스레드에 하나의 유저 스레드가 대응하는 모델
    - many-to-one 보다 concurrency 높음
3. Many-to-Many Model
    
    <img width="313" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/5799c08c-510f-40f6-98e7-a1b2f7a57624">

    - 한 개의 process에 대해 여러개의 kernel thread가 생성됨
    - OS가 충분한 수의 커널 스레드를 생성하도록 함
4. Two-level Model
    
    <img width="567" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/74509151-b69c-4b5f-a31c-7c3f90a08292">

    - many-to-many model에서 확장된 개념

### Pthread

- 유닉스계열 POSIX 시스템에서 병렬적으로 작동하는 소프트웨어를 작성하기 위하여 제공하는 API

## Threading Issues

### Thread Pools

- 미리 스레드 풀에 여러 스레드를 만들어두고 요청이 오면 스레드 풀에 기존에 존재하던 스레드를 할당해주는 방법 사용

장점

- 약간 빠름
- application 내에서 만들 수 있는 thread 수 제한
- creation 하는 작업을 원래의 running task에서 뺄 수 있음

### fork(), exec() system call

- 몇몇 UNIX 시스템은 두 버전의 fork()를 가짐
- exec() system call: 기존 스레드들을 모두 무시하고, 새로운 프로그램으로 대체됨

### Signal Handling (시그널 처리)

- 시그널은 프로세스에게 어떤 사건이 일어났음을 알리기 위해 사용됨
- 두가지 유형
    - Synchronous signal: 어떤 사건을 처리하기 위해 발생
    - Asynchronous signal: 임의 시점에 발생
- 시그널 전달 과정
    - 시그널은 특정사건이 발생해야 생성됨
    - 시그널이 생성되면 프로세스에게 전달됨
    - 시그널이 전달되면 반드시 처리(handling)되어야 함

### Thread Cancellation

thread to be canceled is target thread

<img width="604" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/876beed6-176f-4d25-8f7f-5b9befc86b24">

- Asynchronous cancellation: target thread 바로 종료
- Deferred cancellation: target thread가 주기적으로 종료해야하는지 체크

### Thread-Local Storage

스레드가 같은 프로세스에 속하는 다른 스레드들과 데이터 영역을 공유하지 않고, 고유한 데이터 영역을 가질 수 있도록 지원하는 방법

<br/><br/>

참고
https://rebro.kr/174
