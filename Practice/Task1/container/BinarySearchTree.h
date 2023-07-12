#pragma once

#include "..\core\Object.h"

typedef struct tagBinaryTreeNode {

	struct tagBinaryTreeNode* parent;

	struct tagBinaryTreeNode* left;
	struct tagBinaryTreeNode* right;

	Object* key;
	Object* value;

} BinaryTreeNode;

typedef struct tagBinaryTreeVTABLE {

	void (*emplace)(struct tagBinaryTree* tree, Object* key, Object* value);
	Object* (*get)(struct tagBinaryTree* tree, Object* key, BinaryTreeNode** outNode);
	Object* (*getByStr)(struct tagBinaryTree* tree, const char* key, BinaryTreeNode** outNode);

} BinaryTreeVTABLE;

typedef struct tagBinaryTree { 

	BinaryTreeVTABLE* _;
	BinaryTreeNode* rootNode;

} BinaryTree;

BinaryTree* NewBinaryTree();

void DeleteNode(BinaryTree* tree, BinaryTreeNode* node);
__declspec(dllexport) void ClearTree(BinaryTree* tree);