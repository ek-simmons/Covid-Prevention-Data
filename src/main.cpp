#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "BTree.h"
#include "Trie.h"
#include "Records.h"
using namespace std;

void readFile(const string& fileName, BTree &bTr, Trie &trie);

int main() {
    cout << "Welcome to Covid Prevention Data!" << endl;

    // set up trees
    BTree bTr();
    Trie trie();
    readFile("src/covid.csv", bTr, trie);

    while (true) {
        cout << "Enter 1 to view country data" << endl;
        cout << "Enter 2 to view behavior data" << endl;
        cout << "Enter 3 to toggle clock data" << endl;
        cout << "Enter 0 to quit" << endl;
        cout << "Input: ";

        string dataView;
        cin >> dataView;

        if (dataView == "1") {
            cout << "Enter a country (enter -1 to see list of valid countries): ";
            string country;
            cin >> country;

            if (country == "-1") {
                cout << "Countries include: ";
            }
            else {
                bTr.find(country);
                trie.find(country);
            }
        }
        else if (dataView == "2") {
            cout << "Enter a behavior (enter -1 to see list of valid behaviors): ";
            string behavior;
            cin >> behavior;

            if (behavior == "-1") {
                cout << "Behaviors include: ";
            }
            else {
                bTr.find(behavior);
                trie.find(behavior);
            }
        }
        else if (dataView == "0") {
            cout << "Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid input!" << endl;
        }
    }
}

void readFile(const string& fileName, BTree &bTr, Trie &trie) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Failed to read file" << endl;
        return;
    }

    string line;
    getline(file, line); // read header
    while (getline(file, line)) {
        vector<string> dataNode;
        stringstream lineStream(line);
        string singleData;
        while (getline(lineStream, singleData, ',')) {
            dataNode.push_back(singleData);
        }

        // categorize data based on file
        if (fileName.ends_with("covid.csv")) {
            int day = std::stoi(dataNode[0]);
            int month = std::stoi(dataNode[1]);
            int year = std::stoi(dataNode[2]);
            long long cases = std::stoll(dataNode[3]);
            long long deaths = std::stoll(dataNode[4]);
            string country = dataNode[5];
            long long population = std::stoll(dataNode[7]);

            CaseRecord cRec(country, day, month, year, cases, deaths, population);
            BTree.insert(cRec);
            Trie.insert(cRec);
        }
        else if (fileName.ends_with("covid_behaviors.csv")) {
            string country = dataNode[0];
            string days = dataNode[1];
            double contacts = std::stod(dataNode[3]);
            double timesLeftHome = std::stod(dataNode[4]);
            double masksOutside = std::stod(dataNode[9]);
            double masksGrocery = std::stod(dataNode[10]);
            double masksWork = std::stod(dataNode[12]);
            double avoidanceSymptomaticPeople = std::stod(dataNode[14]);
            double avoidanceGoingOut = std::stod(dataNode[15]);
            double avoidanceWorkOutsideHome = std::stod(dataNode[18]);
            double avoidanceGuests = std::stod(dataNode[20]);
            double avoidanceSmallGatherings = std::stod(dataNode[21]);
            double avoidanceMediumGatherings = std::stod(dataNode[22]);
            double avoidanceLargeGatherings = std::stod(dataNode[23]);
            double outlookOnCovidDanger = std::stod(dataNode[29]);
            double lifeImpacted = std::stod(dataNode[31]);

            BehaviorRecord bRec(country, days, contacts, timesLeftHome, masksOutside, masksGrocery, masksWork,
                avoidanceSymptomaticPeople, avoidanceGoingOut, avoidanceWorkOutsideHome, avoidanceGuests,
                avoidanceSmallGatherings, avoidanceMediumGatherings, avoidanceLargeGatherings,
                outlookOnCovidDanger, lifeImpacted);
            BTree.insert(bRec);
            Trie.insert(bRec);
        }
        else {
            cout << "Error: Unknown file" << endl;
            return;
        }
    }
}