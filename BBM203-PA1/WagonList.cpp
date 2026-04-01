#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    Wagon* current = front;
    while (current) {
        Wagon* next = current-> getNext();
        delete current;
        current = next;
    }
    front =  nullptr;
    rear =  nullptr;
    totalWeight = 0;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    front = other.front;
    rear  = other.rear;
    totalWeight = other.totalWeight;

    other.front = other.rear = nullptr;
    other.totalWeight = 0;
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    if (this != &other) {
        clear();
        front = other.front;
        rear  = other.rear;
        totalWeight = other.totalWeight;

        other.front = other.rear = nullptr;
        other.totalWeight = 0;
    }
    return *this;
}

Wagon *WagonList::findById(int id)
{
    Wagon* current = front;
    while (current != nullptr) {
        if (current->getID() == id)
            return current;
        current = current->getNext();
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    if (w == nullptr) return;
    w->setNext(nullptr);
    w->setPrev(nullptr);

    if (isEmpty()) {
        front = w;
        rear = w;
    }
    else {
        rear->setNext(w);
        w->setPrev(rear);
        rear = w;
    }
    totalWeight += w->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    if (front == nullptr)
        return true;
    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    if (wagon == nullptr)
        return;

    if (isEmpty()) {
        front = wagon;
        rear = wagon;
        wagon->setPrev(nullptr);
        wagon->setNext(nullptr);
    }
    else if (wagon->getWeight() > front->getWeight()) {
        wagon->setPrev(nullptr);
        wagon->setNext(front);
        front->setPrev(wagon);
        front = wagon;
    }
    else {
        Wagon* current = front;
        while (current->getNext() != nullptr) {
            Wagon* next = current->getNext();
            if (next->getWeight() < wagon->getWeight())
                break;
            current = next;
        }
        wagon->setNext(current->getNext());
        wagon->setPrev(current);

        if (current->getNext())
            current->getNext()->setPrev(wagon);
        else
            rear = wagon;
        current->setNext(wagon);
    }
    totalWeight += wagon->getWeight();
}


void WagonList::appendList(WagonList &&other)
{
    if (other.isEmpty()) {
        return;
    }
    if (isEmpty()) {
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
    }
    else {
        rear->setNext(other.front);
        other.front->setPrev(rear);
        rear = other.rear;
        totalWeight += other.totalWeight;
    }
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
}

Wagon *WagonList::detachById(int id)
{
    if (isEmpty())
        return nullptr;
    Wagon* toRemove = findById(id);
    if (toRemove == nullptr)
        return nullptr;

    if (toRemove == front) {
        front = front->getNext();
        if (front != nullptr)
            front->setPrev(nullptr);
        else
            rear = nullptr;
    }
    else if (toRemove == rear) {
        rear = rear->getPrev();
        if (rear != nullptr)
            rear->setNext(nullptr);
        else
            front = nullptr;
    }
    else {
        Wagon* prevWagon = toRemove->getPrev();
        Wagon* nextWagon = toRemove->getNext();

        prevWagon->setNext(nextWagon);
        nextWagon->setPrev(prevWagon);
    }
    totalWeight -= toRemove->getWeight();
    //std::cout << "Wagon " << id << " detached from Wagon List. " << std::endl;
    toRemove->setNext(nullptr);
    toRemove->setPrev(nullptr);
    return toRemove;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    if (isEmpty())
        return newList;

    Wagon* start = findById(id);
    if (start == nullptr)
        return newList;

    if (start == front) {
        newList.front = front;
        newList.rear = rear;
        newList.totalWeight = totalWeight;

        front = nullptr;
        rear = nullptr;
        totalWeight = 0;
        return newList;
    }
    Wagon* prev = start->getPrev();
    if (prev != nullptr)
        prev->setNext(nullptr);
    start->setPrev(nullptr);

    newList.front = start;
    newList.rear = rear;

    int newWeight = 0;
    Wagon* current = newList.front;
    while (current != nullptr) {
        newWeight += current->getWeight();
        current = current->getNext();
    }
    newList.totalWeight = newWeight;

    rear = prev;
    totalWeight -= newWeight;
    return newList;
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
