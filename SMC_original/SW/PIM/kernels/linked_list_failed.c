#include "kernel_params.h"
#include "defs.hh"
#include <stdint.h>
#include <stdio.h>

//*************************************


typedef struct node {
    uint32_t key;      // 4 bytes
    struct node *next; // 4 bytes
} node_t;


#define BLOCK_SIZE sizeof(node_t)
#define NUM_MEM_BLOCKS (PIM_VREG_SIZE/BLOCK_SIZE)
#define BLOCK_BASE_ADDR (&PIM_VREG[0])


typedef struct free_block_node {
    struct free_block_node *next; // 4 bytes
} free_block_node_t;


// assigning values to global variables so that they are placed in data section, not bss
free_block_node_t free_blocks_tail = {(free_block_node_t *)1}; // sentinel
free_block_node_t *free_blocks_head = &free_blocks_tail; // points to an available free block as is (unless all blocks are allocated; in this case points to tail sentinel)

node_t * alloc_memory_block() {
    // return address of allocated block; 0 if memory not available
    node_t *allocated_block = (node_t *)free_blocks_head;
    if (free_blocks_head->next == 0) { // free_blocks_head == free_blocks_tail sentinel, out of memory
        return 0;
    }
    free_blocks_head = free_blocks_head->next;
    return allocated_block;
}

void free_memory_block(node_t *nodeptr) {
    ((free_block_node_t *)nodeptr)->next = free_blocks_head;
    free_blocks_head = (free_block_node_t *)nodeptr;
}


node_t head = {1, (node_t *)1};
node_t tail = {1, (node_t *)1};

void list_init()
{
    int i = 0;
    free_block_node_t *free_block;

    // initialize memory allocation
    free_blocks_tail.next = 0;
    free_blocks_head = &free_blocks_tail;
    for (i = 0; i < NUM_MEM_BLOCKS; i++) {
        free_block = (free_block_node_t *)(BLOCK_BASE_ADDR + (i*BLOCK_SIZE));
        free_block->next = free_blocks_head;
        free_blocks_head = free_block;
    }
    
    // initialize head/tail sentinel nodes
    head.key = 0;
    tail.key = UINT32_MAX;
    head.next = &tail;
    tail.next = 0;
}



/* returns:
    1: if add_key was not in list and successfully added,
    0: if add_key already existed in list or if memory is not available
*/
uint8_t list_add(uint32_t add_key)
{
    node_t *pred = &head;
    node_t *curr = pred->next;
    node_t *new_node;

    while (curr->key < add_key) {
        pred = curr;
        curr = curr->next;
    }

    if (curr->key == add_key) {
        pim_print_msg("key already exists in list");
        return 0;
    }
    new_node = (node_t *)alloc_memory_block();
    if (new_node == 0) {
        pim_print_msg("memory unavailable");
        return 0;
    }
    new_node->key = add_key;
    new_node->next = curr;
    pred->next = new_node;
    return 1;
}


/* returns:
    1: if remove_key was in list and successfully removed, 
    0: if remove_key was not in list
*/
uint8_t list_remove(uint32_t remove_key)
{
    node_t *pred = &head;
    node_t *curr = pred->next;

    while (curr->key < remove_key) {
        pred = curr;
        curr = curr->next;
    }
    if (curr->key != remove_key) {
        return 0;
    }
    pred->next = curr->next;
    free_memory_block(curr);
    return 1;
}

/* returns:
    1: if list contains contains_key
    0: if list does not contain contains_key
*/
uint8_t list_contains(uint32_t contains_key)
{
    node_t *curr = &head;
    while (curr != 0) {
        if (curr->key == contains_key) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}


void list_print()
{
}


void execute_kernel()
{
    /*
    SREG[0]: command type
    SREG[1]: parameter (key for the add/remove/contains calls)
    SREG[7]: return value
    */

    ulong_t command = PIM_SREG[0];
    uint32_t param_key = PIM_SREG[1];

    switch (command) {
        case 0: 
            pim_print_msg("case init");
            list_init();
            break;
        case 1:
            pim_print_msg("case add");
            PIM_SREG[7] = (ulong_t) list_add(param_key);
            break;
        case 2: 
            pim_print_msg("case remove");
            PIM_SREG[7] = (ulong_t) list_remove(param_key);
            break;
        case 3:
            pim_print_msg("case contains");
            PIM_SREG[7] = (ulong_t) list_contains(param_key);
            break;
        case 4:
            pim_print_msg("case print");
            list_print(); // TODO: implement
            break;
        default:
            return;
    }

}
