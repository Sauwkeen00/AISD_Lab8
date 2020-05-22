#include "BinaryTree.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

BinaryTree::BinaryTree()
{
	root = NULL;
}

BinaryTree::~BinaryTree()
{
	removeTree(root);
}
Node *BinaryTree::getRoot()
{
	return root;
}
Node *BinaryTree::minimum(Node *node)
{
	if (node->getLeft() == NULL)
		return node;

	return minimum(node->getLeft());
}
// добавление узла
Node *BinaryTree::addNode(Node *node, Node* parent, double value)
{
	if (node == NULL)
	{
		node = new Node(value, parent, NULL, NULL);
	}
	else
	{
		parent = node;

		if (value < node->getValue())
			node->setLeft(addNode(node->getLeft(), node, value));

		if (value > node->getValue())
			node->setRight(addNode(node->getRight(), node, value));
	}

	return root = node;
}
// удаление из дерева узла
Node *BinaryTree::removeNode(Node *node, double key)
{
	if (node == NULL)
		return node;

	if (key < node->getValue())
		node->setLeft(removeNode(node->getLeft(), key));
	else
		if (key > node->getValue())
			node->setRight(removeNode(node->getRight(), key));
		else
			if (node->getLeft() != NULL && node->getRight() != NULL)
			{
				Node *pmin = minimum(node->getRight());
				node->setValue(pmin->getValue());
				
				node->setRight(removeNode(node->getRight(), node->getValue()));
			}
			else
				if (node->getLeft() != NULL)
				{
					Node *temp = node;
					node = temp->getLeft();
					if (temp->getLeft() != NULL)
						node->setParent(temp->getParent());
					delete temp;
				}
				else
				{
					Node *temp = node;
					node = temp->getRight();
					if (temp->getRight() != NULL)
						node->setParent(temp->getParent());
					delete temp;
				}

	return root = node;
}
// удаление всего дерева
Node *BinaryTree::removeTree(Node *tree)
{
	if (tree != NULL)
	{
		if (tree->getLeft() != NULL)
			tree->setLeft(removeTree(tree->getLeft()));

		if (tree->getRight() != NULL)
			tree->setRight(removeTree(tree->getRight()));

		delete tree;
	}

	return tree;
}
// прямой
void BinaryTree::orderCLR(Node *tree)
{
	//cout << tree->value << endl;

	if (tree->getLeft() != NULL)
		orderCLR(tree->getLeft());

	if (tree->getRight() != NULL)
		orderCLR(tree->getRight());
}
// обратный
void BinaryTree::orderLRC(Node *tree)
{
	if (tree->getLeft() != NULL)
		orderLRC(tree->getLeft());

	if (tree->getRight() != NULL)
		orderLRC(tree->getRight());

	//cout << tree->value << endl;
}
// симметричный
void BinaryTree::orderLCR(Node *tree, int depth)
{
	if (tree->getLeft() != NULL)
		orderLCR(tree->getLeft(), depth + 1);

	tree->setX(x++);
	tree->setY(depth);

	if (tree->getRight() != NULL)
		orderLCR(tree->getRight(), depth + 1);
}

/*
*	Draw tree
*/
// рисование дерева
void BinaryTree::orderDraw(Node *tree, int depth, HDC hDC, RECT rect)
{
	if (tree->getLeft() != NULL)
		orderDraw(tree->getLeft(), depth + 1, hDC, rect);

	HPEN pen;
	HBRUSH brush;

	pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hDC, pen);

	brush = CreateSolidBrush(RGB(255, 100, 100));
	SelectObject(hDC, brush);

	int scale = 40;

	int width = 400;
	int height = 10;

	Ellipse(hDC, tree->getX() * scale + width / 2, tree->getY() * scale,
		tree->getX() * scale + scale + width / 2, tree->getY() * scale + scale);

	if (tree->getParent() != NULL)
	{
		if (tree->getX() < tree->getParent()->getX())
		{
			MoveToEx(hDC, tree->getX() * scale + width / 2 + scale / 2, tree->getY() * scale, NULL);
			LineTo(hDC, tree->getParent()->getX() * scale + width / 2 + scale / 2, tree->getParent()->getY() * scale);
		}
		else
		{
			MoveToEx(hDC, tree->getParent()->getX() * scale + width / 2 + scale, 
				tree->getParent()->getY() * scale + scale / 2, NULL);
			LineTo(hDC, tree->getX() * scale + width / 2 + scale / 2, tree->getY() * scale);
		}
	}

	char text[10];

	sprintf_s(text, "%.1f", tree->getValue());
	SetBkColor(hDC, RGB(255, 100, 100));

	TextOutA(hDC, tree->getX() * scale + width / 2 + scale / 3 - strlen(text),
		tree->getY() * scale + scale / 3, text, strlen(text));

	DeleteObject(brush);
	DeleteObject(pen);

	if (tree->getRight() != NULL)
		orderDraw(tree->getRight(), depth + 1, hDC, rect);

	x = 0;
}

// поиск
Node *BinaryTree::search(Node *root, double key)
{
	if (root->getValue() == key)
		return root;
	else
		if (key < root->getValue())
			return search(root->getLeft(), key);
		else
			if (key > root->getValue())
				return search(root->getRight(), key);
			else
				return NULL;
}

/*
*	Depth of the node
*/
//глубина
int BinaryTree::getNodeDepth(Node *node)
{
	if (node->getParent() == NULL)
		return 0;
	else
		return 1 + getNodeDepth(node->getParent());
}

/*
*	Height of the node
*/
//высота
int BinaryTree::getNodeHeight(Node *node)
{
	if (node == NULL)
		return -1;
	else
		return 1 + max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight()));
}
/*
*	Get the count of nodes in the subtree
*/
//количество узлов
int BinaryTree::getCountNodes(Node *node)
{
	if (node == NULL)
		return 0;

	if (node->getLeft() == NULL && node->getRight() == NULL)
		return 1;

	int left, right;

	if (node->getLeft() != NULL)
		left = getCountNodes(node->getLeft());
	else
		left = 0;

	if (node->getRight() != NULL)
		right = getCountNodes(node->getRight());
	else
		right = 0;

	return left + right + 1;
}

/*
*	Получить список узлов, в которых количество потомков
*	слева не равно количеству узлов справа
*/
//список узлов
void BinaryTree::getListNodes(Node *root, vector<Node*> &list, int level)
{
	if (root->getLeft() != NULL)
		getListNodes(root->getLeft(), list, level);
	
	if (root->getY() == level)
		if (getCountNodes(root->getLeft()) > getCountNodes(root->getRight()))
			list.push_back(root);

	if (root->getRight() != NULL)
		getListNodes(root->getRight(), list, level);
}
//средний узел из узлов
Node *BinaryTree::getMiddleNodeOnLevel(Node *root)
{
	int level = (int)(getNodeHeight(root) / 2);

	vector<Node*> lr;
	getListNodes(root, lr, level);

	unsigned size = lr.size();
	unsigned middle;

	if (size != 0)
	{
		middle = size / 2;
		return lr[middle];
	}
	else
		return NULL;
}