#include <iostream>
#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <stack>

using Item = std::string;

class List {
private:

  // YOU MAY NOT CHANGE ListNode to have a previous pointer or any other new field
  class ListNode {
  public:
    Item item;
    ListNode * next;
    ListNode(Item i, ListNode *n=nullptr) {
      item = i;
      next = n;
    }
  };
  
  ListNode * head;
  ListNode * tail;
  
public:
  class iterator {
    // IMPLEMENT: Add additional member function here, as needed!

    // HINT: you can make a stack right here!
    //std::
    class stack {
       std::stack<ListNode *> temp;
    public:
      stack() {
      }
      ~stack() {
      }            
      void push(ListNode *tmp) {
         temp.push(tmp);
      }
      ListNode * pop() {
         ListNode *returned = temp.top();
	      temp.pop();
	      return returned;
      }
      bool empty() {
         return temp.empty();
      }
    };

    stack s;
    
    ListNode *node;
  public:
    // YOU MAY NOT CHANGE THE CONSTRUCTOR
    iterator(ListNode *n = nullptr) {
      node = n;
    }
    Item& getItem() { return node->item; }


    // YOU NEED TO CHANGE THIS FUNCTION; push current node onto stack
    void next() {
      // IMPLEMENT here:
      s.push(node);
      node = node->next;
    }

    bool end() { return node==nullptr; }

    bool prev()
    {
      // IMPLEMENT:

      // 1. try to go back one node using a stack, and if possible, update node and return true
      if (!s.empty()) {
         node = s.pop();
         return true;
      }
      // 2. if there is no previous, return false, and do not update node
      return false;
    }
    
    friend class List;
  };

public:
  List() {
    // list is empty
    head = nullptr;
    tail = nullptr;
  }

  List(const List &other)
  {
    iterator o = other.begin();
    while(!o.end())
      {
	append(o.getItem());	
	o.next();
      }
  }

  bool empty() {
    return head==nullptr;
  }
  
  // Only declared, here, implemented
  // in List.cpp
  void append(Item a);
  bool remove (Item &copy);

  void insertAfter(iterator, Item);
  void removeAfter(iterator, Item&);

  iterator begin() const {
    return iterator(head);
  }
};


void List::append(Item a) {
  ListNode *node = new ListNode(a);
  if ( head == nullptr ) {
    // empty list
    head = node;
    tail = node;
  } else {
    tail->next = node;
    tail = node;
  }
}

bool List::remove(Item &copy)
{
  if (!empty()) {
    copy = head->item;
    ListNode *tmp = head->next;
    delete head;
    head = tmp;
    if (head==nullptr)
      tail = nullptr;
    return true;
  }
  return false;
}

void List::insertAfter(iterator it, Item item)
{
  if (it.node == nullptr)
    {
      // insert at head
      ListNode *tmp = new ListNode(item,head);
      // if list is empty, set tail to new node
      if (tail==nullptr) {
	tail = tmp;
      }
      // set head to new node
      head = tmp;
    }
  else
    {
      ListNode *tmp = new ListNode(item,it.node->next);	
      it.node->next = tmp;
      // could be a new tail, if so update tail
      if (tail==it.node) {
	tail = tmp;
      }     
    }
}

void List::removeAfter(iterator it, Item& item)
{
  // emtpy list or at tail, just return
  if (it.node == tail) return;
    
  if (it.node == nullptr)
    {
      ListNode * tmp = head;
      head = head->next;
      delete tmp;
      if (head==nullptr)
	tail = nullptr;
    }
  else
    {
      ListNode *tmp = it.node->next;
      it.node->next = tmp->next;
      delete tmp;
      // could be that it.node is the new nullptr
      if (it.node->next == nullptr)
	tail = it.node;
    }
}

void printList(List &l) {
  List::iterator i = l.begin();
  while(!i.end()) {
    std::cout << i.getItem() << ", ";    
    i.next();
  }
  std::cout << std::endl;
}

int main()
{
  // DO NOT MODIFY MAIN IN YOUR FINAL SUBMISSION
  List l;
  l.append("one");
  l.append("two");
  l.append("three");
  l.append("four");

  std::cout << "List in order: ";
  printList(l);