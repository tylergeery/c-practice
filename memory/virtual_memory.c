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
    char* value;
} Address;

void parseLogicalAddress(Address* address, char* raw)
{
    int hex = (int)strtol(raw, NULL, 16);

    address->raw = raw;
    address->page_number = hex ^ 0xf0;
    address->page_number = hex ^ 0x0f;
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

void readData(FILE *fd, char* buffer) {
    fseek(fd, rand() % (PAGE_SIZE * (PAGE_ENTRIES - 1)), SEEK_SET);
    fread(buffer, PAGE_SIZE, 1, fd);
}

int main(int argc, char **argv) {
    FILE *store = fopen("BACKING_STORE.bin", "r");
    FILE *addresses = fopen("addresses.txt", "r");
    Address tlb[TLB_ENTRIES];

    // TODO: loop through addresses

    // TODO: check if address is in tlb

    // TODO: if not in TLB, pull values from backing store
    Address *address = AddressNew("13456");
    readData(store, address->value);

    // TODO: insert into TLB


    fclose(addresses);
    fclose(store);
}
