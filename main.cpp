//
// Binary Search Tree.
//
#include <iostream>
#include <iomanip>
#include <string>

#include "tree.h"

int main() 
{
	Tree<int> myTree = Tree<int>();
	
	//      6
	//	  /   \
	//	 4     8
	//	/ \   / \
	// 3   5 7   9
	myTree.add(6); // root.
	myTree.add(4); // left child.
	myTree.add(8); // right child.

	myTree.add(3);
	myTree.add(5);
	myTree.add(7);
	myTree.add(9);

	// Dfs traversals.
	std::cout << "dfs (recursive) in-order: ";
	myTree.inOrder();	     // Numbers displayed in order.
	std::cout << std::endl;
	std::cout << "dfs (iterative) in-order: ";
	myTree.iInorder();	    // Root should be displayed first.
	std::cout << std::endl;

	std::cout << "dfs (recursive) pre-order: ";
	myTree.preOrder();	    // Root should be displayed first.
	std::cout << std::endl;
	std::cout << "dfs (iterative) pre-order: ";
	myTree.iPreorder();	    // Root should be displayed first.
	std::cout << std::endl;

	std::cout << "dfs (recursive) post-order: ";
	myTree.postOrder();	   // Root should be displayed last.
	std::cout << std::endl;
	std::cout << "dfs (iterative) post-order: ";
	myTree.iPostorder();	    // Root should be displayed first.
	std::cout << std::endl;

	// Bfs.
	std::cout << "bfs (left-right): ";
	myTree.bfs();
	std::cout << std::endl;

	// Find node.
	myTree.find(7) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;
	myTree.find(0) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;
	// Search for node.
	myTree.search(7) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;
	myTree.search(0) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;
	// Iterative search for node.
	myTree.iSearch(7) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;
	myTree.iSearch(0) ? std::cout << "Found" << std::endl : std::cout << "Not in tree" << std::endl;

	// Delete node.
	myTree.remove(8);
	myTree.preOrder();
	std::cout << std::endl;

}
