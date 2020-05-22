#include "Node.h"

Node::Node(double value, Node *parent, Node *left, Node *right)
{
	this->value = value;
	this->parent = parent;
	this->left = left;
	this->right = right;
}

Node::~Node()
{ }

void Node::setValue(double value)
{
	this->value = value;
}

double Node::getValue()
{
	return value;
}

void Node::setParent(Node *parent)
{
	this->parent = parent;
}

Node *Node::getParent()
{
	return parent;
}

void Node::setLeft(Node * left)
{
	this->left = left;
}

Node *Node::getLeft()
{
	return left;
}

void Node::setRight(Node * right)
{
	this->right = right;
}

Node *Node::getRight()
{
	return right;
}

void Node::setX(int X)
{
	this->X = X;
}

int Node::getX()
{
	return X;
}

void Node::setY(int Y)
{
	this->Y = Y;
}

int Node::getY()
{
	return Y;
}