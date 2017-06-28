#include <iostream>
#include <string>
#include <stdlib.h> 		// srand, rand
#include <time.h>   		// time

using namespace std;

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

int main() {
	string s1;
	s1 = "-456";
	cout << "length = " << s1.length() << endl;
	cout << "s1[0] = \'" << s1[0] << "\'" << endl;

	srand (time(NULL));
	s1 = std::to_string(rand()%1001 - 500);
	cout << s1 << " is " << str2int(s1) << endl;
	return 0;
}
