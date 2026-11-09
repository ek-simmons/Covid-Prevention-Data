//
// Created by Sandra Salkini on 11/5/26.
//

#ifndef COVID_PREVENTION_DATA_RECORDS_H
#define COVID_PREVENTION_DATA_RECORDS_H

#endif //COVID_PREVENTION_DATA_RECORDS_H
#pragma once
#include <string>
#include <iostream>

struct CaseRecord {
    int day;
    int month;
    int year;
    long long cases;
    long long deaths;
    std::string country;
    long long population;

    CaseRecord() {
        day = 0;
        month = 0;
        year = 0;
        cases = 0;
        deaths = 0;
        country = "undefined";
        population = 0;
    }

    CaseRecord(std::string country, int day, int month, int year, long long cases, long long deaths, long long population) {
        this->country = country;
        this->day = day;
        this->month = month;
        this->year = year;
        this->cases = cases;
        this->deaths = deaths;
        this->population = population;
    }
};

struct BehaviorRecord {
    std::string country;
    std::string days;
    double contacts = 0;
    double timesLeftHome = 0;
    double masksOutside = 0;
    double masksGrocery = 0;
    double masksWork = 0;
    double avoidanceSymptomaticPeople = 0;
    double avoidanceGoingOut = 0;
    double avoidanceWorkOutsideHome = 0;
    double avoidanceGuests = 0;
    double avoidanceSmallGatherings = 0;
    double avoidanceMediumGatherings = 0;
    double avoidanceLargeGatherings = 0;
    double outlookOnCovidDanger = 0;
    double lifeImpacted = 0;

    BehaviorRecord() {
        country = "undefined";
        days = "0";
        contacts = 0;
        timesLeftHome = 0;
        masksOutside = 0;
        masksGrocery = 0;
        masksWork = 0;
        avoidanceSymptomaticPeople = 0;
        avoidanceGoingOut = 0;
        avoidanceWorkOutsideHome = 0;
        avoidanceGuests = 0;
        avoidanceSmallGatherings = 0;
        avoidanceMediumGatherings = 0;
        avoidanceLargeGatherings = 0;
        outlookOnCovidDanger = 0;
        lifeImpacted = 0;
    }

    BehaviorRecord(std::string country, std::string days, double contacts, double timesLeftHome,
        double masksOutside, double masksGrocery, double masksWork, double avoidanceSymptomaticPeople,
        double avoidanceGoingOut, double avoidanceWorkOutsideHome, double avoidanceGuests,
        double avoidanceSmallGatherings, double avoidanceMediumGatherings,
        double avoidanceLargeGatherings, double outlookOnCovidDanger, double lifeImpacted) {
        this->country = country;
        this->days = days;
        this->contacts = contacts;
        this->timesLeftHome = timesLeftHome;
        this->masksOutside = masksOutside;
        this->masksGrocery = masksGrocery;
        this->masksWork = masksWork;
        this->avoidanceSymptomaticPeople = avoidanceSymptomaticPeople;
        this->avoidanceGoingOut = avoidanceGoingOut;
        this->avoidanceWorkOutsideHome = avoidanceWorkOutsideHome;
        this->avoidanceGuests = avoidanceGuests;
        this->avoidanceSmallGatherings = avoidanceSmallGatherings;
        this->avoidanceMediumGatherings = avoidanceMediumGatherings;
        this->avoidanceLargeGatherings = avoidanceLargeGatherings;
        this->outlookOnCovidDanger = outlookOnCovidDanger;
        this->lifeImpacted = lifeImpacted;
    }
};
