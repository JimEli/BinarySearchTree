//
// Binary Search Tree.
//
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

#include "tree.h"

int main() 
{
	Tree<int> myTree;
	assert(myTree.empty());

/*
	//          6
	//	  /   \
	//	 4     8
	//	/ \   / \
	//     3   5 7   9
	myTree.add(6); // root.
	myTree.add(4); // left child.
	myTree.add(8); // right child.
	myTree.add(3);
	myTree.add(5);
	myTree.add(7);
	myTree.add(9);
*/
	for (int i = 3; i < 10; i++)
		myTree.add(i); 

	// Dfs.
	std::cout << "dfs (recursive) in-order: ";
	myTree.inOrder();	 // Numbers displayed in order.
	std::cout << std::endl;
	std::cout << "dfs (iterative) in-order: ";
	myTree.iInorder();	 // Root should be displayed first.
	std::cout << std::endl;
	//
	std::cout << "dfs (recursive) pre-order: ";
	myTree.preOrder();	 // Root should be displayed first.
	std::cout << std::endl;
	std::cout << "dfs (iterative) pre-order: ";
	myTree.iPreorder();	 // Root should be displayed first.
	std::cout << std::endl;
	//
	std::cout << "dfs (recursive) post-order: ";
	myTree.postOrder();	 // Root should be displayed last.
	std::cout << std::endl;
	std::cout << "dfs (iterative) post-order: ";
	myTree.iPostorder(); // Root should be displayed first.
	std::cout << std::endl;

	// Bfs.
	std::cout << "bfs (left-right): ";
	myTree.bfs(); std::cout << std::endl;

	// Find node.
	myTree.find(0) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;
	std::cout << (myTree.find(7) ? "Found\n" : "Not in tree\n");
	std::cout << (myTree.find(0) ? "Found\n" : "Not in tree\n");
	// Search for node.
	std::cout << (myTree.search(7) ? "Found\n" : "Not in tree\n");
	std::cout << (myTree.search(0) ? "Found\n" : "Not in tree\n");
	// Iterative search for node.
	std::cout << (myTree.iSearch(7) ? "Found\n" : "Not in tree\n");
	std::cout << (myTree.iSearch(0) ? "Found\n" : "Not in tree\n");

	// Delete node.
	std::cout << "delete:\n";
	myTree.remove(8);
	myTree.inOrder(); std::cout << std::endl;
	myTree.preOrder(); std::cout << std::endl;
	myTree.postOrder();	std::cout << std::endl;

	// Balance.
	std::cout << "Balance Tree\nTree ht: " << myTree.getHeight() << std::endl;
	std::cout << "Tree is balanced: " << std::boolalpha << myTree.isBalanced() << "\n";
	myTree.balance();
	myTree.inOrder();
	std::cout << "\nTree ht: " << myTree.getHeight() << std::endl;
	std::cout << "Tree is balanced: " << std::boolalpha << myTree.isBalanced() << "\n";
	std::cout << std::endl;

}
