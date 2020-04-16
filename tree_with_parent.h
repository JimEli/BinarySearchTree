/*************************************************************************
* Title: Binary Search Tree with iterator.
* File: tree.h
* Author: James Eli
* Date: 10/26/2018
*
* Basic tree data structure using smart pointers. Basic tree functionality
* needed is included:
*
*   clear()      // deletes tree.
*   empty()      // returns true if tree is empty.
*   size()       // returns tree size (number of nodes).
*   add(T)       // recursive insert new node. does NOT check if T
*                // already exists.
*   find(T)      // recursively find first occurance of data in tree.
*                // returns true if T is found.
*   inOrder()    // dfs inorder recursive traversal.
*   bfs()        // bfs non-recursive traversal (top down, left to right).
*   getHeight()  // returns height of tree.
*   isBalanced() // returns true if tree is balanced.
*   balance()    // attempts to balance tree.
*
* Notes:
*  (1) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit), and with Eclipse
*      Oxygen.3a Release (4.7.3a), using CDT 9.4.3 MinGw32 gcc-g++ (6.3.0-1).
*************************************************************************
* Change Log:
*  10/26/2018: Initial release. JME
*  04/16/2020: Added parent link and iterators.  JME
*************************************************************************/
#ifndef _MY_TREE_H_
#define _MY_TREE_H_

#include <iostream>  // cout.
#include <memory>    // shared pointers.
#include <algorithm> // max.
#include <string>    // printTree function.
#include "queue.h"   // bfs traversal.
#include "stack.h"   // iterative in-order search.
#include "vector.h"  // vector for building balanced tree.

template <class T>
class tree
{
private:
	struct Node
	{
	private:
		T data; 
		std::shared_ptr<Node> left = nullptr;
		std::shared_ptr<Node> right = nullptr;
		std::shared_ptr<Node> parent = nullptr;

		// Return true if node is leaf.
		bool isLeaf() const { return !left && !right; }

	public:
		explicit Node(T d) : data(d) { }
		Node(std::shared_ptr<Node> p, T d) : parent(p), data(d) { }
		~Node() = default;

		template <typename T>
		friend class tree;
	};

public:
	tree() : root(nullptr) { }
	~tree() = default;

	const tree<T>& operator= (const tree<T>& rhs)
	{
		if (this != &rhs)
		{
			clear(root);
			root = clone(rhs.root);
		}
		return *this;
	}

	//
	// Basic tree functionality.
	//

	void clear() { clear(root); }
	bool empty() const { return (root == nullptr); }
	void add(T data) { add(root, nullptr, data); }
	bool remove(T data) { return remove(root, data); }
	std::size_t size() { return size(root); }

	//
	// Searches.
	//

	// recursive search.
	bool find(T data) const { return find(root, data); }
	// Non-recursive search.
	bool search(T data) const { return search(root, data); }
	// Iterative in-order search.
	bool iSearch(T data) const { return iInorderSearch(root, data); }

	//
	// Traversals.
	//

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
	// Balancing.
	//

	// Get height of node. Used by isBalanced function.
	int getHeight() { return getHeight(root); }
	// Recursive check of tree balance. Returns true if tree is balanced.
	bool isBalanced() { return isBalanced(root); }
	// Attempt to balance tree.
	void balance() { balanceTree(root); }

	//
	// Iterators.
	//
	class iterator
	{
		friend class tree;

	public:
		typedef std::bidirectional_iterator_tag iterator_category;

		iterator() { ptr = nullptr; }
		iterator(std::shared_ptr<Node> p) { ptr = p; }
		iterator(const iterator& it) { ptr = it.ptr; }

		iterator& operator= (const iterator& it)
		{
			ptr = it.ptr;
			return *this;
		}

		bool operator== (const iterator& it) const { return ptr == it.ptr; }
		bool operator!= (const iterator& it) const { return ptr != it.ptr; }
		bool operator< (const iterator& it) const { return **this < *it; }
		bool operator> (const iterator& it) const { return **this > * it; }
		bool operator<= (const iterator& it) const { return **this <= *it; }
		bool operator>= (const iterator& it) const { return **this >= *it; }

