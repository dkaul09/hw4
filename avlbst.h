#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void insert_fix(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
		void remove_fix(AVLNode<Key,Value>* n, int8_t diff);
		void rotateLeft(AVLNode<Key,Value>* x);
		void rotateRight(AVLNode<Key,Value>* Z); 



};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* x){

	AVLNode<Key,Value>* parent = x->getParent(); 
	AVLNode<Key,Value>* y = x->getRight();
	AVLNode<Key,Value> * b = y->getLeft(); //children of y
  
	if(parent == nullptr) {
       this->root_ = y; 
			 x->setRight(nullptr);
       y->setParent(nullptr);
    } else {
        if (parent->getLeft() == x) {
            y->setParent(parent);
            parent->setLeft(y);
						x->setRight(nullptr);
        } else if (parent->getRight() == x) {
            y->setParent(parent);
            parent->setRight(y);
						x->setRight(nullptr);
        }
    }

    y->setLeft(x);
    x->setParent(y);

    if(b != nullptr) {
            x->setRight(b); 
            b->setParent(x);

        } else {
            x->setRight(nullptr);
        }


}
//go over rotate functions 
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* z){

  AVLNode<Key,Value>* parent = z->getParent(); 
	AVLNode<Key,Value>* y = z->getLeft();
	AVLNode<Key,Value>* c = y->getRight();


	if (parent == nullptr) {  //root
        this->root_ = y;
        y->setParent(nullptr);
				z->setLeft(nullptr);
    } else {
        if (parent->getRight() == z) {
            y->setParent(parent);
            parent->setRight(y);
						z->setParent(nullptr);
        } else if (parent->getLeft() == z) {
            y->setParent(parent);
            parent->setLeft(y);
						z->setParent(nullptr);
        }
       //using the slides for reference so should work hopefully
    }
		  y->setRight(z);
      z->setParent(y);
			if(c != nullptr) {
        z->setLeft(c); 
        c->setParent(z);
        } else {
           z->setLeft(nullptr);
        }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO 
		AVLNode <Key, Value> * tree_ptr = static_cast<AVLNode<Key, Value>*>(this->root_);
		if (this->root_== NULL) {
			 tree_ptr = new AVLNode<Key,Value>(new_item.first,new_item.second,NULL);
			 tree_ptr->setBalance(0);
			 this->root_ = tree_ptr;
			 return;
		}  else {
			  AVLNode<Key,Value> * node = NULL; 
				while (tree_ptr != NULL) {
					if (new_item.first == tree_ptr->getKey()) {
						tree_ptr->setValue(new_item.second);
						node = new AVLNode<Key,Value>(tree_ptr->getKey(),new_item.second,tree_ptr->getParent());
						break;
					} else if (new_item.first < tree_ptr->getKey()) {
						 if (tree_ptr->getLeft() == NULL) {
							  node = new AVLNode<Key,Value>(new_item.first,new_item.second,tree_ptr);
							  node->setBalance(0);
							  tree_ptr->setLeft(node);
                node->setParent(tree_ptr);
                break;
						 } else {
                      tree_ptr = tree_ptr->getLeft();
                }
					} else {
                  if (tree_ptr->getRight() == NULL) {
                      node = new AVLNode<Key,Value>(new_item.first,new_item.second,tree_ptr);
								      node->setBalance(0);
							        tree_ptr->setRight(node);
                      node->setParent(tree_ptr);
                      break;
                    } else {
                        tree_ptr = tree_ptr->getRight();
                      }
                  }
				 
				}
				
	      if (tree_ptr->getBalance() == -1 || tree_ptr->getBalance() == 1) {
		       tree_ptr->setBalance(0);
			     return;
	      } else if (tree_ptr->getBalance() == 0){
		       if(tree_ptr->getLeft() == node) {
				    tree_ptr->setBalance(-1);
					  insert_fix(tree_ptr,node);
			   } else if (tree_ptr->getRight() == node) {
				   tree_ptr->setBalance(+1);
					 insert_fix(tree_ptr,node);
			  }
		       
	   }
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node) {
	if (parent == NULL || parent->getParent() == NULL) {
		 return;
	}

  AVLNode <Key, Value> * grandparent_ptr = parent->getParent();

	if(grandparent_ptr->getLeft() == parent) {
		 grandparent_ptr->updateBalance(-1);
		 if (grandparent_ptr->getBalance() == 0) {
			 return;
		 } else if (grandparent_ptr->getBalance() == -1) {
			  insert_fix(grandparent_ptr,parent);
		 } else if (grandparent_ptr->getBalance() == -2) { 
			 if(parent->getLeft() == node){
				 rotateRight(grandparent_ptr);
				 parent->setBalance(0); 
				 grandparent_ptr->setBalance(0);
			 } else if(parent->getRight() == node) {
				  rotateLeft(parent);
				  rotateRight(grandparent_ptr);

				 if (node->getBalance() == -1) {
					 parent->setBalance(0);
					 grandparent_ptr->setBalance(1);
					 node->setBalance(0);

				 } else if(node->getBalance() == 0) {
					 parent->setBalance(0);
					 grandparent_ptr->setBalance(0);
					 node->setBalance(0);


				 } else if(node->getBalance() == 1) {
					 parent->setBalance(-1);
					 grandparent_ptr->setBalance(0);
					 node->setBalance(0);
				 }
			 }
		 }
	} else {
		 grandparent_ptr->updateBalance(1);
		 if (grandparent_ptr->getBalance() == 0) {
			 return;
	   } else if (grandparent_ptr->getBalance() == 1) {
			 insert_fix(grandparent_ptr,parent);
		 } else if (grandparent_ptr->getBalance() == 2)  { 
			 if(parent->getRight() == node){
				 rotateLeft(grandparent_ptr);
				 parent->setBalance(0); 
				 grandparent_ptr->setBalance(0);
		 } else if(parent->getLeft() == node) {

			   rotateRight(parent);
			   rotateLeft(grandparent_ptr);

				if (node->getBalance() == 1) {
					 parent->setBalance(0);
					 grandparent_ptr->setBalance(-1);
					 node->setBalance(0);

				 } else if(node->getBalance() == 0) {
					 parent->setBalance(0);
					 grandparent_ptr->setBalance(0);
					 node->setBalance(0);
				 } else if(node->getBalance() == -1) {
					 parent->setBalance(1);
					 grandparent_ptr->setBalance(0);
					 node->setBalance(0);
				 }

		 }
}
	}
}




/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
		AVLNode <Key, Value> * tree_ptr = static_cast<AVLNode<Key, Value> *>(BinarySearchTree<Key, Value>::root_); 
    while (tree_ptr != NULL)  {
            if (key == tree_ptr->getKey()) {
                break;
            } else if (key < tree_ptr->getKey()) { 
                    tree_ptr = tree_ptr->getLeft();
            } else if (key > tree_ptr->getKey()) {
                    tree_ptr = tree_ptr->getRight();  
            }
    }  

    if (tree_ptr == NULL) {
        return;
    } 

    if (tree_ptr->getLeft() != NULL && tree_ptr->getRight() != NULL) {
        AVLNode<Key,Value> * pred = static_cast<AVLNode<Key, Value> *>(BinarySearchTree<Key, Value>::predecessor(tree_ptr)); 
        nodeSwap(tree_ptr,pred);
    } 

    AVLNode<Key,Value> * parent = tree_ptr->getParent(); 

    int8_t difference;
		if(parent != NULL) {
			 if(parent->getLeft() == tree_ptr) {
				 //parent->updateBalance(1);
				 difference = 1;
			 } else if(parent->getRight() == tree_ptr) {
				 //parent->updateBalance(-1);
				 difference = -1;
			 }
		}

    if (tree_ptr->getLeft() == NULL && tree_ptr->getRight() == NULL) { // if it has no children
        if (tree_ptr == this->root_) {
            delete tree_ptr;
            tree_ptr = NULL;
            this->root_ = NULL;
        } else {
            AVLNode<Key,Value> * parent = tree_ptr->getParent(); 
            if (parent->getLeft() == tree_ptr) {
                parent->setLeft(NULL);
								delete tree_ptr;
                tree_ptr = nullptr;
            } else if (parent->getRight() == tree_ptr) {
                parent->setRight(NULL);
								delete tree_ptr;
                tree_ptr = nullptr;
            }
        }
    } else if (tree_ptr->getLeft() != NULL && tree_ptr->getRight() == NULL) {
             AVLNode<Key,Value> * child = tree_ptr->getLeft(); 
						 if (tree_ptr == this->root_) {
                this->root_ = child; 
                child->setParent(nullptr);
                delete tree_ptr;
            } else {
							AVLNode<Key,Value> * parent_of_node_deleted = tree_ptr->getParent();
							if (parent_of_node_deleted->getLeft() == tree_ptr){
								parent_of_node_deleted->setLeft(child);
								child->setParent(parent_of_node_deleted);
								delete tree_ptr;
							} else {
								parent_of_node_deleted->setRight(child);
								child->setParent(parent_of_node_deleted);
								delete tree_ptr;
								}
						}
							 
						
      } else if (tree_ptr->getLeft() == NULL && tree_ptr->getRight() != NULL) {
             AVLNode<Key,Value> * child = tree_ptr->getRight(); 
						 if (tree_ptr ==  BinarySearchTree<Key, Value>::root_ ) {
                BinarySearchTree<Key, Value>::root_  = child; 
                child->setParent(nullptr);
                delete tree_ptr;
							} else { 
								AVLNode<Key,Value> * parent_of_node_deleted = tree_ptr->getParent();
								if (parent_of_node_deleted->getRight() == tree_ptr){
									parent_of_node_deleted->setRight(child);
									child->setParent(parent_of_node_deleted);
									delete tree_ptr;
								} else {
									parent_of_node_deleted->setLeft(child);
									child->setParent(parent_of_node_deleted);
									delete tree_ptr;
								}
						}
      }
  remove_fix(parent,difference);
}

