#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;

enum color {RED, BLACK};

template <typename Key, typename Element>
class Node {
private:
	// Add class members here
	color c;
	Key k;
	Element e;
	Node<Key, Element>* right;
	Node<Key, Element>* left;
	Node<Key, Element>* parent;

public:
	// Implement each of these methods
//	color c;
//	Key k;
//	Element e;
//	Node *right = NULL;
//	Node *left = NULL;
//	Node *parent = NULL;
	
	Node() {
		c = RED;
		parent = NULL;
		right = NULL;
		left = NULL;
	}

	Node(Key k1, Element e1) {
		c = RED;
		k = k1;
		e = e1;
		parent = NULL;
		right = NULL;
		left = NULL;
	}

	// These implementations are only to get the skeleton
	// code to compile. Ignore the warnings they generate
	// until you replace their implementations with your own
	const Key& getKey() const {
		return k;
	}

	const Element& getElement() const {
		return e;
	}

	Element& getElement() {
		return e;
	}

	color getColor() const {
		return c;
	}
	
	Node* getParent() {
//		if(parent != NULL) {
			return parent;
//		}
//		return NULL;
	}

	Node* getLeft() { 
		if(left == NULL) {
			return NULL;
		}
		return left;
	}

	Node* getRight() { 
		if(right == NULL) {
			return NULL;
		}
		return right;
	}

	void setParent(Node<Key, Element> *parent) {
	//	if(p != NULL) {
		this->parent = parent;
	}

	void setLeft(Node<Key, Element> *left) {
		this->left = left;
	}

	void setRight(Node<Key, Element> *right) {
		this->right = right;
	}

	void setKey(const Key& kk) {
		k = kk;
	}

	void setElement(const Element& ee) {
		e = ee;
	}
	
	void setColor(color cc) {
		c = cc;
	}
};



// The following functions have been provided for you, and do not
// need to be modified:


// Output stream operator overload -- writes the contents of a
// Node to the specified output stream. Note that there must be
// a similar operator overload for Key and Element types in
// order for this to compile.
// Example usage:
//		Node<K,E> n;
//		cout << n << endl;
template <typename Key, typename Element>
ostream& operator<<(ostream& out, const Node<Key, Element>& n) {
	out << n.getKey() << endl;
	out << n.getElement() << endl;
	return out;
}

// Input stream operator overload -- reads the contents of a
// Node from the specified input stream. Note that there must be
// a similar operator overload for Key and Element types in order
// for this to compile.
// Example usage:
//		Node<K,E> n;
//		cin >> n;
template <typename Key, typename Element>
istream& operator>>(istream& in, Node<Key, Element>& n) {
	// Clear existing contents, if any
	n = Node<Key, Element>();

	Key k;
	in >> k;
	Element e;
	in >> e;
	n.setKey(k);
	n.setElement(e);

	return in;
}

#endif
