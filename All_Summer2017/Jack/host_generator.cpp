#include <iostream>
#include <stdlib.h> 		    // srand, rand
#include <time.h>   		    // time
#include <fstream>			    // file stream
#include "LockLinkedList.h" // Linked List

using namespace std;

int main(int argc, char* argv[]){
	int 	left_bound      = 0;
	int 	right_bound     = 100;
	int		initial_size    = 30;
	int 	max_size        = 80;
	int     contain_percent = 10;
	int 	size = 0;
	int 	range           = 101;
	int 	key_value;
	int 	total_ops       = 500;
	int		op_count = 0;
	int		rand_temp;

  if (argc ==  1) {
		cout << "left bound of keys: ";
		cin >> left_bound;
		cout << "right bound of keys: ";
		cin >> right_bound;
		cout << "initial_size: ";
		cin >> initial_size;
		cout << "maximum size: ";
		cin >> max_size;
		range = right_bound - left_bound;
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

		cout << "total number of operations: ";
		cin >> total_ops;
	}		

	LinkedList<int> int_list;
	srand (time(NULL));
	// Initialize the linked list
	ofstream init_txt("init.txt");
	while (size < initial_size){
		key_value = rand()%range + left_bound;
		if (int_list.add(key_value)){
			init_txt << "add(" << key_value << ") yes\n";
			size++;
		}
		else
			init_txt << "add(" << key_value << ") no\n";
	}
	init_txt.close();

	ofstream op_txt;
	op_txt.open("operations.txt");	
	while (op_count < total_ops){
		op_count++;
		key_value = rand()%range + left_bound;
		rand_temp = rand()%100 + 1;
		if (rand_temp <= contain_percent){
			// contains case
			op_txt << "contains(" << key_value << ") ";
			op_txt << (int_list.contains(key_value) ? "yes\n" : "no\n");
		}
		else if (rand_temp > (contain_percent+(100-contain_percent)/2)){
			// remove case
			// remove operations
			if (size > 0){
				op_txt << "remove(" << key_value << ") ";
				if (int_list.remove(key_value)){
					op_txt << "yes\n";
					size--;
				}
				else
					op_txt << "no\n";
			}
			// add operations
			else {
				op_txt << "add(" << key_value << ") ";
				if (int_list.add(key_value)){
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
			if (size < max_size){
				op_txt << "add(" << key_value << ") ";
				if (int_list.add(key_value)){
					op_txt << "yes\n";
					size++;
				}
				else
					op_txt << "no\n";
			}
			// remove operations
			else{
				op_txt << "remove(" << key_value << ") ";
				if (int_list.remove(key_value)){
					op_txt << "yes\n";
					size--;
				}
				else
					op_txt << "no\n";
			}
		}
	}
	cout << "The size of my list is " << int_list.size() << endl;
	cout << "Size count " << size << endl;
	op_txt.close();
	return 0;
}
		
		
	
	
