#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>

struct DocumentItem
{
    std::string documentName;
    int count;

    DocumentItem(std::string name, int c) : documentName(std::move(name)), count(c) {}
};

struct WordItem
{
    std::string word;
    std::vector<DocumentItem> documentDetails;

    WordItem(std::string w) : word(std::move(w)) {}
};

// Function to compare DocumentItems by their document name
bool compareDocumentItems(const DocumentItem& a, const DocumentItem& b)
{
    return a.documentName < b.documentName;
}

// Helper function to convert text to lower case and remove non-alphabet characters
std::string preprocessText(const std::string& text)
{
    std::string result;
    for (char ch : text)
    {
        if (std::isalpha(ch))
        {
            result += std::tolower(ch);
        }
    }
    return result;
}

// Helper function to split a string into tokens by whitespace
std::vector<std::string> tokenizeText(const std::string& text)
{
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token)
    {
        tokens.push_back(preprocessText(token));
    }
    return tokens;
}

template <typename Key, typename Value>
class AVLTree
{
private:
    struct AVLNode
    {
        Key key;
        Value value;
        int height;
        AVLNode* left, * right;

        AVLNode(Key k, Value v)
            : key(std::move(k)), value(std::move(v)), height(1), left(nullptr), right(nullptr) {}
    };

    AVLNode* root;

