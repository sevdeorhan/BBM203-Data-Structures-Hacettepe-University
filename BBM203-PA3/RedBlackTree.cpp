#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name
    if (node == nullptr) {
        return nullptr;
    }
    if (fullName == node->data.fullName) {
        return node;
    }
    if (fullName < node->data.fullName) {
        return find(node->left, fullName);
    }
    return find(node->right, fullName);
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise
    ResearcherNode *newNode = new ResearcherNode(researcher);

    if (newNode->data.capacity > 0) {
        newNode->data.assignedArtifacts = new int[newNode->data.capacity]();
        for (int i = 0; i < newNode->data.numAssigned; ++i) {
            newNode->data.assignedArtifacts[i] = researcher.assignedArtifacts[i];
        }
    } else {
        newNode->data.assignedArtifacts = nullptr;
    }

    bool inserted = false;
    root = bstInsert(root, newNode, inserted);

    if (!inserted) {
        delete newNode;
        return false;
    }
    insertFixup(newNode);
    return true;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).
    if (current == nullptr) {
        inserted = true;
        return node;
    }
    if (node->data.fullName < current->data.fullName) {
        current->left = bstInsert(current->left, node, inserted);
        if (current->left != nullptr) {
            current->left->parent = current;
        }
    } else if (node->data.fullName > current->data.fullName) {
        current->right = bstInsert(current->right, node, inserted);
        if (current->right != nullptr) {
            current->right->parent = current;
        }
    } else {
        inserted = false;
    }
    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.
    while (node != root && node->parent->color == RED) {
        ResearcherNode* parent = node->parent;
        ResearcherNode* grandParent = parent->parent;
        if (parent == grandParent->left){
            ResearcherNode* uncle = grandParent->right;
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                node = grandParent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    grandParent = parent->parent;
                }
                parent->color = BLACK;
                grandParent->color = RED;
                rotateRight(grandParent);
            }
        } else {
            ResearcherNode* uncle = grandParent->left;
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                node = grandParent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    grandParent = parent->parent;
                }
                parent->color = BLACK;
                grandParent->color = RED;
                rotateLeft(grandParent);
            }
        }
    }
    root->color = BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName) //????????
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.
    ResearcherNode *nodeToDelete = find(root, fullName);
    if (!nodeToDelete) {
        return false;
    }

    ResearcherNode *nodeToMove = ((nodeToDelete->left == nullptr) || (nodeToDelete->right == nullptr)) ? nodeToDelete : minimum(nodeToDelete->right);
    ResearcherNode *child = (nodeToMove->left != nullptr) ? nodeToMove->left : nodeToMove->right;

    ResearcherNode *xParent = nodeToMove->parent;
    if (child != nullptr) {
        child->parent = xParent;
    }

    if (nodeToMove->parent == nullptr) {
        root = child;
    } else if (nodeToMove == nodeToMove->parent->left) {
        nodeToMove->parent->left = child;
    } else {
        nodeToMove->parent->right = child;
    }

    if (nodeToMove != nodeToDelete) {
        if (nodeToDelete->data.assignedArtifacts != nullptr) {
            delete[] nodeToDelete->data.assignedArtifacts;
        }

        nodeToDelete->data.fullName = nodeToMove->data.fullName;
        nodeToDelete->data.capacity = nodeToMove->data.capacity;
        nodeToDelete->data.numAssigned = nodeToMove->data.numAssigned;

        nodeToDelete->data.assignedArtifacts = nodeToMove->data.assignedArtifacts;

        nodeToMove->data.assignedArtifacts = nullptr;
    }

    if (nodeToMove->color == BLACK) {
        deleteFixup(child, xParent);
    }
    delete nodeToMove;
    return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.
    ResearcherNode *current = node;
    ResearcherNode *xParent = parent;

    while (current != root && (current == nullptr || current->color == BLACK)) {
        if (current == xParent->left) {
            ResearcherNode *sibling = xParent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                xParent->color = RED;
                rotateLeft(xParent);
                sibling = xParent->right;
            }

            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK)) {
                sibling->color = RED;
                current = xParent;
                xParent = current->parent;
            } else {
                if (sibling->right == nullptr || sibling->right->color == BLACK) {
                    if (sibling->left) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = xParent->right;
                }

                sibling->color = xParent->color;
                xParent->color = BLACK;
                if (sibling->right) {
                    sibling->right->color = BLACK;
                }
                rotateLeft(xParent);
                current = root;
                xParent = nullptr;
            }
        } else {
            ResearcherNode *sibling = xParent->left;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                xParent->color = RED;
                rotateRight(xParent);
                sibling= xParent->left;
            }

            if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                (sibling->left == nullptr || sibling->left->color == BLACK)) {
                sibling->color = RED;
                current = xParent;
                xParent = current->parent;
            } else {
                if (sibling->left == nullptr || sibling->left->color == BLACK) {
                    if (sibling->right) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = xParent->left;
                }

                sibling->color = xParent->color;
                xParent->color = BLACK;
                if (sibling->left) {
                    sibling->left->color = BLACK;
                }
                rotateRight(xParent);
                current = root;
                xParent = nullptr;
            }
        }
    }
    if (current) {
        current->color = BLACK;
    }
}

ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    if (node == nullptr) {
        return nullptr;
    }
    if (node->left) {
        return minimum(node->left);
    }
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.
    if (x == nullptr || x->right == nullptr) {
        return;
    }

    ResearcherNode *rightChild = x->right;
    x->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = x;
    }

    rightChild->parent = x->parent;
    if (x->parent == nullptr) {
        root = rightChild;
    } else if (x == x->parent->left) {
        x->parent->left = rightChild;
    } else {
        x->parent->right = rightChild;
    }

    rightChild->left = x;
    x->parent = rightChild;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    if (y == nullptr || y->left == nullptr) {
        return;
    }
    ResearcherNode *leftChild = y->left;
    y->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = y;
    }

    leftChild->parent = y->parent;

    if (y->parent == nullptr) {
        root = leftChild;
    } else if (y == y->parent->right) {
        y->parent->right = leftChild;
    } else {
        y->parent->left = leftChild;
    }

    leftChild->right = y;
    y->parent = leftChild;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if (node == nullptr) {
        return 0;
    }
    return 1 + getResearcherCount(node->left) + getResearcherCount(node->right);
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if (node == nullptr) {
        return 0;
    }
    return node->data.numAssigned + getTotalLoad(node->left) + getTotalLoad(node->right);

}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.
    if (node) {
        std::cout << node->data.fullName << " "
        << node->data.capacity << " "
        << node->data.numAssigned << std::endl;

        traversePreOrderForStats(node->left);
        traversePreOrderForStats(node->right);
    }
}
