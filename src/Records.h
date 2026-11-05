//
// Created by Sandra Salkini on 11/5/26.
//

#ifndef COVID_PREVENTION_DATA_RECORDS_H
#define COVID_PREVENTION_DATA_RECORDS_H

#endif //COVID_PREVENTION_DATA_RECORDS_H
#pragma once
#include <string>

struct CaseRecord {
    int day = 0;
    int month = 0;
    int year = 0;
    long long cases = 0;
    long long deaths = 0;
    std::string country;
    long long population = 0;
};

struct BehaviorRecord {
    std::string country;
    std::string days;
    double contacts = 0;
    double timesLeftHome = 0;
    double masksOutside = 0;
    double masksWork = 0;
    double avoidanceSymptomaticPeople = 0;
    double avoidanceWorkOutsideHome = 0;
    double avoidanceGuests = 0;
    double avoidanceSmallGatherings = 0;
    double avoidanceMediumGatherings = 0;
    double avoidanceLargeGatherings = 0;
    double outlookOnCovidDanger = 0;
    double lifeImpacted = 0;
};
