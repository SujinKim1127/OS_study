### Race Condition: 경쟁 조건

- 생산자와 소비자 작업이 올바르게 작성되었어도, 이들 작업이 동시에 수행되면 바르게 동작하지 않을수도
- 경쟁 조건에 의해 불확실한(inconsistency) 상태에 도달할 수도
    - 두 개 이상의 프로세스가 동시에 하나의 변수를 조작하는 것을 허용했기 때문
- 여러 프로세스들이 동일한 데이터에 동시에 접근해서 조작할 수도
- 실행 결과 값은 접근과 갱신이 일어나는 순서에 따라 달라질 수도

예방하려면?

⇒ 한번에 오직 하나의 프로세스만 하도록 동기화(synchronization)하자!

### Critical section Problem

- critical section
    - process 간 공유된 자원을 access하는 code
    - 공유 자원 접근 순서에 따라 실행 결과가 달라지는 프로그램의 코드 영역
    - critical section 안에서 race condition 발생
    - 병렬 프로그래밍에서 둘 이상의 스레드가 동시에 접근해서는 안되는 공유 자원을 접근하는 명령문 또는 코드의 일부 영역을 말함
    
    ⇒ 동기화 처리 필요
    
⭐ **critical section 해결 조건** ⭐

1. mutual exclusion
    - 하나의 프로세스가 critical section 에서 실행되고 있으면, 다른 프로세스들은 critical section 에서 실행될 수도 진입할 수도 없음
2. progress
    - critical section에 실행되고 있는 프로세스가 없을 경우, 진입 순서는 프로세스들에 의해 적절히 결정
3. bounded waiting
    - 한 프로세스가 critical section에 진입하려고 요청하면, 요청이 허용될때까지, 다른 프로세스가 critical section에 진입할 수 있는 횟수가 제한되어야 함
    - starvation - free

```tsx
do {
     wait(mutex);  // entry section
     -- critical section --
     signal(mutex); // exit section
     -- reminder section --
}
```

### Peterson’s solution

`turn` 과 `flag` 라는 변수로 critical section에 들어갈 프로세스/스레드를 결정하는 방법

```tsx
do {
	flag[i] = true;
	turn = j;
	while (flag[j] && turn == j);
			-- critical section --
	flag[i] == false;
			-- reminder section --
} while (true);
```

- mutual exclusion
    - Pi는 `flag[j] == false` 거나 `turn == i` 인 두 조건 중 하나가 만족되어야만 critical section에 진입 가능
    - Pi는 `flag[i] == true` 인 상태에서만 critical section 진입 가능
    - Pi와 Pj는 동시에 critical section에 진입 불가능
- progress
    - Pi는 `flag[j] == true` 이고 `turn == j` 인 경우에만 멈춤
- bounded waiting
    - Pi는 Pj의 한 번의 출입 후에 critical section에 진입 가능

### Synchronization Hardware

- 실질적으로 synchronization 하드웨어는 없지만, atomic hardware instruction 방법 사용
- atomic = non-interruptible

### Mutex Locks

- mutex: 프로세스가 서로 간에 공유 자원의 동시 접근을 허용하지 않기 위한 기술
    
    ⇒ 자원에 접근하기 위해 각 프로세스는 lock을 획득하여 접근 허용 권한을 얻어야 한다
    
- busy waiting을 요구하기 때문에 spinlock 이라고 부름

```tsx
do {
	acquire lock
		-- critical section --
	release lock
		-- reminder section --
} while (true);
```

- ex) 열쇠가 있어야 들어갈 수 있는 화장실
    - 화장실 이용자: 프로세스 or 쓰레드
    - 화장실: 공유 자원
    - 화장실 열쇠: 공유자원에 접근하기 위해 필요한 오브젝트

### Semaphore

- `wait()`: acquire, 사용권 획득, S의 초기값은 1
    
    ```tsx
    wait(S) {
    		while (S <= 0)
    				; // busy wait
    		S--;
    }
    ```
    
- `siginal()` : release, S는 절대 - 가 될 수 없다
    
    ```tsx
    signal(S){
    		S++;
    }
    ```
    
- ex) 여러개 칸이 있고 빈 칸의 개수를 보여주는 화장실
    - 화장실에 들어갈때 + 1, 화장실에서 나올때 - 1

**busy waiting이 없는 semaphore**

- semaphore 구조체에 연결된 연결 리스트로 구현
- 대기 중인 프로세스들의 PCB를 연결
- 음수 값은 대기 중인 프로세스 개수 의미
- 양수 값은 이용 가능한 자원의 개수 의미

semaphore의 문제

- Deadlock
- starvation
- priority inversion

### Monitors

semaphores에서 발생하는 문제들을 프로그래밍 언어 자체가 해결해주는 도구


<br/><br/><br/>


> 참고
> 
> https://hun-developer.tistory.com/36
> 
> https://velog.io/@chy0428/OS-%ED%94%84%EB%A1%9C%EC%84%B8%EC%8A%A4-%EB%8F%99%EA%B8%B0%ED%99%94
>
> https://worthpreading.tistory.com/90
