//heap allocator is a memory management system that provides
//dynamic memory allocation for functions like malloc and free

//the heap is a region of memory that is managed by the operating system
//the heap splits the memory into blocks of a fixed size
//allocated blocks are the memory for the program
//free blocks are the memory that is not being used

//the allocator stores metadata about the blocks in the heap


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HEAP_SIZE 1024

typedef struct Block {
    bool is_free;
    size_t size;
    struct Block *next;
} Block;

//static variables in c
//if a static variable is declared in a function, it retains its value between function calls
//if a static variable is declared in a global scope, it is only visible to functions in that file

static char heap[HEAP_SIZE];
static Block *free_list = NULL;

//intialize the heap
void init_heap(){
    free_list = (Block *) heap;
    free_list -> size = HEAP_SIZE - sizeof(Block);
    free_list -> is_free = true;
    free_list -> free_memory = (void *)((char *)free_list + sizeof(Block));
    free_list -> next = NULL;
}


void my_free(void *ptr){
    if (!ptr){
        return;
    }

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block -> is_free = true;
    merge_block();
}

void print_heap(){
    Block *current = free_list;
    printf("Heap:\n");
    while(current){
        printf("Block: %p, size: %lu, is_free: %d\n", current, current -> size, current -> is_free);
        current = current -> next;
    }
}

void *my_malloc(size_t size){
    Block *current = free_list;

    while (current){
        //if the block is free and larger than the requested size
        if (current->is_free && current->size >=size){
            current->is_free = false;
            split_block(current, size);
            return current->free_memory;
        }
        current = current->next;
    }
    return NULL;
}


void split_block(Block *block, size_t size){
    if (block->size > size + sizeof(Block)){
        //if the block is larger than the requested size + metadata
        Block *new_block = (Block *)((char *)block + sizeof(Block) + size);

        new_block->size = block->size - size - sizeof(Block);
        new_block->is_free = true;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;

    }

}

void merge_block(){
    Block *current = free_list;
    while (current & current->next){
        if (current->is_free && current->next->is_free){
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        }
        else {
            current = current->next;
        }
    }
}

int main(){
    init_heap();

    void *ptr1 = my_malloc(100);
    printf("Allocated 100 bytes at %p\n", ptr1);
    print_heap();
    print_heap();

    void *ptr2 = my_malloc(200);
    printf("Allocated 200 bytes at %p\n", ptr2);
    print_heap();

    my_free(ptr1);
    printf("Freed 100 bytes\n");
    print_heap();

    void *ptr3 = my_malloc(50);
    printf("Allocated 50 bytes at %p\n", ptr3);
    print_heap();

    return 0;
}