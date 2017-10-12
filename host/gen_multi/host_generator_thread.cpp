#include <iostream>
#include <thread>           // threads
#include <stdlib.h>         // srand, rand
#include <time.h>           // time
#include <fstream>          // file stream
#include "LinkedList.h"     // Linked List
#include <atomic>           // atomic

using namespace std;

std::atomic<int> op_count;
std::atomic<int> size;
std::atomic<bool> lock (false);

void func_gen(int lbound, int rbound, int cp, int ttop, int maxsz, int id, LinkedList<int>* llp) {

    int         range;
    int         key_value;
    int         rand_temp;
    string      filename = "operations0.txt";
    ofstream    op_txt;

    filename[10] = filename[10] + id;
    op_txt.open(filename.c_str());
    range = rbound - lbound + 1;


    while (op_count < ttop){
        if (!lock.exchange(true)) {
            op_count++;
            //cout << "id: " << id << " op count=" << op_count << endl;

            key_value = rand()%range + lbound;
            rand_temp = rand()%100 + 1;
            if (rand_temp <= cp){
                // contains case
                op_txt << "contains(" << key_value << ") ";
                op_txt << (llp->contains(key_value) ? "yes\n" : "no\n");
            }
            else if (rand_temp > (cp+(100-cp)/2)){
                // remove case
                // remove operations
                if (size > 0){
                    op_txt << "remove(" << key_value << ") ";
                    if (llp->remove(key_value)){
                        op_txt << "yes\n";
                        size--;
                    }
                    else
                        op_txt << "no\n";
                }
                // add operations
                else {
                    op_txt << "add(" << key_value << ") ";
                    if (llp->add(key_value)){
                        op_txt << "yes\n";
                        size++;
                    }
                    else
                        op_txt << "no\n";
                }
            }
            else {
                // add case
                // add operations
                if (size < maxsz){
                    op_txt << "add(" << key_value << ") ";
                    if (llp->add(key_value)){
                        op_txt << "yes\n";
                        size++;
                    }
                    else
                        op_txt << "no\n";
                }
                // remove operations
                else{
                    op_txt << "remove(" << key_value << ") ";
                    if (llp->remove(key_value)){
                        op_txt << "yes\n";
                        size--;
                    }
                    else
                        op_txt << "no\n";
                }
            }
            lock.exchange(false);
        }
    }
    op_txt.close();
    return;
}

int main(int argc, char const *argv[]){
    int     left_bound = 0;
    int     right_bound = 100;
    int     initial_size = 30;
    int     max_size = 50;
    float   contain_percent = 10;
    int     total_ops = 100;
    int     range = 101;
    int     key_value;

    if (argc==1) {
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


    LinkedList<int>* int_list = new LinkedList<int>;
    srand (time(NULL));
    size = 0;
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

    std::thread t1(func_gen, left_bound, right_bound, contain_percent, total_ops, max_size, 1, int_list);
    std::thread t2(func_gen, left_bound, right_bound, contain_percent, total_ops, max_size, 2, int_list);

    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();

    //cout << "The size of my list is " << int_list->size() << endl;
    //cout << "Size count " << size << endl;
    return 0;
}





