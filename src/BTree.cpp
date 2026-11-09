#include "BTree.h"
#include <cctype>
#include <algorithm>

BTree::BTree() {
    root = new BTreeNode(true);
    countryCount = 0;
    recordCount = 0;
}

BTree::~BTree() {
    destroy(root);
}

void BTree::destroy(BTreeNode* node) {
    if (node != nullptr) {
        if (!node->leaf) {
            for (size_t i = 0; i < node->children.size(); i++) {
                destroy(node->children[i]);
            }
        }
        delete node;
    }
}

std::string BTree::normalize(const std::string& name) const {
    std::string key;
    for (char c : name) {
        if (c == '"') continue;
        if (c == '_') c = ' ';
        key += (char)tolower((unsigned)c);
    }
    while (!key.empty() && key.back() == ' ') key.pop_back();
    while (!key.empty() && key.front() == ' ') key.erase(0,1);
    if (key == "united states of america") key = "united states";
    return key;
}

std::string BTree::fixedDisplayName(const std::string& name) const {
    std::string shown;
    for (char c : name) {
        if (c == '"') continue;
        shown += (c == '_') ? ' ' : c;
    }
    return shown;
}

void BTree::insertCase(const CaseRecord& record) {
    std::string key = normalize(record.country);
    if (key.empty()) return;
    std::string display = fixedDisplayName(record.country);

    // If root is full tree grows in height
    if (root->keys.size() == 2 * MIN_DEGREE - 1) {
        BTreeNode* newRoot = new BTreeNode(false);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0, root);
        root = newRoot;
    }
    insertNonFull(root, key, display, &record, nullptr);
    recordCount++;
}

void BTree::insertBehavior(const BehaviorRecord& record) {
    std::string key = normalize(record.country);
    if (key.empty()) return;
    std::string display = fixedDisplayName(record.country);

    if (root->keys.size() == 2 * MIN_DEGREE - 1) {
        BTreeNode* newRoot = new BTreeNode(false);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0, root);
        root = newRoot;
    }
    insertNonFull(root, key, display, nullptr, &record);
    recordCount++;
}

void BTree::insertNonFull(BTreeNode* node, const std::string& key, const std::string& display, const CaseRecord* cRec, const BehaviorRecord* bRec) {
    int i = node->keys.size() - 1;

    // Check if key already exists in this node
    for (int j = 0; j <= i; j++) {
        if (node->keys[j] == key) {
            if (cRec) node->cases[j].push_back(*cRec);
            if (bRec) node->behaviors[j].push_back(*bRec);
            return;
        }
    }

    if (node->leaf) {

        node->keys.push_back("");
        node->displayNames.push_back("");
        node->cases.push_back(std::vector<CaseRecord>());
        node->behaviors.push_back(std::vector<BehaviorRecord>());

        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            node->displayNames[i + 1] = node->displayNames[i];
            node->cases[i + 1] = node->cases[i];
            node->behaviors[i + 1] = node->behaviors[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->displayNames[i + 1] = display;
        node->cases[i+1].clear();
        node->behaviors[i+1].clear();
        if (cRec) node->cases[i + 1].push_back(*cRec);
        if (bRec) node->behaviors[i + 1].push_back(*bRec);
        countryCount++;
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }


        if (node->children[i + 1]->keys.size() == 2 * MIN_DEGREE - 1) {
            splitChild(node, i + 1, node->children[i + 1]);
            if (node->keys[i + 1] < key) {
                i++;
            } else if (node->keys[i + 1] == key) {
                if (cRec) node->cases[i + 1].push_back(*cRec);
                if (bRec) node->behaviors[i + 1].push_back(*bRec);
                return;
            }
        }
        insertNonFull(node->children[i + 1], key, display, cRec, bRec);
    }
}

