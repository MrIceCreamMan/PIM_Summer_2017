//Coarse
#ifndef CoarseLockLinkedList_h
#define CoarseLockLinkedList_h
#include <stdio.h>
#include <iostream>

#if 1
typedef unsigned long ulong_t;
#endif

using namespace std;

struct Node
{
    ulong_t data;
    Node *next;
    pthread_mutex_t list_lock;
};


//_________________ Linked List Template (Sorted) _______________________
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
    pthread_mutex_init(&node1->list_lock,NULL);
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
    pthread_mutex_unlock(&head->list_lock);
    pthread_mutex_destroy(&head->list_lock);
    while (head != tail){
        Node *temp = head;
        head = head->next;
        delete temp;
    }
    delete head;
}

//*******************    contain() function    ***********************
bool LinkedList::contains(ulong_t x){

    pthread_mutex_lock(&head->list_lock);
    Node *curr = head->next;

    while (curr != tail){
        if (x < curr->data) {
            pthread_mutex_unlock(&head->list_lock);
            return false;
        }
        if (curr->data==x) {
            pthread_mutex_unlock(&head->list_lock);
            return true;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&head->list_lock);
    return false;
}


//*******************    add() function    ***********************
bool LinkedList::add(ulong_t x){
    pthread_mutex_lock(&head->list_lock);

    Node *pred = head;
    Node *curr = head->next;

    while (curr != tail){
        if (curr->data > x)
            break;
        if (curr->data==x){
            pthread_mutex_unlock(&head->list_lock);
            return false;
        }
        pred = curr;
        curr = curr->next;
    }
    Node* node1 = new Node;
    node1->data = x;
    node1->next = curr;
    pred->next = node1;
    pthread_mutex_unlock(&head->list_lock);
    return true;
}


//*******************    remove() function    ***********************
bool LinkedList::remove(ulong_t x){
    pthread_mutex_lock(&head->list_lock);

    Node *pred = head;
    Node *curr = head->next;

    while (curr != tail){
        if (x < curr->data) {
            pthread_mutex_unlock(&head->list_lock);
            return false;
        }
        if (x == curr->data){
            pred->next = curr->next;
            delete curr;
            pthread_mutex_unlock(&head->list_lock);
            return true;
        }
        pred = curr;
        curr = curr->next;
    }
    pthread_mutex_unlock(&head->list_lock);
    return false;
}


//********************    print() function    ***********************
void LinkedList::print(){
    pthread_mutex_lock(&head->list_lock);
    Node *temp = head->next;
    cout << endl;
    while (temp != tail){
        cout << temp->data << " ";
        temp = temp->next;
    }
    if (head->next == tail)
        cout << "nothing yet";
    cout << endl;
    pthread_mutex_unlock(&head->list_lock);
    return;
}


#endif
