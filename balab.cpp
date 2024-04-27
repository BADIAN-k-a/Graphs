

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>

// Определение структуры узла дерева
struct Node {
    int data;
    Node* left;
    Node* right;
};

// Функция для создания узла дерева
Node* createNode(int data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Функция для построения дерева из данных в файле
Node* buildTreeFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<std::string>> treeData;

    // Чтение данных из файла и сохранение их в векторе
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> nodeData;
        std::string value;
        while (std::getline(ss, value, ',')) {
            nodeData.push_back(value);
        }
        treeData.push_back(nodeData);
    }

    // Создание узлов дерева и связывание их в соответствии с данными из файла
    Node* root = createNode(std::stoi(treeData[0][0]));
    std::vector<Node*> nodes = { root };
    for (size_t i = 0; i < treeData.size(); i++) {
        std::string leftValue = treeData[i][1];
        std::string rightValue = treeData[i][2];
        if (!leftValue.empty() && leftValue != " ") {
            Node* leftNode = createNode(std::stoi(leftValue));
            nodes[i]->left = leftNode;
            nodes.push_back(leftNode);
        }
        if (!rightValue.empty() && rightValue != " ") {
            Node* rightNode = createNode(std::stoi(rightValue));
            nodes[i]->right = rightNode;
            nodes.push_back(rightNode);
        }
    }

    return root;
}

void pull(Node* source, Node* way) {
    if (way->left == nullptr || way->right == nullptr) {
        if (way->left == nullptr) { way->left = source; }
        else { way->right = source; }
    }
    else {
        pull(source, way->right);
    }
}

Node* swapNodes(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }

    root->left = swapNodes(root->left);
    root->right = swapNodes(root->right);

    if (root->data % 2 == 0) {
        if (root->left != nullptr && root->left->data % 2 == 0) {
            pull(root, root->left);
            Node* temp = root->left;
            root->left = nullptr;
            return temp;
        }
        else if (root->right != nullptr && root->right->data % 2 == 0) {
            pull(root, root->right);
            Node* temp = root->right;
            root->right = nullptr;
            return temp;
        }
    }

    return root;
}

void dump(Node* node, const std::string& prefix = "", bool isRoot = true, bool isLast = true) {
    std::cout << prefix << (isRoot ? "" : (isLast ? "|__ " : "|-- ")) << (node ? std::to_string(node->data) : "") << std::endl;
    if (!node || (!node->left && !node->right))
        return;
    std::vector<Node*> children{ node->left, node->right };
    for (size_t i = 0; i < children.size(); ++i) {
        dump(children[i], prefix + (isRoot ? "" : (isLast ? "    " : "|   ")), false, i + 1 >= children.size());
    }
}



int main() {
    // Построение дерева из файла
    Node* root = buildTreeFromFile("binary_tree100.txt");
    dump(root);

  root = swapNodes(root);
   dump(root);

  

    std::cout << "Modified tree has been written to new.txt." << std::endl;
    unsigned int end_time = clock();
    std::cout << "Time: " << end_time << std::endl;

    return 0;
}

