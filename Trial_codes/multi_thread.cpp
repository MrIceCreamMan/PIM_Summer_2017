#include <iostream>
#include <atomic>   		// atomic
#include <pthread.h>    // thread
#include <cstdlib>      // i need this
using namespace std;


//std::atomic<int> x;
int x;
std::atomic_flag lock;

void *function_1(void* id){
	long tid;
	tid = (long) id;
	
	for (int i=0; i< 1000; i++) {
		while (true)
			if (!lock.test_and_set()) {
				x++;
				lock.clear();
				break;
			}
  }
	cout << "this is thread " << tid << "-----";
	cout << "x is " << x << endl;;
  pthread_exit(NULL);
}


int main() {

	x = 0;
	lock.clear();

	int num_threads = 4;
	pthread_t threads[num_threads];

	int th_ret;
	long int i;

	for ( i=0; i<num_threads; i++) {
		cout << "main(): creating thread " << i << endl;
		th_ret = pthread_create(&threads[i], 0, function_1, (void *) i);
    if (th_ret){
       cout << "Error:unable to create thread," << i << endl;
       exit(-1);
    }
	}
  cout << "x is " << x << endl;
    
  pthread_exit(NULL);
}
