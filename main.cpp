#include <iostream>
#include <fstream>
#include <initializer_list>
#include <queue>

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
};

class BSTree {
public:
    BSTree() {
        root = nullptr;
    }

    BSTree(initializer_list<int> list) : BSTree() {
        for (int value : list) {
            add_element(value);
        }
    }

    ~BSTree() {
        clear_tree(root);
    }

    bool add_element(int value) {
        if (find_element(value)) {
            return false; // элемент уже существует
        }

        Node* newNode = new Node{value, nullptr, nullptr, nullptr};

        if (root == nullptr) {
            root = newNode;
            return true;
        }

        Node* curr = root;
        Node* parent = nullptr;

        while (curr != nullptr) {
            parent = curr;
            if (value < curr->data) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        newNode->parent = parent;
        if (value < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        return true;
    }

    bool delete_element(int value) {
        Node* node = find_node(value);
        if (node == nullptr) {
            return false; // элемент не найден
        }

        if (node->left == nullptr && node->right == nullptr) {
            // Удаление листа
            if (node->parent == nullptr) {
                root = nullptr;
            } else if (node->parent->left == node) {
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
            delete node;
        }
        else if (node->left == nullptr || node->right == nullptr) {
            // Удаление узла с одним потомком
            Node* child = (node->left != nullptr) ? node->left : node->right;
            if (node->parent == nullptr) {
                root = child;
                child->parent = nullptr;
            } else {
                if (node->parent->left == node) {
                    node->parent->left = child;
                } else {
                    node->parent->right = child;
                }
                child->parent = node->parent;
            }
            delete node;
        }
        else {
            // Удаление узла с двумя потомками
            Node* successor = find_min(node->right);
            node->data = successor->data;
            if (successor->parent->left == successor) {
                successor->parent->left = successor->right;
                if (successor->right != nullptr) {
                    successor->right->parent = successor->parent;
                }
            } else {
                successor->parent->right = successor->right;
                if (successor->right != nullptr) {
                    successor->right->parent = successor->parent;
                }
            }
            delete successor;
        }

        return true;
    }

    bool find_element(int value) {
        return find_node(value) != nullptr;
    }

    void print() {
        print_inorder(root);
        cout << endl;
    }

    bool save_to_file(const string& path) {
        ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        save_inorder(root, file);
        file.close();
        return true;
    }

    bool load_from_file(const string& path) {
        ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        clear_tree(root);
        root = nullptr;

        int value;
        while (file >> value) {
            add_element(value);
        }

        file.close();
        return true;
    }

    // Дополнительные методы для удобства
    void print_inorder() {
        print_inorder(root);
        cout << endl;
    }

    void print_preorder() {
        print_preorder(root);
        cout << endl;
    }

    void print_postorder() {
        print_postorder(root);
        cout << endl;
    }

    void print_level_order() {
        if (root == nullptr) return;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            cout << current->data << " ";

            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
        cout << endl;
    }

private:
    Node* root;

    void clear_tree(Node* node) {
        if (node == nullptr) return;

        clear_tree(node->left);
        clear_tree(node->right);
        delete node;
    }

    Node* find_node(int value) {
        Node* curr = root;
        while (curr != nullptr) {
            if (value == curr->data) {
                return curr;
            } else if (value < curr->data) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return nullptr;
    }

    Node* find_min(Node* node) {
        while (node != nullptr && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void print_inorder(Node* node) {
        if (node == nullptr) return;

        print_inorder(node->left);
        cout << node->data << " ";
        print_inorder(node->right);
    }

    void print_preorder(Node* node) {
        if (node == nullptr) return;

        cout << node->data << " ";
        print_preorder(node->left);
        print_preorder(node->right);
    }

    void print_postorder(Node* node) {
        if (node == nullptr) return;

        print_postorder(node->left);
        print_postorder(node->right);
        cout << node->data << " ";
    }

    void save_inorder(Node* node, ofstream& file) {
        if (node == nullptr) return;

        save_inorder(node->left, file);
        file << node->data << " ";
        save_inorder(node->right, file);
    }
};

int main() {
    // Тестирование всех методов
    BSTree tree1;
    tree1.add_element(50);
    tree1.add_element(30);
    tree1.add_element(70);
    tree1.add_element(20);
    tree1.add_element(40);
    tree1.add_element(60);
    tree1.add_element(80);

    cout << "Tree1 (inorder): ";
    tree1.print();

    cout << "Deleting 20: " << tree1.delete_element(20) << endl;
    cout << "Tree1 after deletion: ";
    tree1.print();

    cout << "Find 30: " << tree1.find_element(30) << endl;
    cout << "Find 100: " << tree1.find_element(100) << endl;

    cout << "Saving to file: " << tree1.save_to_file("tree.txt") << endl;

    BSTree tree2;
    cout << "Loading from file: " << tree2.load_from_file("tree.txt") << endl;
    cout << "Tree2 (inorder): ";
    tree2.print();

    BSTree tree3{10, 5, 15, 3, 7, 12, 18};
    cout << "Tree3 (inorder): ";
    tree3.print();
    cout << "Tree3 (level order): ";
    tree3.print_level_order();

    return 0;
}
