//My name is Zhuohao Yang,
//My BU ID is U01579972

#ifndef LinkedList_h
#define LinkedList_h
#include <stdio.h>
#include <iostream>

using namespace std;

template <class T>
struct Node
{
    T data;
    Node *next;
};

template <class T>
class LinkedList{

public:
  LinkedList();       // Constructor: Inits an empty singly-linked list
  ~LinkedList();      // Destructor: Destroys the singly-linked list 
  int size();         // Returns the size of the singly-linked list
  bool contains(T x);  // Returns true if x is in the list, otherwise returns false
  bool add(T x);    // Adds x if not already in the list and returns true, otherwise returns false
  bool remove(T x); // Removes x if in the list and returns true, otherwise returns false
  void print();       // Prints the content of the singly-linked list

private:
  Node<T> *head;         // Pointer to head of singly-linked list
  int listSize;       // Size of singly-linked list
};




template <class T>
LinkedList<T>::LinkedList()
{
  listSize = 0;
  head = NULL;
}

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
  // cout << "LinkedList destructed." << endl;
}

template <class T>
int LinkedList<T>::size(){
  return listSize;
}

template <class T>
bool LinkedList<T>::contains(T x){
  Node<T> *temp = head;
  int i = 0;
  if (listSize==0){
    return false;
  }
  while (i<listSize){
    if (temp->data==x)
      return true;
    temp = temp->next;
    i++;
  }
  return false;
}

template <class T>
bool LinkedList<T>::add(T x){
  if (listSize==0){
    Node<T>* node1 = new Node<T>;
    node1->data = x;
    node1->next = NULL;
    head = node1;
    listSize = 1;
    return true;
  }
  if (this->contains(x)){
    cout << x << " is not added because it is already in the list" << endl;
    return false;
  }
  Node<T>* node_more = new Node<T>;
  node_more->data = x;
  node_more->next = NULL;
  Node<T>* temp = head;
  int i = 0;
  for (;i<listSize-1;i++){
    temp = temp->next;
  }
  temp->next = node_more;
  listSize++;
  return true;
}

template <class T>
bool LinkedList<T>::remove(T x){
  if (listSize == 0)
    return false;
  Node<T> *temp = head;
  if (head->data == x){
    head = head->next;
    delete temp;
    listSize--;
    return true;
  }
  int i = 0;
  while (i<listSize-1){
    if (x == temp->next->data){
      Node<T> *temp2 = temp->next;
      temp->next = temp->next->next;
      delete temp2;
      listSize--;
      return true;
    }
    temp = temp->next;
    i++;
  }
  return false;
}

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
