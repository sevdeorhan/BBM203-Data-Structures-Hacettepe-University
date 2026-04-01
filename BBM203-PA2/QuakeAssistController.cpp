#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    std::stringstream ss(line);
    std::string command;
    ss >> command;

    if (command == "INIT_TEAMS") {
        int numTeams;
        ss >> numTeams;
        return initializeTeams(numTeams);
    } else if (command == "SET_TEAM_CAPACITY") {
        int teamId, capacity;
        ss >> teamId >> capacity;
        return handleSetTeamCapacity(teamId, capacity);
    } else if (command == "ADD_SUPPLY") {
        std::string id, city, supplyType;
        int amount, emergencyLevel;
        ss >> id >> city >> supplyType >> amount >> emergencyLevel;
        return handleAddSupply(id, city, supplyType, amount, emergencyLevel);
    } else if (command == "ADD_RESCUE") {
        std::string id, city, buildingRisk;
        int numPeople, emergencyLevel;
        ss >> id >> city >> numPeople >> buildingRisk >> emergencyLevel;
        return handleAddRescue(id, city, numPeople, buildingRisk , emergencyLevel);
    } else if (command == "REMOVE_REQUEST") {
        std::string id;
        ss >> id;
        return handleRemoveRequest(id);
    } else if (command == "HANDLE_EMERGENCY") {
        int teamId, k ;
        ss >> teamId >> k;
        return handleHandleEmergency(teamId, k);
    } else if (command == "DISPATCH_TEAM") {
        int teamId;
        ss >> teamId;
        return handleDispatchTeam(teamId);
    } else if (command == "PRINT_QUEUES") {
        printQueues();
        return true;
    } else if (command == "PRINT_TEAM") {
        int teamId;
        ss >> teamId;
        printTeam(teamId);
        return true;
    } else if (command == "CLEAR") {
        clear();
        return false;
    } else {
        std::cout << "Error: Unknown command '" << command << "'." << std::endl;
    }
    return true;
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    if (teams != nullptr) {
        delete[] teams;
        teams = nullptr;
    }
    teamCount = numTeams;
    teams = new Team[teamCount];

    for (int i = 0; i < teamCount; i++) {
        teams[i].setId(i);
    }
    std::cout << "Initialized " << teamCount << " teams." << std::endl;
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    for (int i = 0; i < teamCount; i++) {
        if (teams[i].getId() == teamId) {
            return i;
        }
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    int index = findTeamIndexById(teamId);
    if (index == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    teams[index].setMaxLoadCapacity(capacity);
    std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request req(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    if (supplyQueue.enqueue(req)) {
        std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
        return true;
    }
    std::cout << "Error: Memory allocation failed for request " << id << "." << std::endl; //baakkkkk
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request req(id, cityStr, numPeople, riskStr, emergencyLevel);
    if (rescueQueue.enqueue(req)) {
        std::cout << "Request " << id << " added to RESCUE queue." << std::endl;
        return true;
    }
    std::cout << "Error: Memory allocation failed for request " << id << "." << std::endl;  //baakkk
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue.
    if (supplyQueue.removeById(id)) {
        std::cout << "Request " << id << " removed from queues." << std::endl;
        return true;
    }
    if (rescueQueue.removeById(id)) {
        std::cout << "Request " << id << " removed from queues." << std::endl;
        return true;
    }
    std::cout << "Error: Request " << id << " not found." << std::endl;
    return true;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {    //???????
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }

    Team& team = teams[idx];

    int supplyCount = 0;
    int rescueCount = 0;

    for (int i = 0; i < k; i++) {
        if (supplyQueue.isEmpty() && rescueQueue.isEmpty()) {
            break;
        }

        Request sReq, rReq;
        bool hasSupply = supplyQueue.peek(sReq);
        bool hasRescue = rescueQueue.peek(rReq);

        Request chosenReq;
        bool isSupply = false;

        if (hasSupply && hasRescue) {
            int sScore = sReq.computeEmergencyScore();
            int rScore = rReq.computeEmergencyScore();

            if (sScore > rScore) {
                chosenReq = sReq;
                isSupply = true;
            } else {
                chosenReq = rReq;
                isSupply = false;
            }
        } else if (hasSupply) {
            chosenReq = sReq;
            isSupply = true;
        } else {
            chosenReq = rReq;
            isSupply = false;
        }

        if (!team.tryAssignRequest(chosenReq)) {
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            team.rollbackMission(supplyQueue, rescueQueue);
            break;
        }

        Request temp;
        if (isSupply) {
            supplyQueue.dequeue(temp);
            supplyCount++;
        } else {
            rescueQueue.dequeue(temp);
            rescueCount++;
        }
    }

    if (team.hasActiveMission()) {
        std::cout << "Team " << teamId << " assigned "
                  << team.getMissionStack().size() << " requests ("
                  << supplyCount << " SUPPLY, " << rescueCount << " RESCUE), total workload "
                  << team.getCurrentWorkload() << "." << std::endl;
    }

    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    std::cout << "SUPPLY QUEUE:" << std::endl;
    if (!supplyQueue.isEmpty()) {
        Request* data = supplyQueue.getData();
        int capacity = supplyQueue.getCapacity();
        int front = supplyQueue.getFrontIndex();
        int count = supplyQueue.getCount();
        for (int i = 0; i < count; ++i) {
            int index = (front + i) % capacity;
            const Request& req = data[index];

            std::cout << req.getId() << " "
                      << req.getCity() << " "
                      << req.getSupplyType() << " "
                      << req.getAmount() << " "
                      << req.getEmergencyLevel() << std::endl;
        }
    }
    std::cout << "RESCUE QUEUE:" << std::endl;
    if (!rescueQueue.isEmpty()) {
        Request* data = rescueQueue.getData();
        int capacity = rescueQueue.getCapacity();
        int front = rescueQueue.getFrontIndex();
        int count = rescueQueue.getCount();
        for (int i = 0; i < count; ++i) {
            int index = (front + i) % capacity;
            const Request& req = data[index];
            std::cout << req.getId() << " "
            << req.getCity() << " "
            << req.getNumPeople() << " "
            << req.getRisk() << " "
            << req.getEmergencyLevel() << std::endl;
        }
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }
    const Team& team = teams[idx];
    const MissionStack& stack = team.getMissionStack();

    std::cout << "TEAM " << teamId << " STACK:" << std::endl;
    if (!stack.isEmpty()) {
        Request* data = stack.getData();
        int top = stack.getTopIndex();

        for (int i = top; i >= 0; --i) {
            const Request& req = data[i];

            if (req.getType() == "SUPPLY") {
                std::cout << req.getId() << " "
                << req.getCity() << " "
                << req.getSupplyType() << " "
                << req.getAmount() << " "
                << req.getEmergencyLevel() << std::endl;
            } else if (req.getType() == "RESCUE") {
                std::cout << req.getId() << " "
                << req.getCity() << " "
                << req.getNumPeople() << " "
                << req.getRisk() << " "
                << req.getEmergencyLevel() << std::endl;
            }
        }
    }

}

void QuakeAssistController::clear() {
    //Clear data.
    supplyQueue.clear();
    rescueQueue.clear();

    if (teams != nullptr) {
        for (int i = 0; i < teamCount; ++i) {
            teams[i].clearMission();
        }
    }

    std::cout << "System cleared." << std::endl;
}
