#include <stdio.h>
#include <stdlib.h>

#define VIRTUAL_ADDRESS_BITS 18
#define PAGE_SIZE 2048  // 2KB
#define PHYSICAL_MEMORY_SIZE 32768  // 32KB
#define FRAME_SIZE PAGE_SIZE

// 페이지 테이블 엔트리 구조체
typedef struct {
    int valid;  // 유효 비트
    int frame_number;  // 프레임 번호
} PageTableEntry;

// FIFO 페이지 교체 알고리즘에 사용되는 구조체
typedef struct {
    int *pages;  // 페이지 배열
    int capacity;  // 큐의 최대 크기
    int size;  // 현재 큐의 크기
    int front;  // 큐의 맨 앞
    int rear;  // 큐의 맨 뒤
} FifoQueue;

// FIFO 큐 초기화 함수
FifoQueue *initQueue(int capacity) {
    FifoQueue *queue = (FifoQueue *)malloc(sizeof(FifoQueue));
    queue->pages = (int *)malloc(sizeof(int) * capacity);
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    return queue;
}

// 큐에 요소 추가 함수
void enqueue(FifoQueue *queue, int page) {
    if (queue->size < queue->capacity) {
        queue->rear = (queue->rear + 1) % queue->capacity;
        queue->pages[queue->rear] = page;
        queue->size++;
    }
}

// 큐에서 요소 제거 함수
int dequeue(FifoQueue *queue) {
    if (queue->size > 0) {
        int page = queue->pages[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return page;
    }
    return -1;
}

// 메인 함수
int main() {
    // 페이지 테이블 초기화
    PageTableEntry pageTable[VIRTUAL_ADDRESS_BITS];

    // FIFO 큐 초기화
    FifoQueue *fifoQueue = initQueue(PHYSICAL_MEMORY_SIZE / FRAME_SIZE);

    // 랜덤 가상 주소 생성 및 처리
    int virtualAddress;
    int pageFaults = 0;

    srand(time(NULL));  // 랜덤 시드 초기화

    // 10번 반복
    for (int i = 0; i < 10; ++i) {
        // 랜덤 가상 주소 생성
        virtualAddress = rand() % (1 << VIRTUAL_ADDRESS_BITS);

        // 페이지 번호 계산
        int pageNumber = virtualAddress / PAGE_SIZE;

        // 페이지 테이블 엔트리 확인
        if (!pageTable[pageNumber].valid) {
            // Page Fault 발생
            pageFaults++;

            // 큐에서 가장 오래된 페이지 제거
            int replacedPage = dequeue(fifoQueue);

            // 새로운 페이지 추가
            pageTable[pageNumber].valid = 1;
            pageTable[pageNumber].frame_number = replacedPage;

            // 큐에 새로운 페이지 추가
            enqueue(fifoQueue, pageNumber);
        }

        // 결과 출력
        printf("Virtual Address: %d, Page Number: %d, Frame Number: %d, Physical Address: %d, Page Fault: %s\n",
               virtualAddress, pageNumber, pageTable[pageNumber].frame_number,
               pageTable[pageNumber].frame_number * FRAME_SIZE + virtualAddress % PAGE_SIZE,
               pageTable[pageNumber].valid ? "No" : "Yes");
    }

    // Page Fault 횟수 출력
    printf("Total Page Faults: %d\n", pageFaults);

    // 동적 할당 해제
    free(fifoQueue->pages);
    free(fifoQueue);

    return 0;
}