		// pre-increment
		iterator& operator++ ()
		{
			if (ptr->right)
			{
				ptr = ptr->right;
				while (ptr->left)
					ptr = ptr->left;
			}
			else 
			{
				std::shared_ptr<Node> before;

				do {
					before = ptr;
					ptr = ptr->parent;
				} while (ptr && before == ptr->right);
			}
			return *this;
		}
		// post-increment
		iterator operator++ (int)
		{
			iterator old(*this);
			++(*this);
			return old;
		}

		// pre-decrement
		iterator& operator-- ()
		{
			if (ptr->left)
			{
				ptr = ptr->left;
				while (ptr->right) {
					ptr = ptr->right;
				}
			}
			else
			{
				std::shared_ptr<Node> before;
				do {
					before = ptr;
					ptr = ptr->parent;
				} while (ptr && before == ptr->left);
			}
			return *this;
		}
		// post-decrement
		iterator operator-- (int)
		{
			iterator old(*this);
			--(*this);
			return old;
		}

		T& operator* () const { return ptr->data; }
		T* operator-> () const { return &(ptr->data); }
		
	private:
		std::shared_ptr<Node> ptr;
	};

	class const_iterator
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;

		const_iterator() { ptr = nullptr; }
		const_iterator(const std::shared_ptr<Node> p) { ptr = p; }
		const_iterator(const const_iterator& it) { ptr = it.ptr; }
		const_iterator(const iterator& it) { ptr = it.ptr; }

		const_iterator& operator= (const const_iterator& it)
		{
			ptr = it.ptr;
			return *this;
		}

		bool operator== (const const_iterator& it) const { return ptr == it.ptr; }
		bool operator!= (const const_iterator& it) const { return ptr != it.ptr; }
		bool operator< (const const_iterator& it) const { return **this < *it; }
		bool operator> (const const_iterator& it) const { return **this > * it; }
		bool operator<= (const const_iterator& it) const { return **this <= *it; }
		bool operator>= (const const_iterator& it) const { return **this >= *it; }

		// pre-increment
		const_iterator& operator++ ()
		{
			if (ptr->right)
			{
				ptr = ptr->right;
				while (ptr->left) 
					ptr = ptr->left;
			}
			else
			{
				std::shared_ptr<Node> before;

				do {
					before = ptr;
					ptr = ptr->parent;
				} while (ptr && before == ptr->right);
			}
			return *this;
		}
		// post-increment
		const_iterator operator++ (int)
		{
			const_iterator old(*this);
			++(*this);
			return old;
		}

		// pre-decrement
		const_iterator& operator-- ()
		{
			if (ptr->left)
			{
				ptr = ptr->left;
				while (ptr->right) {
					ptr = ptr->right;
				}
			}
			else
			{
				const std::shared_ptr<Node> before;
				do {
					before = ptr;
					ptr = ptr->parent;
				} while (ptr && before == ptr->left);
			}
			return *this;
		}
		// post-decrement
		const_iterator operator-- (int)
		{
			const_iterator old(*this);
			--(*this);
			return old;
		}

		const T& operator* () const { return (const T&)(ptr->data); }
		const T* operator-> () const { return &(ptr->data); }

