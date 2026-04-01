#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    int destIndex = static_cast<int>(dest);
    WagonList* blocksSort[NUM_CARGOTYPES_INT] = {nullptr};
    int blockCount = 0;

    WagonList* hazardousBlock = nullptr;
    for (int cargo = 0; cargo < NUM_CARGOTYPES_INT; ++cargo) {
        WagonList& currentBlock = blockTrains[destIndex][cargo];
        if (currentBlock.isEmpty()) {
            continue;
        }
        if (static_cast<CargoType>(cargo) == CargoType::HAZARDOUS) {
            hazardousBlock = &currentBlock;
        } else {
            blocksSort[blockCount] = &currentBlock;
            blockCount++;
        }
    }
    Train* newTrain = new Train(trainName, dest);
    for (int i = 0; i < blockCount; ++i)
    {
        int heaviestWeightFound = -1;
        int heaviestIndex = -1;

        for (int j = 0; j < blockCount; ++j)
        {
            if (blocksSort[j] == nullptr) {
                continue;
            }
            int currentBlockWeight = blocksSort[j]->getFront()->getWeight();

            if (currentBlockWeight > heaviestWeightFound) {
                heaviestWeightFound = currentBlockWeight;
                heaviestIndex = j;
            }
        }
        if (heaviestIndex != -1)
        {
            newTrain->appendWagonList(*blocksSort[heaviestIndex]);

            blocksSort[heaviestIndex] = nullptr;
        }
    }

    if (hazardousBlock != nullptr && !hazardousBlock->isEmpty())
    {
        Wagon* current = hazardousBlock->getFront();
        if (current && current->getNext()) {
            Wagon* extra = current->getNext();
            while (extra != nullptr) {
                Wagon* next = extra->getNext();
                Wagon* detached = hazardousBlock->detachById(extra->getID());
                if (detached) {
                    std::cout << "Wagon " << detached->getID() << " detached from Wagon List. " << std::endl;
                    delete detached;
                }
                extra = next;
            }
        }
        newTrain->appendWagonList(*hazardousBlock);
    }
    if (newTrain->getWagons().isEmpty())
    {
        delete newTrain;
        return nullptr;
    }
    return newTrain;

}

bool ClassificationYard::isEmpty() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
        for (int j = 0; j < NUM_CARGOTYPES_INT; ++j) {
            if (!blockTrains[i][j].isEmpty())
                return false;
        }
    }
    return true;
}

void ClassificationYard::clear()
{
    for (int i=0; i<static_cast<int>(Destination::NUM_DESTINATIONS); ++i) {
        for (int j=0; j<static_cast<int>(CargoType::NUM_CARGOTYPES); ++j) {
            blockTrains[i][j].clear();
        }
    }
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}