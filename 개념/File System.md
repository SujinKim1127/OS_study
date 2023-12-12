### 파일

- 사용자 관점
    - 정보를 저장하고 관리하는 논리적인 단위
- 컴퓨터 관점
    - 정보를 저장하는 컨테이너
    - 파일이 저장되는 저장 장치
        - HDD, USB, SSD
        - RAM

파일관리에서 OS의 역할

- 파일 생성, 기록, 읽기의 모든 과정 관리

### 파일 입출력 구조

- OS는 논리 블록 주소 사용
    - 논리 블록 주소 Logial Block Address, LBA
        - 저장 매체를 1차원의 연속된 데이터 블록들로 처리
        - 모든 블록들을 0번 부터 시작하는 블록 번호 매김
- 응용프로그램은 파일 내 바이트 주소 사용
    - 바이트 주소 = 파일 내 바이트 위치 (offset)

### 파일 주소 변환

- 파일을 블록 크기로 분할하고 각 블록을 디스크에 분산 저장

> 파일 내 바이트 주소 → 논리 블록 주소 → 물리 주소
> 
- OS는 파일 내 바이트 주소를 논리 블록 주소로 변환
- 디스크 장치의 펌웨어가 논리 블록 주소를 물리 주소로 변환

### 파일 시스템 구조

<img width="471" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/1fbb57b7-023e-4527-ab48-77137bca3254">

트리 계층 구조로 파일 시스템 구성

### 디렉토리와 파일 경로명

- 디렉토리
    - 논리적인 관점
        - 여러 파일 혹은 서브 디렉토리를 포함하는 컨테이너
        - 파일에 대한 경로 제공
    - 물리적인 관점
        - 디렉토리도 파일로 구현됨
        - 디렉토리는 파일이나 서브 디렉토리의 이름, 이들에 관한 위치 정보, 혹은 속성 등을 저장하는 특별한 파일
- 파일 이름과 경로명
    - 파일경로명
        - 루트 디렉토리에서부터 파일에 이르기까지의 계층 경로 모두 포함

### 파일 시스템 구현 이슈

- 디스크에 파일 시스템 포맷
    - 디스크 장치에 비어 있는 블록들의 리스트를 어떻게 관리할 것인가?
- 파일 블록 할당/배치 관리
    - 파일 블록들을 디스크의 어느 영역에 분산 배치할 것인가?
- 파일 블록 위치 관리
    - 파일 블록들이 저장된 디스크 내 위치들을 어떻게 관리할 것인가

## FAT

### FAT 파일 시스템

- 파일 시스템 구조
    - 부트 섹터
        - 섹터 1개, 운영체제 초기 코드를 적재하고 실행시키는 코드
    - FAT1, FAT2
        - FAT는 파일 블록들의 할당 테이블
        - FAT2는 FAT1의 복사본
    - 루트디렉토리
        - 고정 크기이므로 루트 디렉토리에 생성되는 파일이나 서브디렉토리의 개수는 유한
    - 데이터 블록들
        - 파일 블록들이 저장되는 곳. 파일은 블록들로 분할되어 분산 저장
- 디렉토리
    - 파일이나 서브디렉토리의 목록을 담은 특수 파일
        - 파일 이름은 8.3 형식
    - 루트 디렉토리나 서브 디렉토리의 구조 동일
    - 디렉토리 항목
        - 32바이트 크기로 하나의 파일에 대한 메타 정보 저장

### 파일 블록 배치 (File Allocation)

- FAT 파일 시스템의 파일 블록 저장
    - 파일 데이터를 블록 단위로 디스크에 분산 저장
    - 파일 메타 데이터는 디렉토리에 저장
    - 저장된 파일 블록들의 위치는 FAT 테이블에 기록
- FAT 테이블
    - 파일 시스템에 생성된 모든 파일에 대해, 저장된 파일 블록 번호들이 담겨 있는 테이블
    
    테이블 항목
    
    - 디스크 블록 번호 = FAT 항목 번호
    - FAT 항목에는 다음 디스크 블록 번호 저장(다음 FAT 항목 가리킴)
    - FAT 항목에 저장된 번호가 -1 이면 파일 끝을 나타냄
    - FAT 항목에 저장된 번호가 0이면 빈 블록을 나타냄
    - FAT 항목들은 연결 리스트로 연결됨
    - FAT가 손상되면 심각함 → FAT2 백업으로 해결

### 파일이 저장된 모든 블록 알아내기

- 파일이 저장된 모든 블록 알아내기
    - 먼저 파일이 포함된 디렉토리 항목 검색
    
    디렉토리 항목 저장 요소
    
    - 해당 파일이 시작되는 FAT 항목 번호
    - 해당 패일의 크기
    - 디렉토리 항목을 이용하여 FAT 테이블을 연결 리스트 방식으로 검색하여 파일이 저장된 블록들을 알아냄
- 하나의 파일을 읽는데 여러 번의 디스크 탐색(seek)필요
- FAT 한 항목의 크기가 16bits, 블록이 4KB 일때, FAT 파일 시스템이 저장할 수 있는 최대 데이터 양
    - 접근 가능한 총 블록수 2^16 - 2 = 대략 2^16개
    - (2^16) * 4KB = 2^16 * 2^12 bytes = 2^28 bytes = 256 * 2^20 bytes = 256MB
    

