### System Call

- 커널이 자신을 보호하기 위해 만든 인터페이스
- 사용자으로부터 컴퓨터 자원을 보호하기 위해 자원에 직접 접근하는 것을 차단
  ⇒ 자원을 사용하려면 system call 을 이용하여 호출
- API
  - 운영체제의 API가 system call을 호출
  - 사용하기 편리하게 응용 프로그램에서 사용할 수 있도록, OS가 제공하는 기능을 제어할 수 있게 만든 인터페이스

### Simple Structure - MS-DOS

- not divided into modules
- 최소 공간에서 가장 기능적인 것을 제공하기 위해 작성
- 일반 앱이 하드웨어에 직접적으로 접근 가능
- 유저와 커널 구분 못함

### Non Simple Structure - UNIX
<img width="388" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/5cd48b6f-646d-453a-97f4-4505fed0f8c3">


- Layer 0 : hardware
- beyond simple but not fully layered
- 유지보수에 돈이 많이 든다
- 레이어 간의 절차 엄격 → 성능 저하
- 각 계층은 바로 아래 계층에서 제공하는 기능과 서비스만 사용

장점

- 구축과 디버깅의 단순함
- 각 계층은 연산들이 어떻게 구현되었는지 알 필요 X

단점

- 각 계층의 기능을 명확하게 정의하기 어려움

### Microkernel System Structure

- mach
- 커널 사이즈 최소화
- primitive한 것만 존재
- memory resident system program 최소화

### 리눅스 시스템 구조

- 모듈식 구조
- 단순하지만 확장이 어려움
- SCI 오버헤드 X
- 커널 내 통신 빠름 → 속도, 효율성 높음
