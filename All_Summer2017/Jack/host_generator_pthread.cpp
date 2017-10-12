#include <iostream>
#include <pthread.h>             // pthreads
#include <stdlib.h>              // srand, rand
#include <time.h>                // time
#include <fstream>               // file stream
//#include "FGLockLinkedList.h"    // Linked List
#include "CoarseLinkedList.h"    // Linked List
#include <atomic>                // atomic

using namespace std;

std::atomic<int> op_count;
std::atomic<int> size;

struct thread_data{
    int lb;
    int rb;
    int cp;
    int ttop;
    int maxsz;
    int id;
    LinkedList* llp;
};

void *func_gen(void *thread_arg) {
    struct thread_data* mydata;
    mydata = (struct thread_data *) thread_arg;

    cout << "this is a message from thread " << mydata->id << endl;

    int         range;
    int         key_value;
    int         rand_temp;
    string      filename = "operations0.txt";
    ofstream    op_txt;

    filename[10] = filename[10] + mydata->id;
    op_txt.open(filename.c_str());
    range = mydata->rb - mydata->lb + 1;


    while (op_count < mydata->ttop){
        op_count++;
        key_value = rand()%range + mydata->lb;
        rand_temp = rand()%100 + 1;

        if (rand_temp <= mydata->cp){
            // contains case
            op_txt << "contains(" << key_value << ") ";
            op_txt << (mydata->llp->contains(key_value) ? "yes\n" : "no\n");
        }
        else if (rand_temp > (mydata->cp+(100-mydata->cp)/2)){
            // remove case
            op_txt << "remove(" << key_value << ") ";
            if (mydata->llp->remove(key_value)){
                op_txt << "yes\n";
                size--;
            }
            else{
                op_txt << "no\n";
			}
        }
        else {
            // add case
            // add operations
            if (size < mydata->maxsz){
                op_txt << "add(" << key_value << ") ";
                if (mydata->llp->add(key_value)){
                    op_txt << "yes\n";
                    size++;
                }
                else{
                    op_txt << "no\n";
				}
            }
            // remove operations
            else{
                op_txt << "remove(" << key_value << ") ";
                if (mydata->llp->remove(key_value)){
                    op_txt << "yes\n";
                    size--;
                }
                else {
                    op_txt << "no\n";
				}
            }
        }
    }
    op_txt.close();
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
    // default numbers for debuging
    int     left_bound      = 1;
    int     right_bound     = 100;
    int     initial_size    = 10;
    int     max_size        = 100;
    int     contain_percent = 30;
    int     total_ops       = 10000;
    int     range           = 101;
    int     key_value;
    int     NUM_THREADS     = 4;

    if (argc!=1) {
        cout << "left bound of keys: ";
        cin >> left_bound;
        cout << "right bound of keys: ";
        cin >> right_bound;
        cout << "initial_size: ";
        cin >> initial_size;
        cout << "maximum size: ";
        cin >> max_size;
        range = right_bound - left_bound + 1;
        // checks for valid inputs
        if (initial_size > max_size){
            cout << "Error: initial size is greater than the maximum size\n";
            return 0;
        }
        if (initial_size > range){
            cout << "Error: the range of right and left bounds is less than the initial size\n";
            return 0;
        }
        // check for valid percentage
        cout << "the percentage of \"contains\" operation: ";
        cin >> contain_percent;
        if (contain_percent > 100){
            cout << "Error: the percentage of \"contains\" operation is greater than 100\n";
            return 0;
        }
        if (contain_percent < 0){
            cout << "Error: the percentage of \"contains\" operation is less than 0\n";
            return 0;
        }

        cout << "total number of operations: ";
        cin >> total_ops;
    }


    LinkedList* int_list = new LinkedList;
    srand (time(NULL));
    op_count = 0;

    // Initialize the linked list
    ofstream init_txt("init.txt");
    while (size < initial_size){
        key_value = rand()%range + left_bound;
        if (int_list->add(key_value)){
            init_txt << "add(" << key_value << ") yes\n";
            size++;
        }
        else
            init_txt << "add(" << key_value << ") no\n";
    }
    init_txt.close();

    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];

    int th_ret;
    int i;

    for ( i=0; i < NUM_THREADS; i++) {
        cout << "main(): creating thread " << i << endl;
        td[i].lb = left_bound;
        td[i].rb = right_bound;
        td[i].ttop = total_ops;
        td[i].maxsz = max_size;
        td[i].cp = contain_percent;
        td[i].id = i;
        td[i].llp = int_list;
        th_ret = pthread_create(&threads[i], 0, func_gen, (void *)&td[i]);
        if (th_ret){
           cout << "Error:unable to create thread," << i << endl;
           exit(-1);
        }
    }

    pthread_exit(NULL);
    return 0;
}