    // Utility functions to get height and balance factor of a node
    int height(AVLNode* N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    int getBalance(AVLNode* N)
    {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    // Rotations
    AVLNode* rightRotate(AVLNode* y)
    {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    AVLNode* leftRotate(AVLNode* x)
    {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    // Insertion
    AVLNode* insert(AVLNode* node, Key key, Value value)
    {
        // 1. Perform the normal BST insertion for the key

        if (node == nullptr)
            return new AVLNode(std::move(key), std::move(value));

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else
        { // Equal keys are not allowed in BST
            return node;
        }

        // 2. Update height of this ancestor node (the height of the tree rooted
        node->height = 1 + std::max(height(node->left), height(node->right));

        // 3. Get the balance factor of this ancestor node to check whether
        // this node became unbalanced, and if so, there are 4 cases
        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the (unchanged) node pointer
        return node;
    }

    // Utility function to insert a new key-value pair and balance the tree
    void insert_new(Key key, Value value)
    {
        root = insert(root, key, value);
    }

    // Search function
    Value search(AVLNode* root, Key key)
    {
        // Base cases: root is null or key is present at root
        if (root == nullptr || root->key == key)
            return root ? root->value : nullptr;

        // Key is greater than root's key
        if (root->key < key)
            return search(root->right, key);

        // Key is smaller than root's key
        return search(root->left, key);
    }

    void clearTree(AVLNode* node)
    {
        if (node)
        {
            clearTree(node->left);
            clearTree(node->right);
            delete node->value; // Delete the WordItem object
            delete node;        // Delete the node itself
        }
    }

    // Utility function to remove a node
    AVLNode* remove(AVLNode* node, Key key)
    {
        // STEP 1: Perform standard BST delete
        if (node == nullptr)
            return node;

        // If the key to be deleted is smaller than the node's key,
        // then it lies in left subtree
        if (key < node->key)
            node->left = remove(node->left, key);

        // If the key to be deleted is greater than the node's key,
        // then it lies in right subtree
        else if (key > node->key)
            node->right = remove(node->right, key);

        // if key is same as node's key, then this is the node to be deleted
        else
        {
            // node with only one child or no child
            if ((node->left == nullptr) || (node->right == nullptr))
            {
                AVLNode* temp = node->left ? node->left : node->right;

                // No child case
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else               // One child case
                    *node = *temp; // Copy the contents of the non-empty child
                delete temp;
            }
            else
            {
                // node with two children: Get the inorder successor (smallest
                // in the right subtree)
                AVLNode* temp = minValueNode(node->right);

                // Copy the inorder successor's data to this node
                node->key = temp->key;

                // Delete the inorder successor
                node->right = remove(node->right, temp->key);
            }
        }

        // If the tree had only one node then return
        if (node == nullptr)
            return node;

        // STEP 2: Update height of the current node
        node->height = 1 + std::max(height(node->left), height(node->right));

        // STEP 3: Get the balance factor of this node (to check whether
        // this node became unbalanced)
        int balance = getBalance(node);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode* minValueNode(AVLNode* node)
    {
        AVLNode* current = node;

        // loop down to find the leftmost leaf
        while (current->left != nullptr)
            current = current->left;

        return current;
    }

public:
    AVLTree() : root(nullptr) {}

    // Public interface to search the tree
    Value search(Key key)
    {
        return search(root, key);
    }

    // Public interface to insert into the tree
    void insert(Key key, Value value)
    {
        root = insert(root, key, value);
    }

    // Destructor to deallocate nodes
    ~AVLTree()
    {
        // Implement a recursive node deletion function
        clear();
    }

    // Function to clear the tree
    void clear(AVLNode* node)
    {
        if (node)
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
    void clear()
    {
        clearTree(root);
        root = nullptr;
    }
    // Public interface to remove a node from the tree
    void remove(Key key)
    {
        root = remove(root, key);
    }
};

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>

int main()
{
    AVLTree<std::string, WordItem*> myTree;
    int numFiles;
    std::string fileName, line, word;

    // Get the number of documents
    std::cout << "Enter number of input files: ";
    std::cin >> numFiles;
    std::cin.ignore();

    // Process each document
    for (int i = 0; i < numFiles; ++i)
    {
        std::cout << "Enter " << i + 1 << ". file name: ";
        getline(std::cin, fileName);

        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Could not open file: " << fileName << std::endl;
            continue;
        }

        while (file >> word)
        {
            std::string processedWord = preprocessText(word);
            WordItem* wordItem = myTree.search(processedWord);

            if (wordItem)
            {
                // Update or add new document
                auto& docs = wordItem->documentDetails;
                auto it = std::find_if(docs.begin(), docs.end(),
                    [&fileName](const DocumentItem& item)
                    { return item.documentName == fileName; });
                if (it != docs.end())
                {
                    it->count++;
                }
                else
                {
                    docs.emplace_back(fileName, 1);
                }
            }
            else
            {
                // Insert new WordItem
                WordItem* newWordItem = new WordItem(processedWord);
                newWordItem->documentDetails.emplace_back(fileName, 1);
                myTree.insert(processedWord, newWordItem);
            }
        }
        file.close();
    }

    // Command processing loop
    while (true)
    {
        std::cout << "\nEnter queried words in one line: ";
        getline(std::cin, line);
        if (line == "ENDOFINPUT")
        {
            break; // Terminate the program
        }

        if (line.rfind("REMOVE ", 0) == 0)
        {
            std::string wordToRemove = line.substr(7);
            // Logic to remove 'wordToRemove' from the AVL tree
            myTree.remove(wordToRemove); // Implement this method in AVLTree
            std::cout << wordToRemove << " has been REMOVED\n";
            continue;
        }

        std::vector<std::string> queryWords = tokenizeText(line);

        // Map to track the count of each word in each document
        std::map<std::string, std::map<std::string, int>> docWordCount;

        for (const auto& qWord : queryWords)
        {
            WordItem* wordItem = myTree.search(qWord);
            if (wordItem)
            {
                for (const auto& doc : wordItem->documentDetails)
                {
                    docWordCount[doc.documentName][qWord] = doc.count;
                }
            }
        }

        // Filtering and printing results
        bool found = false;
        for (const auto& doc : docWordCount)
        {
            if (doc.second.size() == queryWords.size())
            {
                found = true;
                std::cout << "in Document " << doc.first << ", ";
                for (const auto& qWord : queryWords)
                {
                    std::cout << qWord << " found " << doc.second.at(qWord) << " times";
                    if (&qWord != &queryWords.back())
                    {
                        std::cout << ", ";
                    }
                }
                std::cout << ".\n";
            }
        }

        if (!found)
        {
            std::cout << "No document contains the given query\n";
        }
    }

    // Memory cleanup
    myTree.clear();

    return 0;
}
