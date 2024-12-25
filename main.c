#include "third_party/nugget/common/syscalls/syscalls.h"

volatile int state = 0;

int main() {
    void *main_label_addr, *manipulated_memory_label_addr, *cache_invalidate_destination_label_addr;
    main_label_addr = &&main_start;
    manipulated_memory_label_addr = &&manipulated_memory;
    cache_invalidate_destination_label_addr = &&cache_invalidate_destination;
    void *jmp_target = manipulated_memory_label_addr + 8;
    ramsyscall_printf("main_start adress: %p\n", main_label_addr);
    ramsyscall_printf("manipulated_memory adress: %p\n", manipulated_memory_label_addr);
    ramsyscall_printf("cache_invalidate_destination adress: %p\n", cache_invalidate_destination_label_addr);
    ramsyscall_printf("jmp_target adress: %p\n", jmp_target);
    register void *address asm("$t0") = (void *)main_label_addr;
    register void *address2 asm("$t1") = (void *)cache_invalidate_destination_label_addr;
    goto cache_invalidate_destination;
    
manipulated_memory:
    __asm__ volatile (
        "nop\n"
        "nop\n"
        "jr $t0\n"   // Jump to the address stored in the pointer
        "nop\n"     // Delay slot (no operation)
    );
main_start:
    // ramsyscall_printf("Main state: %d\n", state);
    switch (state)
    {
    case 0:
        state++;
        register void *address asm("$t0") = (void *)main_label_addr;
        register void *address2 asm("$t1") = (void *)cache_invalidate_destination_label_addr;
        register void *address6 asm("$t3") = (void *)manipulated_memory_label_addr+8;
        __asm__ volatile (
            "jr $t3\n"   // Jump to the address stored in the pointer
            "nop\n"     // Delay slot (no operation)
        );
        break;
    case 1:
        int* alteredAddress = manipulated_memory_label_addr;
        *alteredAddress = 0x01200008;
        state++;
        register void *addres3 asm("$t0") = (void *)main_label_addr;
        register void *address4 asm("$t1") = (void *)cache_invalidate_destination_label_addr;
        goto manipulated_memory;
        break;
    
    default:
    end_label:
        goto end_label;
        break;
    }
    
cache_invalidate_destination:
    if(state != 0)
        ramsyscall_printf("cache has been invalidate to access this part\n");
    goto main_start;
}
