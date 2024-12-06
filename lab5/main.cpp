#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <chrono>

class SuffixTree {
    public:
        class Node {

            friend class SuffixTree;

            public:
                Node(std::shared_ptr<Node> link, int start, int end) : 
                    suffixLink(link), first(start), last(end) {}
                ~Node() = default;
            private:
                std::map<char, std::shared_ptr<Node>> children;
                std::shared_ptr<Node> suffixLink;
                int first;
                int last;
        };

        SuffixTree(const std::string &_text) : 
            text(_text), root(std::make_shared<Node>(nullptr, -1, -1)) {}

        void Build();
        void LexicographicDFS(std::shared_ptr<Node> node, int label_height);

        int EdgeLength(std::shared_ptr<Node> node, int pos) {
            return std::min(node->last, pos + 1) - node->first;
        }

        void LexicographicTraversal() {
            LexicographicDFS(root->children.begin()->second, 0);
        }
private:
    void Extend(int pos);
    bool WalkOnEdge(int edgeLen, std::shared_ptr<Node> next);
    void AddSuffixLink(std::shared_ptr<Node> node);
    std::shared_ptr<Node> root;
    std::shared_ptr<Node> lastCreatedNode = nullptr;

    std::string text;

    std::shared_ptr<Node> activeNode = nullptr;
    int activeEdge = -1;
    int activeLength = 0;
    int remainingSuffixCount = 0;
    int endLeaf = -1;
};

void SuffixTree::Build() {
    activeNode = lastCreatedNode = root->suffixLink = root;

    for (size_t i = 0; i < text.size(); i++)
        Extend(i);
}

void SuffixTree::Extend(int idx) {
    lastCreatedNode = root;
    ++remainingSuffixCount;

    while(remainingSuffixCount) {
        if (!activeLength) {
            activeEdge = idx;
        }
        auto nodeIter = activeNode->children.find(text[activeEdge]);
        if (nodeIter == activeNode->children.end()) { 
            activeNode->children[text[activeEdge]] = std::make_shared<Node>(root, idx, text.size());
            AddSuffixLink(activeNode);
        } else {
            std::shared_ptr<Node> next = nodeIter->second;
            if (WalkOnEdge(idx, next)) {
                continue;
            }
            if (text[next->first + activeLength] == text[idx]) {
                ++activeLength;
                AddSuffixLink(activeNode);
                break;
            }

            std::shared_ptr<Node> split = std::make_shared<Node>(root, next->first, next->first + activeLength);
      
            activeNode->children[text[activeEdge]] = split;


            split->children[text[idx]] = std::make_shared<Node>(root, idx, text.size());
            next->first += activeLength;
            split->children[text[next->first]] = next;

            AddSuffixLink(split);
        }
        --remainingSuffixCount;
        if (activeNode == root && activeLength) {
            --activeLength;
            ++activeEdge;
        } else {
            activeNode = (activeNode->suffixLink) ? activeNode->suffixLink : root;
        }
    }
}

bool SuffixTree::WalkOnEdge(int edgeLen, std::shared_ptr<Node> next) {
    if (activeLength >= EdgeLength(next, edgeLen)) {
        activeEdge += EdgeLength(next, edgeLen);
        activeLength -= EdgeLength(next, edgeLen);
        activeNode = next;
        return true;
    }
    return false;
}

void SuffixTree::AddSuffixLink(std::shared_ptr<Node> node) {
    if (lastCreatedNode != root) {
        lastCreatedNode->suffixLink = node;
    }
    lastCreatedNode = node;
}

void SuffixTree::LexicographicDFS(std::shared_ptr<Node> node, int curSize) {
    if (node == nullptr) return;

    if (node->first != -1) {
        for (int i = node->first; i < node->last; i++) {
            ++curSize;
            //std::cout << text[i];
            if (curSize == text.size() / 2) {
                return;
            }
        }
    }
    if (!node->children.size()) {
        return;
    }
    LexicographicDFS(node->children.begin()->second, curSize);
}

int main() {
    std::string text;
    while (std::cin >> text) {
        SuffixTree tree(text + text);
        auto begin = std::chrono::steady_clock::now();
        tree.Build();
        tree.LexicographicTraversal();
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    }
}