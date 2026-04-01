#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::~TrainTrack()
{
    while (!isEmpty()) {
        Train* departed = departTrain();
        if (departed != nullptr) {
            delete departed;
        }
    }
}

// Given to you ready
std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    if (train == nullptr) {
        return;
    }
    train->setNext(nullptr);

    if (isEmpty()) {
        firstLocomotive = lastLocomotive = train;
    }
    else {
        lastLocomotive->setNext(train);
        lastLocomotive = train;
    }
    totalWeight += train->getTotalWeight();

    while (autoDispatch && totalWeight > AUTO_DISPATCH_LIMIT) {
        Train* departed = departTrain();
        if (departed != nullptr) {
            std::cout << "Train " << departed->getName()
                      << " departed from Track " << destinationToString(departed->getDestination()) << ".\n";
            std::cout << "Auto-dispatch: departing "
                      << departed->getName() << " to make room.\n";
            delete departed;
        } else break;
    }
}

Train *TrainTrack::departTrain()
{
    if (isEmpty()) return nullptr;

    Train* removed = firstLocomotive;

    firstLocomotive = removed->getNext();
    if (firstLocomotive == nullptr) {
        lastLocomotive = nullptr;
    }

    totalWeight -= removed->getTotalWeight();
    removed->setNext(nullptr);
    return removed;
}

bool TrainTrack::isEmpty() const
{
    return firstLocomotive == nullptr;
}

Train *TrainTrack::findTrain(const std::string &name) const
{
    Train* current = firstLocomotive;
    while (current) {
        if (current->getName() == name) return current;
        current = current->getNext();
    }
    return nullptr;
}

// Given to you ready
void TrainTrack::printTrack() const
{
    if (isEmpty())
        return;

    Train *current = firstLocomotive;
    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}