#include "Trie.h"
#include <cctype>


//constructor/destructor
Trie::Trie() {
    root = new Node();
    countryCount = 0;
    recordCount = 0;
}
Trie::~Trie() {
    destroy(root);
}

//post-order traversal
void Trie::destroy(Trie::Node* root) {
    if (node==nullptr) return;
    for (int i = 0; i < ABC_SIZE; i++) {
        destroy(node->children[i]);
    }
    delete node;
}

//name helpers

std::string Trie::normalize(const std::string& name) {
    std::string key;
    for (char c : name) {
        if (c == '"') continue;
        if (c == '_') c = ' ';
        key += (char)tolower((unsigned)c);
    }
    while (!key.empty() && key.back() == ' ') {
        key.pop_back();
    }
    while (!key.empty() && key.front() == ' ') {
        key.erase(0,1);
    }
    if (key == "united states of america") key = "united states";
    return key;
}

std::string Trie::fixedDisplayName(const std::string& name) {
    std::string shown;
    for (char c : name) {
        if (c == '"') continue;
        shown+=(c=='_')? ' ' : c;
    }
    return shown;
}

//Trie operations
Trie::Node* Trie::findNode(const std::string& normalizedKey) const {
    Node* current = root;
    for (char c : normalizedKey) {
        unsigned char index = (unsigned char)c;
        if (current->children[index]==nullptr) {
            return nullptr;
        }
        current = current->children[index];
    }
    return current;
}

Trie::Node* Trie::getOrCreateNode(const std::string& originalName) {
    std::string key = normalize(originalName);
    Node* current = root;
    for (char c : key) {
        unsigned char index=(unsigned char)c;
        if (current->children[index]==nullptr) {
            current->children[index] = new Node();
        }
        current = current->children[index];
    }
    if (current==isEndofName) {
        current->isEndofName = true;
        current->displayName = fixedDisplayName(originalName);
        countryCount++;
    }
    return current;
}

void Trie::insertCase(const caseRecord& record) {
    if (normalize(record.country).empty()) return;
    Node* node = getOrCreateNode(record.country);

    node->displayName = fixedDisplayName(record.country);
    node->behaviors.push_back(record);
    recordCount++;
}

const std::vector<caseRecord>* Trie::getCases(const std::string& country) const {
    Node* node = findNode(normalize(country));
    if (node==nullptr || !node->isEndOfName) return nullptr;
    return &node->behaviors;
}

const std::vector<BehaviorRecord>* Trie::getBehaviors(const std::string& country) const {
    Node* node = findNode(normalize(country));
    if (node==nullptr || !node->isEndOfName) return nullptr;
    return &node->behaviors;
}

bool Trie::contains(const std::string& country) const {
    Node* node = findNode(normalize(country));
    return node!=nullptr; && node->isEndofName;
}

//Listing names

void Trie::collectNames(Node* node, std::vector<std::string>& names) const {
    if (node==nullptr) return;
    if (node->isEndofName) {
        names.push_back(node->displayName);
    }
    for (int i = 0; i < ABC_SIZE; i++) {
        collectNames(node->children[i], names);
    }
}

std::vector<std::string> Trie::startsWith(const std::string& prefix) const {
    std::vector<std::string> names;
    std::string key = normalize(prefix);
    Node* start = key.empty() ? root : findNode(key);
    collectNames(start, names);
    return names;
}

int Trie::getCountryCount() const {
    return countryCount;
}

long long Trie::getRecordCount() const {
    return recordCount;
}