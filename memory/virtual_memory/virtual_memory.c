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
    char* raw;
    int frame;
} Address;

typedef struct {
    char *value;
} Frame;

typedef struct {
    int iter;
    Address* addresses;
} TLB;

void parseLogicalAddress(Address* address, char* raw)
{
    int hex = (int)strtol(raw, NULL, 10);

    address->raw = raw;
    address->page_number = hex >> 8;
    address->offset = hex & 0x00ff;
}

void readData(FILE *fd, char* buffer) {
    fseek(fd, rand() % (PAGE_SIZE * (PAGE_ENTRIES - 1)), SEEK_SET);
    fread(buffer, PAGE_SIZE, 1, fd);
}

Address* AddressNew(char *raw) {
    Address *address = malloc(sizeof(Address));
    address->raw = malloc(sizeof(char[6]));
    address->value = malloc(sizeof(char[PAGE_SIZE]));

    parseLogicalAddress(address, raw);

    return address;
}

void AddressFree(Address* address) {
    free(address->raw);
    free(address);
}


int inTLB(TLB* tlb, Address* address) {
    for (int i = 0; i < TLB_ENTRIES; i++) {
        printf("Comparing values: %s %s\n", tlb->addresses[i]->raw, address->raw);
        if (strcmp(tlb->addresses[i]->raw, address->raw) == 0) {
            address.frame = i;
            return 1;
        }
    }

    return false;
}

void tlbInsertAddress(TLB* tlb, Address* address) {
    tlb->iter = (tlb->iter + 1) % TLB_ENTRIES;

    free(tlb->addresses[tlb->iter]);
    tlb->addresses[tlb->iter] = address;
}

TLB* TLBNew() {
    TLB* tlb = malloc(sizeof(TLB));
    tlb->iter = 0;
    tlb->addresses = malloc(sizeof(*Address) * TLB_ENTRIES);
}

void TLBFree() {
    for (int i = 0; i < TLB_ENTRIES; i++) {
        free(tlb->addresses[i]);
    }

    free(tlb->addresses);
    free(tlb);
}

Frame* FrameNew(char* value) {
    Frame frame = malloc(sizeof(*Frame));
    frame->value = malloc(sizeof(char) * PAGE_SIZE);
}

void FrameFree(Frame* frame) {
    free(frame->value);
    free(frame);
}

Frame* MemoryNew() {
    Frame* frames = malloc(sizeof(*Frame) * FRAME_COUNT);

    for (int i = 0; i < FRAME_COUNT; i++) {
        frames[i] = FrameNew(NULL);
    }

    return frames;
}

void MemoryFree(Frame* frames) {
    for (int i = 0; i < FRAME_COUNT; i++) {
        free(frames[i]);
    }

    free(frames);
}

int main(int argc, char **argv) {
    FILE *store = fopen("BACKING_STORE.bin", "r");
    FILE *addresses = fopen("addresses.txt", "r");

    TLB* tlb = TLBNew();
    Frame* memory = MemoryNew();
    int page_table[PAGE_ENTRIES];

    tlb_hit_count = 0;
    page_hit_count = 0;
    page_fault_count = 0;

    // TODO: loop through addresses
    for (;;) {


        // TODO: check if address is in tlb


        if (inTLB(tlb, virtual_address)) {
            tlb_hit_count++;
        } else {
            Address* address = AddressNew("13456");
            tlbInsertAddress(tlb, address)

            // TODO: check if in page table
            if (page_table[address->page_number] != 0) {
                page_hit_count++;

            } else {
                page_fault_count++;
                // TODO if not in page table, pull values from backing store

                readData(store, address->value);

                // TODO: insert into TLB
            }

        }

        printf("Virtual address: %d, Physical address: %d, Value: %c\n", virtual_address, physical_address, value);
    }

    printf("\nSummary:\n - TLB Hits: %d\n - Page Hits: %d\n- Page Faults: %d\n\n", tlb_hit_count, page_hit_count, page_fault_count);

    MemoryFree(memory);
    TLBFree(tlb);

    fclose(addresses);
    fclose(store);
}
