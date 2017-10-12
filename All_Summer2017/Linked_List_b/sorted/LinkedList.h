// sorted list
#ifndef LockLinkedList_h
#define LockLinkedList_h
#include <stdio.h>
#include <iostream>
#if 1
typedef unsigned long ulong_t;
#define UINT32_MAX 0xffffffff
#endif

using namespace std;

struct Node
{
    ulong_t data;
    Node *next;
};

class LinkedList{
public:
    LinkedList();             // Constructor: Inits an empty singly-linked list
    ~LinkedList();            // Destructor: Destroys the singly-linked list
    bool contains(ulong_t x); // Returns true if x is in the list, otherwise returns false
    bool add(ulong_t x);      // Returns true when successfully added, otherwise returns false
    bool remove(ulong_t x);   // Returns true when successfully removed, otherwise returns false
    void print();             // Prints the content of the singly-linked list

private:
    Node *head;
    Node *tail;
};

//**********************    constructor    **************************
LinkedList::LinkedList()
{
    Node* node1 = new Node;
    node1->data = 0;
    Node* node2 = new Node;
    node2->data = UINT32_MAX;
    node2->next = NULL;
    tail = node2;
    node1->next = tail;
    head = node1;
}

//**********************    destructor     **************************
LinkedList::~LinkedList()
{
    while (head->next != tail){
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}


//*******************    contain() function    ***********************
bool LinkedList::contains(ulong_t x){
    Node *curr = head->next;

    while (curr != tail){
        if (x < curr->data) {
            return false;
        }
        if (curr->data==x) {
            return true;
        }
        curr = curr->next;
    }
    //cout << "should not get here in contains() LinkedLisk.h " << endl;
    return false;
}


//*******************    add() function    ***********************
bool LinkedList::add(ulong_t x){
    if (x == head->data) {
        return false;
    }
    if (x < head->data) {
        cout << "x is less than the head, error!!!" << endl;
        return true;
    }
    Node *pred = head;
    Node *curr = head->next;

    while (curr != tail){
        if (curr->data > x)
            break;
        if (curr->data==x){
            return false;
        }
        pred = curr;
        curr = curr->next;
    }
    Node* node1 = new Node;
    node1->data = x;
    node1->next = curr;
    pred->next = node1;
    return true;
}


//*******************    remove() function    ***********************
bool LinkedList::remove(ulong_t x){    
    // head cannot be removed
    if (head->data == x){
        return false;
    }
    Node *pred = head;
    Node *curr = head->next;    

    while (curr != tail){
        if (x < curr->data) {
          return false;
        }
        if (curr->data == x){
          pred->next = curr->next;
          delete curr;
          return true;
        }
        pred = curr;
        curr = curr->next;
    }
    return false;
}


//********************    print() function    ***********************
void LinkedList::print(){
    Node *temp = head->next;
    cout << endl;
    while (temp != tail){
        cout << temp->data << " ";
        temp = temp->next;
    }
    if (head->next == tail)
        cout << "nothing yet";
    cout << endl;
	return;
}

#endif
