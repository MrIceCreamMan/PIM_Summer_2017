#include "defs.hh"
#include "kernel_params.h"
#include <stdint.h>
#include <stdio.h>


/******************************************************************************/
typedef struct node {
    ulong_t key;      // 4 bytes
    struct node *next; // 4 bytes
} node_t;


#define BLOCK_SIZE sizeof(node_t)
#define NUM_MEM_BLOCKS (PIM_VREG_SIZE/BLOCK_SIZE)
#define BLOCK_BASE_ADDR (&PIM_VREG[0])
#define FREE_BLOCKS_HEAD PIM_SREG[3]
#define LINKED_LIST_HEAD ((node_t *)&(PIM_SREG[4])) // spans over SREG[4] and [5]
#define LINKED_LIST_TAIL ((node_t *)&(PIM_SREG[6])) // spans over SREG[6] and [7]


typedef struct free_block_node {
    struct free_block_node *next; // 4 bytes
} free_block_node_t;


node_t * alloc_memory_block() {
    // return address of allocated block; 0 if memory not available
    // if FREE_BLOCKS_HEAD == 0, memory is not available.
    node_t *mem_block = (node_t *)FREE_BLOCKS_HEAD;
    if (mem_block != 0) {
        FREE_BLOCKS_HEAD = (ulong_t)(((free_block_node_t *)mem_block)->next);
    }
    pim_print_hex("allocated block", (ulong_t)mem_block);
    pim_print_hex("current free blocks list head", (ulong_t)FREE_BLOCKS_HEAD);
    pim_print_hex("    points to", (ulong_t)((free_block_node_t *)FREE_BLOCKS_HEAD)->next);
    return mem_block;
}


void free_memory_block(node_t *nodeptr) {
    ((free_block_node_t *)nodeptr)->next = (free_block_node_t *)FREE_BLOCKS_HEAD;
    FREE_BLOCKS_HEAD = (ulong_t)nodeptr;
    pim_print_hex("current free blocks list head", (ulong_t)FREE_BLOCKS_HEAD);
    pim_print_hex("    points to", (ulong_t)((free_block_node_t *)FREE_BLOCKS_HEAD)->next);
}


void list_init()
{
    int i = 0;
    free_block_node_t *free_block;
 
    // initialize free blocks list   
    FREE_BLOCKS_HEAD = 0;
    for (i = 0; i < NUM_MEM_BLOCKS; i++) {
        free_block = (free_block_node_t *)(BLOCK_BASE_ADDR + (i*BLOCK_SIZE));
        free_block->next = (free_block_node_t *)FREE_BLOCKS_HEAD;
        FREE_BLOCKS_HEAD = (ulong_t)free_block;
    }

    // initialize head/tail sentinel nodes
    LINKED_LIST_HEAD->key = 0;
    LINKED_LIST_HEAD->next = LINKED_LIST_TAIL;
    LINKED_LIST_TAIL->key = UINT32_MAX;
    LINKED_LIST_TAIL->next = (node_t *)0;
}


/* returns:
    1: if add_key was not in list and successfully added,
    0: if add_key already existed in list or if memory is not available
*/
ulong_t list_add(ulong_t add_key)
{
    node_t *pred = LINKED_LIST_HEAD;
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

    new_node = alloc_memory_block();
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
ulong_t list_remove(ulong_t remove_key)
{
    node_t *pred = LINKED_LIST_HEAD;
    node_t *curr = pred->next;

    while (curr->key < remove_key) {
        pred = curr;
        curr = curr->next;
    }
    if (curr->key != remove_key) {
        pim_print_msg("remove_key not in list");
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
ulong_t list_contains(ulong_t contains_key)
{
    node_t *curr = LINKED_LIST_HEAD;
    while (curr->key < contains_key) {
        curr = curr->next;
    }
    if (curr->key == contains_key) {
        return 1;
    } else {
        return 0;
    }
}


void execute_kernel()
{
    /*
    SREG[0]: command type
    SREG[1]: parameter (key for the add/remove/contains calls)
    SREG[2]: return value
    SREG[3]: free blocks list head
    SREG[4]: linked list sentinel head
    SREG[5]: 
    SREG[6]: linked list sentinel tail
    SREG[7]: 
    */

    ulong_t command = PIM_SREG[0];
    ulong_t param_key = PIM_SREG[1];
    pim_print_msg("inside execute kernel");

    switch (command) {
        case 0: 
            //pim_print_msg("case init");
            list_init();
            break;
        case 1:
            //pim_print_msg("case add");
            PIM_SREG[2] = (ulong_t) list_add(param_key);
            break;
        case 2: 
            //pim_print_msg("case remove");
            PIM_SREG[2] = (ulong_t) list_remove(param_key);
            break;
        case 3:
            //pim_print_msg("case contains");
            PIM_SREG[2] = (ulong_t) list_contains(param_key);
            break;
        case 4:
            pim_print_msg("case print");
            //list_print(); // TODO: implement
            break;
        default:
            pim_print_msg("default case");
            return;
    }
}



