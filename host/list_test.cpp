#include <iostream>
#include <fstream>          // file stream
#include <string>           // string
#include "LinkedList.h"     // Linked List
using namespace std;

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

int str2int(string str) {
    string nstr = str;
    int sign = 1;
    int result = 0;
    int size = str.length();
    int power = 1;
    int num = 0;

    if ( str[0] == '-' ){
        size = str.length() - 1;
        nstr = str.substr(1,size);
        sign = 0;
    }

    for (int i = size-1; i >= 0; i--){
        num = nstr[i] - '0';
        num = num*power;
        result = result + num;
        power = power*10;
    }
    
    if (sign == 1)
        return result;
    else
        return (0-result);
}


int main(int argc, char const *argv[]) {
    ifstream infile1;
    if (argc == 2) {
        infile1.open(argv[1]);
    }
    else if (argc == 1) {
        infile1.open("operations.txt");
    }
    else {
        cout << "file cannot be opened" << endl;
        return 0;
    }
    
    string str1;
    bool res1;
    string command1;
    int number1;
    LinkedList<int> int_list;

    while(true){
		  if (!(infile1>>str1)){
		      cout << "file 1 finished" << endl;
		      infile1.close();
		      break;
		  }
	//cout << "t1: " << str1 << endl;  //*******************************
		  if (str1[0] == 'a') {
		      number1 = str2int(str1.substr(4,str1.length()-5));
		      res1 = int_list.add(number1);
		      infile1 >> str1;
		      if (str1[0] == 'y' && !res1) {
		        cout << "add " << number1 << " res1: " << res1 << endl;
						break;
		      }
		      else if (str1[0] == 'n' && res1) {
		        cout << "add " << number1 << " res1: " << res1 << endl;
						break;
		      }
		  }
		  else if (str1[0] == 'c') {
		      number1 = str2int(str1.substr(9,str1.length()-10));
		      res1 = int_list.contains(number1);
		      infile1 >> str1;
		      if (str1[0] == 'y' && !res1) {
		        cout << "contain " << number1 << "res1: " << res1 << endl;
						break;
		      }
		      else if (str1[0] == 'n' && res1) {
		        cout << "contain " << number1 << "res1: " << res1 << endl;
						break;
		      }
		  }
		  else if (str1[0] == 'r') {
		      number1 = str2int(str1.substr(7,str1.length()-8));
		      res1 = int_list.remove(number1);
		      infile1 >> str1;
		      if (str1[0] == 'y' && !res1) {
		        cout << "remove " << number1 << "res1: " << res1 << endl;
						break;
		      }
		      else if (str1[0] == 'n' && res1) {
		        cout << "remove " << number1 << "res1: " << res1 << endl;
						break;
		      }
		  }
		  else {
		      cout << " weired string: " << str1 << endl;
		      infile1.close();
					break;
		  }
    }
		int_list.print();
		return 0;
}
