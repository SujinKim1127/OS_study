#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PAGE_SIZE 4096   // 2KB
#define PHYSICAL_MEMORY_SIZE 32768   // 32KB
#define VIRTUAL_ADDRESS_BITS 18

typedef struct {
    bool referenced;
    unsigned int virtual_page;
} PageTableEntry;

int main() {
    // 초기화
    int num_frames = PHYSICAL_MEMORY_SIZE / PAGE_SIZE;
    PageTableEntry *page_table = (PageTableEntry *)malloc(num_frames * sizeof(PageTableEntry));
    unsigned int *physical_memory = (unsigned int *)malloc(PHYSICAL_MEMORY_SIZE);

    int page_faults = 0;

    // 랜덤 가상 주소 10개 생성
    unsigned int virtual_addresses[10];
    printf("랜덤 가상 주소 10개: ");
    for (int i = 0; i < 10; ++i) {
        virtual_addresses[i] = rand() % (1 << VIRTUAL_ADDRESS_BITS);
        printf("%u ", virtual_addresses[i]);
    }
    printf("\n\n");

    // 가상 주소에 대한 페이지 교체 시뮬레이션
    for (int i = 0; i < 10; ++i) {
        unsigned int virtual_address = virtual_addresses[i];
        unsigned int virtual_page_number = virtual_address / PAGE_SIZE;

        // 페이지 테이블 엔트리 확인
        int frame_index = -1;
        for (int j = 0; j < num_frames; ++j) {
            if (page_table[j].virtual_page == virtual_page_number) {
                frame_index = j;
                page_table[j].referenced = true;
                break;
            }
        }

        if (frame_index == -1) {
            // 페이지 폴트 발생
            page_faults++;

            // Second-Chance 알고리즘 적용
            while (1) {
                if (!page_table[frame_index].referenced) {
                    // 현재 프레임이 참조되지 않았으면 교체
                    break;
                } else {
                    // 참조 비트를 초기화하고 다음 프레임 확인
                    page_table[frame_index].referenced = false;
                    frame_index = (frame_index + 1) % num_frames;
                }
            }

            // 새로운 페이지 로드
            page_table[frame_index].virtual_page = virtual_page_number;
            page_table[frame_index].referenced = true;
        }

        // 결과 출력
        unsigned int physical_address = frame_index * PAGE_SIZE + (virtual_address % PAGE_SIZE);
        printf("가상 주소: %u, 페이지 번호: %u, 프레임 번호: %d, 물리 주소: %u, 페이지 폴트: %s\n",
               virtual_address, virtual_page_number, frame_index, physical_address,
               (frame_index == -1) ? "Yes" : "No");
    }

    // 페이지 폴트 횟수 출력
    printf("\n페이지 폴트 발생 횟수: %d\n", page_faults);

    // 메모리 해제
    free(page_table);
    free(physical_memory);

    return 0;
}
