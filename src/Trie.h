#ifndef COVIDPREVENTIONDATA_TRIE_H
#define COVIDPREVENTIONDATA_TRIE_H
#pragma once
#include <string>
#include <vector>

class Trie {
private:
    const int ABC_SIZE = 256;

    struct Node {
        Node* children[ABC_SIZE];
        bool isEnd;
        std::string displayName;
        std::vector<CaseRecord> cases;
        std::vector<BehaviorRecord> behaviors;
        Node() {
            isEnd = false;
            for (int i = 0; i < ABC_SIZE; i++) {
                children[i] = nullptr;
            }
        }
    };
    Node* root;
    int countryCount; //num of diff countries stored
    long long recordCount; //total num of rows stored

    std::string normalize(const std::string& name);
    std::string fixedDisplayName(const std::string& name);
    Node* findNode(const std::string& normalizedKey) const;
    Node* getOrCreateNode(const std::string& normalizedKey) const;
    void collectNames (Node* node, std::vector<std::string>& names) const;
    void destroy(Node* node);

public:
    Trie();
    ~Trie();

    //Insert O(L)
    void insertCase(const CaseRecord& record);
    void insertBehavior(const BehaviorRecord& record);

    //Search O(L)
    const std::vector<CaseRecord>& getCases() (const std::string& country) const;
    const std::vector<BehaviorRecord>& getBehaviors() (const std::string& country) const;
    bool contains (const std::string& country) const;

    //autocomplete
    std::vector<std::string> startsWith(const std::string& prefix) const;

    //ABC order
    std::vector<std::string> getAllCountries() const;

    int getCountryCount() const;
    long long getRecordCount() const;
};


#endif //COVIDPREVENTIONDATA_TRIE_H