#include "AVLTree.h"

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
    if (node)
    {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root
    ArtifactNode* y = x->right;
    ArtifactNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    int hLeftX  = height(x->left);
    int hRightX = height(x->right);

    if (hLeftX > hRightX) {
        x->height = hLeftX + 1;
    } else {
        x->height = hRightX + 1;
    }

    int hLeftY  = height(y->left);
    int hRightY = height(y->right);

    if (hLeftY > hRightY) {
        y->height = hLeftY + 1;
    } else {
        y->height = hRightY + 1;
    }
    return y;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).
    ArtifactNode* x = y->left;
    ArtifactNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    int hLeftY  = height(y->left);
    int hRightY = height(y->right);

    if (hLeftY > hRightY) {
        y->height = hLeftY + 1;
    } else {
        y->height = hRightY + 1;
    }

    int hLeftX  = height(x->left);
    int hRightX = height(x->right);

    if (hLeftX > hRightX) {
        x->height = hLeftX + 1;
    } else {
        x->height = hRightX + 1;
    }
    return x;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.
    if (node == nullptr) {
        return nullptr;
    }
    if (node->left) {
        return findMinNode(node->left);
    }
    return node;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.
    if (node == nullptr) {
        inserted = true;
        return new ArtifactNode(artifact);
    }
    if (artifact.artifactID < node->data.artifactID) {
        node->left = insert(node->left, artifact, inserted);
    } else if (artifact.artifactID > node->data.artifactID) {
        node->right = insert(node->right, artifact, inserted);
    } else {
        inserted = false;
        return node;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && artifact.artifactID < node->left->data.artifactID) {
        return rotateRight(node);
    }
    if (balance < -1 && artifact.artifactID > node->right->data.artifactID) {
        return rotateLeft(node);
    }
    if (balance > 1 && artifact.artifactID > node->left->data.artifactID) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && artifact.artifactID < node->right->data.artifactID) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.
    if (node == nullptr) {
        return node;
    }
    if (artifactID < node->data.artifactID) {
        node->left = remove(node->left, artifactID, removed);
    } else if (artifactID > node->data.artifactID) {
        node->right = remove(node->right, artifactID, removed);
    } else {
        removed = true;
        if ((node->left == nullptr) || (node->right == nullptr)) {

            ArtifactNode *temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            ArtifactNode *temp = findMinNode(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data.artifactID, removed);
        }
    }
    if (node == nullptr) {
        return node;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO
    if (node == nullptr) {
        return nullptr;
    }
    if (artifactID < node->data.artifactID) {
        return find(node->left, artifactID);
    } else if (artifactID > node->data.artifactID) {
        return find(node->right, artifactID);
    } else {
        return node;
    }
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{
    //assign artifact to researcher
    ArtifactNode *node = findArtifact(artifactID);
    if (node) {
        node->data.assignedToName = researcherName;
    }
}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode *node = findArtifact(artifactID);
    if (node) {
        node->data.assignedToName = "";
    }
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>
    if (node) {
        printUnassigned(node->left);
        if (node->data.assignedToName == "") {
            std::cout << node->data.artifactID << " "
                      << node->data.name << " "
                      << node->data.rarityLevel << " "
                      << node->data.researchValue << std::endl;
        }
        printUnassigned(node->right);
    }
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.
    if (node == nullptr) {
        return 0;
    }
    return 1 + getArtifactCount(node->left) + getArtifactCount(node->right);
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.
    if (node == nullptr) {
        return 0;
    }
    return node->data.rarityLevel + getTotalRarity(node->left) + getTotalRarity(node->right);
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.
    if (node) {
        traversePostOrderForStats(node->left);
        traversePostOrderForStats(node->right);
        std::cout << node->data.artifactID << " "
                  << node->data.name << " "
                  << node->data.rarityLevel << " "
                  << node->data.researchValue << " ";

        if (node->data.assignedToName == "") {
            std::cout << "UNASSIGNED" << std::endl;
        } else {
            std::cout << node->data.assignedToName << std::endl;
        }
    }
}

void AVLTree::printMatchingRarity(int minRarity) const {
    printMatchingRarity(root, minRarity);
}

void AVLTree::printMatchingRarity(ArtifactNode *node, int minRarity) const {
    if (node) {
        printMatchingRarity(node->left, minRarity);
        if (node->data.rarityLevel >= minRarity) {
            std::cout << node->data.artifactID << " "
                      << node->data.name << " "
                      << node->data.rarityLevel << " "
                      << node->data.researchValue;

            if (node->data.assignedToName != "") {
                std::cout << " ASSIGNED:" << node->data.assignedToName << std::endl;
            } else {
                std::cout << " UNASSIGNED" << std::endl;
            }
        }
        printMatchingRarity(node->right, minRarity);
    }
}