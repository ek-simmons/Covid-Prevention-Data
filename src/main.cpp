#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "BTree.h"
#include "Trie.h"
#include "Records.h"
using namespace std;

long long safeStoll(const string& s) {
    try { return s.empty() ? 0 : stoll(s); } catch (...) { return 0; }
}
int safeStoi(const string& s) {
    try { return s.empty() ? 0 : stoi(s); } catch (...) { return 0; }
}
double safeStod(const string& s) {
    try { return s.empty() ? 0.0 : stod(s); } catch (...) { return 0.0; }
}

void readFile(const string& fileName, Trie& trie, BTree& btree);

int main() {
    Trie trie;
    BTree btree;

    cout << "Loading datasets into B-Tree and Trie..." << endl;
    readFile("covid.csv", trie, btree);
    readFile("covid_behaviors.csv", trie, btree);

    cout << "Data loaded successfully.\n";
    cout << "Total countries stored: " << trie.getCountryCount() << endl;
    cout << "Total records stored: " << trie.getRecordCount() << endl;

    int choice;
    while (true) {
        cout << "\n--- COVID Data Tracker ---" << endl;
        cout << "1. Search Country Data\n2. Exit\nChoice: ";
        cin >> choice;

        if (choice == 2) {
            break;
        } else if (choice == 1) {
            cout << "Enter country name: ";
            string country;
            cin.ignore();
            getline(cin, country);

            if (trie.contains(country)) {
                const auto* cases = trie.getCases(country);
                const auto* behaviors = trie.getBehaviors(country);

                cout << "\n--- Records for " << country << " ---\n";
                cout << "Total Case Logs: " << (cases ? cases->size() : 0) << endl;
                cout << "Total Behavior Logs: " << (behaviors ? behaviors->size() : 0) << endl;
            } else {
                cout << "\nCountry not found in the database.\n";
            }
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}

void readFile(const string& fileName, Trie& trie, BTree& btree) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Failed to read file " << fileName << endl;
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
        if (fileName.find("covid.csv") != string::npos && dataNode.size() >= 8) {
            CaseRecord c;
            c.day = safeStoi(dataNode[0]);
            c.month = safeStoi(dataNode[1]);
            c.year = safeStoi(dataNode[2]);
            c.cases = safeStoll(dataNode[3]);
            c.deaths = safeStoll(dataNode[4]);
            c.country = dataNode[5];
            c.population = safeStoll(dataNode[7]);

            trie.insertCase(c);
            btree.insertCase(c);
        }

        else if (fileName.find("covid_behaviors.csv") != string::npos && dataNode.size() >= 32) {
            BehaviorRecord b;
            b.country = dataNode[0];
            b.days = dataNode[1];
            b.contacts = safeStod(dataNode[3]);
            b.timesLeftHome = safeStod(dataNode[4]);
            b.masksOutside = safeStod(dataNode[9]);
            b.masksWork = safeStod(dataNode[12]);
            b.avoidanceSymptomaticPeople = safeStod(dataNode[14]);
            b.avoidanceWorkOutsideHome = safeStod(dataNode[18]);
            b.avoidanceGuests = safeStod(dataNode[20]);
            b.avoidanceSmallGatherings = safeStod(dataNode[21]);
            b.avoidanceMediumGatherings = safeStod(dataNode[22]);
            b.avoidanceLargeGatherings = safeStod(dataNode[23]);
            b.outlookOnCovidDanger = safeStod(dataNode[29]);
            b.lifeImpacted = safeStod(dataNode[31]);

            trie.insertBehavior(b);
            btree.insertBehavior(b);
        }
    }
}