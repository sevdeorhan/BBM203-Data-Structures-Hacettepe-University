# 🏛️ Artifact Managment System

A sophisticated data management and assignment simulation developed for **Hacettepe University BBM203** (Data Structures). The system handles the cataloging of valuable artifacts and coordinates their distribution among specialized researchers using self-balancing search trees.

## ✨ Key Features
* **Balanced Data Management:** Implements two distinct types of self-balancing binary search trees to ensure $O(\log n)$ performance for all operations.
* **Dual-Tree Architecture:**
    * **AVL Tree:** Used for indexing **Artifacts** by their unique IDs. Features automatic rotations (Left, Right, Left-Right, Right-Left) to maintain a strict height balance.
    * **Red-Black Tree:** Used for managing **Researchers** by their names. Implements complex insertion and deletion fix-up logic using node coloring and rotations.
* **Resource Assignment Logic:** Manages the lifecycle of an artifact, from being hired/added to being requested by and returned from researchers.
* **Statistical Reporting:** Generates comprehensive system snapshots including average rarity levels, researcher loads, and tree traversals (Pre-order and Post-order).

## 🛠 Technical Stack
* **Language:** C++11.
* **Algorithms:** Self-balancing Binary Search Trees (AVL & Red-Black).
* **Memory Management:** Dynamic memory allocation with custom destructors to ensure zero memory leaks.

## 🚀 Execution
To compile the system and run the stewardship simulation:

```bash
g++ -std=c++11 *.cpp -o ArtifactManager
./ArtifactManager commands.txt output.txt
