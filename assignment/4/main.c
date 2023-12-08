#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Optimal 
typedef struct {
    int valid;
    int page_number;
    int last_used;
} OptimalPageTableEntry;

void initializePageTable(OptimalPageTableEntry *page_table, int page_table_size) {
    for (int i = 0; i < page_table_size; i++) {
        page_table[i].valid = 0;
        page_table[i].page_number = -1;
        page_table[i].last_used = 0;
    }
}

// LRU
typedef struct {
    int valid;
    int page_number;
    int last_used;
} LRUPageTableEntry;


void initializeLRUPageTable(LRUPageTableEntry *page_table, int page_table_size) {
    for (int i = 0; i < page_table_size; i++) {
        page_table[i].valid = 0;
        page_table[i].page_number = -1;
        page_table[i].last_used = 0;
    }
}

void simulateLRU(int virtual_addresses[], int num_addresses, int virtual_address_bits, int page_size, int physical_memory_size) {
    int virtual_address;
    int page_table_size = physical_memory_size / page_size;

    LRUPageTableEntry *page_table = (LRUPageTableEntry *)malloc(page_table_size * sizeof(LRUPageTableEntry));
    initializeLRUPageTable(page_table, page_table_size);

    int page_faults = 0;

    // 가상 주소 처리
    printf("\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");
    // 파일을 쓰기 모드로 엽니다.
    FILE *file = fopen("output.lru", "w");
    fprintf(file, "\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");

    for (int i = 0; i < num_addresses; i++) {
        virtual_address = virtual_addresses[i];

        // 가상 주소 해석
        int page_number = virtual_address / page_size;
        int offset = virtual_address % page_size;

        // 페이지 테이블 검사
        int frame_number = -1;
        for (int j = 0; j < page_table_size; j++) {
            if (page_table[j].valid && page_table[j].page_number == page_number) {
                frame_number = j;
                page_table[j].last_used = i;
                break;
            }
        }

        // 페이지 부재 처리
        if (frame_number == -1) {
            page_faults++;

            // LRU 교체 알고리즘 적용
            int oldest_frame = 0;
            for (int j = 1; j < page_table_size; j++) {
                if (page_table[j].last_used < page_table[oldest_frame].last_used) {
                    oldest_frame = j;
                }
            }

            frame_number = oldest_frame;
            page_table[frame_number].valid = 1;
            page_table[frame_number].page_number = page_number;
            page_table[frame_number].last_used = i;
        }

        // 물리 주소 계산
        int physical_address = frame_number * page_size + offset;

        // 결과 출력
        if(i < 14){
            printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
            i+1, virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "H" : "F");
        }
        else if (i == 15) printf("=============================================================================================================\n");
        else if (i == 4999) {
            printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
            i+1, virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "H" : "F");
        }
        fprintf(file, "%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
         i+1, virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "H" : "F");
    }
    // 페이지 부재 횟수 출력
    printf("\nPage Faults: %d\n", page_faults);
    fprintf(file, "\nPage Faults: %d\n", page_faults);
    free(page_table);
    // 파일을 닫습니다.
    fclose(file);
}

// FIFO
typedef struct {
    int valid;  // 유효 비트
    int frame_number;  // 프레임 번호
} FIFOPageTableEntry;

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

// Second-Chance
typedef struct {
    int valid;
    int referenced;
    int page_number;
    int last_used;
} SCPageTableEntry;

void initializeSCPageTable(SCPageTableEntry *page_table, int page_table_size) {
    for (int i = 0; i < page_table_size; i++) {
        page_table[i].valid = 0;
        page_table[i].referenced = 0;
        page_table[i].page_number = -1;
        page_table[i].last_used = 0;
    }
}


