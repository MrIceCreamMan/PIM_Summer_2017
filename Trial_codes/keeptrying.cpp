#include <iostream>
using namespace std;

int main() {
	int i = 0;
	int j = 8;
	while (i<6){
		i++;
		j++;
		cout << "i=" << i << endl;
		while (true) {
			if (j%7 == 1){
				cout << "j ===== " << j << endl;
				break;
			}
			j++;
		}
	}
	return 0;
}
	
