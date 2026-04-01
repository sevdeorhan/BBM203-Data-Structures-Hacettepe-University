#include "Request.h"

Request::Request()
    : id(""),
      type("SUPPLY"),
      city(""),
      supplyType("OTHER"),
      amount(0),
      numPeople(0),
      risk("LOW"),
      emergencyLevel(1) {
}

Request::Request(const std::string& id,
                 const std::string& city,
                 const std::string& supplyType,
                 int amount,
                 int emergencyLevel)
    : id(id),
      type("SUPPLY"),
      city(city),
      supplyType(supplyType),
      amount(amount),
      numPeople(0),
      risk("LOW"),
      emergencyLevel(emergencyLevel) {
}

Request::Request(const std::string& id,
                 const std::string& city,
                 int numPeople,
                 const std::string& risk,
                 int emergencyLevel)
    : id(id),
      type("RESCUE"),
      city(city),
      supplyType("OTHER"),
      amount(0),
      numPeople(numPeople),
      risk(risk),
      emergencyLevel(emergencyLevel) {
}

const std::string& Request::getType() const {
    return type;
}

const std::string& Request::getId() const {
    return id;
}

const std::string& Request::getCity() const {
    return city;
}

int Request::getEmergencyLevel() const {
    return emergencyLevel;
}

const std::string& Request::getSupplyType() const {
    return supplyType;
}

int Request::getAmount() const {
    return amount;
}

int Request::getNumPeople() const {
    return numPeople;
}

const std::string& Request::getRisk() const {
    return risk;
}

int Request::getRiskMultiplier() const {
    if (risk == "HIGH") return 3;
    if (risk == "MEDIUM") return 2;
    return 1; // LOW or anything else
}

int Request::computeEmergencyScore() const {
    // TODO: Compute emergency score as explained in the PDF.
    // SUPPLY: emergencyLevel * 10 + min(amount, 50)
    // RESCUE: emergencyLevel * 10 + numPeople * riskMultiplier
    int emergencyScore = 0;
    if (type == "SUPPLY") {
        int finishedamount = (amount > 50) ? 50 : amount;
        emergencyScore = emergencyLevel * 10 + finishedamount;
    } else if (type == "RESCUE") {
        emergencyScore = emergencyLevel * 10 + numPeople * getRiskMultiplier();
    }
    return emergencyScore;
}

int Request::computeWorkloadContribution() const {
    // TODO: Compute workload score as explained in the PDF.
    // SUPPLY: workload += amount
    // RESCUE: workload += numPeople * riskMultiplier
    if (type == "SUPPLY") {
        return amount;
    } else if (type == "RESCUE") {
        return numPeople * getRiskMultiplier();
    }
    return 0;
}
