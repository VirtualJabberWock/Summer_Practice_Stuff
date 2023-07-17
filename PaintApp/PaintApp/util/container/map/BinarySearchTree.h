#pragma once

#include "..\Map.h"

typedef struct tagBinaryTreeNode {

	struct tagBinaryTreeNode* parent;

	struct tagBinaryTreeNode* left;
	struct tagBinaryTreeNode* right;

	Object* key;
	Object* value;

} BinaryTreeNode;


typedef struct tagBinaryTree { 

	EXTENDS_MAP;

	BinaryTreeNode* rootNode;

} BinaryTree;

BinaryTree* NewBinaryTree();

void DeleteNode(BinaryTree* tree, BinaryTreeNode* node);
void ClearTree(BinaryTree* tree);
void PrintBranch(BinaryTreeNode* tree, objectInteractFunc printFunc);
