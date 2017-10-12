#include <iostream>
#include <string>
using namespace std;


//-------------------------------- String to Int Function --------------------------------//
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


//-------------------------------- the main Function --------------------------------//
int main(){
	string num_s = "123";
	int    num_i = str2int(num_s);
	cout << num_i+3 << endl;
	return 0;
}
