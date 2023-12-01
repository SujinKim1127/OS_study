#include <stdio.h>
#include <stdlib.h>

#define VIRTUAL_ADDRESS_BITS 18
#define PAGE_SIZE 2048 // 2KB
#define PHYSICAL_MEMORY_SIZE 32768 // 32KB

typedef struct {
    int valid;
    int page_number;
    int last_used;
} PageTableEntry;

void initializePageTable(PageTableEntry *page_table, int page_table_size) {
    for (int i = 0; i < page_table_size; i++) {
        page_table[i].valid = 0;
        page_table[i].page_number = -1;
        page_table[i].last_used = 0;
    }
}

int main() {
    int virtual_address;
    int page_table_size = PHYSICAL_MEMORY_SIZE / PAGE_SIZE;
    int *virtual_addresses = (int *)malloc(10 * sizeof(int));

    // 랜덤 가상 주소 10개 생성
    printf("Random Virtual Addresses:\n");
    for (int i = 0; i < 10; i++) {
        virtual_addresses[i] = rand() % (1 << VIRTUAL_ADDRESS_BITS);
        printf("%d\n", virtual_addresses[i]);
    }

    PageTableEntry *page_table = (PageTableEntry *)malloc(page_table_size * sizeof(PageTableEntry));
    initializePageTable(page_table, page_table_size);

    int page_faults = 0;

// 가상 주소 처리
printf("\nVirtual Address | Page Number | Frame Number | Physical Address | Page Fault\n");
for (int i = 0; i < 10; i++) {
    virtual_address = virtual_addresses[i];

    // 가상 주소 해석
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

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
    int physical_address = frame_number * PAGE_SIZE + offset;

    // 결과 출력
    printf("%-16d%-14d%-14d%-18d%s\n", virtual_address, page_number, frame_number, physical_address, (frame_number == -1) ? "Yes" : "No");
    }
    // 페이지 부재 횟수 출력
    printf("\nPage Faults: %d\n", page_faults);

    free(virtual_addresses);
    free(page_table);

    return 0;
}
