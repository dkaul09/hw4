#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int depth(Node* root);


bool equalPaths(Node * root)
{
    // Add your code below  

    if (root == NULL) {
        return true;

    } 

		if (root->left == NULL && root->right == NULL) {
			return true;
		}


    int left_subtree_depth = depth(root->left);
    int right_subtree_depth = depth(root->right);  

		if (root->left == NULL && right_subtree_depth == 1) {
			return true;
		} else if (root->right == NULL && left_subtree_depth == 1) {
			return true;
		} else if (left_subtree_depth == right_subtree_depth) {
        return true;
    }   else {
        return false;
    }
}

int depth(Node * root) {
    if (root == NULL) {
        return 0;
    } 

    int left_depth = depth(root->left); 
    int right_depth = depth(root->right);

    if (left_depth > right_depth) {
        return left_depth + 1;
    } else {
         return right_depth + 1;
        }
}
