#ifndef LockLinkedList_h
#define LockLinkedList_h
#include <stdio.h>
#include <iostream>
#include <atomic>           // atomic

using namespace std;

template <class T>
struct Node
{
    T data;
    Node *next;
    std::atomic_flag lock;
};


//_________________ Linked List Template (Sorted) _______________________
template <class T>
class LinkedList{

public:
  LinkedList();       // Constructor: Inits an empty singly-linked list
  ~LinkedList();      // Destructor: Destroys the singly-linked list 
  int size();         // Returns the size of the singly-linked list
  bool contains(T x); // Returns true if x is in the list, otherwise returns false
  bool add(T x);      // Returns true when successfully added, otherwise returns false
  bool remove(T x);   // Returns true when successfully removed, otherwise returns false
  void print();       // Prints the content of the singly-linked list

private:
  Node<T> *head;      // Pointer to head of singly-linked list
	std::atomic_flag head_lock;
  int listSize;       // Size of singly-linked list
};

//**********************    constructor    **************************
template <class T>
LinkedList<T>::LinkedList()
{
  listSize = 0;
  head = NULL;
	head_lock.clear();
}

//**********************    destructor     **************************
template <class T>
LinkedList<T>::~LinkedList()
{
  int i = 0;
  while (i<listSize){
    Node<T> *temp = head;
    head = head->next;
    delete temp;
    i++;
  }
}

//*******************    size() function      ************************
template <class T>
int LinkedList<T>::size(){
  return listSize;
}


//*******************    contain() function    ***********************
template <class T>
bool LinkedList<T>::contains(T x){
	while (true)
		if (!head_lock.test_and_set()){
			if (listSize==0){
				head_lock.clear();
				return false;
			}
  		Node<T> *pred = head;
  		Node<T> *curr = head;
			while (true)
				if (!curr->lock.test_and_set())
					break;				
			head_lock.clear();
			int i = 0;

			while (i<listSize){
				if (curr->data > x) {
					curr->lock.clear();
				  return false;
				}
				if (curr->data==x) {
					curr->lock.clear();
				  return true;
				}
    		pred = curr;
				if (curr->next != NULL) {
					curr = curr->next;
				  while (true)
				  	if (!curr->lock.test_and_set())
				  		break;
				}
				pred->lock.clear();
				i++;
			}
			return false;
		}
}


//*******************    add() function    ***********************
template <class T>
bool LinkedList<T>::add(T x){
	while (true)
		if (!head_lock.test_and_set()){
			//cout << " hello " << endl;
  		if (listSize==0){								// first node to be added
    		Node<T>* node1 = new Node<T>;
   	 		node1->lock.clear();
    		node1->data = x;
    		node1->next = NULL;
    		head = node1;
    		listSize = 1;
				head_lock.clear();
    		return true;
  		}  		
  		if (x == head->data){				    // no need to add
				head_lock.clear();
    		return false;
  		}
  		if (x < head->data) {						// replace head with x
    		Node<T>* node1 = new Node<T>;
   		  node1->lock.clear();
    		node1->data = x;
    		node1->next = head;
    		head = node1;
    		listSize++;
				head_lock.clear();
    		return true;
  		}
  		Node<T> *pred = head;
			while (true)
				if (!pred->lock.test_and_set())
					break;
  		Node<T> *curr = head->next;
			if (curr != NULL)
				while (true)
					if (!curr->lock.test_and_set())
						break;

			head_lock.clear();
  		int i = 1;
  		while (i<listSize){
    		if (curr->data > x)
      		break;
    		if (curr->data==x){
					pred->lock.clear();
					curr->lock.clear();
      		return false;
				}
				pred->lock.clear();
    		pred = curr;
				curr = curr->next;
				if (curr != NULL)
				  while (true)
				  	if (!curr->lock.test_and_set())
				  		break;
    		i++;
  		}
  		Node<T>* node1 = new Node<T>;
  		node1->lock.clear();
  		node1->data = x;
  		node1->next = curr;
  		pred->next = node1;
  		listSize++;

			pred->lock.clear();
			if (curr != NULL)
				curr->lock.clear();
  		return true;
		}
}


//*******************    remove() function    ***********************
template <class T>
bool LinkedList<T>::remove(T x){
	while (true)
		if (!head_lock.test_and_set()){
			if (listSize == 0){
				head_lock.clear();
				return false;
			}
			Node<T> *pred = head;
			Node<T> *curr = head;
			while (true)
				if (!curr->lock.test_and_set())
					break;				

			if (head->data == x){
				if (head->next != NULL) {
				  while (true)
				  	if (!head->next->lock.test_and_set())
				  		break;
				}
				head = head->next;
				delete pred;
				listSize--;
				if (head != NULL)
					head->lock.clear();
				head_lock.clear();
				return true;
			}

			head_lock.clear();
			if (curr->next != NULL)
				while (true)
					if (!curr->next->lock.test_and_set())
						break;
			else {
				curr->lock.clear();
				return false;
			}
			curr = curr->next;

			int i = 1;
			while (i<listSize){
				if (curr->data > x) {
					pred->lock.clear();
					curr->lock.clear();
				  return false;
				}
				if (curr->data == x){
					if (curr->next != NULL)
						while (true)
							if (!curr->next->lock.test_and_set())
								break;
				  pred->next = curr->next;
				  delete curr;
				  listSize--;
					pred->lock.clear();
					if (pred->next!=NULL)
						pred->next->lock.clear();
				  return true;
				}
				pred->lock.clear();
				pred = curr;
				if (curr->next != NULL) {
					curr = curr->next;
				  while (true)
				  	if (!curr->lock.test_and_set())
				  		break;
				}
				i++;
			}
			curr->lock.clear();
			return false;
		}
}


//********************    print() function    ***********************
template <class T>
void LinkedList<T>::print(){
  Node<T> *temp = head;
  int i = 0;
  cout << endl;
  while (i<listSize){
    cout << temp->data << " ";
    temp = temp->next;
    i++;
  }
  if (listSize==0)
    cout << "nothing yet";
  cout << endl;
}


#endif
