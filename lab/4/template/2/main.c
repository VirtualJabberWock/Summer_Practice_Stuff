#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef char * KeyType;
typedef char * ValueType;



typedef struct _TreeNode {

    KeyType key;
    ValueType value;
    struct _TreeNode *left;
    struct _TreeNode *right;

} TreeNode;


typedef struct _Tree {
    
    TreeNode *root;

} Tree;



void InsertTreeNode (Tree *tree, TreeNode *newNode) {

}



TreeNode * CreateAndInitTreeNode (KeyType key, ValueType value) {

TreeNode *node = (TreeNode*) malloc (sizeof(TreeNode));

    //node->key <- key
    //node->value <- value

    return node;
}



TreeNode * SearchValueByKey (Tree *tree, KeyType key) {


}


// buf["key1"] -> "value1"
ValueType GetValueByKey (Tree *tree, KeyType key) {

    //node = SearchValueByKey (root, key)
    //if (node) 
    //    return node->value;
    //else
    //  return NULL

}


void InsertTreeValueByKey (Tree *tree, KeyType key, ValueType value) {

    //node CreateAndInitTreeNode (key, value);
    //InsertTreeNode (root, node);

}


int main () {


    Tree tree;

    InsertTreeValueByKey (&tree, "some_key", "some_value");
    ValueType value = GetValueByKey (&tree, "some_key");


    return 0;
}
