#include <iostream>
#include <string.h>
#include <fstream>

enum RBTColor { Black, Red };

const int STR_LEN = 280;

void ToLow(char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        str[i] = tolower(str[i]);
    }
}

class TRBTreeNode {
    friend class TRBTree;
    public:
        TRBTreeNode* left;
        TRBTreeNode* right;
        TRBTreeNode* parent;
        RBTColor color;
        char key[STR_LEN];
        unsigned long long value;
    public:
        TRBTreeNode(RBTColor c, TRBTreeNode* l, TRBTreeNode* r, TRBTreeNode* p, unsigned long long v, const char* k) : 
            color(c), left(l), right(r), parent(p), value(v) {
                strcpy(key, k);
            }
        TRBTreeNode(RBTColor c, TRBTreeNode* l, TRBTreeNode* r, TRBTreeNode* p, unsigned long long v) : 
            color(c), left(l), right(r), parent(p), value(v) {
                memset(key, 0, STR_LEN);
            }
        ~TRBTreeNode() = default;
};

class TRBTree {
    private:
        TRBTreeNode* root;
        TRBTreeNode* nil;
    public:
        TRBTreeNode* Search(const char *key) const;
        bool Insert(const char *str, unsigned long long value);
        bool Remove(TRBTreeNode *z);
        bool SaveTree(std::ofstream &ofs);
        bool LoadTree(std::ifstream &ifs);
        const TRBTreeNode *GetNil() const;
        void Destroy();
        TRBTree();
        ~TRBTree();
    private:
        TRBTreeNode* Search(TRBTreeNode* node, const char *key) const;
        void LeftRotation(TRBTreeNode* node);
        void RightRotation(TRBTreeNode* node);
        void InsertFix(TRBTreeNode* node);
        void Destroy(TRBTreeNode *node);
        void RemoveFixUp(TRBTreeNode* &root, TRBTreeNode *node);
        void SaveTree(TRBTreeNode *node, std::ofstream &ofs);
        void LoadTree(TRBTreeNode *node, std::ifstream &ifs);
};

const TRBTreeNode *TRBTree::GetNil() const {
    return nil;
}

TRBTree::TRBTree() {
    try {
        nil = new TRBTreeNode(Black, nullptr, nullptr, nullptr, -1, "c");
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space\n";
        exit(EXIT_SUCCESS);
    }
    root = nil;
}

TRBTree::~TRBTree() {
    Destroy(root);
    delete nil;
}

TRBTreeNode* TRBTree::Search(const char *key) const {
    return Search(root, key);
}

TRBTreeNode *TRBTree::Search(TRBTreeNode* node, const char *key) const {
    TRBTreeNode *tmp = node;
    while (tmp != nil && strcmp(tmp->key, key) != 0 && tmp != nullptr) {
        if (strcmp(key, tmp->key) < 0) {
            tmp = tmp->left;
        } else {
            tmp = tmp->right; 
        }
    }
    return tmp;
}

void TRBTree::LeftRotation(TRBTreeNode* x) {
    TRBTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != nil) {
        y->left->parent = x;
    }
    if (y != nil) {
        y->parent = x->parent;
    }
    if (x->parent == nil || !x->parent) {
        root = y;
    } else {
        if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    y->left = x;
    if (x != nil) {
        x->parent = y;
    }
}

void TRBTree::RightRotation(TRBTreeNode* x) {
    TRBTreeNode *y = x->left;
    x->left = y->right;
    if (y->right != nil) {
        y->right->parent = x;
    }
    if (y != nil) {
        y->parent = x->parent;
    }
    if (!x->parent || x->parent == nil) {
        root = y;
    } else {
        if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
    }
    y->right = x;
    if (x != nil) {
        x->parent = y;
    }
}

bool TRBTree::Insert(const char *key, unsigned long long value) {
    TRBTreeNode *x = root;
	TRBTreeNode *y = nil;
	while (x != nil) {
		y = x;
        if (strcmp(x->key, key) == 0) {
            return false;
        }
		if (strcmp(key, x->key) < 0) {
			x = x->left;
        } else {
			x = x->right;
        }
	}
    TRBTreeNode *node;
    try {
        node = new TRBTreeNode(Red, nil, nil, nil, value, key);
    } catch (const std::bad_alloc& ex) {
        std::cout << "ERROR: fail to allocate the requested storage space\n";
        exit(EXIT_SUCCESS);
    }
	node->parent = y;
	if(y != nil) {
		if (strcmp(key, y->key) < 0)
			y->left = node;
		else
			y->right = node;
	} else {
		root = node;
    }
    node->color = Red;
    InsertFix(node);
    return true;
}

