#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

class TrieNode {
public:
    TrieNode() : isEndOfWord(false) {
        children.resize(26, nullptr);
    }

    std::vector<TrieNode*> children;
    bool isEndOfWord;
};

class Trie {
public:
    Trie() : root(new TrieNode()) {}

    void insert(const std::string &word) {
        TrieNode *current = root.get();
        for (char c : word) {
            int index = c - 'a';
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
    }

    std::vector<std::string> autocomplete(const std::string &prefix) {
        std::vector<std::string> suggestions;
        TrieNode *current = root.get();
        for (char c : prefix) {
            int index = c - 'a';
            if (!current->children[index]) {
                return suggestions; // No words with given prefix
            }
            current = current->children[index];
        }
        autocompleteHelper(prefix, current, suggestions);
        return suggestions;
    }

private:
    std::unique_ptr<TrieNode> root;

    void autocompleteHelper(const std::string &prefix, TrieNode *node, std::vector<std::string> &suggestions, std::string word = "") {
        if (node->isEndOfWord) {
            suggestions.push_back(prefix + word);
        }
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                autocompleteHelper(prefix, node->children[i], suggestions, word + char('a' + i));
            }
        }
    }
};

int main() {
    Trie trie;
    std::vector<std::string> wordList = {"hello", "world", "how", "are", "you", "doing", "today"};

    for (const auto &word : wordList) {
        trie.insert(word);
    }

    std::string input;
    char ch;

    std::cout << "Enter a word: ";
    while (std::cin.get(ch) && ch != '\n') {
        if (ch == '\t') { // Autocomplete on tab press
        std::cout<<std::flush;
            std::vector<std::string> suggestions = trie.autocomplete(input);
            if (!suggestions.empty()) {
                input = suggestions[0];
                std::cout << input.substr(input.length() - input.length() + input.size() - input.size()); // Print the rest of the completed word
            }
        } else if (ch == '\b') { // Handle backspace
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b"; // Move cursor back, erase character, move cursor back again
            }
        } else { // Handle printable characters
            input += ch;
            std::vector<std::string> suggestions = trie.autocomplete(input);
            if (!suggestions.empty()) {
                input = suggestions[0];
                std::cout << input.substr(input.length() - input.length() + input.size() - input.size()); // Print the rest of the completed word
            }
        }

        std::cout << std::flush;
    }

    std::cout << std::endl << "You entered: " << input << std::endl;

    return 0;
}

