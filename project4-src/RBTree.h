#ifndef PROJ4_RBTREE_H
#define PROJ4_RBTREE_H

#include "Node.h"
#include <iostream>
#include <queue>

using namespace std;

template <typename Key, typename Element>
class RBTree {
private:
	// Add class members here
	Node<Key, Element> *root;
	int num = 0;
public:
//	Node<Key, Element> *root;
	// Implement each of these methods
    RBTree() {
   	root = NULL;
    }
    void rotateLeft(Node<Key, Element> *x);
    void rotateRight(Node<Key, Element> *y);
    void insert(Key k,  Element e);
    Node<Key, Element>* search(const Key& k);
    void del(const Key& k);
    void inorder(ostream& out) const;
 
    Node<Key, Element>* insertHelper(Node<Key, Element> *root, Node<Key, Element> *z) {
	if(root == NULL) {
		return z;
	}

	if(z->getKey() < root->getKey()) {
		root->setLeft(insertHelper(root->getLeft(), z));
		root->getLeft()->setParent(root);
	} else if(z->getKey() > root->getKey()) {
		root->setRight(insertHelper(root->getRight(), z));
		root->getRight()->setParent(root);
	}

	return root;
    }

    void levelorderHelper(ostream& out, Node<Key, Element> *root) const {
	if(root == NULL) {
		return;
	}

	queue<Node<Key, Element> *> qq;
	qq.push(root);

	while(!qq.empty()) {
		Node<Key, Element> *t = qq.front();
		qq.pop();

		out << t->getKey() << endl;
		out << t->getElement() << endl;
		out << "" << endl;

		if(t->getLeft() != NULL) {
			qq.push(t->getLeft());
		}

		if(t->getRight() != NULL) {
			qq.push(t->getRight());
		}
	}
    }

    void inorderHelper(ostream& out, Node<Key, Element> *root) const {
//	out << "eeeeeeee" << endl;
	if(root == NULL) {
		return;
	}

	inorderHelper(out, root->getLeft());
	out << root->getKey() << endl;
	out << root->getElement() << endl;
	out << "" << endl;
//	out << "eeeeeeeeeee" << endl;
	inorderHelper(out, root->getRight());
    }

    void levelOrder(ostream& out) const;
	size_t size() const;
	Node<Key, Element>* getRoot();
};

template <typename Key, typename Element>
void RBTree<Key, Element>::rotateLeft(Node<Key, Element> *x) {
	Node<Key, Element> *xr = x->getRight();
	
	x->setRight(xr->getLeft());

	if(x->getRight() != NULL) {
		x->getRight()->setParent(x);
	}

	xr->setParent(x->getParent());

	if(x->getParent() == NULL) {
		root = xr;
	} else if(x == x->getParent()->getLeft()) {
		x->getParent()->setLeft(xr);
	} else {
		x->getParent()->setRight(xr);
	}

	xr->setLeft(x);
	x->setParent(xr);

/*
	y = x->getRight();

	if(y->getParent() != NULL) {
		x->setRight(y->getLeft());
	}

	if(y->getLeft() != NULL) {
		y->getLeft()->setParent(x);
	}

	y->setParent(x->getParent());
//	y->setLeft(x);

	if(x->getParent() == NULL) {
		root = y;
	} else if(x == x->getParent()->getLeft()) {
		x->getParent()->setLeft(y);
	} else {
		x->getParent()->setRight(y);
	}

	y->setLeft(x);
	x->setParent(y);
*/
}

template <typename Key, typename Element>
void RBTree<Key, Element>::rotateRight(Node<Key, Element> *y) {
	Node<Key, Element> *yl = y->getLeft();

//	if(yl->getRight() != NULL) {
		y->setLeft(yl->getRight());
//	}

	if(y->getLeft() != NULL) {
		y->getLeft()->setParent(y);
	}
	
	yl->setParent(y->getParent());

	if(y->getParent() == NULL) {
		root = yl;
	} else if(y == y->getParent()->getLeft()) {
		y->getParent()->setLeft(yl);
	} else {
		y->getParent()->setRight(yl);
	}

	yl->setRight(y);
	y->setParent(yl);

/*
	x = y->getLeft();
	
	if(x->getRight() != NULL) {
		y->setLeft(x->getRight());
	}

	if(x->getRight()) {
		x->getRight()->setParent(y);
	}

	x->setParent(y->getParent());
//	x->setRight(y);

	if(y->getParent() == NULL) {
		root = x;
	} else if(y == y->getParent()->getLeft()) {
		y->getParent()->setLeft(x);
	} else {
		y->getParent()->setRight(x);
	}

	x->setRight(y);
	y->setParent(x);
*/
}

// Function to insert a new node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::insert(Key k, Element e) {
//	Node<Key, Element> *node;
//	Node<Key, Element> *parent;
	Node<Key, Element> *z = new Node<Key, Element>();
	z->setKey(k);
	z->setElement(e);

	root = insertHelper(root, z);

//	parent = NULL;
//	node = root;

//	if(root == NULL) {
//		root = z;
//	} else {
//		root = root;
//	}

