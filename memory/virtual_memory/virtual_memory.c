#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PAGE_ENTRIES 256
#define PAGE_SIZE 256
#define TLB_ENTRIES 16
#define FRAME_COUNT 256

typedef struct {
    int page_number;
    int offset;
    char raw[7];
} Address;

typedef struct {
    char value[PAGE_ENTRIES][PAGE_SIZE];
} Frame;

typedef struct {
    int iter;
    Address* addresses;
} TLB;

void parseLogicalAddress(Address* address, char* raw)
{
    int hex = (int)strtol(raw, NULL, 10);
    raw[strcspn(raw, "\n")] = 0;

    strcpy(address->raw, raw);
    address->page_number = hex >> 8;
    address->offset = hex & 0x00ff;
}

void readData(FILE *fd, char* buffer) {
    fseek(fd, rand() % (PAGE_SIZE * (PAGE_ENTRIES - 1)), SEEK_SET);
    fread(buffer, PAGE_SIZE, 1, fd);
}

Address* AddressNew(char *raw) {
    Address *address = malloc(sizeof(Address));

    parseLogicalAddress(address, raw);

    return address;
}

void AddressFree(Address* address) {
    free(address);
}


int inTLB(TLB* tlb, Address* address) {
    for (int i = 0; i < TLB_ENTRIES; i++) {
        if (strcmp(tlb->addresses[i].raw, "") == 0) {
            break;
        }

        if (strcmp(tlb->addresses[i].raw, address->raw) == 0) {
            printf("Found match: %s at pos=%d\n", address->raw, i);
            return 1;
        }
    }

    return 0;
}

void tlbInsertAddress(TLB* tlb, Address* address) {
    tlb->iter = (tlb->iter + 1) % TLB_ENTRIES;
    strcpy(tlb->addresses[tlb->iter].raw, address->raw);
    tlb->addresses[tlb->iter].page_number = address->page_number;
    tlb->addresses[tlb->iter].offset = address->offset;
}

TLB* TLBNew() {
    TLB* tlb = malloc(sizeof(TLB));
    tlb->iter = -1;
    tlb->addresses = malloc(sizeof(Address) * TLB_ENTRIES);

    return tlb;
}

void TLBFree(TLB* tlb) {
    free(tlb->addresses);
    free(tlb);
}

Frame* MemoryNew() {
    Frame* frames = malloc(sizeof(Frame) * FRAME_COUNT);

    return frames;
}

void MemoryFree(Frame* frames) {
    free(frames);
}

int main(int argc, char **argv) {
    ssize_t read;
    size_t len = 0;
    char* virtual_address = malloc(7);
    int page_table[PAGE_ENTRIES];

    int tlb_hit_count = 0;
    int page_hit_count = 0;
    int page_fault_count = 0;

    FILE *store = fopen("BACKING_STORE.bin", "r");
    FILE *addresses = fopen("addresses.txt", "r");

    TLB* tlb = TLBNew();
    Frame* memory = MemoryNew();

    for (int i = 0; i < PAGE_ENTRIES; i++) {
        page_table[i] = -1;
    }

    while ((read = getline(&virtual_address, &len, addresses)) != -1) {
        int deletable = 0;
        Address* address = AddressNew(virtual_address);

        if (inTLB(tlb, address)) {
            tlb_hit_count++;
            deletable = 1;
        } else {
            tlbInsertAddress(tlb, address);

            // check if in page table
            if (page_table[address->page_number] != -1) {
                page_hit_count++;
            } else {
                page_fault_count++;

                readData(store, memory->value[address->page_number]);
                page_table[address->page_number] = address->page_number;
            }
        }

        int physical_address = address->page_number * PAGE_SIZE + address->offset;
        printf("Virtual address: %s, Physical address: %d, Value: %c\n", address->raw, physical_address, memory->value[address->page_number][address->offset]);

        AddressFree(address);
    }

    printf("\nSummary:\n - TLB Hits: %d\n - Page Hits: %d\n- Page Faults: %d\n\n", tlb_hit_count, page_hit_count, page_fault_count);

    MemoryFree(memory);
    TLBFree(tlb);

    fclose(addresses);
    fclose(store);
}
