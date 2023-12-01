#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VIRTUAL_ADDRESS_BITS 18
#define PAGE_SIZE 2048 // 2KB
#define PHYSICAL_MEMORY_SIZE 32768 // 32KB
#define PAGE_TABLE_SIZE (1 << (VIRTUAL_ADDRESS_BITS - (int)log2(PAGE_SIZE)))

typedef struct {
    int frameNumber;
    bool valid;
    int lastAccessed; // For Optimal algorithm
} PageTableEntry;

int pageFaults = 0;

void initializePageTable(PageTableEntry pageTable[]) {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        pageTable[i].valid = false;
    }
}

int getPageNumber(int virtualAddress) {
    return virtualAddress / PAGE_SIZE;
}

int getOffset(int virtualAddress) {
    return virtualAddress % PAGE_SIZE;
}

int findOptimalPage(PageTableEntry pageTable[], int accessSequence[], int startIdx) {
    int maxIdx = startIdx;
    for (int i = startIdx + 1; i < PAGE_TABLE_SIZE; ++i) {
        if (!pageTable[i].valid) {
            return i;
        }
        if (accessSequence[maxIdx] < accessSequence[i]) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

void handlePageAccess(PageTableEntry pageTable[], int virtualAddress, int accessSequence[]) {
    int pageNumber = getPageNumber(virtualAddress);
    int offset = getOffset(virtualAddress);

    if (!pageTable[pageNumber].valid) {
        // Page Fault
        ++pageFaults;
        int optimalPage = findOptimalPage(pageTable, accessSequence, 0);
        pageTable[optimalPage].frameNumber = pageNumber;
        pageTable[optimalPage].valid = true;
        pageTable[optimalPage].lastAccessed = accessSequence[pageNumber];
    }

    // Print information
    printf("Virtual Address: %d, Page Number: %d, Frame Number: %d, Physical Address: %d, Page Fault: %s\n",
           virtualAddress, pageNumber, pageTable[pageNumber].frameNumber,
           pageTable[pageNumber].frameNumber * PAGE_SIZE + offset,
           pageTable[pageNumber].valid ? "No" : "Yes");
}

int main() {
    PageTableEntry pageTable[PAGE_TABLE_SIZE];
    int accessSequence[10];

    // Initialize page table
    initializePageTable(pageTable);

    // Generate random virtual addresses
    srand(42); // Seed for reproducibility
    printf("Access Sequence: ");
    for (int i = 0; i < 10; ++i) {
        accessSequence[i] = rand() % PAGE_TABLE_SIZE;
        printf("%d ", accessSequence[i]);
    }
    printf("\n");

    // Simulate page accesses
    for (int i = 0; i < 10; ++i) {
        handlePageAccess(pageTable, accessSequence[i] * PAGE_SIZE + rand() % PAGE_SIZE, accessSequence);
    }

    // Print total page faults
    printf("Total Page Faults: %d\n", pageFaults);

    return 0;
}
