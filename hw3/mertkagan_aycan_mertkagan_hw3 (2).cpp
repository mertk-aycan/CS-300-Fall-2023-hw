#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

struct DocumentItem
{
    std::string documentName;
    int count;

    DocumentItem(const std::string& name, int c)
        : documentName(name), count(c) {}
};

struct WordItem
{
    std::string word;
    std::vector<DocumentItem> documentDetails;
};

template <typename Key, typename Value>
class BST
{
private:
    struct BSTNode
    {
        Key key;
        Value value;
        BSTNode* left, * right;

        BSTNode(Key k, Value v)
            : key(std::move(k)), value(std::move(v)), left(nullptr), right(nullptr) {}
    };

    BSTNode* root;

    // Insertion
    BSTNode* insert(BSTNode* node, Key key, Value value)
    {
        if (node == nullptr)
            return new BSTNode(std::move(key), std::move(value));

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);

        return node;
    }

    // Search function
    Value search(BSTNode* root, Key key)
    {
        if (root == nullptr || root->key == key)
            return root ? root->value : nullptr; // Correctly returning nullptr

        if (root->key < key)
            return search(root->right, key);

        return search(root->left, key);
    }

public:
    BST() : root(nullptr) {}

    // Destructor to handle memory deallocation
    ~BST()
    {
        clear(root);
    }

    Value search(Key key)
    {
        return search(root, key);
    }

    void insert(Key key, Value value)
    {
        root = insert(root, key, value);
    }

    void clear(BSTNode* node)
    {
        if (node == nullptr)
            return;

        clear(node->left);
        clear(node->right);
        delete node;
    }
};

#include <list>
#include <vector>
#include <string>

#include <iostream>
#include <string>

template <typename Key, typename Value>
class HashTable
{
private:
    struct HashNode
    {
        Key key;
        Value value;
        bool occupied; // Indicates whether the node is occupied

        HashNode() : occupied(false) {}
        HashNode(const Key& k, const Value& v) : key(k), value(v), occupied(true) {}
    };

    HashNode* table;
    size_t tableSize;
    size_t itemCount;
    float loadFactorThreshold;

    size_t hashFunction(const Key& key, size_t tableSize)
    {
        size_t hash = 0;
        for (char ch : key)
        {
            hash = (hash * 31) ^ ch;
        }
        return hash % tableSize;
    }

