#include <iostream>
#include <fstream>          // file stream
#include <string>           // string
#include "LinkedList.h"     // Linked List
using namespace std;
typedef unsigned long ulong_t;

//-------------------------------- String Compare Function --------------------------------//

bool strcomp(string s1, string s2){
    if (s1.length() == s2.length()){
        for (int i = 0; i < s1.length(); i++){
            if (s1[i] != s2[i])
                return false;
        }
        return true;
    }
    else
        return false;
}



//-------------------------------- String to ulong_t Function --------------------------------//
ulong_t str2ulong_t(string str) {
    ulong_t result = 0;
    int     size   = str.length();
    int     power  = 1;
    int     num    = 0;

    for (int i = size-1; i >= 0; i--){
        num = str[i] - '0';
        num = num*power;
        result = result + num;
        power = power*10;
    }

    return result;
}


//--------------------------------------- Main Function --------------------------------------//
int main(int argc, char const *argv[]) {

    LinkedList    my_list;
	ulong_t       init_size;
	ifstream      infile;
    string        temp;
	int           state      = 0;
	
	infile.open("system.terminal");

	while (state != 3) {
		if (state == 0) {
			infile >> temp;
			if ( strcomp("(main.cpp):",temp) )
				state = 1;
		}
		else if (state == 1) {
			infile >> temp;
			if ( strcomp("(main.cpp):",temp) )
				state = 1;
			else if ( strcomp("added",temp) )
				state = 2;
			else
				state = 0;
		}
		else if (state == 2) {
			infile >> temp;
			init_size = str2ulong_t(temp);
			infile >> temp; // elements
			infile >> temp; // to
			infile >> temp; // linked
			infile >> temp; // list
			infile >> temp; // ...
			infile >> temp; // printing
			infile >> temp; // content:
			state = 3;
		}
	}

//******************************* Initialized the list *******************************//

	for (ulong_t i=0; i<init_size; i++) {
		infile >> temp;
        my_list.add( str2ulong_t(temp) );
    }

//***************************** Stream in the operations *****************************//    

	// check for the total operations
	infile >> temp; // (main.cpp):
	infile >> temp; // testing
	infile >> temp; // with
	infile >> temp; // a
	infile >> temp; // total
	infile >> temp; // of 
	infile >> temp;
	const ulong_t ttop = str2ulong_t(temp);
 	
	char             com_typ[ttop];
	ulong_t          com_val[ttop];
	bool             com_res[ttop];
	ulong_t          com_stp[ttop];
	int              count           = 0;

	while ( infile >> temp ) {
		if ( strcomp("Print_from_Thread_ID" , temp)) {
            infile >> temp; // ID:
			infile >> temp;
			com_typ[count] = temp[0];
			infile >> temp;
			com_val[count] = str2ulong_t(temp);
			infile >> temp;
			com_res[count] = (temp[0] == '1');
			infile >> temp; // timestamp:
			infile >> temp;
			com_stp[count] = str2ulong_t(temp);
			count++;
		}
	}
	infile.close();
    
//************************** Sort the commands by time stamp *************************//

for (int i=0; i<ttop; i++) {
	ulong_t temp_min = com_stp[i];
	int     j_min    = i;
	for (int j=i+1; j<ttop; j++) {
		if(com_stp[j] < temp_min) {
			j_min    = j;
			temp_min = com_stp[j];
		}
	}
	char    tp_typ = com_typ[i];
	ulong_t tp_val = com_val[i];
	bool    tp_res = com_res[i];
	ulong_t tp_stp = com_stp[i];
	com_typ[i] = com_typ[j_min];
	com_val[i] = com_val[j_min];
	com_res[i] = com_res[j_min];
	com_stp[i] = com_stp[j_min];
	com_typ[j_min] = tp_typ;
	com_val[j_min] = tp_val;
	com_res[j_min] = tp_res;
	com_stp[j_min] = tp_stp;
}

//******************************* Check for consistency ******************************//
	int  ii = 0;
    while( ii < ttop){
        if (com_typ[ii] == 'a') {
            if (com_res[ii] != my_list.add(com_val[ii])) {
				cout << "   add " << com_val[ii];
				cout << "\t at time stamp: " << ii << endl;
				return 0;
            }
        }
        else if (com_typ[ii] == 'c') {
            if (com_res[ii] != my_list.contains(com_val[ii])) {
				cout << "   contains " << com_val[ii];
				cout << "\t at time stamp: " << ii << endl;
				return 0;
            }
        }
        else if (com_typ[ii] == 'r') {
            if (com_res[ii] != my_list.remove(com_val[ii])) {
				cout << "   remove " << com_val[ii];
				cout << "\t at time stamp: " << ii << endl;
				return 0;
            }
        }
        else {
            cout << " Invalid char in com_type: " << com_typ[ii] << "at " << ii << endl;
            return 0;
        }
		ii++;
    } // big while
	cout << "\n\tTest passed.\n" << endl;
	return 0;
}
