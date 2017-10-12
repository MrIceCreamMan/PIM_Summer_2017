#!/bin/bash

echo -e "
#include \"defs.hh\"
#define $OFFLOADED_KERNEL_NAME
#define SMC_BURST_SIZE_B $SMC_BURST_SIZE_B
#define NAME \"$OFFLOADED_KERNEL_NAME\"
#define FILE_NAME \"${OFFLOADED_KERNEL_NAME}.hex\"
#define KEY_LOWER_BOUND $KEY_LOWER_BOUND
#define KEY_UPPER_BOUND $KEY_UPPER_BOUND
#define MAX_LIST_SIZE $MAX_LIST_SIZE
#define INITIAL_LIST_SIZE $INITIAL_LIST_SIZE
#define TOTAL_NUM_OPS $TOTAL_NUM_OPS
#define READ_ONLY_PERCENTAGE $READ_ONLY_PERCENTAGE
#define REQUIRED_MEM_SIZE 0 
" > _app_params.h

echo -e "
#define $OFFLOADED_KERNEL_NAME
" > kernel_params.h

print_msg "Building App ..."
${HOST_CROSS_COMPILE}g++ main.cc -static -L./ -lpimapi -lpthread -lstdc++ -o main -Wall $HOST_OPT_LEVEL