void BTree::splitChild(BTreeNode* parent, int i, BTreeNode* fullChild) {
    BTreeNode* newChild = new BTreeNode(fullChild->leaf);


    for (int j = 0; j < MIN_DEGREE - 1; j++) {
        newChild->keys.push_back(fullChild->keys[j + MIN_DEGREE]);
        newChild->displayNames.push_back(fullChild->displayNames[j + MIN_DEGREE]);
        newChild->cases.push_back(fullChild->cases[j + MIN_DEGREE]);
        newChild->behaviors.push_back(fullChild->behaviors[j + MIN_DEGREE]);
    }


    if (!fullChild->leaf) {
        for (int j = 0; j < MIN_DEGREE; j++) {
            newChild->children.push_back(fullChild->children[j + MIN_DEGREE]);
        }
    }


    parent->children.insert(parent->children.begin() + i + 1, newChild);
    parent->keys.insert(parent->keys.begin() + i, fullChild->keys[MIN_DEGREE - 1]);
    parent->displayNames.insert(parent->displayNames.begin() + i, fullChild->displayNames[MIN_DEGREE - 1]);
    parent->cases.insert(parent->cases.begin() + i, fullChild->cases[MIN_DEGREE - 1]);
    parent->behaviors.insert(parent->behaviors.begin() + i, fullChild->behaviors[MIN_DEGREE - 1]);


    fullChild->keys.resize(MIN_DEGREE - 1);
    fullChild->displayNames.resize(MIN_DEGREE - 1);
    fullChild->cases.resize(MIN_DEGREE - 1);
    fullChild->behaviors.resize(MIN_DEGREE - 1);
    if (!fullChild->leaf) {
        fullChild->children.resize(MIN_DEGREE);
    }
}

BTree::BTreeNode* BTree::searchNode(BTreeNode* node, const std::string& key, int& index) const {
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i]) {
        i++;
    }

    if (i < node->keys.size() && key == node->keys[i]) {
        index = i;
        return node;
    }

    if (node->leaf) {
        return nullptr;
    }
    return searchNode(node->children[i], key, index);
}

const std::vector<CaseRecord>* BTree::getCases(const std::string& country) const {
    std::string key = normalize(country);
    int index;
    BTreeNode* result = searchNode(root, key, index);
    if (result != nullptr) {
        return &(result->cases[index]);
    }
    return nullptr;
}

const std::vector<BehaviorRecord>* BTree::getBehaviors(const std::string& country) const {
    std::string key = normalize(country);
    int index;
    BTreeNode* result = searchNode(root, key, index);
    if (result != nullptr) {
        return &(result->behaviors[index]);
    }
    return nullptr;
}

bool BTree::contains(const std::string& country) const {
    std::string key = normalize(country);
    int index;
    return searchNode(root, key, index) != nullptr;
}

void BTree::collectNames(BTreeNode* node, const std::string& prefix, std::vector<std::string>& names) const {
    if (node == nullptr) return;

    int i;
    for (i = 0; i < node->keys.size(); i++) {
        if (!node->leaf) {
            collectNames(node->children[i], prefix, names);
        }
        if (node->keys[i].substr(0, prefix.length()) == prefix) {
            names.push_back(node->displayNames[i]);
        }
    }
    if (!node->leaf) {
        collectNames(node->children[i], prefix, names);
    }
}

std::vector<std::string> BTree::startsWith(const std::string& prefix) const {
    std::vector<std::string> names;
    collectNames(root, normalize(prefix), names);
    return names;
}

void BTree::collectAllNames(BTreeNode* node, std::vector<std::string>& names) const {
    if (node == nullptr) return;

    int i;
    for (i = 0; i < node->keys.size(); i++) {
        if (!node->leaf) {
            collectAllNames(node->children[i], names);
        }
        names.push_back(node->displayNames[i]);
    }
    if (!node->leaf) {
        collectAllNames(node->children[i], names);
    }
}

std::vector<std::string> BTree::getAllCountries() const {
    std::vector<std::string> names;
    collectAllNames(root, names);
    return names;
}

int BTree::getCountryCount() const {
    return countryCount;
}

long long BTree::getRecordCount() const {
    return recordCount;
}