## Unix

### Unix 파일 시스템 구조

<img width="499" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/4a9a68a6-0937-4640-a407-2bfc109f6a1d">

- 부트 블록(boot block)
    - 부팅시 메모리에 적재되어 실행되는 코드
    - OS를 적재하는 기능
- 수퍼 블록(super block)
    - 파일 시스템 메타 정보 저장
- i-node와 i-node 리스트
    - i-node: 파일 하나당 1개의 i-node 필요, 파일 메타 정보 저장
    - i-node 리스트: i-node들의 테이블
        - i-node 리스트의 크기는 포맷 시 결정
        - 포맷 후 i-node 개수는 고정
    - 파일이 생성될때마다 빈 i-node 할당, 파일 메타 정보 기록
    - i-node 번호는 0부터 시작. OS마다 첫 i-node 번호는 조금씩 다름
    - 루트디렉토리의 i-node 번호는 수퍼 블록에 기록
        - 리눅스 2
        - 유닉스 1
        - 0번 i-node는 오류 처리를 위해 예약
- 데이터 블록
    - 파일과 디렉토리가 저장되는 공간

### 수퍼 블록

- 파일 시스템의 크기와 상태 정보
- 파일 시스템 내의 자유 블록 수
- 자유 블록들의 리스트
- 자유 블록 리스트에서 요청 시 할당할 다음 블록 인덱스
- 파일 시스템 내의 inode 리스트 크기
- 파일 시스템 내의 자유 inode들의 리스트
- 파일 시스템 내의 자유 inode 리스트에서 요청 시 할당할 다음 자유 inode 인덱스
- 파일 시스템의 논리 블록 크기
- **루트 디렉토리의 i-node 번호**
- 수퍼 블록이 갱신된 최근 시간

### i-node에 저장된 정보

- 파일 타입과 접근 권한
- 파일 소유자 ID (user ID)
- 파일 그룹 ID (파일 접근 권한을 가진 그룹 ID)
- 파일 크기
- 마지막 파일 접근 시각
- 마지막 파일 수정 시각
- 마지막 i-node 수정 시각
- 파일 블록들에 대한 인덱스

### Unix 파일 시스템의 파일 블록 배치

- 파일을 블록 단위로 분산 배치
- i-node에 15개의 인덱스를 두고 파일 블록들의 위치 정보 저장
    - 12개의 직접 인덱스
        - 12개의 파일 블록 번호 (파일 앞부분 12개 블록)
        - 12개의 직접 인덱스로 가리킬 수 있는 파일 크기: 12 * 4 KB = 48KB
    - 1개의 간접 인덱스
        - 파일 크기가 12개의 블록을 넘어갈 때 사용
        - 인덱스가 가리키는 한 개의 디스크 블록에 파일 블록 번호들이 들어 있음
        - 한 블록이 4KB이고, 블록 번호가 32비트(4바이트)일때, 간접 인덱스로 가리킬 수 있는 파일 블록 수: 4KB/4B = 1024 블록
        - 파일 크기는 1024 * 4KB = 4 * 2^20 bytes = 4MB
    - 1개의 2중 간접 인덱스
        - 2중 간접 인덱스로 가리킬 수 있는 파일 블록 수: 1024 * 1024 * 1024 블록
        - 2중 간접 인덱스로 가리킬 수 있는 파일 크기: 1024 * 1024 * 4KB = 4 * 2^30 byte = 4GB
    - 1개의 3중 간접 인텍스
        - 3중 간접 인덱스로 가리킬 수 있는 파일 블록 수: 1024 * 1024 * 1024 블록
        - 3중 간접 인덱스로 가리킬 수 있는 파일 크기: 1024 * 1024 * 1024 * 4KB = 4*2^40bytes = 4TB
- Unix 파일 시스템에서 파일 최대 크기
    - 48KB + 4MB + 4GB + 4TB

### 파일의 i-node 찾기

<img width="485" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/e6b62d55-6c19-415d-a435-bf7c8725d623">

- 파일블록들의 위치는 i-node의 15개 인덱스를 통해 알 수 있다
- → 파일의 i-node 먼저 찾기

`/usr/source/main.c` 파일 찾는 과정

1. 루트디렉토리(`/`)의 i-node 번호 알아내기
    - 루트디렉토리의 i-node번호는 수퍼 블록에
2. 루트디렉토리(`/`)의 i-node 읽기
    - 수퍼 블록에 적힌 루트디렉토리의 i-node 번호로부터 i-node 읽기
3. `/usr`의 i-node 알아내기
4. `/usr`디렉토리를 읽고 `/usr/source` 파일의 i-node 번호 알아내기
5. `/usr/source` 디렉토리 읽고 `/usr/source/main.c` 파일의 i-node 번호 알아내기
6. `/usr/source/main.c` 파일 읽기

### 파입 입출력 연산

- 커널은 파일 입출력을 위한 시스템 호출 함수 제공
    - open(), read(), write(), close(), chmod(), create(), mount(), unmount()