//	z->setKey(k);
//	z->setElement(e);
//	z->setColor(RED);
//	cout << z->getColor() << endl;
//	if(z == NULL) {
//		cout << "z is NULL" << endl;
//	} else {
//		cout << "z is not NULL" << endl;
//	}
/*
	while(node != NULL) {
		cout << "root != NULL" << endl;
		parent = node;
		if(k < node->getKey()) {
			node = node->getLeft();
		} else {
			node = node->getRight();
		}
	}

	if(parent == NULL) {
		z = node;
//		z->setKey(k);
//		z->setElement(e);
		z->setColor(BLACK);
		root = z;
//		z->setParent(NULL);
//		z->setLeft(NULL);
//		z->setRight(NULL);
	} else {
		z->setKey(k);
		z->setElement(e);
		z->setColor(RED);
		z->setParent(parent);
//		z->setLeft(NULL);
//		z->setRight(NULL);

		if(z->getKey() < parent->getKey()) {
			parent->setLeft(z);
		} else {
			parent->setRight(z);
		}
	}
*/
	Node<Key, Element> *uncle;
	Node<Key, Element> *par;
	Node<Key, Element> *gp;
	while((z != root) && (z->getColor() != BLACK) && (z->getParent()->getColor() == RED)) {

		par = z->getParent();
		gp = z->getParent()->getParent();

		if(par == gp->getLeft()) {
			uncle = gp->getRight();
		/*} else {
			uncle = z->getParent()->getParent()->getLeft();
		}*/
	
			if(uncle != NULL && uncle->getColor() == RED) {
				gp->setColor(RED);
				par->setColor(BLACK);
				uncle->setColor(BLACK);
			//	z->getParent()->getParent()->setColor(RED);
				z = gp;
			} else {
				if(z == par->getRight()) {
				//	z = z->getParent();
					rotateLeft(par);
					z = par;
					par = z->getParent();
				}
	
//				z->getParent()->setColor(BLACK);
//				z->getParent()->getParent()->setColor(RED);
				rotateRight(gp);
				color pc = par->getColor();
				color gc = gp->getColor();
				par->setColor(gc);
				gp->setColor(pc);
//		side ? rotateRight(z->getParent()->getParent()) : rotateLeft(z->getParent()->getParent());
				z = par;
			}
		} else {
			uncle = gp->getLeft();
			
			if(uncle != NULL && uncle->getColor() == RED) {
				gp->setColor(RED);
				par->setColor(BLACK);
				uncle->setColor(BLACK);
				z = gp;
			} else {
				if(z == par->getLeft()) {
					rotateRight(par);
					z = par;
					par = z->getParent();
				} 	

				rotateLeft(gp);
				color pc = par->getColor();
				color gc = gp->getColor();
				par->setColor(gc);
				gp->setColor(pc);
				z = par;
			}
		}

	}

	num++;	
	root->setColor(BLACK);
}

// Function to search a node with given data
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::search(const Key& k) {
	Node<Key, Element> *node = root;
	
	while(node != NULL) {
		if(node->getKey() > k) {
			node = node->getLeft();
		} else if(node->getKey() < k) {
			node = node->getRight();
		} else {
			return node;
		}
	}

	return NULL;
}

// Function to delete a node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::del(const Key& k) {}

// Function to do inorder traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::inorder(ostream& out) const {
	inorderHelper(out, root);
}
/*
template <typename Key, typename Element>
void RBTree<Key, Element>::inorderHelper(Node<Key, Element> *r) {
	if(r == NULL) {
		return;
	}

	inorderHelper(r->getLeft());
	cout << r->getKey() << endl;
	inorderHelper(r->getRight());
}

*/
// Function to perform level-order traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::levelOrder(ostream& out) const {
	levelorderHelper(out, root);
}

// Function to return the number of nodes in the RBTree
template <typename Key, typename Element>
size_t RBTree<Key, Element>::size() const { return num; }

// Function to return a pointer to the root of the RBTree
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::getRoot() { return root; }


// The following functions have been provided for you, and do not
// need to be modified:


// Output stream operator overload -- writes the contents of the
// entire RBTree to the specified output stream, using an in-order
// traversal. Note that there must be a similar operator overload
// for Key and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cout << rbt << endl;
template <typename Key, typename Element>
ostream& operator<<(ostream& out, const RBTree<Key, Element>& rbtree) {
	out << rbtree.size() << endl;
	rbtree.inorder(out);
	return out;
}

// Input stream operator overload -- reads the contents of an RBTree
// from the specified input stream, inserting each Node as it is
// read. Note that there must be a similar operator overload for Key
// and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cin >> rbt;
template <typename Key, typename Element>
istream& operator>>(istream& in, RBTree<Key, Element>& rbtree) {
	// Clear existing contents, if any
	rbtree = RBTree<Key, Element>();

	// Read in the number of elements to read
	size_t size;
	in >> size;
	for (size_t i = 0; i < size; i++) {
		// Read in each node as it appears in the stream
		Node<Key, Element> n;
		in >> n;
		// Insert the node into the RBTree
		rbtree.insert(n.getKey(), n.getElement());
	}

	return in;
}

#endif //PROJ4_RBTREE_H
