#include <random>
#include <iostream>
#include <chrono>

#include <cstddef>
using namespace std;
template <typename Item>
class TList {
private:

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

  size_t size;
  
public:
  class iterator {
    ListNode *node;
  public:
    iterator(ListNode *n = nullptr) {
      node = n;
    }

    Item& getItem() { return node->item; }
    void next() { node = node->next; }
    bool end() { return node==nullptr; }

    friend class TList<Item>;
  };

  
public:
  TList() {
    // list is empty
    head = nullptr;
    tail = nullptr;
    size = 0;
  }

  ~TList() {
    // delete the list
    while(head != nullptr) {
      ListNode *n = head;
      head=head->next;
      delete n;
    }
  }
  
  size_t getSize() { return size; }
  
  bool empty() {
    return head==nullptr;
  }
  
  void append(Item a) {
    ListNode *node = new ListNode(a);
    size++;
    if ( head == nullptr ) {
      // empty list
      head = node;
      tail = node;
    } else {
      tail->next = node;
      tail = node;
    }
  }

  bool remove (Item &copy) {
    if (!empty()) {
      size--;
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

  iterator begin() {
    return iterator(head);
  }

  void insertAfter(iterator it, Item item)
  {
    size++;

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

  void removeAfter(iterator it, Item& item)
  {
    // emtpy list or at tail, just return
    if (it.node == tail) return;

    size--;
    
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
  
};

class HashTableChain {
protected:
  
  TList<int> *table;
  size_t maxChainLength;

  // Make sure size field always shows size of table
  size_t size;
  
  size_t hash(int key, int sz) { return ( (size_t)key * 997) % sz; }
  
  void reset(int new_size) {
     //cout<< "Table has been resized! Good job." << endl << "All chains are <= maxChainLength." << endl;
    TList<int> *newTable = new TList<int> [size];
    for(unsigned int i = 0; i < size; i++) {
       newTable[i] = table[i];
    }
    delete [] table;
    size = new_size;
    table = new TList<int>[size];
    for(unsigned int i = 0; i < size; i++) {
       table[i] = newTable[i];
    }
    delete [] newTable;
  }
  
public:
  HashTableChain(int htsize=1000, size_t maxChainL=10)
  {
    size = htsize;
    maxChainLength = maxChainL;

    table = new TList<int> [size];
  }

  ~HashTableChain()
  {
    // this will call the destructor on each list in the table
    delete [] table;
  }

  
  void insert(int data) {
    
    int index = hash(data,size);
    if(index > maxChainLength) {
        HashTableChain::reset(size * 2);
     }
    TList<int>::iterator ins;
    table[index].insertAfter(ins, data);
    
    // TODO: CHECK TO SEE IF RESIZE IS NEEDED AND IF SO CALL resize
  }

  //override keyword asks compiler to do extra checks
  bool find(int data) 
  {
    int index = hash(data,size);

    auto it = table[index].begin();
    while(!it.end())
      {
	if ( it.getItem() == data )
	  return true;
	it.next();
      }
    
    return false;
  }

  void remove(int data) 
  {
    int index = hash(data,size);
    TList<int>::iterator prev;
    auto it = table[index].begin();
    while(!it.end())
      {
	if (it.getItem()==data)
	  {
	    int rm;
	    table[index].removeAfter(prev,rm);
	    return;
	  }
	prev = it;
	it.next();
      }    
  }

  // DO NOT REMOVE OR MODIFY
  size_t getTableSize() { return size; }

  int getNumElements() {
    int num = 0;
    for(size_t i=0; i<getTableSize(); i++)
      {
	num += table[i].getSize();
      }
    return num;
  }

  bool checkChainLength() {
    //int num = 0;
    for(size_t i=0; i<getTableSize(); i++)
      {
	if (table[i].getSize() > maxChainLength)
	  return false;
      }
    return true;
  }
};

using namespace std::chrono;

int main()
{
  HashTableChain myset(10,3);

  size_t sz = myset.getTableSize();
  
  std::cout << "Hash table is " << myset.getTableSize()
	    << " entries long." << std::endl;

  // random number generation
  std::random_device generator;
  std::uniform_int_distribution<int> dist(0,1000000);

  for(size_t i=0; i<40; i++) {
    myset.insert(dist(generator));
  }

  if ( myset.getTableSize() > sz ) {
    std::cout << "Table has been resized! Good job." << std::endl;
  } else {
    std::cout << "Table was not resized. Did you to remember to update size field?" << std::endl;
  }

  if ( myset.checkChainLength() )
    {
      std::cout << "All chains are <= maxChainLength." << std::endl;
    }
  else
    {
      std::cout << "At least one chain is > maxChainLength." << std::endl;
    }
  
 return 0;
}