	private:
		std::shared_ptr<Node> ptr;
	};

	class reverse_iterator
	{
		friend class tree;

	public:
		reverse_iterator() { ptr = nullptr; }
		reverse_iterator(std::shared_ptr<Node> p) { ptr = p; }
		reverse_iterator(const reverse_iterator& it) { ptr = it.ptr; }

		reverse_iterator& operator= (const reverse_iterator& it)
		{
			ptr = it.ptr;
			return *this;
		}

		bool operator== (const reverse_iterator& it) const { return ptr == it.ptr; }
		bool operator!= (const reverse_iterator& it) const { return ptr != it.ptr; }
		bool operator< (const reverse_iterator& it) const { return **this > * it; }
		bool operator> (const reverse_iterator& it) const { return **this < *it; }
		bool operator<= (const reverse_iterator& it) const { return **this >= *it; }
		bool operator>= (const reverse_iterator& it) const { return **this <= *it; }

		// pre-increment
		reverse_iterator& operator++ ()
		{
			if (ptr->left)
			{
				ptr = ptr->left;
				while (ptr->right) {
					ptr = ptr->right;
				}
			}
			else
			{
				std::shared_ptr<Node> before;
				do {
					before = ptr;
					ptr = ptr->parent;
				} while (ptr && before == ptr->left);
			}
			return *this;
		}
		// post-increment
		reverse_iterator operator++ (int)
		{
			iterator old(*this);
			--(*this);
			return old;
		}

		// pre-decrement
		reverse_iterator& operator-- ()
		{
			if (ptr->right)
			{
				ptr = ptr->right;
				while (ptr->left)
					ptr = ptr->left;
			}
			else
			{
				std::shared_ptr<Node> before;

				do {
					before = ptr;
					ptr = ptr->parent;
				} while (ptr && before == ptr->right);
			}
			return *this;
		}
		// post-decrement
		reverse_iterator operator-- (int)
		{
			iterator old(*this);
			++(*this);
			return old;
		}

		T& operator* () const { return ptr->data; }
		T* operator-> () const { return &(ptr->data); }

	private:
		std::shared_ptr<Node> ptr;
	};

	iterator begin()
	{
		std::shared_ptr<Node> ptr = root;

		while (ptr->left)
			ptr = ptr->left;

		return iterator(ptr);
	}
	const_iterator begin() const
	{
		std::shared_ptr<Node> ptr = root;

		while (ptr->left)
			ptr = ptr->left;

		return const_iterator(ptr);
	}
	const_iterator cbegin() const
	{
		const std::shared_ptr<Node> ptr = root;

		while (ptr->left)
			ptr = ptr->left;

		return const_iterator(ptr);
	}
	reverse_iterator rbegin()
	{
		std::shared_ptr<Node> ptr = root;

		while (ptr->right)
			ptr = ptr->right;

		return reverse_iterator(ptr);
	}

	iterator end()
	{
		std::shared_ptr<Node> ptr = root;

		while (ptr->right)
			ptr = ptr->right;
		
		return iterator(ptr->right); // return iterator(nullptr);
	}
	const_iterator end() const 
	{
		std::shared_ptr<Node> ptr = root;

		while (ptr->right)
			ptr = ptr->right;

		return const_iterator(ptr->right);
	}
	const_iterator cend() const 
	{ 
		const std::shared_ptr<Node> ptr = root;

		while (ptr->right)
			ptr = ptr->right;

		return const_iterator(ptr->right);
	}
	reverse_iterator rend()
	{
		std::shared_ptr<Node> ptr = root;

		while (ptr->left)
			ptr = ptr->left;

		return reverse_iterator(ptr->left);
	}

protected:
	// Tree root node.
	std::shared_ptr<Node> root;

private:
	// Internal method to clone subtree.
	std::shared_ptr<Node> clone(std::shared_ptr<Node> t)
	{
		if (t == nullptr)
			return nullptr;
		return std::make_shared<Node>(Node(t->element, clone(t->left), clone(t->right)));
	}

	// Delete all nodes of tree.
	void clear(std::shared_ptr<Node>& node)
	{
		if (node->left)
			clear(node->left);

		if (node->right)
			clear(node->right);

		node.reset(); //node.~shared_ptr();
	}

	// Add new node to tree.
	void add(std::shared_ptr<Node>& node, std::shared_ptr<Node> parentNode, T& data)
	{
		if (!node)
			node = std::make_shared<Node>(parentNode, data);
		else
			data < node->data ? add(node->left, node, data) : add(node->right, node, data);
	}

	// Number of nodes in tree.
	std::size_t size(std::shared_ptr<Node>& node)
	{
		if (!node)
			return 0;
		else
			return(size(node->left) + 1 + size(node->right));
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
					if (node->left && node->right)
					{
						// Node has 2 children, pick smaller one and replace.
						T dataReplace = min(node->right);
						remove(dataReplace);
						node->data = dataReplace;
					}
					else
					{
						// 1 child, promote it.
						std::shared_ptr<Node> nodeReplace, parent;
						node->left ? nodeReplace = node->left : nodeReplace = node->right;
						parent = node->parent;
						node.reset();
						node = nodeReplace;    // Replace with the only child node.
						node->parent = parent; // Update parent link.
					}
				}

				return true;
			}

			return remove(node->left, data) || remove(node->right, data);
		}
	}

	// Recursively find first occurance of data in tree.
	bool find(std::shared_ptr<Node> node, T& data) const
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
	bool search(std::shared_ptr<Node> node, T& data) const
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

//		std::cout << node->data << " ";

		std::cout << node->data << "(";
		if (node->parent)
			std::cout << node->parent->data << ") ";
		else
		    std::cout << "x) ";

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

				std::cout << node->data;

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
