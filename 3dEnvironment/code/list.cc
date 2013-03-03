#include "list.h"


List::List()
{
  last=first=0;
  count=0;  
}

List::~List()
{

}

//add an item to a doubly linked list
void List::add_item(Object *item,int index)
{
  node *thisnode;

  /* first node does not exist */
  if(!first)
    {
      thisnode=new node;
      thisnode->prev=0; 
      thisnode->next=0;
      thisnode->bucket=item;
	  thisnode->index=index;
      last=first=thisnode;
    }
  else
    {
      thisnode=last->next=new node;
      thisnode->prev=last;
      thisnode->next=0;
      thisnode->bucket=item;
	  thisnode->index=index;
      last=thisnode;
    }
  count++;
}

Object *List::get_item(int x)
{
	node *thisitem=first;
	while(thisitem)
	{
		if(thisitem->index==x)
		break;
		thisitem=thisitem->next;
    }
	return thisitem->bucket;
}

void List::DeleteAllItems()
{
	node *thisitem=first;
	while(thisitem)
	{
		delete thisitem->bucket;
		node *anode=thisitem->next;
		delete thisitem;
		thisitem=anode;
    }

}

