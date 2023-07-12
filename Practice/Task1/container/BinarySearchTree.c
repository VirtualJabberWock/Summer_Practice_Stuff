#include "BinarySearchTree.h"
#include "..\core\String.h"


static BinaryTreeNode* CreateNode(BinaryTreeNode* parent) {

	BinaryTreeNode* node = calloc(1, sizeof(BinaryTreeNode));
	if (node == 0) return 0;
	node->key = 0;
	node->value = 0;
	node->parent = parent;
	return node;
}

static void emplace(BinaryTree* tree, Object* key, Object* value) {

	if (key == 0 || tree == 0 || value == 0) {
		return;
	}

	if (tree->rootNode == 0) {
		tree->rootNode = CreateNode(0);
		tree->rootNode->key = key;
		tree->rootNode->value = value;
		return;
	}

	BinaryTreeNode* node = tree->rootNode;
	BinaryTreeNode* prevNode = node;
	int lastCompare = 0;

	/*int leftTurns = 0;
	int rightTurns = 0;*/

	while (node != 0)
	{
		prevNode = node;
		int check = compareObjects(key, node->key);
		if (check == 0) {
			node->value = value;
			DestroyObject(&key);
			return;
		}
		lastCompare = (check >= 0);
		node = lastCompare ? node->right : node->left;
	}

	if (lastCompare) {
		if (prevNode->right == 0) {
			prevNode->right = CreateNode(prevNode);
		}
		prevNode->right->key = key;
		prevNode->right->value = value;
	}
	else {
		if (prevNode->left == 0) {
			prevNode->left = CreateNode(prevNode);
		}
		prevNode->left->key = key;
		prevNode->left->value = value;
	}
}

static Object* get(BinaryTree* tree, Object* key, BinaryTreeNode** outNode) {
	
	BinaryTreeNode* node = tree->rootNode;

	while (node != 0) {
		char check = compareObjects(key, node->key);
		if (check == 0) {
			break;
		}
		if (check > 0) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}

	if (node == 0) {
		if (outNode != 0) *outNode = 0;
		return 0;
	}
	else {
		if (outNode != 0) *outNode = node;
		return node->value;
	}
}

static Object* getByStr(BinaryTree* tree, const char* str, BinaryTreeNode** outNode) 
{
	String* temp = NewString(str);
	Object* obj = get(tree, temp, outNode);
	DestroyObject(&temp);
	return obj;
}

BinaryTreeVTABLE* BINARY_TREE_VTABLE[3] = {
	emplace, get, getByStr
};

BinaryTree* NewBinaryTree()
{
	BinaryTree* tree = calloc(1, sizeof(BinaryTree));
	if (tree == 0) return 0;
	tree->rootNode = 0;
	tree->_ = BINARY_TREE_VTABLE;
	return tree;
}

static inline void SetNodeInParent(BinaryTree* tree, BinaryTreeNode* node, BinaryTreeNode* to)
{
	if (node->parent == 0) {
		tree->rootNode = to;
		return;
	}

	BinaryTreeNode* parent = node->parent;

	if (parent->left == node) {
		free(parent->left);
		parent->left = to;
	}
	if (parent->right == node) {
		free(parent->right);
		parent->right = to;
	}
}

static inline void EraseNodeData(BinaryTreeNode* node) {
	DestroyObject(&node->key);
	DestroyObject(&node->value);
}

static inline int DeleteNodeWithOneLeaf(BinaryTree* tree, BinaryTreeNode* node)
{
	if (node->left == 0)
	{
		if (node->parent != 0) {
			SetNodeInParent(tree, node, node->right);
		}
		else {
			tree->rootNode = node->right;
		}
		EraseNodeData(node);
		free(node);
		return 1;
	}
	if (node->right == 0)
	{
		if (node->parent != 0) {
			SetNodeInParent(tree, node, node->left);
		}
		else {
			tree->rootNode = node->left;
		}
		EraseNodeData(node);
		free(node);
		return 1;
	}
	return 0;
}

static BinaryTreeNode* getMaxNodeInBranch(BinaryTreeNode* node) {
	if (node == 0) return 0;
	while (node->right != 0) {
		node = node->right;
	}
	return node;
}

static BinaryTreeNode* getMinNodeInBranch(BinaryTreeNode* node) {
	if (node == 0) return 0;
	while (node->left != 0) {
		node = node->left;
	}
	return node;
}

void DeleteNode(BinaryTree* tree, BinaryTreeNode* node)
{
	while (1) {
		if (node == 0) return;
		if (node->left == 0 && node->right == 0) { // Leaf
			EraseNodeData(node);
			SetNodeInParent(tree, node, 0);
			return;
		}
		if (DeleteNodeWithOneLeaf(tree, node)) {
			return;
		}
		BinaryTreeNode* mostRightInLeft = getMaxNodeInBranch(node->left);
		node->key = mostRightInLeft->key;
		node->value = mostRightInLeft->value;
		mostRightInLeft->key = 0;
		mostRightInLeft->value = 0;
		node = mostRightInLeft;
	}
}

void ClearNode(BinaryTreeNode* node) {
	if (node == 0) return;
	ClearNode(node->left);
	ClearNode(node->right);
	DestroyObject(&node->key);
	DestroyObject(&node->value);
	free(node);
}

void ClearTree(BinaryTree* tree)
{
	BinaryTreeNode* node = tree->rootNode;
	ClearNode(node);
}

