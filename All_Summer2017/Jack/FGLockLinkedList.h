//Fine Grained Lock
#ifndef FGLockLinkedList_h
#define FGLockLinkedList_h
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
    pthread_mutex_t node_lock;
};

//_________________ Fine Grained Linked List  (Sorted) _______________________
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
	pthread_mutex_init(&node1->node_lock,NULL);
    Node* node2 = new Node;
    node2->data = UINT32_MAX;
    node2->next = NULL;
	pthread_mutex_init(&node1->node_lock,NULL);
    tail = node2;
    node1->next = tail;
    head = node1;
}

//**********************    destructor     **************************
LinkedList::~LinkedList()
{
    while (head != tail){
        Node *temp = head;
        head = head->next;
		pthread_mutex_unlock(&temp->node_lock);
		pthread_mutex_destroy(&temp->node_lock);
        delete temp;
    }
	pthread_mutex_unlock(&head->node_lock);
	pthread_mutex_destroy(&head->node_lock);
    delete head;
}

//*******************    contain() function    ***********************
bool LinkedList::contains(ulong_t x){

    Node *pred = head;
    Node *curr = head;
    pthread_mutex_lock(&curr->node_lock);
    curr = curr->next;
    pthread_mutex_lock(&curr->node_lock);
    pthread_mutex_unlock(&pred->node_lock);

    while (curr != tail){
        if (x < curr->data) {
            pthread_mutex_unlock(&curr->node_lock);
            return false;
        }
        if (curr->data==x) {
            pthread_mutex_unlock(&curr->node_lock);
            return true;
        }
        pred = curr;
        curr = curr->next;
        pthread_mutex_lock(&curr->node_lock);
        pthread_mutex_unlock(&pred->node_lock);
    }
    pthread_mutex_unlock(&curr->node_lock);
    return false;
}


//*******************    add() function    ***********************
bool LinkedList::add(ulong_t x){

    Node *pred = head;
    pthread_mutex_lock(&pred->node_lock);
    Node *curr = head->next;
    pthread_mutex_lock(&curr->node_lock);

    while (curr != tail){
        if (x < curr->data)
            break;
        if (x == curr->data){
            pthread_mutex_unlock(&pred->node_lock);
            pthread_mutex_unlock(&curr->node_lock);
            return false;
        }
        pthread_mutex_unlock(&pred->node_lock);
        pred = curr;
        curr = curr->next;
        pthread_mutex_lock(&curr->node_lock);
    }
    Node* node1 = new Node;
    node1->data = x;
    node1->next = curr;
	pthread_mutex_init(&node1->node_lock,NULL);
    pred->next = node1;
    pthread_mutex_unlock(&pred->node_lock);
    pthread_mutex_unlock(&curr->node_lock);
    return true;
}


//*******************    remove() function    ***********************
bool LinkedList::remove(ulong_t x){

    Node *pred = head;
    pthread_mutex_lock(&pred->node_lock);
    Node *curr = head->next;
    pthread_mutex_lock(&curr->node_lock);

    while (curr != tail){
        if (x < curr->data) {
            pthread_mutex_unlock(&pred->node_lock);
            pthread_mutex_unlock(&curr->node_lock);
            return false;
        }
        if (x == curr->data){
            pred->next = curr->next;
            pthread_mutex_unlock(&curr->node_lock);
			pthread_mutex_destroy(&curr->node_lock);
            delete curr;
            pthread_mutex_unlock(&pred->node_lock);
            return true;
        }
        pthread_mutex_unlock(&pred->node_lock);
        pred = curr;
        curr = curr->next;
        pthread_mutex_lock(&curr->node_lock);
    }
    pthread_mutex_unlock(&pred->node_lock);
    pthread_mutex_unlock(&curr->node_lock);
    return false;
}


//********************    print() function    ***********************
void LinkedList::print(){
    pthread_mutex_lock(&head->node_lock);
    Node *temp = head->next;
    cout << endl;
    while (temp != tail){
        cout << temp->data << " ";
        temp = temp->next;
    }
    if (head->next == tail)
        cout << "nothing yet";
    cout << endl;
    pthread_mutex_unlock(&head->node_lock);
    return;
}


#endif
