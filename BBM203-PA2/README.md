# 🚑 QuakeAssist: Disaster Relief Management System

A mission-critical logistics and task management simulation developed for **Hacettepe University BBM203** (Data Structures). The system coordinates emergency requests and mission assignments following a disaster scenario.

## ✨ Key Features
* **Request & Mission Coordination:** Manages incoming aid requests and organizes them into executable missions based on priority and resource availability.
* **Dynamic Logistics Engine:**
    * **Queues (FIFO):** Implemented for incoming aid requests to ensure fair and organized processing.
    * **Stacks (LIFO):** Utilized for mission management, allowing for tracking and backtracking of completed tasks.
* **Emergency Response Logic:** Simulates a real-world controller that dispatches aid by matching requests with available mission stacks.
* **Robust Data Processing:** Handles complex command sets to update stocks, process requests, and generate status reports.

## 🛠 Technical Stack
* **Language:** C++11.
* **Core Structures:** Linked-List based Stacks and Queues.
* **Memory Management:** Manual dynamic memory allocation with a focus on pointer safety and memory leak prevention (RAII).

## 🚀 Execution
To compile the project and run the controller with the provided data:

```bash
g++ -std=c++11 *.cpp -o QuakeAssist
./QuakeAssist requests.txt missions.txt commands.txt output.txt
