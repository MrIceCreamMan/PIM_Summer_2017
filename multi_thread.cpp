#include <iostream>
#include <thread>    		// threads
#include <stdlib.h> 		// srand, rand
#include <time.h>   		// time
#include <fstream>  		// file stream
#include <atomic>   		// atomic
using namespace std;


std::atomic<int> x;
std::atomic<bool> lock (false);

void function_1(int id){
	//int val = 500;
	ofstream	op_txt[2];
	string  	filename = "operations0.txt";
	filename[10] = filename[10] + id;
	op_txt[id-1].open(filename.c_str());

	while (x<100){
		if (!lock.exchange(true)) {
    		x++;
			cout << "id: " << id << " x=" << x << endl;
			op_txt[id-1] << "id: " << id << " x=" << x << endl;
			lock.exchange(false);
		}

	}
	return;
}


int main() {

	int val;
	cin >> val;
	cout << val << endl;

	x = 0;

    std::thread t1(function_1,1);
	std::thread t2(function_1,2);

	if (t1.joinable())
        t1.join();
	if (t2.joinable())
		t2.join();
    
    return 0;
}
