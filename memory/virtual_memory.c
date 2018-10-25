
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
    address->raw = raw;

    // TODO: parse raw address string

    // TODO: set page number && offset
}

int main(int argc, char **argv) {
    // TODO: get list of page references

    // TODO: loop through addresses

    Address address;
    parseLogicalAddress(&address, "ff");

    // TODO: check if address is in tlb

    // TODO: if not in TLB, pull values from backing store


}
