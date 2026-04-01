# 🚂 Rail Marshal: Railway Management System

A high-performance railway management and logistics simulation developed for **Hacettepe University BBM203** (Data Structures). The project focuses on efficient data organization and algorithmic sorting within a train classification yard.

## ✨ Key Features
* **Train Classification Yard:** Simulates a real-world marshalling yard where trains are disassembled and reassembled based on their destinations.
* **Dynamic Logistics:** Manages train arrivals and departures across multiple tracks using advanced data structures.
* **Complex Data Handling:**
    * **Linked Lists:** Used to represent train cars and tracks for flexible memory management.
    * **Stacks (LIFO):** Implemented for classification tracks to simulate the "first-in, last-out" nature of dead-end railway sidings.
* **Efficient Sorting:** Groups train cars by destination to optimize departure sequences.

## 🛠 Technical Stack
* **Language:** C++11.
* **Core Structures:** Doubly Linked Lists, Stacks, and custom Node implementations.
* **Memory Management:** Manual memory allocation and deallocation (RAII principles) to prevent memory leaks in a dynamic environment.

## 🚀 Execution
To compile the project and run the simulation with a command file:

```bash
g++ -std=c++11 *.cpp -o RailMarshal
./RailMarshal initial_tracks.txt commands.txt output.txt
