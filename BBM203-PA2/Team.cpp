#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    int newReqLoad = req.computeWorkloadContribution();
    if (newReqLoad + currentWorkload > maxLoadCapacity) {
        return false;
    }
    if (!missionStack.push(req)) {
        return false;
    }
    currentWorkload += newReqLoad;
    return true;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.
    int count = missionStack.size();

    if (count == 0) {
        currentWorkload = 0;
        return;
    }

    Request* newArray = new Request[count];

    for (int i = 0; i < count; i++) {
        missionStack.pop(newArray[i]);
    }
    for (int i = count - 1; i >= 0; i--) {
        if (newArray[i].getType() == "SUPPLY"){
            supplyQueue.enqueue(newArray[i]);
        } else if (newArray[i].getType() == "RESCUE"){
            rescueQueue.enqueue(newArray[i]);
        }
    }
    delete[] newArray;

    missionStack.clear();
    currentWorkload = 0;
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
