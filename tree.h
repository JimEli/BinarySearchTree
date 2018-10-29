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

#include <iostream>  // cout.
#include <memory>    // shared pointers.
#include <algorithm> // max.

#include "queue.h"   // bfs traversal.
#include "stack.h"   // iterative in-order search.
#include "vector.h"  // vector for building balanced tree.

// Visual Leak Detector.
#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"

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
		// Return true if node is leaf.
		bool isLeaf() const { return !left && !right; }

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
			clear(root);
			root = clone(rhs.root);
		}
		return *this;
	}

	//
	//
	// Basic tree functionality.

	void clear() { clear(root); }
	bool empty() const { return (root == nullptr); }
	void add(T data) { add(root, data); }
	bool remove(T data) { return remove(root, data); }

	//
	//
	// Searches.

	// Non-recursive search.
	bool find(T data) const { return find(root, data); }
	bool search(T data) const { return search(root, data); }
	// Iterative in-order search.
	bool iSearch(T data) const { return iInorderSearch(root, data); }

	//
	//
	// Traversals.

	// Dfs traversals (recursive).
	void inOrder() const { inOrder(root); }
	void preOrder() const { preOrder(root); }
	void postOrder() const { postOrder(root); }
	// Dfs traversals (iterative).
	void iInorder() const { iInorder(root); }
	void iPreorder() const { iPreorder(root); }
	void iPostorder() const { iPostorder(root); }

	// Bfs traversal (top down, left to right).
	void bfs() const { bfs(root); }
	
	//
	//
	// Balancing.

	// Get height of node. Used by isBalanced function.
	int getHeight() { return getHeight(root); }
	// Recursive check of tree balance. Returns true if tree is balanced.
	bool isBalanced() { return isBalanced(root); }
	// Attempt to balance tree.
	void balance() { balanceTree(root); }

