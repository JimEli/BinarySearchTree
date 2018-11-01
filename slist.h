/*************************************************************************
* Title: Templated Singly Linked List
* File: list.h
* Author: James Eli
* Date: 9/24/2018
*
* Minimal Singly Linked List Class using shared pointers. Far from
* complete.
*
* Singly linked list constructed of a private node structure. Each node
* contains two fields (data and next). The data element is templated to
* be of any type. The list maintinas a head and tail pointer. List member 
* functions (only) include:
*   back()      // Returns element at tail of list.
*   empty()     // Returns true if list is empty.
*	add()       // Adds new node to tail of list.
*	clear()     // Clears list of all elements.
*   find()      // find element.
*   remove()    // Remove element from list.
*	operator<<  // Overloaded << operator prints list contents.
*   iterator incorporates only needed functionality for the demo.
*
* Notes:
*  (1) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0
*  (2) Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using
*      CDT 9.4.3 and MinGw gcc-g++ (6.3.0-1).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-269.
*************************************************************************
* Change Log:
*   09/24/2018: Initial release. JME
*   09/29/2018: Added remove function. JME
*   09/29/2018: Added very basic iterator support. JME
*   09/29/2018: Converted to use smart pointers. JME
*************************************************************************/
#ifndef SL_LIST_H
#define SL_LIST_H

#include <iostream> // cout
#include <memory>   // smart pointer

namespace myList 
{
	// Individual list node.
	template <typename T>
	struct Node
	{
		Node() { next = nullptr; }
		Node(T e) { element = e; next = nullptr; }
		~Node() { }

		T element;                     // Data.
		std::shared_ptr<Node<T>> next; // Link to next node.
	};

	template <typename T>
	class list
	{
	private:
		// Head and tail node pointers. 
		std::shared_ptr<Node<T>> head;
		std::shared_ptr<Node<T>> tail;
		// Count of nodes in list.
		std::size_t count; 

	public:
		// Ctor.
		list() : head(nullptr), tail(nullptr), count(0) { }
		// Dtor.
		~list() { } // Use of smart pointers precludes doing something here.

		T& back() const;               // Returns element at tail of list.
		bool empty() const;            // Returns true if list is empty.
		void add(const T&);            // Adds new node to tail of list.
		void remove(T);                 // Remove fisrt occurance of element from list.
		std::shared_ptr<Node<T>> find(const T&); // Find element in list.

		// Overloaded output operator to display list contents.
		friend std::ostream& operator<< (std::ostream& os, const list<T>& list)
		{
			for (auto node = list.head; node; node = node->next)
				os << node->element;
			return os << std::endl;
		}

		//
		// Inner iterator class. Member typedefs provided through inheritance of std::iterator.
		class iterator : public std::iterator<std::forward_iterator_tag, T>
		{
		private:
			std::shared_ptr<Node<T>> pNode;

			// Ctor is private, so only friends can create instances.
			iterator(std::shared_ptr<Node<T>> n) : pNode(n) { }
			iterator() : pNode(nullptr) { }

			friend class list;

		public:
			// Dtor.
			~iterator() { }

			// Overload comparison operators.
			bool operator== (const iterator& it) const { return pNode == it.pNode; }
			bool operator!= (const iterator& it) const { return pNode != it.pNode; }

			// Overload dereference and pointer operators, returns element.
			T& operator* () { return pNode->element; }
			T* operator-> () { return &pNode->element; }

			// Overload prefix increment operator, moves to next node.
			iterator& operator++ ()
			{
				pNode = pNode->next;
				return *this;
			}

		}; // End iterator inner class.

		// Begin and end iterators.
		iterator begin() const { return iterator(head); }
		//iterator end() const { return iterator(tail); }
		iterator end() const { return iterator(tail->next); }
	};


	template <typename T>
	bool list<T>::empty() const { return this->count == 0; }

	template <typename T>
	T& list<T>::back() const { return this->tail->element; }

	// Adds new node to tail of list.
	template <typename T>
	void list<T>::add(const T& e)
	{
		auto newNode{ std::make_shared<Node<T>>(e) };

		if (this->head == nullptr) // Update head pointer.
			this->head = newNode;

		if (this->tail)            // Update tail pointer.
			this->tail->next = newNode;
		this->tail = newNode;

		this->count++;             // Increment node counter.
	}

	// Find and return node.
	template <typename T>
	std::shared_ptr<Node<T>> list<T>::find(const T& e)
	{
		if (this->empty())         // Can't find in empty list.
			return nullptr;

		std::shared_ptr<Node<T>> node { this->head };

		// Walk list looking for node match.
		do {
			if (node->element == e)
				return node;       // Found it!

			node = node->next;
		
		} while (node);

		return nullptr;            // Failed to find node.
	}

	// Find and delete node.
	template <typename T>
	void list<T>::remove(T e)
	{
		if (this->empty())         // Check for empty list.
			return;

		std::shared_ptr<Node<T>> node { this->head };
		std::shared_ptr<Node<T>> prev { node };

		do {                      
			// Loop through all nodes, looking for match.
			if (node->element == e)
			{
				// Decrement count, check if empty?
				if (!--count)
				{
					head = nullptr;
					tail = nullptr;
					node.~shared_ptr();
					return;
				}
				// Removing head node?
				if (node == head)
				{
					head = node->next;
					node.~shared_ptr();
					return;
				}
				// Removing tail node?
				else if (node->next == nullptr)
				{
					tail = prev;
					prev->next = nullptr;
					node.~shared_ptr();
					return;
				}
				// Removing an interior node.
				else
				{
					prev->next = node->next;
					node.~shared_ptr();
					return;
				}
			}
			// Advance to next node.
			prev = node;
			node = node->next;
		} while (node);
	}

}

#endif
