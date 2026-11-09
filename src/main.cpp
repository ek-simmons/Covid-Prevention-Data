#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
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

string removeQuotes(string text);

string formatPrintStr(string str, int length);

string daysToDate(string days);

int main() {
    Trie trie;
    BTree btree;

    cout << "Loading datasets into B-Tree and Trie..." << endl;
    readFile("../src/covid.csv", trie, btree);
    readFile("../src/covid_behaviors.csv", trie, btree);

    cout << "Data loaded successfully.\n";
    cout << "Total countries stored: " << trie.getCountryCount() << endl;
    cout << "Total records stored: " << trie.getRecordCount() << endl;

    string choice;
    while (true) {
        cout << "\n--- COVID Data Tracker ---" << endl;
        cout << "1. Search Country Data\n2. Exit\nChoice: ";
        cin >> choice;

        if (choice == "2") {
            break;
        } else if (choice == "1") {
            cout << "Enter country name: ";
            string country;
            cin.ignore();
            getline(cin, country);

            if (trie.contains(country)) {
                const auto* cases = trie.getCases(country);
                const auto* behaviors = trie.getBehaviors(country);

                cout << "\n--- Records for " << country << " ---\n";
                cout << "Total Case Logs: " << cases->size() << endl;
                cout << "Total Behavior Logs: " << behaviors->size() << endl << endl;

                if (cases->size() == 0 || behaviors->size() == 0) {
                    cout << "Insufficient data to build table." << endl;
                }
                else {
                    cout << "          |       | Times Left |          | Avoidance |Gathering |          | Belief of" << endl;
                    cout << "   Date   | Cases |    Home    | Mask Use | Going Out |Attendance| Contacts |  Danger" << endl;
                    cout << "----------|-------|------------|----------|-----------|----------|----------|----------" << endl;
                }

                for (int behavIdx = 0; behavIdx < behaviors->size(); behavIdx++) {
                    int caseIdx = 0;
                    string date = daysToDate(behaviors->at(behavIdx).days);
                    bool presentInBoth = false;
                    for (; caseIdx < cases->size(); caseIdx++) {
                        string caseDate = to_string(cases->at(caseIdx).month) + "/" + to_string(cases->at(caseIdx).day) + "/" + to_string(cases->at(caseIdx).year);
                        if (date == caseDate) {
                            presentInBoth = true;
                            break;
                        }
                    }
                    if (!presentInBoth) {
                        break;
                    }

                    // string popPercent = to_string(cases->at(caseIdx).cases / cases->at(caseIdx).population * 100); // difference too small to represent in doubles
                    string maskUse = to_string((behaviors->at(behavIdx).masksGrocery + behaviors->at(behavIdx).masksOutside + behaviors->at(behavIdx).masksWork) / 3);
                    string gatherings = to_string((behaviors->at(behavIdx).avoidanceSmallGatherings + behaviors->at(behavIdx).avoidanceMediumGatherings + behaviors->at(behavIdx).avoidanceLargeGatherings) / 3);

                    cout << formatPrintStr(date, 10) << "|"
                    << formatPrintStr(to_string(cases->at(caseIdx).cases), 7) << "|"
                    << formatPrintStr(to_string(behaviors->at(behavIdx).timesLeftHome), 12) << "|"
                    << formatPrintStr(maskUse, 10) << "|"
                    << formatPrintStr(to_string(behaviors->at(behavIdx).avoidanceGoingOut), 11) << "|"
                    << formatPrintStr(gatherings, 10) << "|"
                    << formatPrintStr(to_string(behaviors->at(behavIdx).contacts), 10) << "|"
                    << formatPrintStr(to_string(behaviors->at(behavIdx).outlookOnCovidDanger), 10)
                    << endl;
                }
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
            singleData = removeQuotes(singleData);
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

        else if (fileName.find("src/covid_behaviors.csv") != string::npos && dataNode.size() >= 32) {
            BehaviorRecord b;
            b.country = dataNode[0];
            b.days = dataNode[1];
            b.contacts = safeStod(dataNode[3]);
            b.timesLeftHome = safeStod(dataNode[4]);
            b.masksOutside = safeStod(dataNode[9]);
            b.masksWork = safeStod(dataNode[12]);
            b.avoidanceSymptomaticPeople = safeStod(dataNode[14]);
            b.avoidanceGoingOut = safeStod(dataNode[15]);
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

string removeQuotes(string text) {
    return text.substr(1, text.length() - 1);
}

string formatPrintStr(string str, int length) {
    if (str.length() < length) {
        int difference = length - str.length();
        for (int i = 0; i < difference; i++) {
            str += " ";
        }
    }
    return str;
}

string daysToDate(string days) {
    chrono::year_month_day startDate = 2020y / 03 / 29;
    chrono::year_month_day date = chrono::sys_days(startDate) + chrono::days{stoi(days)};

    string dateStr = to_string(static_cast<unsigned>(date.month())) + "/" + to_string(static_cast<unsigned>(date.day())) + "/" + to_string(static_cast<int>(date.year()));
    return dateStr;
}