private:
	// Tree root node.
	std::shared_ptr<Node> root;

	// Internal method to clone subtree.
	std::shared_ptr<Node> clone(std::shared_ptr<Node> t)
	{
		if (t == nullptr)
			return nullptr;
		return std::make_shared<Node>(Node(t->element, clone(t->left), clone(t->right)));
	}

	// Delete all nodes of tree.
	void clear(std::shared_ptr<Node> &node)
	{
		if (node->left)
			clear(node->left);

		if (node->right)
			clear(node->right);

		node.reset(); //node.~shared_ptr();
	}

	// Add new node to tree.
	void add(std::shared_ptr<Node> &node, T &data)
	{
		if (!node)
			node = std::make_shared<Node>(data);
		else
			data < node->data ? add(node->left, data) : add(node->right, data);
	}

	// Return minimum value of either child nodes.
	T min(std::shared_ptr<Node> node) const
	{
		if (node->isLeaf())
			return node->data;

		return node->left ? min(node->left) : min(node->right);
	}

	// Remove first instance of data from tree.
	bool remove(std::shared_ptr<Node>& node, T data)
	{
		if (!node)
			return false;
		else
		{
			if (node->data == data)
			{
				if (node->isLeaf())
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
						node.reset();   // Decrement use count.
						node = tempPtr; // Replace with only child.
					}
				}

				return true;
			}

			return remove(node->left, data) || remove(node->right, data);
		}
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

	// Iterative in-order search using a stack.
	bool iInorderSearch(std::shared_ptr<Node> p, T target) const
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

	// Iterative in-order traversal.
	void iInorder(std::shared_ptr<Node> p) const
	{
		Stack<std::shared_ptr<Node>> stack;

		while (p)
		{
			while (p)
			{
				if (p->right)
					stack.push(p->right);
				stack.push(p);
				p = p->left;
			}
			p = stack.pop();
			while (!stack.empty() && !p->right)
			{
				std::cout << p->data << " ";
				p = stack.pop();
			}
			std::cout << p->data << " ";
			if (!stack.empty())
				p = stack.pop();
			else
				p = nullptr;
		}
	}

	// Iterative preorder travesal.
	void iPreorder(std::shared_ptr<Node> node) const
	{
		Stack<std::shared_ptr<Node>> stack;

		if (node)
		{
			stack.push(node);

			while (!stack.empty())
			{
				node = stack.pop();

				std::cout << node->data << " ";

				if (node->right)
					stack.push(node->right);

				if (node->left)
					stack.push(node->left);
			}
		}
	}
	
	// Iterative post-order traversal.
	void iPostorder(std::shared_ptr<Node> p) const
	{
		Stack<std::shared_ptr<Node>> stack;
		std::shared_ptr<Node> q = p;

		while (p)
		{
			for (; p->left; p = p->left)
				stack.push(p);

			while (!p->right || p->right == q)
			{
				std::cout << p->data << " ";
				q = p;
				if (stack.empty())
					return;
				p = stack.pop();
			}
			stack.push(p);
			p = p->right;
		}
	}

	// Bfs traversal (top down, left to right).
	void bfs(std::shared_ptr<Node> node) const
	{
		Queue<std::shared_ptr<Node>> q;

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

	// Get height of node. Used by isBalanced function.
	static int getHeight(std::shared_ptr<Node> node)
	{
		return node == nullptr ? 0 : std::max(getHeight(node->left), getHeight(node->right)) + 1;
	}

	// Check if tree is balanced.
	static bool isBalanced(std::shared_ptr<Node> node)
	{
		if (node == nullptr)
			return true;

		int left = getHeight(node->left);
		int right = getHeight(node->right);

		return (abs(left - right) <= 1 && isBalanced(node->left) && isBalanced(node->right));
	}
	/*
	// Returns true if tree is balanced. Second parameter stores tree height.
	// Initially, we need to pass a pointer to a location with value as 0.
	static bool isBal(BSTNode<T>* node, int* height)
	{
		// lh --> Height of left and right subtrees.
		int lh = 0, rh = 0;
		// l will be true if left subtree is balanced, r will be true if right subtree is balanced.
		int l = 0, r = 0;

		if (node == nullptr)
		{
			*height = 0;
			return 1;
		}

		// Get heights of left and right subtrees in lh and rh, store returned values in l and r.
		l = isBal(node->left, &lh);
		r = isBal(node->right, &rh);

		// Height of current node is max of heights of left and right subtrees plus 1.
		*height = (lh > rh ? lh : rh) + 1;

		// If difference between heights of left/right subtrees is
		// more than 2 then this node is not balanced, return 0.
		if ((lh - rh >= 2) || (rh - lh >= 2))
			return 0;
		// If node is balanced & left/right subtrees are balanced, return true.
		else
			return (l && r);
	}

	int height(std::shared_ptr<Node> temp)
	{
		int h = 0;

		if (temp != nullptr)
		{
			int l_height = height(temp->left);
			int r_height = height(temp->right);
			int max_height = std::max(l_height, r_height);

			h = max_height + 1;
		}

		return h;
	}
	*/

	// Balance tree helper method, builds tree from (sorted) array of data elements.
	void buildTree(Vector<T>& data, int start, int end)
	{
		if (start <= end)
		{
			int mid = (start + end) / 2;

			add(data[mid]);

			buildTree(data, start, mid - 1);
			buildTree(data, mid + 1, end);
		}
	}

	// Balance tree helper method, constructs sorted array of tree data via inOrder traversal.
	void makeArray(std::shared_ptr<Node> node, Vector<T>& data)
	{
		// Base case.
		if (!node)
			return;

		// Store tree nodes inorder.
		makeArray(node->left, data);
		data.push_back(node->data);
		makeArray(node->right, data);
	}

	// Attempt to reconstruct tree as balanced. 
	void balanceTree(std::shared_ptr<Node> node)
	{
		// Store nodes in sorted order.
		Vector<T> data;
		makeArray(node, data);

		// Reconstruct a balanced tree.
		clear(root);
		buildTree(data, 0, data.size() - 1);
	}

};
#endif
