#include <iostream>
#include "FGLockLinkedList.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    LinkedList intList;
    
    cout << "Inserting: 5, 20, 7, -2, 50, -50\n";
    intList.add(5);
    intList.add(20);
    intList.add(7);
    intList.add(-2);
    intList.add(50);
    intList.add(-50);
    
    //cout << "The size of my list is " << intList.size() << endl;
    cout << "This is the content of my list: ";
    intList.print();
    
    cout << "Is 20 is in my list? " << (intList.contains(20) ? "yes" : "no") << endl;
    cout << "Is 10 is in my list? " << (intList.contains(10) ? "yes" : "no") << endl;
    cout << "Deleting: -50\n";
    intList.remove(-50);
    
    cout << "This is the content of my list: ";
    intList.print();
#if 0
    cout << "=================================\n";
    LinkedList charList;
    
    cout << "Inserting: 'B', 't', 'V', '2', 'Z'\n";
    charList.add('B');
    charList.add('t');
    charList.add('V');
    charList.add('2');
    charList.add('Z');
    
    cout << "The size of my list is " << charList.size() << endl;
    cout << "This is the content of my list: ";
    charList.print();
    
    cout << "Is 'V' is in my list? " << (charList.contains('V') ? "yes" : "no") << endl;
    cout << "Is 'v' is in my list? " << (charList.contains('v') ? "yes" : "no") << endl;
    cout << "Deleting: 'Z'\n";
    charList.remove('Z');
    
    cout << "This is the content of my list: ";
    charList.print();
#endif

    return 0;
}
