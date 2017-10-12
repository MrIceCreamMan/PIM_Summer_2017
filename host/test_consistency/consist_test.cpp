#include <iostream>
#include <fstream>          // file stream
#include <string>           // string
#include "LinkedList.h"    // Linked List
#include <vector>           // store the commands
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


//-------------------------------- Consistency Main Function --------------------------------//
int main(int argc, char const *argv[]) {

	// argv[1] states how many threads there are.
	if (argc != 2) {
		cout << "argc != 2, specify how many threads and try again" << endl;
		return 0;
	}
	const unsigned long thread_num = str2ulong_t(argv[1]);

//******************************* Initialized the list *******************************//
    LinkedList int_list;
	string     init_cmd;
	ulong_t    init_num;
	string     init_file_res;
	bool       init_list_res;

	ifstream init_txt("init.txt");

	while (init_txt >> init_cmd) {
        init_num = str2ulong_t(init_cmd.substr(4,init_cmd.length()-5));
        init_list_res = int_list.add(init_num);
        init_txt >> init_file_res;
        if (init_file_res[0] == 'y' && !init_list_res) {
			cout << "add(" << init_num << ") fail but input file says \'yes\'." << endl;
			return 0;
        }
        else if (init_file_res[0] == 'n' && init_list_res) {
			cout << "add(" << init_num << ") success but input file says \'no\'." << endl;
			return 0;
        }
    }
    init_txt.close();



//******************************* Stream in the operations *******************************//
	if (thread_num >= 10) {
		cout << "number of files are greater than 10" << endl;
		return 0;
	}
    ifstream                 infiles[thread_num];
	string                   filename;
    string                   temp;
	ulong_t                  tp_num;
	string                   tp_res;
	
	vector< vector<char> >     com_typ;
	vector< vector<ulong_t> >  com_val;
	vector< vector<bool> >     com_res;

	filename = "operations0.txt";

	for (int i=0; i<thread_num; i++) {
        infiles[i].open(filename.c_str());
		vector<char>     initialize_typ;
		vector<ulong_t>  initialize_val;
		vector<bool>     initialize_res;
		com_typ.push_back(initialize_typ);
		com_val.push_back(initialize_val);
		com_res.push_back(initialize_res);

		while (infiles[i]>>temp) {
            if (temp[0] == 'a') {
                tp_num = str2ulong_t(temp.substr(4,temp.length()-5));
				com_typ[i].push_back('a');
				com_val[i].push_back(tp_num);
                infiles[i] >> tp_res;
				if (tp_res[0] == 'y')
					com_res[i].push_back(true);
				else
					com_res[i].push_back(false);
				}
            else if (temp[0] == 'c') {
                tp_num = str2ulong_t(temp.substr(9,temp.length()-10));
				com_typ[i].push_back('c');
				com_val[i].push_back(tp_num);
                infiles[i] >> tp_res;
				if (tp_res[0] == 'y')
					com_res[i].push_back(true);
				else
					com_res[i].push_back(false);
			}
            else if (temp[0] == 'r') {
                tp_num = str2ulong_t(temp.substr(7,temp.length()-8));
				com_typ[i].push_back('r');
				com_val[i].push_back(tp_num);
                infiles[i] >> tp_res;
				if (tp_res[0] == 'y')
					com_res[i].push_back(true);
				else
					com_res[i].push_back(false);
			}
		}
		infiles[i].close();
    	filename[10]++;
	}
    
//******************************* Check for consistency *******************************//
/*
	for (int i = 0; i<thread_num; i++)
		for (int j = 0; j<com_typ[i].size(); j++)
			cout << com_typ[i][j] << " " << com_val[i][j] << " " << com_res[i][j] << endl;
*/
	int curr_loc[thread_num];
	int pred_loc[thread_num];
	for (int i = 0; i<thread_num; i++) {
		curr_loc[i] = 0;
		pred_loc[i] = 0;
	}

    int  dont_rmv     = 0;
    int  dont_add     = 0;
	int  curr_t       = 0;
	bool stored_res;
	bool calcud_res;
	int  state        = 1;
	int  srch_fin     = 0;
	char srch_com;
	int  srch_num;
	int  comfound;

    while(true){
        if (state == 1) { // executing commands on the main threads only
			curr_t = 0;
			if (com_typ[curr_t].size() == curr_loc[curr_t]) {
				cout << "\n\tTest passed.\n" << endl;
				return 0;
			}
            if (com_typ[curr_t][curr_loc[curr_t]] == 'a') {
                tp_num = com_val[curr_t][curr_loc[curr_t]];
                calcud_res = int_list.add(tp_num);
				stored_res = com_res[curr_t][curr_loc[curr_t]];
                if (stored_res && !calcud_res) {
                    state = 2;        // stored yes, calculated no.
					comfound = 0;
                    srch_com = 'r';
                    srch_num = tp_num;
                    dont_rmv = 1;
				cout << "   srch com= " << srch_com << "\t srch num= " << srch_num;
				cout << "\t line: " << curr_loc[0]+1 << endl;
                }
                else if (!stored_res && calcud_res) {
                    state = 2;        // stored no, calculated yes.
					comfound = 0;
                    srch_com = 'a';
                    srch_num = tp_num;
				cout << "   srch com= " << srch_com << "\t srch num= " << srch_num;
				cout << "\t line: " << curr_loc[0]+1 << endl;
                }
                else
                    state = 1;        // do nothing.
            }
            else if (com_typ[curr_t][curr_loc[curr_t]] == 'c') {
                tp_num = com_val[curr_t][curr_loc[curr_t]];
                calcud_res = int_list.contains(tp_num);
				stored_res = com_res[curr_t][curr_loc[curr_t]];
                if (stored_res && !calcud_res) {
                    state = 2;        // stored yes, calculated no.
					comfound = 0;
                    srch_com = 'a';
                    srch_num = tp_num;
				cout << "   srch com= " << srch_com << "\t srch num= " << srch_num;
				cout << "\t line: " << curr_loc[0]+1 << endl;
                }
                else if (!stored_res && calcud_res) {
                    state = 2;        // stored no, calculated yes.
					comfound = 0;
                    srch_com = 'r';
                    srch_num = tp_num;
				cout << "   srch com= " << srch_com << "\t srch num= " << srch_num;
				cout << "\t line: " << curr_loc[0]+1 << endl;
                }
                else
                    state = 1;        // do nothing.
            }
            else if (com_typ[curr_t][curr_loc[curr_t]] == 'r') {
                tp_num = com_val[curr_t][curr_loc[curr_t]];
                calcud_res = int_list.remove(tp_num);
				stored_res = com_res[curr_t][curr_loc[curr_t]];
                if (stored_res && !calcud_res) {
                    state = 2;        // stored yes, calculated no.
					comfound = 0;
                    srch_com = 'a';
                    srch_num = tp_num;
                    dont_add = 1;
				cout << "   srch com= " << srch_com << "\t srch num= " << srch_num;
				cout << "\t line: " << curr_loc[0]+1 << endl;
                }
                else if (!stored_res && calcud_res) {
                    state = 2;        // stored no, calculated yes.
					comfound = 0;
                    srch_com = 'r';
                    srch_num = tp_num;
				cout << "   srch com= " << srch_com << "\t srch num= " << srch_num;
				cout << "\t line: " << curr_loc[0]+1 << endl;
                }
                else
                    state = 1;        // do nothing.
            }
            else {
                cout << " Invalid char in com_type: " << com_typ[curr_t][curr_loc[curr_t]] << endl;
                return 0;
            }
			curr_loc[curr_t]++;
        }
        else if (state == 2) { // search for the conflicting command on other threads
			curr_t = 1;
			while ( curr_t<thread_num && comfound == 0 ) {
				curr_loc[curr_t] = pred_loc[curr_t];
				while (curr_loc[curr_t] < com_typ[curr_t].size() ) {
					char    tp_com = com_typ[curr_t][curr_loc[curr_t]];
					ulong_t tp_val = com_val[curr_t][curr_loc[curr_t]];
					bool    tp_res = com_res[curr_t][curr_loc[curr_t]];
					if ( tp_com == srch_com && tp_val == srch_num  && tp_res) { // com found
				        state = 3;
						comfound = 1;
						cout << "found at thread " << curr_t << " location " << curr_loc[curr_t]+1;
						cout << endl;
						break;
					}
					curr_loc[curr_t]++;
				}
				if (comfound == 0)
					curr_t++;
			} // curr_t<thread_num && comfound == 0
			if (comfound == 0) {
				cout <<  "\n\tCommand: " << srch_com << " " << srch_num << " at line ";
				cout << curr_loc[0] << " is not found in other threads. Test failed.\n";
				return 0;
			}
        } // else bracket
		else {// state == 3, executing all successful commands before the command just found
			while (pred_loc[curr_t] < curr_loc[curr_t]) {
				if ( com_res[curr_t][pred_loc[curr_t]] ) { // commands with return true
					if ( com_typ[curr_t][pred_loc[curr_t]] == 'a')
						int_list.add(com_val[curr_t][pred_loc[curr_t]]);
					else if ( com_typ[curr_t][pred_loc[curr_t]] == 'r')
						int_list.remove(com_val[curr_t][pred_loc[curr_t]]);
				}
				pred_loc[curr_t]++;
			}
			if (srch_com == 'a' && dont_add == 0)
				int_list.add(srch_num);
			else if (srch_com == 'r' && dont_rmv == 0)
				int_list.remove(srch_num);
			dont_rmv = 0;
			dont_add = 0;
			pred_loc[curr_t]++;
			state = 1;
		}
    } // big while
	cout << " should never get here \n";
	return 0;
}
