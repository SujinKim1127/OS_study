### Overview of Mass Storage Structure

HDD 접근 시간

1. random access time
2. transfer time
3. disk controller overhead

<img width="479" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/24c26c27-00bb-44dc-99fe-d3358bca4a36">

- transfer rate: 드라이브와 컴퓨터 간의 데이터 흐름의 속도
- positioning time (random-access time): 탐색 시간 + 회전지연시간
- seek time: disk arm을 원하는 cylinder로 이동하는데 필요한 시간
- rotation latency: 원하는 sector가 disk head 위치까지 회전하는데 걸리는 시간

### Hard Disk Performance

- Access Latency = Average access time = 평균 탐색 시간 + 평균 지연
    
    = average seek time + average latency (1/2 * latency (60/RPM))
    
- Average I/O time
    
    = average access time + (amount to transfer / transfer rate) + controller overhead
    
<img width="501" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/f4a51e9c-ff1b-4d6e-ae49-072e7d3d2817">

### Disk Scheduling

- 접근 시간 최소화
- 전송 대역폭(bandwidth) 최대화
    - 전송 대역폭 = 전송된 총 바이트 수 / 첫번째 서비스 요청과 마지막 전송 완료 사이의 전체 시간
- seek time 최소화
- 원하는 드라이브와 컨트롤러가 쉬고 있으면 system call 요청은 바로 시작되지만, 바쁘면 그 요청은 그 드라이브의 큐에 들어가 기다려야 하는데 이때 큐의 순서를 조정하여 성능 향상 가능

### FCFS Scheduling

<img width="490" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/bd9be445-ca0f-4ca3-bfc9-682fd9d3ae0d">

⇒ total head movement = 640 cylinders

- 디스크 스케줄링의 가장 간단한 형태
- 공평해보이지만 빠른 서비스를 제공하지는 못함

### SSTF Scheduling

<img width="492" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/20177a2a-f0e0-4433-8320-60815258c841">

⇒ total head movement = 236 cylinders

- 현 위치에서 seek time이 가장 짧은 위치로 먼저 접근
- starvation 문제 (가까운것만 계속 처리하니까)

### SCAN Scheduling

<img width="495" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/b30d3ab0-241b-4a23-81d9-9474c0675804">

⇒ total head movement = 236 cylinders

- disk arm이 디스크의 한 끝에서 시작하여 다른 끝으로 이동하며,
- 가는 길에 있는 요청 모두 처리
- 다른 한쪽 끝에 도달하면 역방향으로 이동하면서 오는 길에 있는 요청 모두 처리
- 디스크 양쪽을 계속해서 가로지르며 왕복

### C-SCAN (Circular SCAN) Scheduling

<img width="482" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/3a38c0dd-e54d-4a20-88e7-50d1bb2dbe24">

⇒ total head movement = 382 cylinders

- 각 요청에 걸리는 시간을 좀 더 균등하게 하기 위한 SCAN의 변형
- 한쪽으로 헤드를 이동해 가면서 요청 처리
- 한쪽 끝에 다다르면 처음 시작했던 자리로 돌아가서 서비스 시작

### Selecting a Disk-Scheduling Algorithm

- SSTF가 흔하고 자연스러움
- SCAN & C-SCAN은 heavy loads on the disk 일때 젤 적합
    - less starvation

### NVM (Nonvolatile Memory Device)

- 비휘발성 메모리인 SSD와 USB 드라이브
- HDD보다 신뢰성이 높음
- 탐색이나 지연시간이 없기 때문에 속도 빠름
- 메가바이트 당 가격 높고, 용량은 작고, 수명이 짧아 용도가 제한적

NAND 반도체는 자체적인 저장 및 신뢰성 문제 있음

- 페이지 단위로 읽고 쓰기는 가능하지만 데이터 덮어쓰기 불가능
- 삭제는 여러 페이지로 구성된 블록 단위로 이루어짐

### NAND 플래시 제어기 알고리즘

- 덮어 쓸 수 없기 때문에 유효한 데이터와 유효하지 않은 데이터가 포함된 페이지 존재
- 유효한 데이터를 포함하는 논리 블록을 추적하기 위해 컨트롤러는 플래시 변환 계층(Flash Translation Layer, FTL)을 유지
- 유효하지 않는 가용 공간을 가비지 수집으로 처리
- 쓰기 성능 향상을 위해 과잉 공급(over-provisioning) 기법 사용
- 과잉 공급 공간은 마모 평준화(**wear leveling**)에 도움 됨

### NVM 스케줄링

- 간단한 FCFS 정책 사용
- Linux의 NOOP 스케줄러는 FCFS 정책 사용하지만 인접한 요청을 병합하도록 수정
- NVM 쓰기 성능은 장치가 얼마나 찼는지, 얼마나 마모되었는지에 따라 달라짐
- 응용 프로그램이 아니라 가비지 수집 및 공간 관리를 수행하는 NVM 장치에 의한 I/O 요청 생성을 쓰기 증폭(write amplication)이라 하며 장치의 쓰기 성능에 영향을 줌

### Swap-Space Management

swapping은 사용 가능 물리 메모리가 매우 작아질때 활성화

사용 가능 메모리를 만들기 위해 (사용 빈도가 적은) 프로세스들은 메모리에서 swap space로 이동

swap space

- 일반 파일 시스템이 차지하고 있는 공간
- 별도의 raw 파티션

swap-space management

- 커널은 swap maps을 사용하여 swap-space 상태를 추적

## RAID

### RAID Structure

- RAID (Redundant Array of Inexpensive Disk)
    
    : 여러개의 디스크는 redundancy를 이용해 안정성 제공
    
- 고장 발생까지의 평균 시간 증가
- 평균 수리 시간 - 다른 고장이 발생할 수 있는 노출 시간
- 위의 요소를 기준으로 데이터 손실까지의 평균
- write performance를 향상시키기 위해 NVRAM 비번하게 결합

### RAID Levels

<img width="427" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/8a5b79d5-38a4-40a0-b747-0b61b24a5b9e">

- RAID 0: non-redundant striping
- RAID 1: mirrored disks
- RAID 2: memory-style error-correcting codes
- RAID 3: bit-interleaved parity
- RAID 4: block-interleaved parity
- RAID 5: block-interleaved distributed parity
- RAID 6: P + Q redundancy

<br/><br/><br/>

참고

[https://velog.io/@pjy05200/운영체제-공룡책-CH-11.-Mass-Storage-Structure](https://velog.io/@pjy05200/%EC%9A%B4%EC%98%81%EC%B2%B4%EC%A0%9C-%EA%B3%B5%EB%A3%A1%EC%B1%85-CH-11.-Mass-Storage-Structure)

https://jehunseo.tistory.com/205
