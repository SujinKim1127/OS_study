# Deadlock

프로세스들이 서로 가진 자원을 기다리며 block 되어 더 이상 진행이 될 수 없는 상태

- 요청(request): 만약 요청이 즉시 승인될 수 없으면, 요청하는 프로세스는 자원을 획득할 때까지 대기
- 사용(use): 프로세스는 자원을 이용
- 방출(release): 프로세스는 자원을 놓아줌
- allocate: 자원을 받음

### Deadlock Characterization

deadlock이 발생하려면 아래의 4가지 조건을 만족해야한다

- mutual exclusion
    - 매 순간 하나의 프로세스만이 자원 사용 가능
- hold and wait
    - 자원을 가진 프로세스가 다른 자원을 기다릴때, 보유하고 있는 자원을 놓지 않고 계속 가지고 있음
- no preemption
    - 프로세스는 OS에 의해 강제로 자원을 빼앗기지 않음
- circular wait
    - 자원을 기다리는 프로세스 간에 사이클 형성되어야 함
        
        ex) 프로세스 p0, p1, … , pn이 있을때 p0은 p1은 기다리고 p1은 p2를 기다리고, pn은 p0을 기다림
        

### Deadlock Prevention

deadlock을 발생하게 하는 4가지 조건 중에 적어도 하나가 일어나지 않게 보장함으로써 deadlock 예방 가능

- 상호 배제 (mutual exclusion)
    - 공유 가능한 자원들의 배타적인 접근 요구 X
    - ex) 읽기 전용 파일에 대해서는 배타적 접근 필요X
        
        프린터와 같은 비공유 자원의 경우는 배타적 접근 보장되어야 함
        
    - critical section problem을 해결하기 위해서는 이 조건은 반드시 만족해야함 → 공유자원이 존재하면 이 조건은 만족해야됨
- 점유하며 대기(Hold and wait)
    - 프로세스가 자원을 요청할 때 다른 어떤 자원도 점유하면 안됨
    - 방법 1) 프로세스가 실행 시작 전에 필요한 모든 자원을 요청하여 모두 할당 받음
        
        → 낮은 자원 이용률 야기
        
    - 방법 2) 프로세스가 아무 자원도 점유하고 있지 않을 때만 자원을 요청하도록
        
        → 순차적 자원 할당이 이루어짐. 기아 발생할 수도
        
- 비선점(No Preemption)
    - 한 개 이상의 자원을 점유한 프로세스가 즉시 할당 받을 수 없는 추가의 자원을 요청하면, 현재 잡고 있는 모든 자원을 내려 놓도록 한다
    - 프로세스가 어떤 자원을 기다려야 하는 경우 보유하고 있던 자원이 선점됨
    - 모든 필요한 자원을 얻을 수 있을때 그 프로세스는 다시 시작
- 순환 대기(Circular Wait)
    - 모든 자원 타입들에 대해 전체적으로 순서(order) 부여
    - 각 프로세스는 오름차순으로만 자원 요청 가능

deadlock prevention의 부작용

- 효율성과 처리량 감소
- starvation 발생할 수도

### Deadlock Avoidance

deadlock이 발생할 가능성이 있는 경우에 아예 자원을 할당하지 않는 방식

- safe sequence: 프로세스의 sequence <P1, P2, P3 … , Pn>이 있을때, Pi의 자원 요청이 ‘가용 자원 + 모든 Pj (j < i)의 보유 자원’에 의해 충족되는 경우 sequence를 safe하다고 함
- safe state: 시스템 내의 프로세스들에 대한 safe sequence가 존재하는 상태

<img width="370" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/34f5b16b-aba3-4cbb-9487-5bdb17d713cf">

deadlock avoidance는 시스템이 unsafe state에 들어가지 않는 것을 보장

## Avoidance Algorithms

### 1. Resource-Allocation Graph (자원 할당 그래프 알고리즘)

<img width="234" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/6803ba77-025d-4602-906c-e5ca10e420fa">

- 실선: request edge
- 간선: assignment edge

deadlock을 피하려면 request edge가 assignment edge로 변경될 때 점선을 포함하여 사이클이 생기지 않는 경우에만 요청된 자원 할당

### 2. Banker’s algorithm

자원할당그래프가 하나의 타입에 자원 인스턴스가 여러개 있는 경우에는 적용을 못하는 문제점 해결

- 프로세스는 자신이 필요로 하는 자원의 최대 개수를 미리 신고
- 시스템은 자원의 최대 개수를 허용했을 경우에도 안전 상태에 있도록 판단

[ 가정 ]

- 모든 프로세스는 자원의 최대 사용량을 미리 명시
- 프로세스가 요청 자원을 모두 할당받은 경우 유한 시간 안에 자원들을 다시 반납
    
    ![image](https://github.com/SujinKim1127/OS_study/assets/58413633/b87961a9-33ba-43d7-8da6-ed1dd86ff61c)
    

### Deadlock Detection and Recovery

detection 알고리즘에 deadlock이 존재한다는 것을 파악하면, 여러가지 회복 대안 가능

- 운영자에 의한 해결
    - deadlock 발생을 운영자에게 알림
    - 운영자가 deadlock을 수작업으로 처리하도록
        
        → 비현실적임
        
- 프로세스 종료 (process termination)
    - 순환 대기를 깨뜨리기 위해 하나 이상의 프로세스를 종료시킴
    - deadlock 프로세스 모두 중지 (비용 많이 든다)
    - deadlock이 제거될때까지 하나씩 중지 (오버헤드 발생)
    
    프로세스 종료 기준
    
    - 프로세스 우선 순위
    - 프로세스가 얼마나 계산되었고, 종료될때까지 얼마나 남았는지
    - 프로세스가 사용한 자원
    - 프로세스가 완료되기 위해 필요한 자원
    - 얼마나 많은 프로세스들이 종료될 필요가 있는지
    - 프로세스가 대화형인가, 일괄처리형인가
- 자원 선점 (resource preemption)
    - deadlock이 깨질때까지 프로세스로부터 자원을 선점해 다른 프로세스에게 주어야 함
    
    자원 선점 3가지 요소
    
    - 희생자 선택(selection of a victim): 비용 최소화 고려
    - rollback: 프로세스를 안전한 상태로 되돌리고 그 상태에서부터 재시작
    - starvation(기아): 같은 프로세스가 항상 희생자로 선택될 수도

<br/><br/><br/>

> 참고
>
> https://rebro.kr/177
