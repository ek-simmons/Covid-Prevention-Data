#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "BTree.h"
#include "Trie.h"
using namespace std;

void readFile(const string& fileName);

int main() {

}

void readFile(const string& fileName) {
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
            string day = dataNode[0];
            string month = dataNode[1];
            string year = dataNode[2];
            string cases = dataNode[3];
            string deaths = dataNode[4];
            string country = dataNode[5];
            string population = dataNode[7];
        }
        else if (fileName.ends_with("covid_behaviors.csv")) {
            string country = dataNode[0];
            string days = dataNode[1];
            string contacts = dataNode[3];
            string timesLeftHome = dataNode[4];
            string masksOutside = dataNode[9];
            string masksGrocery = dataNode[10];
            string masksWork = dataNode[12];
            string avoidanceSymptomaticPeople = dataNode[14];
            string avoidanceGoingOut = dataNode[15];
            string avoidanceWorkOutsideHome = dataNode[18];
            string avoidanceGuests = dataNode[20];
            string avoidanceSmallGatherings = dataNode[21];
            string avoidanceMediumGatherings = dataNode[22];
            string avoidanceLargeGatherings = dataNode[23];
            string outlookOnCovidDanger = dataNode[29];
            string lifeImpacted = dataNode[31];
        }
        else {
            cout << "Error: Unknown file" << endl;
            return;
        }
    }
}