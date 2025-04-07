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

    void setKey(const Key& k){
      const_cast<Key&>(this -> item_.first) = k;
    }

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
    AVLNode<Key, Value>* leftrotation(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rightrotation(AVLNode<Key, Value>* node);


    // Add helper functions here


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this -> root_ == nullptr){
      this -> root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
    }
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this -> root_);
    AVLNode<Key, Value>* temp_parent = nullptr;
    while(temp != nullptr){
      temp_parent = temp;
      if(new_item.first == temp -> getKey()){
        temp -> setValue(new_item.second);
        return;
      }
      else if(new_item.first > temp -> getKey()){
        temp = temp -> getRight();
      }
      else{
        temp = temp -> getLeft();
      }
    }
    AVLNode<Key, Value>* to_insert = new AVLNode<Key, Value>(new_item.first, new_item.second, temp_parent);
    to_insert -> setBalance(0);
    if(new_item.first > temp_parent -> getKey()){
      temp_parent -> setRight(to_insert);
      temp_parent -> updateBalance(+1);
    }
    else{
      temp_parent -> setLeft(to_insert);
      temp_parent -> updateBalance(-1);
    }
    AVLNode<Key, Value>* child = to_insert;
    while(temp_parent != nullptr){
      int8_t balance = temp_parent -> getBalance();
      if(balance == 0){
        break;
      }
      if(balance == +2 || balance == -2){
        if(balance == -2){
          AVLNode<Key, Value>* lChild = temp_parent -> getLeft();
          if(lChild == nullptr){
            break;
          }
          if(lChild -> getBalance() == -1){
            rightrotation(temp_parent);
            temp_parent -> setBalance(0);
            lChild -> setBalance(0);
            break;
          }
          else if(lChild -> getBalance() == 0){
            rightrotation(temp_parent);
            temp_parent -> setBalance(0);
            lChild -> setBalance(0);
            break;
          } 
        else{
          AVLNode<Key, Value>* left_right = lChild -> getRight();
          leftrotation(lChild);
          rightrotation(temp_parent);
          if(left_right -> getBalance() == -1){
            temp_parent -> setBalance(0);
            lChild -> setBalance(+1);
          }
          else if(left_right -> getBalance() == 0){
            temp_parent -> setBalance(0);
            lChild -> setBalance(0);
          }
          else{
            temp_parent -> setBalance(-1);
            lChild -> setBalance(0);
          }
          left_right -> setBalance(0);
          break;
        }
      }
    else{
      AVLNode<Key, Value>* rChild = temp_parent -> getRight();
      if(rChild == nullptr){
        break;
      }
      if(rChild -> getBalance() == 1){
        leftrotation(temp_parent);
        temp_parent -> setBalance(0);
        rChild -> setBalance(0);
        break;
      }
      else if(rChild -> getBalance() == 0){
        leftrotation(temp_parent);
        temp_parent -> setBalance(0);
        rChild -> setBalance(0);
        break;
      }
      else{
        AVLNode<Key, Value>* right_left = rChild -> getLeft();
        rightrotation(rChild);
        leftrotation(temp_parent);
        if(right_left -> getBalance() == +1){
          temp_parent -> setBalance(0);
          rChild -> setBalance(-1);
        }
        else if(right_left -> getBalance() == 0){
          temp_parent -> setBalance(0);
          rChild -> setBalance(0);
        }
        else{
          temp_parent -> setBalance(+1);
          rChild -> setBalance(0);
        }
        rChild -> setBalance(0);
        break;
      }
    }
  }
      child = temp_parent;
      temp_parent = temp_parent -> getParent();
      if(temp_parent != nullptr){
        if(temp_parent -> getLeft() == child){
          temp_parent -> updateBalance(-1);
        }
        else{
          temp_parent -> updateBalance(+1);
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
    Node<Key, Value>* node_loc = this -> internalFind(key);
    if(node_loc == nullptr){
      return;
    }
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(node_loc);
    if(node -> getLeft() != nullptr && node -> getRight() != nullptr){
      Node<Key, Value>* pred_loc = BinarySearchTree<Key, Value>::predecessor(node);
      if(pred_loc != nullptr){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(pred_loc);
        AVLNode<Key, Value>* oldParent = pred -> getParent();
        bool sameParent = (oldParent == node);
        AVLNode<Key, Value>* oldLeft = pred -> getLeft();
        this -> nodeSwap(node, pred);
        if(!sameParent){
          if(oldLeft){
            oldLeft -> setParent(oldParent);
          }
          if(oldParent -> getRight() == node){
            oldParent -> setRight(oldLeft);
          }
          else{
            oldParent -> setLeft(oldLeft);
          }
        }
      }
    }
    AVLNode<Key, Value>* parent = node -> getParent();
    AVLNode<Key, Value>* child = nullptr;
    if (node -> getRight() != nullptr){
      child = node -> getRight();
    }
    else{
      child = node -> getLeft();
    }
    if(child != nullptr){
      child -> setParent(parent);
    }
    bool flag = false;
    if(parent == nullptr){
      this -> root_ = child;
    }
    else{
        if(parent -> getLeft() == node){
          parent -> setLeft(child);
          flag = true;
        }
        else{
          parent -> setRight(child);
          flag = false;
        }
      }
  delete node;
  AVLNode<Key, Value>* current = parent;
  while(current != nullptr){
    flag ? (current -> updateBalance(+1)) : (current -> updateBalance(-1));
    int8_t upd_bal = current -> getBalance();
    if(upd_bal == 2 || upd_bal == -2){
      if(upd_bal == 2){
        AVLNode<Key, Value>* rHeavy = current -> getRight();
        if(!rHeavy){
          break;
        }
        int8_t rBal = rHeavy -> getBalance();
        if(rBal == 1){
          leftrotation(current);
          current -> setBalance(0);
          rHeavy -> setBalance(0);
        }
        else if(rBal == 0){
          leftrotation(current);
          current -> setBalance(+1);
          rHeavy -> setBalance(-1);
        }
        else{
          AVLNode<Key,Value>* rlImbal = rHeavy -> getLeft();
          rightrotation(rHeavy);
          leftrotation(current);
          if(rlImbal -> getBalance() == +1){
            current -> setBalance(0);
            rHeavy -> setBalance(-1);
          }
          else if(rlImbal -> getBalance() == 0){
            current -> setBalance(0);
            rHeavy -> setBalance(0);
          }
          else{
            current -> setBalance(+1);
            rHeavy -> setBalance(0);
          }
          rlImbal -> setBalance(0);
        }
      }
      else{
        AVLNode<Key, Value>* lHeavy = current -> getLeft();
        if(!lHeavy){
          break;
        }
        int8_t lBal = lHeavy -> getBalance();
        if(lBal == -1){
          rightrotation(current);
          current -> setBalance(0);
          lHeavy -> setBalance(0);
        }
        else if(lBal == 0){
          rightrotation(current);
          current -> setBalance(-1);
          lHeavy -> setBalance(+1);
        }
        else{
          AVLNode<Key, Value>* lrImbal = lHeavy -> getRight();
          leftrotation(lHeavy);
          rightrotation(current);
          if(lrImbal -> getBalance() == -1){
            current -> setBalance(0);
            lHeavy -> setBalance(+1);
          }
          else if(lrImbal -> getBalance() == 0){
            current -> setBalance(0);
            lHeavy -> setBalance(0);
          }
          else{
            current -> setBalance(-1);
            lHeavy -> setBalance(0);
          }
          lrImbal -> setBalance(0);
        }
      }
    }
    AVLNode<Key, Value>* p_up = current -> getParent();
    if(p_up == nullptr){
      break;
    }
    current = p_up;
    flag = (p_up -> getLeft() == current);
  }
}


template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::leftrotation(AVLNode<Key, Value>* node){
  AVLNode<Key, Value>* rChild = node -> getRight();
  if(!rChild){
    return node;
  }
  node -> setRight(rChild->getLeft());
  if(rChild -> getLeft() != nullptr){
    rChild -> getLeft() -> setParent(node);
  }
  rChild -> setParent(node -> getParent());
  if(node -> getParent() == nullptr){
    this -> root_ = rChild;
  }
  else{
    if(node -> getParent() -> getLeft() == node){
      node -> getParent() -> setLeft(rChild);
    }
    else{
      node -> getParent() -> setRight(rChild);
    }
  }
  rChild -> setLeft(node);
  node -> setParent(rChild);
  return rChild;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rightrotation(AVLNode<Key, Value>* node){
  AVLNode<Key, Value>* lChild = node -> getLeft();
  if(!lChild){
    return node;
  }
  node -> setLeft(lChild -> getRight());
  if(lChild -> getRight() != nullptr){
    lChild -> getRight() -> setParent(node);
  }
  lChild -> setParent(node -> getParent());
  if(node -> getParent() == nullptr){
    this -> root_ = lChild;
  }
  else{
    if(node -> getParent() -> getRight() != node){
      node -> getParent() -> setLeft(lChild);
    }
    else{
      node -> getParent() -> setRight(lChild);
    }
  }
  lChild -> setRight(node);
  node -> setParent(lChild);
  return lChild;
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
