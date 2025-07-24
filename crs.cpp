#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    vector<string> suggestions;
    bool isEndOfWord = false;
};

class Trie {
private:
    TrieNode* root;

    void dfs(TrieNode* node, vector<string>& results) {
        if (results.size() >= 10) return;
        if (node->isEndOfWord) {
            results.insert(results.end(), node->suggestions.begin(), node->suggestions.end());
        }
        for (auto& child : node->children) {
            dfs(child.second, results);
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        string lowerWord = word;
        transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
        TrieNode* node = root;
        for (char ch : lowerWord) {
            if (!node->children.count(ch))
                node->children[ch] = new TrieNode();
            node = node->children[ch];
        }
        node->isEndOfWord = true;
        node->suggestions.push_back(word);
    }

    vector<string> searchByPrefix(const string& prefix) {
        string lowerPrefix = prefix;
        transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(), ::tolower);
        TrieNode* node = root;
        for (char ch : lowerPrefix) {
            if (!node->children.count(ch))
                return {};
            node = node->children[ch];
        }
        vector<string> results;
        dfs(node, results);
        sort(results.begin(), results.end());
        return results;
    }
};

class CodeRetrievalSystem {
private:
    Trie trie;
    unordered_map<string, string> descToCode;

public:
    void addCodeMapping(const string& code, const string& description) {
        trie.insert(description);
        descToCode[description] = code;
    }

    vector<string> getSuggestions(const string& prefix) {
        return trie.searchByPrefix(prefix);
    }

    string getCodeFromDescription(const string& desc) {
        return descToCode.count(desc) ? descToCode[desc] : "Code not found";
    }

    void loadMappingsFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string code, description;
            if (getline(ss, code, '|') && getline(ss, description)) {
                addCodeMapping(code, description);
            }
        }
        file.close();
    }
};

int main(int argc, char* argv[]) {
    CodeRetrievalSystem system;
    system.loadMappingsFromFile("data.txt");

    if (argc < 3) {
        cout << "Usage:\n";
        cout << "  crs suggest <prefix>\n";
        cout << "  crs getcode <description>\n";
        return 1;
    }

    string command = argv[1];
    string input = argv[2];

    if (command == "suggest") {
        auto suggestions = system.getSuggestions(input);
        for (const auto& suggestion : suggestions) {
            cout << suggestion << endl;
        }
    } else if (command == "getcode") {
        cout << system.getCodeFromDescription(input) << endl;
    } else {
        cout << "Unknown command." << endl;
    }

    return 0;
}
