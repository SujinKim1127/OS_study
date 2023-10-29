### Limit register vs. Base register (=relocation register)

- Limit register: 프로세스 메모리의 크기 지정
- Base register: 프로세스 메모리의 시작 주소를 가짐

### Address Binding

프로그램의 명령어와 데이터를 메모리에 적재할 때 메모리의 주소를 결정하는 것

- Compile time
    - 시작 위치 변경이 필요하면 재컴파일해야 됨
    - 컴파일러는 **absolute** code 생성

absolute code: 한번 컴파일된 메모리 주소는 절대적이라 바꿀 수 없다

- Load time
    - 컴파일러가 **relocatable** code 를 생성한 경우에 가능
- Execution time

### Logical vs. Physical Address Space

Logical Address

- 프로세스가 보는 공간
- CPU가 보는 주소

Physical Address

- 메모리에 실제로 올라가는 위치

### Memory-Management Unit (MMU)

- Logical address를 physical address로 매핑해주는 하드웨어 device
- 레지스터 두 개를 이용하여 주소변환
    - Base register: relocation register
- User program
    - logical address만 다룸
    - 실제 physical address 볼 수 없음

### Dynamic Loading

- 필요할때만 메모리에 load
- 코드 양이 많을때 유용
- memory-space utilization 향상

### Dynamic Linking

- Linking: 프로그램을 작성하고 compile하고 link에서 실행파일을 만든다. 이때 linking은 컴파일된 파일들을 묶어서 하나의 실행파일을 만든다
- static linking
    - 시스템 라이브러리와 프로그램 코드가 실행 파일 코드에 포함된 경우
- dynamic linking
    - 실행시간까지 linking 연기
    - 라이브러리 호출 부분에 stub이라는 작은 코드를 넣는다
    - 별도의 라이브러리 파일이 존재하고 실행파일에는 해당 파일의 위치를 나타내는 코드만 넣는다
    - 라이브러리가 memory address에 있는지 OS가 체크
        - address space에 있으면 그 루틴의 주소로
        - 없으면 address space에 추가
        

### 메모리 할당

메모리에 여러개의 프로세스를 배치하는 방법

- 고정 분할 방식(fixed size programming)
    - 메모리를 동일한 크기의 파티션으로 분할
    - 하나의 프로세스는 하나의 파티션에 할당
    - 구현/관리는 쉬우나 제약이 많아 요즘은 사용X
    - 비연속 메모리 할당
    - 메모리 낭비가 심함
- 가변 분할 방식(variable size partitioning)
    - OS는 어느 부분이 사용되고 어디가 가용 공간인지 테이블로 관리
    - OS는 가용 공간에 프로세스 할당
    - OS는 항시 가용 공간의 크기와 입력 큐 유지
    - 연속 메모리 할당(Contiguous Allocation)
    - 외부 단편화 문제 발생
        - 메모리 활용률 떨어짐

### Contiguous Memory Allocation

- Multiple-partition allocation
- fixed partioning

### Dynamic Storage-Allocation Problem

가변 분할 방식의 외부 단편화 문제를 해결하기 위한 방법

- first-fit
    - 단편화 고려X
    - 연속되는 메모리 공간 중에 첫번째 공간에 프로세스 배치
- best-fit
    - 빈 공간 적당한 크기 가운데 **가장 작은 공간**에 프로세스 배치
- worst-fit
    - 300 / 500 있고 250이 필요할때 500에 요청하는거
    - 빈 공간 중 크기가 가장 큰 공간에 프로세스 배치

### Fragmentation

- 외부단편화: 총 메모리 공간은 충분하지만 여유 공간이 조각되어 있어 할당 못하는 상황 (너무 커서 못들어감)
    - 해결방법: compaction
        - 메모리에 분산되어 있는 단편화된 공간들을 통합하여 하나의 커다란 빈 공간을 만드는 작업
        - relocation이 dynamic 할때만 가능
- 내부단편화: 프로세스가 필요한 양보다 더 큰 메모리가 할당되어어서 프로세스에서 사용하는 메모리 공간이 낭비되는 현상

## Non-contiguous Memory Allocation 기법

### Segmentation

- 내부단편화 해결, 외부단편화 존재
- 연속적인 공간에 저장되어 있음
- 메모리에 적재될때 빈공간을 찾아 할당하는 기법
- mapping을 위해 세그먼트 테이블 필요
- 중간에 프로세스가 메모리를 해제하면 hole 발생
- 프로세스에 존재하는 스택, 데이터, 힙, 코드 등의 영역을 개별로는 연속 할당
- 각 영역 자체는 불연속적으로 메모리에 배치
- 세그먼트의 크기는 동적으로 할당 → 이로 인한 내부단편화는 발생X

### Paging

- 사용하지 않는 프레임을 페이지에 옮기고, 필요한 메모리를 페이지 단위로 프레임에 옮기는 기법
- logical에서 physical 주소로 바꾸는 과정을 위해 page table 사용
- 외부 단편화 해결
- 내부 단편화 존재
- page table
    - physical memory 각 페이지의 기본 주소 포함
    - 각 프로세스 별로 생성됨
 ![image](https://github.com/SujinKim1127/OS_study/assets/58413633/d98771b3-790a-40bf-92b2-390cd3e17c33)

### Frame table

- 프로세스가 실행을 요구하면 ,몇 개의 페이지가 필요한지 조사하여 할당
- 프레임 테이블을 OS가 유지
- 프레임 할당 정보를 저장
- 프로세스를 생성할 때 할당되지 않은 프레임을 적절히 할당
- 프로세스가 종료될 때 사용한 프레임을 할당 가능한 상태로 변경해서 저장

### TLB
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/5c8eb924-6476-419d-9316-2aef0134dfc2)

