#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    wagons.clear();
    totalWeight = 0;
    nextLocomotive = nullptr;
}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{
    if (wagons.isEmpty())
        return nullptr;

    Wagon* current = wagons.getRear();
    int runningWeight = 0;
    Wagon* overloaded = nullptr;

    while (current != nullptr) {
        if (runningWeight > current->getMaxCouplerLoad()) {
            overloaded = current;
            break;
        }
        runningWeight += current->getWeight();
        current = current->getPrev();
    }
    if (overloaded == nullptr)
        return nullptr;

    Wagon* splitPoint = overloaded->getNext();
    if (splitPoint == nullptr)
        return nullptr;

    WagonList newWagonList = wagons.splitAtById(splitPoint->getID());

    std::string newName = name + "_split_" + std::to_string(splitCounter);
    Train* newTrain = new Train(newName, destination);
    newTrain->appendWagonList(newWagonList);

    totalWeight = wagons.getTotalWeight();

    std::cout << "Train " << name << " split due to coupler overload before Wagon "
              << splitPoint->getID() << std::endl;
    std::cout<< newTrain->wagons << std::endl;
    return newTrain;
}