void TRBTree::InsertFix(TRBTreeNode* node) {
    TRBTreeNode *parent, *gparent, *uncle;
    while (node != root && node->parent->color == Red) {
        if (node->parent->parent->left == node->parent) {
            uncle = node->parent->parent->right;
            if (uncle->color == Red) {
                node->parent->color = Black;
                uncle->color = Black;
                node->parent->parent->color = Red;
                node = node->parent->parent;
            } else {
                if (node->parent->right == node) {
                    node = node->parent;
                    LeftRotation(node);
                }
                node->parent->parent->color = Red;
                node->parent->color = Black;
                RightRotation(node->parent->parent);
            }
        } else {
            uncle = node->parent->parent->left;
            if (uncle->color == Red) {
                node->parent->color = Black;
                uncle->color = Black;
                node->parent->parent->color = Red;
                node = node->parent->parent;
            } else {
                if (node->parent->left == node) {
                    node = node->parent;
                    RightRotation(node);
                }
                node->parent->parent->color = Red;
                node->parent->color = Black;
                LeftRotation(node->parent->parent);
            }
        }
    }
    root->color = Black;
}

void TRBTree::Destroy() {
    Destroy(root);
    root = nil;
}

void TRBTree::Destroy(TRBTreeNode* node) {
    if (node != nil) {
        if (node->left != nil) {
            Destroy(node->left);
        }
        if (node->right != nil) {
            Destroy(node->right);
        }
        delete node;
    }
}

bool TRBTree::Remove(TRBTreeNode *node) {
    TRBTreeNode *child, *successor;
    if (node == nil) {
        return false;
    }
    if (node->left == nil || node->right == nil) {
        successor = node;
    } else {
        successor = node->right;
        while (successor->left != nil) {
            successor = successor->left;
        }
    }
    if (successor->left != nil) {
        child = successor->left;
    } else {
        child = successor->right;
    }
    child->parent = successor->parent;
    if (successor->parent == nil) {
        root = child;
    } else if (successor == successor->parent->left) {
        successor->parent->left = child;
    } else if (successor == successor->parent->right) {
        successor->parent->right = child;
    }
    if (successor != node) {
        strcpy(node->key, successor->key);
        node->value = successor->value;
    }
    if (successor->color == Black && root != nil) {
        RemoveFixUp(root, child);
    }
    delete successor;
    return true;
}

void TRBTree::RemoveFixUp(TRBTreeNode* &root, TRBTreeNode *node) {
    TRBTreeNode *other;
    while (node != root && node->color == Black) {
        if (node->parent->left == node) {
            other = node->parent->right;
            if (other->color == Red) {
                other->color = Black;
                node->parent->color = Red;
                LeftRotation(node->parent);
                other = node->parent->right;
            } 
            if (other->left->color == Black && other->right->color == Black) {
                other->color = Red;
                node = node->parent;
            } else {
                if (other->right->color == Black) {
                    other->left->color = Black;
                    other->color = Red;
                    RightRotation(other);
                    other = node->parent->right;
                }
                other->color = node->parent->color;
                node->parent->color = Black;
                other->right->color = Black;
                LeftRotation(node->parent);
                node = root;
            }
        } else {
			other = node->parent->left;
            if (other->color == Red) {
                other->color = Black;
                node->parent->color = Red;
                RightRotation(node->parent);
                other = node->parent->left;
            } 
            if (other->right->color == Black && other->left->color == Black) {
                other->color = Red;
                node = node->parent;
            } else {
                if (other->left->color == Black) {
                    other->right->color = Black;
                    other->color = Red;
                    LeftRotation(other);
                    other = node->parent->left;
                }
                other->color = node->parent->color;
                node->parent->color = Black;
                other->left->color = Black;
                RightRotation(node->parent);
                node = root;
            }
		}
	}
    node->color = Black;
}

bool TRBTree::SaveTree(std::ofstream& file) {
    if (root != nil) {
        SaveTree(root, file);
        file.close();
        return true;
    } else {
        return false;
    }
}

