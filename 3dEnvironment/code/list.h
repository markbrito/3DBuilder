#ifndef LIST_H
#define LIST_H

class Object{};

//An item in a list (doubly linked list)
class node
{
public:
	node *prev;
	node *next;
	int index;
	Object *bucket;
};

//Represents a list of items by pointing to the first and last
//item in the list and the item count.  All items are accessable
//because each item points to an item before and after it thereby
//creating a chain of items. (doubly linked list)
class List
{
protected:
	int count;
	node *first;
	node *last;
public:
	void DeleteAllItems();
	List();
	~List();
	void add_item(Object *item,int index);
	Object *get_item(int x);
	int get_count(){return count;}
};

#endif


