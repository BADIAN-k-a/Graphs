#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector> 
#include <map> 
#include <queue>
#include <ctime>
#include <string>
using namespace std;

// Определение структуры узла дерева 
struct Node {
    int data;
    Node* left;
    Node* right;
    string path; // Добавляем поле для хранения пути
};

// Функция для создания узла дерева 
Node* createNode(int data, const string& path) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->path = path;
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
    Node* root = createNode(std::stoi(treeData[0][0]), "root");
    std::vector<Node*> nodes = { root };
    for (size_t i = 0; i < treeData.size(); i++) {
        std::string leftValue = treeData[i][1];
        std::string rightValue = treeData[i][2];
        string path = nodes[i]->path;
        if (!leftValue.empty() && leftValue != " ") {
            Node* leftNode = createNode(std::stoi(leftValue), path + "->left");
            nodes[i]->left = leftNode;
            nodes.push_back(leftNode);
        }
        if (!rightValue.empty() && rightValue != " ") {
            Node* rightNode = createNode(std::stoi(rightValue), path + "->right");
            nodes[i]->right = rightNode;
            nodes.push_back(rightNode);
        }
    }

    return root;
}

// Функция для поиска всех узлов с одинаковыми значениями 
void findNodesWithSameValue(Node* root) {
    map<int, vector<Node*>> valueMap;
    queue<Node*> nodeQueue;
    nodeQueue.push(root);
    while (!nodeQueue.empty()) {
        Node* current = nodeQueue.front();
        nodeQueue.pop();
        valueMap[current->data].push_back(current);
        if (current->left != nullptr) {
            nodeQueue.push(current->left);
        }
        if (current->right != nullptr) {
            nodeQueue.push(current->right);
        }
    }
    for (const auto& entry : valueMap) {
        if (entry.second.size() > 1) {
            cout << "Nodes with number " << entry.first << "(" << entry.second.size() << "):" << endl;
            for (Node* node : entry.second) {
                cout << node->path << endl;
            }
            cout << endl;
        }
    }
}
void dump0(Node* node, const string& prefix = "", bool root = true, bool last = true) {
    cout << prefix << (root ? "" : (last ? "|__ " : "|-- ")) << (node ? to_string(node->data) : "") << endl;
    if (!node || (!node->left && !node->right))
        return;
    vector<Node*> children{ node->left, node->right };
    for (size_t i = 0; i < children.size(); ++i) {
        dump0(children[i], prefix + (root ? "" : (last ? "    " : "|   ")), false, i + 1 >= children.size());
    }
}

int main() {
    // Построение дерева из файла 
    Node* root = buildTreeFromFile("old.txt");

    // Поиск и вывод узлов с одинаковыми значениями 
    findNodesWithSameValue(root);

    // Вывод дерева на экран
    dump0(root);

    return 0;
}