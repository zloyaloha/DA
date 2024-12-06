#include <map>
#include <string>

class Node {
    public:
        std::map<char, Node *> children;
        int start;
        Node(int pos) : start(pos) {}
};

class SuffixTree {
    public:
        Node* root;
        std::string text;
    
    SuffixTree(const std::string &str) : text(str) {
        root = new Node(0);
    }

    void build() {
        for (int i = 0; i < text.size(); ++i) {
            addSubstring(i);
        }
    }

    void addSubstring(const int pos) {
        Node *current = root;
        for (int i = pos; i < text.size(); ++i) {
            if (current->children.find(text[i]) == current->children.end()) {
                current->children[text[i]] = new Node(i);
            }
            current = current->children[text[i]];
        }
    }
};

int main() {
    std::string text = "banana";
    SuffixTree tree(text);
    tree.build();
}