    void rehash()
    {
        size_t oldSize = tableSize;
        HashNode* oldTable = table;
        size_t newSize = oldSize * 2 + 1;
        table = new HashNode[newSize];
        tableSize = newSize;
        itemCount = 0;

        for (size_t i = 0; i < oldSize; ++i)
        {
            if (oldTable[i].occupied)
            {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }

        delete[] oldTable;

        float loadFactor = static_cast<float>(itemCount) / newSize;
        std::cout << "rehashed...\n"
            << "previous table size:" << oldSize
            << ", new table size: " << newSize
            << ", current unique word count " << itemCount
            << ", current load factor: " << loadFactor << std::endl;
    }

public:
    HashTable(size_t initialSize = 53, float loadFactor = 0.8)
        : tableSize(initialSize), itemCount(0), loadFactorThreshold(loadFactor)
    {
        table = new HashNode[initialSize];
    }

    void insert(const Key& key, const Value& value)
    {
        size_t index = hashFunction(key, tableSize);
        size_t i = 0;
        while (table[index].occupied)
        {
            if (table[index].key == key)
            {
                table[index].value = value; // Update existing value
                return;
            }
            i++;
            index = (index + i * i) % tableSize; // Quadratic probing
        }

        table[index] = HashNode(key, value);
        itemCount++;

        if (static_cast<float>(itemCount) / tableSize > loadFactorThreshold)
        {
            rehash();
        }
    }

    Value search(const Key& key)
    {
        size_t index = hashFunction(key, tableSize);
        size_t i = 0;
        while (table[index].occupied)
        {
            if (table[index].key == key)
            {
                return table[index].value; // Found
            }
            i++;
            index = (index + i * i) % tableSize; // Quadratic probing
            if (i > tableSize)
                break; // Avoid infinite loop
        }
        return nullptr; // Not found
    }

    ~HashTable()
    {
        for (size_t i = 0; i < tableSize; ++i)
        {
            if (table[i].occupied && table[i].value)
                delete table[i].value; // Assuming Value is a pointer type
        }
        delete[] table;
    }

    size_t getItemCount() const
    {
        return itemCount;
    }

    size_t getTableSize() const
    {
        return tableSize;
    }
};


// Helper function to update the document item
#include <algorithm> // only for the following function
void updateDocumentItem(WordItem* wordItem, const std::string& documentName)
{
    auto& documents = wordItem->documentDetails;
    auto it = std::find_if(documents.begin(), documents.end(),
        [&documentName](const DocumentItem& item)
        {
            return item.documentName == documentName;
        });
    if (it != documents.end())
    {
        it->count++;
    }
    else
    {
        documents.emplace_back(documentName, 1);
    }
}


// Helper function to preprocess and tokenize text
#include <vector>
#include <string>
#include <map>

std::vector<std::string> preprocessAndTokenize(const std::string& text)
{
    std::string lowerText = text;
    for (char& c : lowerText)
        c = tolower(c);

    std::vector<std::string> tokens;
    std::string currentWord;

    for (char c : lowerText)
    {
        if (isalpha(c))
        {
            currentWord += c;
        }
        else if (!currentWord.empty())
        {
            tokens.push_back(currentWord);
            currentWord.clear();
        }
    }

    if (!currentWord.empty())
    {
        tokens.push_back(currentWord);
    }

    return tokens;
}

int main()
{
    BST<std::string, WordItem*> bst;
    HashTable<std::string, WordItem*> hashTable;

    int numFiles;
    std::cout << "Enter number of input files: ";    std::cin >> numFiles;
    std::cin.ignore();
    // vector for filenames
    std::vector<std::string> fileNames;

    // Get all file names first
    for (int i = 0; i < numFiles; ++i)
    {
        std::string fileName;
        std::cout << "Enter " << i + 1 << ". file name: ";
        std::getline(std::cin, fileName);
        fileNames.push_back(fileName);
    }

    size_t uniqueWordCount = 0;

    // Process each document
    for (const auto& fileName : fileNames)
    {
        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line))
        {
            auto words = preprocessAndTokenize(line);
            for (const auto& word : words)
            {
                // Update or insert in BST
                WordItem* bstItem = bst.search(word);
                if (!bstItem)
                {
                    // Increment the unique word c
                    uniqueWordCount++;

                    // create documentItem
                    DocumentItem documentItem{ fileName, 0 };

                    // make a vector out of it
                    std::vector<DocumentItem> documentDetails;
                    documentDetails.push_back(documentItem);

                    // create wordItem
                    bstItem = new WordItem{ word, documentDetails };

                    // insert into BST
                    bst.insert(word, bstItem);
                }
                updateDocumentItem(bstItem, fileName);

                // Update or insert in Hash Table
                WordItem* hashItem = hashTable.search(word);
                if (!hashItem)
                {
                    // create documentItem
                    DocumentItem documentItem{ fileName, 0 };

                    // make a vector out of it
                    std::vector<DocumentItem> documentDetails;
                    documentDetails.push_back(documentItem);

                    // create wordItem
                    hashItem = new WordItem{ word, documentDetails };

                    // insert into Hash Table
                    hashTable.insert(word, hashItem);
                }
                updateDocumentItem(hashItem, fileName); // Use the same function as for BST
            }
        }
        file.close();
    }

    // Display the unique word count and current load ratio
    std::cout << "After preprocessing, the unique word count is " << uniqueWordCount << ". ";
    std::cout << "Current load ratio is " << static_cast<float>(hashTable.getItemCount()) / hashTable.getTableSize() << std::endl;

    // Query processing
    std::string queryLine;
    std::cout << "Enter queried words in one line: ";
    std::getline(std::cin, queryLine);
    auto queryWords = preprocessAndTokenize(queryLine);

    // Prepare a map to track the count of each word in each document
    std::map<std::string, std::map<std::string, int>> docWordCountMap;

    // Performance comparison for BST
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& word : queryWords)
    {
        WordItem* bstItem = bst.search(word);
        if (bstItem)
        {
            for (const auto& docItem : bstItem->documentDetails)
            {
                docWordCountMap[docItem.documentName][word] = docItem.count;
            }
        }
    }

    int k = 100;
    // for k, run search to benchmark
    for (int i = 0; i < k; i++)
    {
        for (const auto& word : queryWords)
        {
            WordItem* bstItem = bst.search(word);
            // if (bstItem)
            // {
            //     for (const auto &docItem : bstItem->documentDetails)
            //     {
            //         docWordCountMap[docItem.documentName][word] = docItem.count;
            //     }
            // }
        }
    }

    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start);

    // Performance comparison for Hash Table
    start = std::chrono::high_resolution_clock::now();
    std::map<std::string, std::map<std::string, int>> docWordCountMap2;

    for (const auto& word : queryWords)
    {
        WordItem* hashItem = hashTable.search(word);
        if (hashItem)
        {
            for (const auto& docItem : hashItem->documentDetails)
            {
                docWordCountMap2[docItem.documentName][word] = docItem.count;
            }
        }
    }

    // for k, run search to benchmark
    for (int i = 0; i < k; i++)
    {
        for (const auto& word : queryWords)
        {
            WordItem* hashItem = hashTable.search(word);
            // if (hashItem)
            // {
            //     for (const auto &docItem : hashItem->documentDetails)
            //     {
            //         docWordCountMap2[docItem.documentName][word] = docItem.count;
            //     }
            // }
        }
    }

    bool foundAnyInBST = false;
    bool foundAnyInHashTable = false;

    // Display the results for bst
    for (const auto& docEntry : docWordCountMap)
    {
        bool foundAll = true;
        for (const auto& word : queryWords)
        {
            if (docEntry.second.find(word) == docEntry.second.end())
            {
                foundAll = false;
                break;
            }
        }

        if (foundAll)
        {
            foundAnyInBST = true;
            std::cout << "in Document " << docEntry.first << ", ";
            bool firstWord = true;
            for (const auto& word : queryWords)
            {
                if (!firstWord)
                    std::cout << ", ";
                std::cout << word << " found " << docEntry.second.at(word) << " times";
                firstWord = false;
            }
            std::cout << ".\n";
        }
    }

    if (!foundAnyInBST)
    {
        std::cout << "No document contains the given query\n";
    }

    // Display the results for hashtable
    for (const auto& docEntry : docWordCountMap2)
    {
        bool foundAll = true;
        for (const auto& word : queryWords)
        {
            if (docEntry.second.find(word) == docEntry.second.end())
            {
                foundAll = false;
                break;
            }
        }

        if (foundAll)
        {
            foundAnyInHashTable = true;
            std::cout << "in Document " << docEntry.first << ", ";
            bool firstWord = true;
            for (const auto& word : queryWords)
            {
                if (!firstWord)
                    std::cout << ", ";
                std::cout << word << " found " << docEntry.second.at(word) << " times";
                firstWord = false;
            }
            std::cout << ".\n";
        }
    }

    if (!foundAnyInHashTable)
    {
        std::cout << "No document contains the given query\n";
    }

    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start);

    // Output the results and performance data
    std::cout << "BST Time: " << BSTTime.count() << " nanoseconds\n";
    std::cout << "HashTable Time: " << HTTime.count() << " nanoseconds\n";
    // Calculate and display speedup
    double speedUp = static_cast<double>(BSTTime.count()) / HTTime.count();
    std::cout << "Speed Up (BST/HashTable): " << speedUp << std::endl;

    return 0;
}