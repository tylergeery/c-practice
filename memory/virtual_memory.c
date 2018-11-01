#include <stdio.h>
#include <string.h>

#define PAGE_ENTRIES 256
#define PAGE_SIZE 256
#define TLB_ENTRIES 16
#define FRAME_COUNT 256

typedef struct {
    int page_number;
    int offset;
    char* raw;
} Address;

char* tlb[TLB_ENTRIES];

void parseLogicalAddress(Address* address, char* raw)
{
    int hex = (int)strtol(raw, NULL, 16);

    address->raw = raw;
    address->page_number = hex ^ 0xf0;
    address->page_number = hex ^ 0x0f;
}

char* readData(FILE *fd) {
    char buffer[PAGE_SIZE];

    fseek(fd, rand() % (PAGE_SIZE * (PAGE_ENTRIES - 1)), SEEK_SET);
    fread(buffer, PAGE_SIZE, 1, fd);

    return buffer;
}

int main(int argc, char **argv) {
    FILE *fd = fopen("BACKING_STORE.bin", "r");

    // TODO: get list of page references

    // TODO: loop through addresses

    Address address;
    parseLogicalAddress(&address, "ff");

    // TODO: check if address is in tlb

    // TODO: if not in TLB, pull values from backing store
    char *data = readData(fd);

    // TODO: insert into TLB


    fclose(fd);
}
