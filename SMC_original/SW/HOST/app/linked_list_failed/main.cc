#include "pim_api.hh"
#include "app_utils.hh"
#include "_app_params.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>         // srand(), rand()
#include <time.h>           // time()
#include "LinkedList.h"     // my linked list to check the return values


int main(int argc, char *argv[])
{
    PIMAPI *pim = new PIMAPI(); // instantiate the PIM API

    cout << "(main.cpp): Kernel Name: " << FILE_NAME << endl;
    cout << "(main.cpp): Offloading the computation kernel ... " << endl;
    ____TIME_STAMP(1);
    pim->offload_kernel((char*)FILE_NAME);

    /* PIM scalar registers
    SREG[0]: command to execute 
                0 -- init(), 
                1 -- add(x), 
                2 -- remove(x), 
                3 -- contains(x), 
                4 -- print_list()  (prints list on PIM's end)
    SREG[1]: parameter for command (the x value for add/remove/contains commands)
    SREG[7]: return value
    */

    cout << "(main.cpp): lower bound for keys -- " << KEY_LOWER_BOUND << endl;
    cout << "(main.cpp): upper bound for keys -- " << KEY_UPPER_BOUND << endl;
    cout << "(main.cpp): maximum size of linked list -- " << MAX_LIST_SIZE << endl;
    cout << "(main.cpp): initial size of linked list -- " << INITIAL_LIST_SIZE << endl;

#if 0
    int range;
    range = KEY_UPPER_BOUND - KEY_LOWER_BOUND;
    // checks for valid inputs
    if (INITIAL_LIST_SIZE > MAX_LIST_SIZE){
        cout << "Error: initial size is greater than the maximum size\n";
        return 0;
    }
    if (INITIAL_LIST_SIZE > range){
        cout << "Error: the range of right and left bounds is less than the initial size\n";
        return 0;
    }

    LinkedList<int> int_list;    // my data structure to check the returns from PIM
    srand (time(NULL));          // seed the RNG with time
    int     size = 0;            // size of linked list
    int     key_value;           // this variable holds the key for a node
    bool    torf;                // True of False
    ulong_t pim_res;
    ulong_t cmp_res;
#endif

    cout << "(main.cpp): Create initial linked list ... " << endl;
    ____TIME_STAMP(3);
    pim->write_sreg(0, 0);       // (register number, command type)
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();

#if 0
    // very simple tests
    ____TIME_STAMP(3);
    pim->write_sreg(0,4);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();

    ____TIME_STAMP(3);
    pim->write_sreg(0,1);
    pim->write_sreg(1,2);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    ____TIME_STAMP(3);
    pim->write_sreg(0,4);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
#if 0 
    pim->write_sreg(0,1);
    pim->write_sreg(1,6);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    pim->write_sreg(0,1);
    pim->write_sreg(1,3);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    pim->write_sreg(0,1);
    pim->write_sreg(1,5);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    pim->write_sreg(0,1);
    pim->write_sreg(1,1);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    pim->write_sreg(0,1);
    pim->write_sreg(1,8);
    ____TIME_STAMP(5);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
    
    pim->write_sreg(0,4);
    pim->start_computation(PIMAPI::CMD_RUN_KERNEL);
    pim->wait_for_completion();
#endif
#endif

#if 0
    // populate linked list with initial values
    while (size < INITIAL_LIST_SIZE){
        key_value = rand()%range + KEY_LOWER_BOUND;      // generate a random value in the range
        pim->write_sreg(0,1);                            // add command
        pim->write_sreg(1,key_value);                    // add(key_value) to PIM list
        pim->start_computation(PIMAPI::CMD_RUN_KERNEL);  // PIM go!!!
        pim->wait_for_completion();                      // Hurry up PIM, people are waiting
        pim_res = pim->read_sreg(7);

        torf = int_list.add(key_value);                  // add(key_value) to my list
        // Check if the value was successfully added
        if (torf){
            cmp_res = 1;
            size++;
        }
        else
            cmp_res = 0;
        if (cmp_res != pim_res){
            cout << "The result of PIM does not agree with my linked List. Terminate Program.\n";
            return 0;
        }
    }

    int op_count = 0;            // count for the number of operations already generated
    int rand_temp;               // decide what types (add, remove, contains) should be generated
    
    cout << "(main.cpp): testing with a total of " << TOTAL_NUM_OPS << "operations." << endl;
    cout << "(main.cpp): percentage of read-only (contains) operations is ";
    cout << READ_ONLY_PERCENTAGE << endl;
    // TODO: generate random operations and execute on linked list

    while (op_count < TOTAL_NUM_OPS){
        op_count++;
        key_value = rand()%range + KEY_LOWER_BOUND;
        rand_temp = rand()%100 + 1;
        if (rand_temp <= READ_ONLY_PERCENTAGE){
            // contains case
            pim->write_sreg(0,3);                           // _______________________________
            pim->write_sreg(1,key_value);                   // 
            pim->start_computation(PIMAPI::CMD_RUN_KERNEL); // send contains(key_value) to PIM
            pim->wait_for_completion();                     // and wait for the result
            pim_res = pim->read_sreg(7);                    // _______________________________

            torf = int_list.contains(key_value);            // send contains(key_value) to my list
            if (torf)
                cmp_res = 1;
            else
                cmp_res = 0;
            if (cmp_res != pim_res){
                cout << "Error: The result of PIM does not agree with my linked List.\n";
                return 0;
            }        
        }
        else if (rand_temp > (READ_ONLY_PERCENTAGE+(100-READ_ONLY_PERCENTAGE)/2)){
            // remove case:    if the list is empty, then remove() becomes add()
            // remove operations
            if (size > 0){
                pim->write_sreg(0,2);                           // _______________________________
                pim->write_sreg(1,key_value);                   // 
                pim->start_computation(PIMAPI::CMD_RUN_KERNEL); // send remove(key_value) to PIM
                pim->wait_for_completion();                     // and wait for the result
                pim_res = pim->read_sreg(7);                    // _______________________________

                torf = int_list.remove(key_value);              // send remove(key_value) to my list
                if (torf){
                    cmp_res = 1;
                    size--;
                }
                else
                    cmp_res = 0;
                if (cmp_res != pim_res){
                    cout << "Error: The result of PIM does not agree with my linked List.\n";
                    return 0;
                }
            }
            // add operations
            else {
                pim->write_sreg(0,1);                           // _______________________________
                pim->write_sreg(1,key_value);                   // 
                pim->start_computation(PIMAPI::CMD_RUN_KERNEL); // send add(key_value) to PIM
                pim->wait_for_completion();                     // and wait for the result
                pim_res = pim->read_sreg(7);                    // _______________________________

                torf = int_list.add(key_value);                 // send add(key_value) to my list
                if (torf){
                    cmp_res = 1;
                    size++;
                }
                else
                    cmp_res = 0;
                if (cmp_res != pim_res){
                    cout << "Error: The result of PIM does not agree with my linked List.\n";
                    return 0;
                }
            }
        }
        else {
            // add case:    If the list reaches max size, then add() becomes remove()
            // add operations
            if (size < max_size){
                pim->write_sreg(0,1);                           // _______________________________
                pim->write_sreg(1,key_value);                   // 
                pim->start_computation(PIMAPI::CMD_RUN_KERNEL); // send add(key_value) to PIM
                pim->wait_for_completion();                     // and wait for the result
                pim_res = pim->read_sreg(7);                    // _______________________________

                torf = int_list.add(key_value);                 // send add(key_value) to my list
                if (torf){
                    cmp_res = 1;
                    size++;
                }
                else
                    cmp_res = 0;
                if (cmp_res != pim_res){
                    cout << "Error: The result of PIM does not agree with my linked List.\n";
                    return 0;
                }
            }
            // remove operations
            else{
                pim->write_sreg(0,2);                           // _______________________________
                pim->write_sreg(1,key_value);                   // 
                pim->start_computation(PIMAPI::CMD_RUN_KERNEL); // send remove(key_value) to PIM
                pim->wait_for_completion();                     // and wait for the result
                pim_res = pim->read_sreg(7);                    // _______________________________

                torf = int_list.remove(key_value);              // send remove(key_value) to my list
                if (torf){
                    cmp_res = 1;
                    size--;
                }
                else
                    cmp_res = 0;
                if (cmp_res != pim_res){
                    cout << "Error: The result of PIM does not agree with my linked List.\n";
                    return 0;
                }
            }
        }
    }
#endif

    cout << "Exiting gem5 ..." << endl;
    pim->give_m5_command(PIMAPI::M5_EXIT);
    return 0;
}
