template<class Key, class Value> 
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>* n, int8_t diff) {
	 if (n == NULL) {
		 return;
	 }

	 AVLNode<Key,Value> * parent =  n->getParent();
	 int8_t next_diff;

	 if (parent != NULL) {
		 if (parent->getLeft() == n) {
			 next_diff = 1;
		 } else if (parent->getRight() == n) {
			 next_diff = -1;
		 }
	 }

	 if (diff == -1) {
		 if (n->getBalance() + diff == -2 ) {
			 AVLNode<Key,Value> * c =  n->getLeft(); 

			 if(c->getBalance() == -1){
				 rotateRight(n);
				 n->setBalance(0);
				 c->setBalance(0);
				 remove_fix(parent,next_diff);
			 } else if(c->getBalance() == 0) {
				 rotateRight(n);
				 n->setBalance(-1);
				 c->setBalance(1);
			 } else if (c->getBalance() == 1) {
				 AVLNode<Key,Value> * g =  c->getRight();
				 rotateLeft(c);
				 rotateRight(n);

				if (g->getBalance()== 1) {
					n->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				} else if(g->getBalance() == 0) {
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				} else if (g->getBalance() == -1) {
					n->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);

				}
				remove_fix(parent,next_diff);

			 }
     } else if (n->getBalance() + diff == -1) {
			  n->setBalance(-1);
				return;
		 } else if(n->getBalance() +  diff == 0) {
			  n->setBalance(0);
			  remove_fix(parent,next_diff);
		 }
	 } else if(diff == 1) {
		 if (n->getBalance() + diff == 2) {
			  AVLNode<Key,Value> * c =  n->getRight(); 
				if (c->getBalance() == 1)  {
					rotateLeft(n);
					n->setBalance(0);
				  c->setBalance(0);
				  remove_fix(parent,next_diff);
				} else if(c->getBalance() == 0) {
					rotateLeft(n);
				  n->setBalance(1);
				  c->setBalance(-1);
				} else if (c->getBalance() == -1) {
					AVLNode<Key,Value> * g =  c->getLeft();
				  rotateRight(c);
				  rotateLeft(n);

				if (g->getBalance()== -1) {
					n->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				} else if(g->getBalance() == 0) {
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				} else if (g->getBalance() == 1) {
					n->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);

				}
				remove_fix(parent,next_diff);



				}

		 } else if(n->getBalance() +  diff == 1) {
			 n->setBalance(1);
			 return;
		 } else if(n->getBalance() +  diff == 0) {
			  n->setBalance(0);
			  remove_fix(parent,next_diff);
		 }
	 }

	 }






template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
