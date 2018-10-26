/*************************************************************************
* Title: Binary Search Tree
* File: tree.h
* Author: James Eli
* Date: 10/26/2018
*
* Basic tree data structure using smart pointers.
*
* Notes:
*  (1) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit), and with Eclipse
*      Oxygen.3a Release (4.7.3a), using CDT 9.4.3/MinGw32 gcc-g++ (6.3.0-1).
*************************************************************************
* Change Log:
*  10/26/2018: Initial release. JME
*************************************************************************/
#ifndef _MY_TREE_H_
#define _MY_TREE_H_

#include <iostream> // cout
#include <memory>   // shared pointers

#include "queue.h"  // bfs traversal
#include "stack.h" // iterative in-order search

template <class T>
class Tree
{
private:
	struct Node
	{
	private:
		T data;                      // Node data element.
		std::shared_ptr<Node> left;  // Left child.
		std::shared_ptr<Node> right; // Right child.

		bool isChild() const { return !left && !right; }

	public:
		explicit Node(T data) : left(nullptr), right(nullptr), data(data) { }
		~Node() = default;

		template <typename T>
		friend class Tree;
	};

public:
	Tree() : root(nullptr) { }
	~Tree() { clear(root); }

	const Tree<T>& operator= (const Tree<T> &rhs)
	{
		if (this != &rhs)
		{
			clean(root);
			root = clone(rhs.root);
		}
		return *this;
	}

	// Internal method to clone subtree.
	std::shared_ptr<Node> clone(std::shared_ptr<Node> t)
	{
		if (t == nullptr)
			return nullptr;
		return std::make_shared<Node>(Node(t->element, clone(t->left), clone(t->right)));
	}

	// Basic tree functionality.
	void clear() { clear(root); }
	bool isEmpty() const { return (root == nullptr); }
	void add(T data) { add(root, data); }
	bool remove(T data) { return remove(root, data); }

	// Non-recursive search.
	bool find(T data) const { return find(root, data); }
	//bool search(T data) const { return search(root, data); }
	// Iterative in-order search.
	bool search(T data) const { return iterativeInorderSearch(root, data); }

	// Dfs traversals (recursive).
	void inOrder() const { inOrder(root); }
	void preOrder() const { preOrder(root); }
	void postOrder() const { postOrder(root); }
	
	// Bfs traversal (top down, left to right).
	void bfs()
	{
		Queue<std::shared_ptr<Node>> q;
		std::shared_ptr<Node> node = root;

		if (node != nullptr)
		{
			q.enqueue(node);

			while (!q.empty())
			{
				node = q.front();
				q.dequeue();

				std::cout << node->data << " ";

				if (node->left != nullptr)
					q.enqueue(node->left);

				if (node->right != nullptr)
					q.enqueue(node->right);
			}
		}
	}

private:
	// Tree root node.
	std::shared_ptr<Node> root;

	// Add new node to tree.
	void add(std::shared_ptr<Node> &node, T &data)
	{
		if (!node)
			node = std::make_shared<Node>(data);
		else
			data < node->data ? add(node->left, data) : add(node->right, data);
	}

	// Non-recursive search.
	bool search(std::shared_ptr<Node> node, const T& data) const
	{
		while (node != nullptr)
			if (data == node->data)
				return true;
			else if (data < node->data)
				node = node->left;
			else
				node = node->right;
		return false;
	}

	// Find first occurance of data in tree.
	bool find(std::shared_ptr<Node> node, T &data) const
	{
		if (!node)
			return false;
		else
		{
			if (node->data == data)
				return true;

			return find(node->left, data) || find(node->right, data);
		}
	}

	// Return minimum value of either child nodes.
	T min(std::shared_ptr<Node> node) const
	{
		if (node->isChild())
			return node->data;

		return node->left ? min(node->left) : min(node->right);
	}

	// Remove first instance of data from tree.
	bool remove(std::shared_ptr<Node> &node, T data)
	{
		if (!node)
			return false;
		else
		{
			if (node->data == data)
			{
				if (node->isChild())
					node.reset();
				else
				{
					// Does node have 2 children?
					if (node->left && node->right)
					{
						T dataReplace = min(node->right);

						remove(dataReplace);
						node->data = dataReplace;
					}
					else
					{
						// 1 child.
						std::shared_ptr<Node> tempPtr;

						node->left ? tempPtr = node->left : tempPtr = node->right;
						tempPtr.reset();
					}
				}

				return true;
			}

			return remove(node->left, data) || remove(node->right, data);
		}
	}

	// Delete all nodes of tree.
	void clear(std::shared_ptr<Node> &node)
	{
		if (node->left)
			clear(node->left);

		if (node->right)
			clear(node->right);

		node.reset();
	}

	// Dfs traversals.
	void inOrder(const std::shared_ptr<Node> node) const
	{
		if (node->left)
			inOrder(node->left);

		std::cout << node->data << " ";

		if (node->right)
			inOrder(node->right);
	}

	void preOrder(const std::shared_ptr<Node> node) const
	{
		std::cout << node->data << " ";

		if (node->left)
			preOrder(node->left);

		if (node->right)
			preOrder(node->right);
	}
	
	void postOrder(const std::shared_ptr<Node> node) const
	{
		if (node->left)
			postOrder(node->left);

		if (node->right)
			postOrder(node->right);

		std::cout << node->data << " ";
	}

	// Iterative in-order search using a stack.
	bool iterativeInorderSearch(std::shared_ptr<Node> p, T target) const
	{
		Stack<std::shared_ptr<Node>> stack;

		while (p != nullptr)
		{
			while (p != nullptr)
			{                                // stack the right child (if any)
				if (p->right)                // and the node itself when going
					stack.push(p->right);    // to the left;
				stack.push(p);
				p = p->left;
			}

			p = stack.pop();                 // pop a node with no left child

			while (!stack.empty() && p->right == nullptr)
			{                                // with no right child;
				std::cout << p->data << " "; // visit it and all nodes
				if (p->data == target)
					return true;
				p = stack.pop();
			}

			std::cout << p->data << " ";     // visit also the first node with

			if (p->data == target)
				return true;

			if (!stack.empty())              // a right child (if any);
				p = stack.pop();
			else
				p = nullptr;
		}

		return false;
	}

};
#endif
