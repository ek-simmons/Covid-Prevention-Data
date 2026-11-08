#ifndef COVIDPREVENTIONDATA_BTREE_H
#define COVIDPREVENTIONDATA_BTREE_H
#pragma once
#include <vector>
#include <string>
#include "Records.h"

class BTree {
private:
    static const int MIN_DEGREE = 3;

    struct BTreeNode {
        std::vector<std::string> keys;
        std::vector<std::string> displayNames;
        std::vector<std::vector<CaseRecord>> cases;
        std::vector<std::vector<BehaviorRecord>> behaviors;
        std::vector<BTreeNode*> children;
        bool leaf;

        BTreeNode(bool isLeaf) {
            leaf = isLeaf;
        }
    };

    BTreeNode* root;
    int countryCount;
    long long recordCount;

    std::string normalize(const std::string& name) const;
    std::string fixedDisplayName(const std::string& name) const;

    // B-Tree core operations
    void insertNonFull(BTreeNode* node, const std::string& key, const std::string& display, const CaseRecord* cRec, const BehaviorRecord* bRec);
    void splitChild(BTreeNode* parent, int i, BTreeNode* fullChild);
    BTreeNode* searchNode(BTreeNode* node, const std::string& key, int& index) const;
    void destroy(BTreeNode* node);
    void collectNames(BTreeNode* node, const std::string& prefix, std::vector<std::string>& names) const;
    void collectAllNames(BTreeNode* node, std::vector<std::string>& names) const;

public:
    BTree();
    ~BTree();

    void insertCase(const CaseRecord& record);
    void insertBehavior(const BehaviorRecord& record);

    const std::vector<CaseRecord>* getCases(const std::string& country) const;
    const std::vector<BehaviorRecord>* getBehaviors(const std::string& country) const;
    bool contains(const std::string& country) const;

    std::vector<std::string> startsWith(const std::string& prefix) const;
    std::vector<std::string> getAllCountries() const;

    int getCountryCount() const;
    long long getRecordCount() const;
};

#endif