- 파일 찾기
    - 파일 경로명으로부터 파일의 i-node 찾기
    - i-node에 파일 타입, 접근 권한, 파일 데이터가 담겨 있는 블록 번호 등등 저장되어 있음
    - 파일 입출력을 위한 시스템 호출은 커널에 의해 수행

### 파일 열기 open() 과정

<img width="488" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/9cd6c38a-93d1-4e56-9937-a80757d8b83f">

1. 파일 이름으로 i-node 번호 알아내기
    - 파일이 존재하지 않거나 접근 권한이 허용되지 않으면 오류 리턴
2. 디스크 i-node를 커널 메모리의 i-node 테이블에 적재
3. 오픈 파일 테이블에 새 항목 만들기
    - i-node 주소 기록
4. 프로세스별 오픈 파일 테이블에 새 항목 만들기
    - 프로세스별 오픈 파일 테이블에 항목 할당
    - 파일 테이블의 주소 기록
5. open()은 프로세스별 오픈 파일 테이블 항목 번호 리턴

### 파일 읽기 read() 과정

<img width="488" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/e0a66bf3-11a3-48a8-b5d2-af49fc76ab53">

1. read(fd, … )는 fd 번호의 프로세스별 오픈 파일 테이블 참조
2. 파일 테이블 참조
3. i-node 참조
    - i-node에서 파일 블록들의 리스트 확보
    - 파일 테이블 항목에 적힌 offset 확인
    - offset을 파일 블록 번호로 변환
4. 해당 블록이 버퍼 캐시에 있는지 확인
5. 버퍼 캐시로부터 사용자 영역으로 블록 복사

### 파일 쓰기 write() 과정

<img width="492" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/38c6cc00-df3d-4c41-b152-b35d4bea4438">

1. write(fd, … )는 fd 번호의 프로세스별 파일 테이블을 참조
2. 파일 테이블 참조
3. i-node 참조
4. 해당 블록이 버퍼 캐시에 있는지 확인
5. 사용자 공간의 버퍼에서 버퍼 캐시로 쓰기
6. 추후 버퍼 캐시가 교체되거나 플러시 될때, 버퍼 캐시의 내용이 저장 장치에 기록

### 파일 닫기 close() 과정

<img width="493" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/82c4904a-54ee-4e2a-a4bc-305c0bd17210">

1. 프로세스의 오픈 파일 테이블 항목에 기록된 내용 지우기
2. 프로세스의 오픈 파일 테이블 항목을 지우기 전, 파일 테이블의 항목을 찾고 (지우고) 반환하기
3. 파일 테이블을 항목을 반환하기 전, 메모리 i-node의 사용 해제
4. 버퍼 캐시에 있는 이 파일의 블록들이 수정되었거나 새로 만든 블록인 경우 디스크에 기록 

## File System Implementation

### File-System Structure

<img width="493" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/80d5e053-efda-4a8b-a6e0-5504ccedb751">

File Control Block: 파일에 관한 정보를 담는 구조

### Allocation Methods - Contiguous

<img width="481" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/b915f8f1-4be7-4cf3-a840-eb11089daa0b">

Contiguous allocation: 파일들이 연속된 block들을 차지

- 대부분 최고 성능
- 간단함 - starting location과 길이(블럭 갯수)만 필요
- 파일을 저장할 공간을 찾을때, 얼마나 필요할지 예측할때도 문제
- external fragmentation
- compaction 필요

### Allocation Methods - Linked

<img width="491" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/acbda7a2-2444-4e2f-a2aa-8f15b21eaeb8">

Linked Allocation

- 각각의 block은 다음 block의 포인터를 포함
- no compaction, external fragmentation
- 새로운 block이 필요할때 free space management system 호출

### Linked Allocation의 변형 - FAT

<img width="476" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/499cf2ae-6285-477f-b73f-becba37a1ca5">

FAT

- 디스크 블록마다 한 개의 항목을 가지고 다음 디스크 블록을 가리킴
- 디렉토리 항목은 파일의 첫 번째 블록 번호를 가리킴

### Allocation Methods - Indexed

<img width="468" alt="image" src="https://github.com/SujinKim1127/OS_study/assets/58413633/bb3c0ed9-d187-471a-ae62-082b1c1ef3d0">

Indexed allocation

- 각 파일은 data block에 대한 포인터의 index block를 가짐
- index table 필요
- random access
- external fragmentation없이 dynamic access 가능
- index block으로 인한 overhead 있을수도

### Free-Space Management

- 사용가능한 블럭들을 track하기 위해 free-space list 유지
- Bit vector or Bit map
- Bit map requires extra space
    
    ex)
    
    - block size = 4KB = 2^12 bytes
    - disk size = 2^40 bytes (1 TB)
    - n = 2^40/2^12 = 2^28 bits (or 32MB)

### Linked Free Space List on Disk

Linked list

- 연속적인 공간 얻을 수 없음
- 공간 낭비 없음
- no need to traverse the entire list

<br/><br/><br/>

참고

https://blog.naver.com/qkr0990/220561854875
