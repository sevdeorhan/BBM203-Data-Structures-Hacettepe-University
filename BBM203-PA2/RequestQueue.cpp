#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    if (isFull()) {

        int newCapacity = (capacity == 0) ? 4 : capacity * 2;

        if (!resize(newCapacity)) {
            return false;
        }
    }
    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % capacity;
    }
    data[rear] = req;
    count++;

    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    if (isEmpty()){
        return false;
    }
    outReq = data[front];

    front = (front + 1) % capacity;
    count--;

    if (count == 0) {
        front = 0;
        rear = 0;
    }
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if (isEmpty()) {
        return false;
    }
    outReq = data[front];
    return true;

}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    if (isEmpty()) {
        return false;
    }

    int removeidx = -1;

    for (int i = 0; i < count; i++) {
        int curridx = (front + i) % capacity;
        if (data[curridx].getId() == id) {
            removeidx = curridx;
            break;
        }
    }
    if (removeidx == -1) {
        return false;
    }

    while (removeidx != rear) {
        int nextidx = (removeidx + 1) % capacity;
        data[removeidx] = data[nextidx];
        removeidx = nextidx;
    }

    rear = (rear - 1 + capacity) % capacity;
    count--;

    if (count == 0) {
        front = -1;
        rear = -1;
    }

    return true;

}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    if (newCapacity <= count) {
        return false;
    }

    Request* newData = new Request[newCapacity];

    int current = front;
    for (int i = 0; i < count; i++) {
        newData[i] = data[current];
        current = nextIndex(current);
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
    if (count == 0) {
        front = 0;
        rear  = 0;
    } else {
        front = 0;
        rear  = count - 1;
    }
    return true;
}
