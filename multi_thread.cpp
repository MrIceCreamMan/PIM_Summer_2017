#include <iostream>
#include <thread>
#include <string>
using namespace std;

void function_1(){
    cout << "Beauty is only skin-deep" << endl;
}

class Fctor {
public:
    void operator()(string& msg) {
        cout << "t1 says: " << msg << endl;
    }
};

int main() {
    string s = "Where there is no trust, there is no love";
   // std::thread t1((Fctor()),s);
    //thread t1(function_1_;

    try {
        cout << "from main: " << s << endl;
    } catch (...) {
        //t1.join();
        throw;
    }

    //if (t1.joinable())
        //t1.join();
    
    return 0;
}
