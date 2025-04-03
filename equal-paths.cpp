#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr){
      return true;
    }
    int leafDepth = -1;
    return checkPath(root, 0, leafDepth);
}

bool checkPath(Node* root, int depth, int& leafDepth){
  if(root == nullptr){
    return true;
  }
  if(root -> left == nullptr && root -> right == nullptr){
    if(leafDepth == -1){
      leafDepth = depth;
      return true;
    }
    return (depth == leafDepth);
  }
  bool leftChild = true, rightChild = true;
  if(root -> left){
    leftChild = checkPath(root -> left, depth + 1, leafDepth);
  }
  if(root -> right){
    rightChild = checkPath(root -> right, depth + 1, leafDepth);
  }
  return (leftChild && rightChild);
}