- CPU안 MMU에 위치하는 페이지 캐시
- 페이지 번호, 프레임 번호, 프로세스 번호 저장
- 테이블을 거치지 않고 바로 실제 메모리 주소로 변환하는 기능 수행
- 만약 페이지 번호가 없으면 TLB miss

### Effective Memory Access Time

문제

- TLB hit ratio : ɑ = 80%
- TLB success time: Ɛ = 20ns for TLB search
- memory access time: 100ns
    
    ⇒ EAT = 0.80 * 100 + 0.20 * 200 = 120ns    (20% 느려짐)
    

### Page Table problem

메모리는 한정적인 자원이랑 페이지테이블에 많은 용량을 내어줄 수 없음

메모리 공간의 비효율적인 사용을 가능한 효율적으로 사용할 수 있도록 제시한 3가지 page table

### Hierarchical page table

- 기존의 프로세스 하나당 하나의 page table을 사용하던 것을 여러개로 나누어 사용하는 방식
- 계층을 나눈 횟수만큼 level 증가

참조 과정

1. outer page number (p1)은 outer page table의 index를 의미, MMU는 이를 바탕으로 inner page table 인덱싱
2. inner page number (p2)는 inner page table의 index를 의미, page table entry를 찾고 offset(d)과 함께 최종 물리 주소로 변환하기
- 2번 나누면 2-level page table, 3번 나누면 3-level page table
  ![image](https://github.com/SujinKim1127/OS_study/assets/58413633/2060f583-aa00-4ac5-9e89-63e2bbe957fe)
- 2-level page table의 메모리 참조 횟수 → 총 3회
    - outer page table 참조 1회
    - inner page table 참조 1회
    - 최종 변환된 물리 주소 참조 1회
    
    ⇒ level이 커질 수록 메모리 자주 참조해야하는 단점 존재
    

### Hased Page Tables

- page number에 해시 함수를 적용한 값을 바탕으로 hash table의 entry 참조
- page table에 hash function을 돌린 다음에 indexing을 하는것
    
    ⇒ entry 개수를 원하는 만큼 줄일 수 있음
    
- 해시형 테이블의 각 항목은 linked list 가짐 (pointer 필요)
    - page, frame 정보 포함
- 해시 값이 가상 페이지 번호가 되는 형식의 페이지 테이블
- 주로 주소 공간이 32bit보다 큰 경우 사용
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/dbfe285c-66e2-4b20-bae5-693d4878fe0c)

과정

1. virtual page number가 오면
2. hash 함수를 통해 해시값 추출
3. 해당 값을 hash 테이블에서 연결리스트를 따라감
4. virtual page number와 일치하는 값이 있으면 그에 대응하는 프레임 번호를 가져와서 physical address 계산

### Inverted Page Table
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/69705833-b44c-403e-8389-77b1cded1a55)
- 메모리 프레임마다 한 항목 씩 할당
- 각 항목은 프레임에 올라와 있는 page address
- 그 페이지를 소유하고 있는 process id 표시
    
    ⇒ 시스템에는 단 하나의 page table만 존재하게 되어 공간 절약 가능
    
- page entry 개수 = frame 개수
- 모든 프로세스가 하나 공통의 page table 공유
    
    ⇒ 장점. 시스템 하나당 하나의 page table만 가지고 있으면 됨. 
    
    테이블로 인해 메모리가 낭비되는 문제 해결 가능
    
- *~~TLB 문제: 모든 프로세스가 하나의 TLB 공유~~*
- entry 내용 = process id, page number
- 단점: 느림
    
    → 마지막에 frame number 계산하는 과정 필요, 접근하는데 시간 오래 걸림
    

### Swapping
![image](https://github.com/SujinKim1127/OS_study/assets/58413633/3ab023b9-cae9-480e-81b3-19f75a2c6d3f)
- 메모리가 부족할때 디스크 메모리와 교환하는 것
- Backing store: 잠깐 어디 빼놓는 공간
- memory와 I/O에 문제가 생겨서 swapping 하는 것


---

[출처]

https://ws-pace.tistory.com/26

[https://velog.io/@chappi/OS는-할껀데-핵심만-합니다.-13편-가변-분할-메모리와-고정-분할-메모리-단편화](https://velog.io/@chappi/OS%EB%8A%94-%ED%95%A0%EA%BB%80%EB%8D%B0-%ED%95%B5%EC%8B%AC%EB%A7%8C-%ED%95%A9%EB%8B%88%EB%8B%A4.-13%ED%8E%B8-%EA%B0%80%EB%B3%80-%EB%B6%84%ED%95%A0-%EB%A9%94%EB%AA%A8%EB%A6%AC%EC%99%80-%EA%B3%A0%EC%A0%95-%EB%B6%84%ED%95%A0-%EB%A9%94%EB%AA%A8%EB%A6%AC-%EB%8B%A8%ED%8E%B8%ED%99%94)

https://code-lab1.tistory.com/54

[https://junghyun100.github.io/메모리단편화/](https://junghyun100.github.io/%EB%A9%94%EB%AA%A8%EB%A6%AC%EB%8B%A8%ED%8E%B8%ED%99%94/)

https://itstory1592.tistory.com/103

https://code-lab1.tistory.com/56

https://itstory1592.tistory.com/103

https://resilient-923.tistory.com/397










