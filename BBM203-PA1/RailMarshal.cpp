#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
        departureYard[i] = TrainTrack(static_cast<Destination>(i));
    }
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "ADD_WAGON") {
        int id, weight, maxLoad;
        std::string cargoStr, destStr;

        if (!(ss >> id >> cargoStr >> destStr >> weight >> maxLoad)) {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }

        CargoType cargo = parseCargo(cargoStr);
        Destination dest = parseDestination(destStr);

        Wagon* w = new Wagon(id, cargo, dest, weight, maxLoad);
        classificationYard.insertWagon(w);
        std::cout << "Wagon " << *w << " added to yard." << std::endl;

    } else if (cmd == "REMOVE_WAGON") {
        int id;
        if (!(ss >> id)) {
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }

        Wagon* removedWagon = nullptr;
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {
            for (int j = 0; j < NUM_CARGOTYPES_INT; ++j) {
                removedWagon = classificationYard.getBlockTrain(i, j).detachById(id);
                if (removedWagon != nullptr) {
                    break;
                }
            }
            if (removedWagon != nullptr) {
                break;
            }
        }

        if (removedWagon != nullptr) {
            std::cout << "Wagon " << id << " detached from Wagon List. " << std::endl;
            std::cout << "Wagon " << id << " removed." << std::endl;
            delete removedWagon;
        } else {
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
        }

    } else if (cmd == "ASSEMBLE_TRAIN") {
        std::string destStr;
        if (!(ss >> destStr)) {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }

        Destination dest = parseDestination(destStr);
        TrainTrack& track = getDepartureYard(dest);

        std::string trainName = track.generateTrainName();

        Train* newTrain = classificationYard.assembleTrain(dest, trainName);

        if (newTrain == nullptr) {
            std::cout << "No wagons to assemble for " << destStr << std::endl;
        } else {
            Train* trainToVerify = newTrain;
            int splitCounter = 1;

            while (true) {
                Train* splitPart = trainToVerify->verifyCouplersAndSplit(splitCounter++);

                if (splitPart == nullptr) {
                    break;
                } else {
                    track.addTrain(splitPart);
                    std::cout << "Train " << splitPart->getName() << " assembled after split with "
                              << splitPart->getWagons() << " wagons." << std::endl;
                }
            }
            std::cout << "Train " << newTrain->getName() << " assembled with " << newTrain->getWagons() << " wagons." << std::endl;
            track.addTrain(newTrain);
        }

    } else if (cmd == "DISPATCH_TRAIN") {
        std::string destStr;
        if (!(ss >> destStr)) {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        dispatchFromTrack(dest);

    } else if (cmd == "PRINT_YARD") {
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();

    } else if (cmd == "PRINT_TRACK") {
        std::string destStr;
        if (!(ss >> destStr)) {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        getDepartureYard(dest).printTrack();

    } else if (cmd == "AUTO_DISPATCH") {
        std::string onOff;
        if (!(ss >> onOff)) {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }
        if (onOff == "ON" || onOff == "on") {
            TrainTrack::autoDispatch = true;
            std::cout << "Auto dispatch enabled\n";
        } else if (onOff == "OFF" || onOff == "off") {
            TrainTrack::autoDispatch = false;
            std::cout << "Auto dispatch disabled\n";
        } else {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }

    } else if (cmd == "CLEAR") {
        classificationYard.clear();
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i) {

            TrainTrack& track = departureYard[i];
            while (!track.isEmpty()) {
                Train* t = track.departTrain();
                if (t != nullptr) {
                    delete t;
                }
            }
            departureYard[i] = TrainTrack(static_cast<Destination>(i));
        }
        std::cout << "System cleared." << std::endl;

    } else {
        std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    }
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    int destIndex = static_cast<int>(track);
    TrainTrack& tr = departureYard[destIndex];

    if (tr.isEmpty()) {
        std::cout << "Error: No trains to dispatch from Track " << destinationToString(track) << ".\n";
        return;
    }

    Train* t = tr.departTrain();
    if (t != nullptr) {
        std::cout << "Train " << t->getName()
          << " departed from Track " << destinationToString(track) << ".\n";
        std::cout << "Dispatching " << t->getName()
                  << " (" << t->getTotalWeight() << " tons)." << std::endl;
        delete t;
    }
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