void TRBTree::SaveTree(TRBTreeNode* x, std::ofstream& file) {
    bool left = (x->left != nil);
    bool right = (x->right != nil);
    int size = strlen(x->key);
    file.write((char*)&(size), sizeof(int));
    file.write(x->key, sizeof(char) * size);
    file.write((char*)&left, sizeof(bool));
    file.write((char*)&right, sizeof(bool));
    file.write((char*)&x->value, sizeof(unsigned long long));
    file.write((char*)&x->color, sizeof(int));
    if (left) {
        SaveTree(x->left, file);
    }
    if (right) {
        SaveTree(x->right, file);
    }
}

void TRBTree::LoadTree(TRBTreeNode* x, std::ifstream& file){
    bool left, right;
    int size;
    file.read((char*)&size, sizeof(int));
    file.read(x->key, sizeof(char) * size);
    file.read((char*)&left, sizeof(bool));
    file.read((char*)&right, sizeof(bool));
    file.read((char*)&x->value, sizeof(unsigned long long));
    file.read((char*)&x->color, sizeof(RBTColor));

    if (left) {
        x->left = new TRBTreeNode(Red, nil, nil, x, -1);
        LoadTree(x->left, file);
    }
    if (right) {
        x->right = new TRBTreeNode(Red, nil, nil, x, -1);
        LoadTree(x->right, file);
    }
}

bool TRBTree::LoadTree(std::ifstream& file) {
    Destroy();
    if (file.peek() != EOF) {
        try {
            root = new TRBTreeNode(Black, nil, nil, nil, -1);
        } catch (const std::bad_alloc& ex) {
            std::cout << "ERROR: fail to allocate the requested storage space\n";
            exit(EXIT_SUCCESS);
        }
        LoadTree(root, file);
    }
    file.close();
    return true;
}

int countBlackNodes(TRBTreeNode* root, int& blackCount) {
    if (!root) {
        return 1;
    }
    blackCount += (root->color == Black);
    int leftBlackCount = 0, rightBlackCount = 0;
    int leftCount = countBlackNodes(root->left, leftBlackCount);
    int rightCount = countBlackNodes(root->right, rightBlackCount);
    if (root->color == Red) {
        if (root->left != nullptr && root->left->color == Red) {
            return -1;
        }
        if (root->right != nullptr && root->right->color == Red) {
            return -1;
        }
    }
    if (leftCount == -1 || rightCount == -1 || leftBlackCount != rightBlackCount) {
        return -1;
    }
    return leftCount + rightCount + (root->color == Black);
}

bool check(TRBTreeNode* root) {
    if (root->color != Black) {
        return false;
    }
    int blackCount = 0;
    int totalBlackCount = countBlackNodes(root, blackCount);
    return totalBlackCount != -1;
}

int main() {
    TRBTreeNode *r = new TRBTreeNode(Black, nullptr, nullptr, nullptr, 123);
    TRBTreeNode *r1 = new TRBTreeNode(Red, nullptr, nullptr, nullptr, 123);
    TRBTreeNode *r2 = new TRBTreeNode(Red, nullptr, nullptr, nullptr, 123);
    TRBTreeNode *r3 = new TRBTreeNode(Black, nullptr, nullptr, nullptr, 123);
    TRBTreeNode *r4 = new TRBTreeNode(Black, nullptr, nullptr, nullptr, 123);
    r->left = r1;
    r1->left = r3;
    r1->right = r4;
    std::cout << check(r) << std::endl;
}

// int main() {
//     std::ios::sync_with_stdio(false);
//     std::cin.tie(0);
//     std::cout.tie(0);
//     TRBTree root;
//     char in[STR_LEN];
//     unsigned long long i;
//     while (std::cin >> in) {
//         if (in[0] == '+') {
//             std::cin >> in;
//             std::cin >> i;
//             ToLow(in);
//             root.Insert(in, i);
//         } else if (in[0] == '-') {
//             std::cin >> in;
//             ToLow(in);
//             root.Remove(root.Search(in));
//         } else if (in[0] == '!') {
//             std::cin >> in;
//             if (in[0] == 'L' || in[0] == 'l') {
//                 std::cin >> in;
//                 std::ifstream file = std::ifstream(in, std::ios::binary);
//                 root.LoadTree(file);
//                 file.close();
//             } else {
//                 std::cin >> in;
//                 std::ofstream file = std::ofstream(in, std::ios::binary | std::ios::trunc);
//                 root.SaveTree(file);
//             }
//             std::cout << "OK\n"; 
//         } else {
//             ToLow(in);
//             TRBTreeNode* res = root.Search(in);
//         }
//     }
// }