int main() {
    int A, B, C, D, E;
    int virtual_address_bits, page_size, physical_memory_size;

    int num_addresses = 5000;
    int *virtual_addresses = (int *)malloc(num_addresses * sizeof(int));

    printf("A. Simulation에 사용할 가상주소 길이를 선택하시오 (1. 18bits 2. 19bits 3. 20bits): ");
    scanf("%d", &A);
    if (A == 1) virtual_address_bits = 18;
    if (A == 2) virtual_address_bits = 19;
    if (A == 3) virtual_address_bits = 20;
    
    printf("B. Simulation에 사용할 페이지(프레임)의 크기를 선택하시오 (1. 1KB 2. 2KB 3. 4KB): ");
    scanf("%d", &B);
    if (B == 1) page_size = 1024;
    if (B == 2) page_size = 2048;
    if (B == 3) page_size = 4096;
    
    printf("C. Simulation에 물리메모리의 크기를 선택하시오 (1. 32KB 2. 64KB): ");
    scanf("%d", &C);
    if (C == 1) physical_memory_size = 32768;
    if (C == 2) physical_memory_size = 65536;


    printf("D. Simulation에 적용할 Page Replacement 알고리즘을 선택하시오 \n (1. Optimal  2. FIFO   3. LRU  4. Second-Chance): ");
    scanf("%d", &D);

    printf("E. 가상주소 스트링 입력방식을 선택하시오 \n (1. input.in 자동 생성   2. 기존 파일 사용): ");
    scanf("%d", &E);
    char fileName;
    if(E == 1) {
        // 시드를 현재 시간으로 초기화하여 매번 다른 랜덤 값을 얻을 수 있도록 합니다.
        srand(time(NULL));

        // 파일을 쓰기 모드로 엽니다.
        FILE *file = fopen("input.in", "w");

        // 파일이 정상적으로 열렸는지 확인합니다.
        if (file == NULL) {
            perror("파일을 열 수 없습니다.");
            return 1;
        }

        // 가상주소를 랜덤으로 생성하여 파일에 씁니다.
        for (int i = 0; i < num_addresses; ++i) {
            // 0부터 9999 사이의 랜덤 가상주소를 생성합니다.
            int virtual_address = rand() % 10000;
            virtual_addresses[i] = virtual_address;
            // 파일에 가상주소를 씁니다.
            fprintf(file, "%d\n", virtual_address);
        }

        // 파일을 닫습니다.
        fclose(file);

        printf("input.in 파일이 생성되었습니다.\n");    
    }
    if(E == 2) {
        printf("F. 입력 파일 이름을 입력하시오: ");
        scanf("%s", &fileName);

        // Read random addresses from the "input.in" file
        FILE *input_file = fopen("input.in", "r");
        if (input_file == NULL) {
            perror("Error opening input file");
            return 1;
        }

        for (int i = 0; i < num_addresses; i++) {
            if (fscanf(input_file, "%d", &virtual_addresses[i]) != 1) {
                fprintf(stderr, "Error reading from input file\n");
                fclose(input_file);
                free(virtual_addresses);
                return 1;
            }
        }
        fclose(input_file);
    }

    if (D == 1) {       // Optimal
        int page_table_size = physical_memory_size / page_size;
        OptimalPageTableEntry *page_table = (OptimalPageTableEntry *)malloc(page_table_size * sizeof(OptimalPageTableEntry));
        initializePageTable(page_table, page_table_size);

        int page_faults = 0;

        // 파일을 쓰기 모드로 엽니다.
        FILE *file = fopen("output.opt", "w");

        // 파일이 정상적으로 열렸는지 확인합니다.
        if (file == NULL) {
            perror("파일을 열 수 없습니다.");
            return 1;
        }
        fprintf(file, "\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");

    // 가상 주소 처리
        printf("\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");
        for (int i = 0; i < 5000; i++) {
            int virtual_address = virtual_addresses[i];

            // 가상 주소 해석
            int page_number = virtual_address / page_size;
            int offset = virtual_address % page_size;

            // 페이지 테이블 검사
            int frame_number = -1;
            for (int j = 0; j < page_table_size; j++) {
                if (page_table[j].valid && page_table[j].page_number == page_number) {
                    frame_number = j;
                    page_table[j].last_used = i;
                    break;
                }
            }

            // 페이지 부재 처리
            if (frame_number == -1) {
                page_faults++;

                // LRU 교체 알고리즘 적용
                int oldest_frame = 0;
                for (int j = 1; j < page_table_size; j++) {
                    if (page_table[j].last_used < page_table[oldest_frame].last_used) {
                        oldest_frame = j;
                    }
                }

                frame_number = oldest_frame;
                page_table[frame_number].valid = 1;
                page_table[frame_number].page_number = page_number;
                page_table[frame_number].last_used = i;
            }

            // 물리 주소 계산
            int physical_address = frame_number * page_size + offset;

            // 결과 출력
            if(i < 14){
                printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
                i+1, virtual_address, page_number, frame_number,
                physical_address, (frame_number == -1) ? "H" : "F");
            }
            else if (i == 15) printf("=============================================================================================================\n");
            else if (i == 4999) {
                printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
                i+1, virtual_address, page_number, frame_number,
                physical_address, (frame_number == -1) ? "H" : "F");
            }
            // 파일에 입력
            fprintf(file, "%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
            i+1, virtual_address, page_number, frame_number,
            physical_address, (frame_number == -1) ? "H" : "F");

        }
        // 페이지 부재 횟수 출력
        printf("\nPage Faults: %d\n", page_faults);
        fprintf(file, "\nPage Faults: %d\n", page_faults);

        free(virtual_addresses);
        free(page_table);
        // 파일을 닫습니다.
        fclose(file);

        return 0;
    }
    if (D == 2) {       // FIFO
        // 페이지 테이블 초기화
        FIFOPageTableEntry pageTable[virtual_address_bits];

        // FIFO 큐 초기화
        FifoQueue *fifoQueue = initQueue(physical_memory_size / page_size);

        int pageFaults = 0;
        int virtualAddress;
        // 파일을 쓰기 모드로 엽니다.
        FILE *file = fopen("output.fifo", "w");

        // 파일이 정상적으로 열렸는지 확인합니다.
        if (file == NULL) {
            perror("파일을 열 수 없습니다.");
            return 1;
        }
        // 파일에 쓰기
        fprintf(file, "\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");
        printf("\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");
        for (int i = 0; i < 5000; ++i) {
            virtualAddress = virtual_addresses[i];
            // 페이지 번호 계산
            int pageNumber = virtualAddress / page_size;
            // 페이지 테이블 엔트리 확인
            if (!pageTable[pageNumber].valid) {
                // Page Fault 발생
                pageFaults++;
                // 큐에서 가장 오래된 페이지 제거
                int replacedPage = dequeue(fifoQueue);
                
                // 새로운 페이지 추가
                pageTable[pageNumber].valid = 1;
                pageTable[pageNumber].frame_number = fifoQueue->rear;


                // 큐에 새로운 페이지 추가
                enqueue(fifoQueue, pageNumber);
            }

            // 결과 출력
            if(i < 14){
                printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
                i+1, virtualAddress, pageNumber, pageTable[pageNumber].frame_number,
                pageTable[pageNumber].frame_number * page_size + virtualAddress % page_size,
                pageTable[pageNumber].valid ? "F" : "H");
            }
            else if (i == 15) printf("=============================================================================================================\n");
            else if (i == 4999) {
                printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
                i+1, virtualAddress, pageNumber, pageTable[pageNumber].frame_number,
                pageTable[pageNumber].frame_number * page_size + virtualAddress % page_size,
                pageTable[pageNumber].valid ? "F" : "H");
            }
            fprintf(file, "%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
            i+1, virtualAddress, pageNumber, pageTable[pageNumber].frame_number,
             pageTable[pageNumber].frame_number * page_size + virtualAddress % page_size,
              pageTable[pageNumber].valid ? "F" : "H");

        }

        // Page Fault 횟수 출력
        printf("Total Page Faults: %d\n", pageFaults);
        fprintf(file, "Total Page Faults: %d\n", pageFaults);    
        // 동적 할당 해제
        free(fifoQueue->pages);
        free(fifoQueue);
        // 파일을 닫습니다.
        fclose(file);
        return 0;
    }
    if (D == 3) {       // LRU
        simulateLRU(virtual_addresses, num_addresses, virtual_address_bits, page_size, physical_memory_size);
        free(virtual_addresses);
    }
    if (D == 4) {       // Second-Chance
        int page_table_size = physical_memory_size / page_size;
        SCPageTableEntry *page_table = (SCPageTableEntry *)malloc(page_table_size * sizeof(SCPageTableEntry));
        initializeSCPageTable(page_table, page_table_size);

        int page_faults = 0;
        // 파일을 쓰기 모드로 엽니다.
        FILE *file = fopen("output.sc", "w");

        // 파일이 정상적으로 열렸는지 확인합니다.
        if (file == NULL) {
            perror("파일을 열 수 없습니다.");
            return 1;
        }
        // 파일에 쓰기
        fprintf(file, "\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");
        printf("\nNo. \t\t V.A \t\t Page No. \t\t Frame No. \t\t P.A. \t\t Page Fault\n");
        int virtual_address;
        // 가상 주소에 대한 페이지 교체 시뮬레이션
        for (int i = 0; i < 5000; ++i) {
            virtual_address = virtual_addresses[i];

            // 가상 주소 해석
            int page_number = virtual_address / page_size;
            int offset = virtual_address % page_size;

            // 페이지 테이블 검사
            int frame_number = -1;
            for (int j = 0; j < page_table_size; j++) {
                if (page_table[j].valid && page_table[j].page_number == page_number) {
                    frame_number = j;
                    page_table[j].referenced = 1; 
                    page_table[j].last_used = i;
                    break;
                }
            }

            // 페이지 부재 처리
            if (frame_number == -1) {
                page_faults++;

                // Second-Chance (Clock) algorithm
                while (1) {
                    frame_number = (frame_number + 1) % page_table_size;

                    if (!page_table[frame_number].referenced) {
                        // If the page is not referenced, replace it
                        break;
                    } else {
                        // Reset the referenced bit
                        page_table[frame_number].referenced = 0;
                    }
                }

                // Update the selected frame with the new page
                page_table[frame_number].valid = 1;
                page_table[frame_number].page_number = page_number;
                page_table[frame_number].referenced = 1;
                page_table[frame_number].last_used = i;
            }

            // 물리 주소 계산
            int physical_address = frame_number * page_size + offset;

            // 결과 출력
            if(i < 14){
                printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
                i+1, virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "H" : "F");
            }
            else if (i == 15) printf("=============================================================================================================\n");
            else if (i == 4999) {
                printf("%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
                i + 1, virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "H" : "F");
            }
            fprintf(file, "%d \t\t %-16d %-14d \t %-14d \t %-18d%s\n",
            i + 1, virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "H" : "F");

        }

        // 페이지 부재 횟수 출력
        printf("\nPage Faults: %d\n", page_faults);
        fprintf(file, "total page faults:  %d\n", page_faults);
        free(virtual_addresses);
        free(page_table);

        fclose(file);
        return 0;
    }

    return 0;
}
