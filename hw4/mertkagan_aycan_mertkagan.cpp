
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <random>
#include <ctime>

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
    bool operator==(const WordItem& other) const {
        return word == other.word;
    }
    bool operator<(const WordItem& other) const {
        return word < other.word;
    }
    bool operator>(const WordItem& other) const {
        return word > other.word;
    }
    bool operator<=(const WordItem& other) const {
        return word <= other.word;
    }
    bool operator>=(const WordItem& other) const {
        return word >= other.word;
    }

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

        //float loadFactor = static_cast<float>(itemCount) / newSize;
        //std::cout << "rehashed...\n"
        //          << "previous table size:" << oldSize
        //          << ", new table size: " << newSize
        //          << ", current unique word count " << itemCount
        //          << ", current load factor: " << loadFactor << std::endl;
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

#include <algorithm>
#include <vector>
#include <random>

// Function prototypes for Quick Sort
template <typename T>
std::vector<T> quickSortFirstPivot(std::vector<T> arr);
template <typename T>
std::vector<T> quickSortRandomPivot(std::vector<T> arr);
template <typename T>
std::vector<T> quickSortMedianPivot(std::vector<T> arr);

// Helper functions for Quick Sort
template <typename T>
void quickSortHelper(std::vector<T>& arr, int low, int high, int pivotType);
template <typename T>
int partitionFirstPivot(std::vector<T>& arr, int low, int high);
template <typename T>
int partitionRandomPivot(std::vector<T>& arr, int low, int high);
template <typename T>
int partitionMedianPivot(std::vector<T>& arr, int low, int high);

// Quick Sort with the first element as pivot
template <typename T>
std::vector<T> quickSortFirstPivot(std::vector<T> arr) {
    quickSortHelper(arr, 0, arr.size() - 1, 0);
    return arr;
}

// Quick Sort with a random element as pivot
template <typename T>
std::vector<T> quickSortRandomPivot(std::vector<T> arr) {
    quickSortHelper(arr, 0, arr.size() - 1, 1);
    return arr;
}

// Quick Sort with the median as pivot
template <typename T>
std::vector<T> quickSortMedianPivot(std::vector<T> arr) {
    quickSortHelper(arr, 0, arr.size() - 1, 2);
    return arr;
}

// Quick Sort helper function
template <typename T>
void quickSortHelper(std::vector<T>& arr, int low, int high, int pivotType) {
    if (low < high) {
        int pi;
        switch (pivotType) {
        case 0: pi = partitionFirstPivot(arr, low, high); break;
        case 1: pi = partitionRandomPivot(arr, low, high); break;
        case 2: pi = partitionMedianPivot(arr, low, high); break;
        default: pi = partitionFirstPivot(arr, low, high);
        }
        quickSortHelper(arr, low, pi - 1, pivotType);
        quickSortHelper(arr, pi + 1, high, pivotType);
    }
}

// Partition using first element as pivot
template <typename T>
int partitionFirstPivot(std::vector<T>& arr, int low, int high) {
    T pivot = arr[low];
    int i = low;

    for (int j = low + 1; j <= high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i], arr[low]);
    return i;
}

// Partition using random element as pivot
template <typename T>
int partitionRandomPivot(std::vector<T>& arr, int low, int high) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);
    int randomPivotIndex = dis(gen);
    std::swap(arr[randomPivotIndex], arr[low]); // Move the pivot to the start
    return partitionFirstPivot(arr, low, high); // Use the same partition as the first pivot
}

// Partition using median as pivot
template <typename T>
int partitionMedianPivot(std::vector<T>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    std::nth_element(arr.begin() + low, arr.begin() + mid, arr.begin() + high);
    std::swap(arr[mid], arr[low]); // Move the median to the start
    return partitionFirstPivot(arr, low, high); // Use the same partition as the first pivot
}

template <typename T>
std::vector<T> insertionSort(std::vector<T> arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        T key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    return arr;
}

template <typename T>
void mergeInPlace(std::vector<T>& arr, int l, int m, int r);

template <typename T>
void mergeSortInPlace(std::vector<T>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSortInPlace(arr, l, m);
        mergeSortInPlace(arr, m + 1, r);

        mergeInPlace(arr, l, m, r);
    }
}

template <typename T>
void mergeInPlace(std::vector<T>& arr, int l, int m, int r) {
    int i = l; // Starting index for the left subarray
    int j = m + 1; // Starting index for the right subarray

    if (arr[m] <= arr[j]) {
        // The two subarrays are already in place
        return;
    }

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            i++;
        }
        else {
            T value = arr[j];
            int index = j;

            // Shift all the elements between i and j to the right by one.
            while (index != i) {
                arr[index] = arr[index - 1];
                index--;
            }
            arr[i] = value;

            // Update all the pointers
            i++;
            m++;
            j++;
        }
    }
}



template <typename T>
void heapify(std::vector<T>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
std::vector<T> heapSort(std::vector<T> arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    return arr;
}


// Helper function to update the document item
//#include <algorithm> // only for the following function
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

int binarySearch(const std::vector<WordItem>& items, const std::string& target) {
    int low = 0;
    int high = items.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        const WordItem& midItem = items[mid];

        if (midItem.word == target) {
            return mid; // Word found
        }
        if (midItem.word < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return -1; // Word not found
}


int main()
{
    BST<std::string, WordItem*> bst;
    HashTable<std::string, WordItem*> hashTable;
    std::vector<WordItem> vecForWordItems; // Vector to store WordItems

    int numFiles;
    std::cout << "Enter number of input files: ";
    std::cin >> numFiles;
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

                // Logic for vector
                auto it = std::find_if(vecForWordItems.begin(), vecForWordItems.end(),
                    [&word](const WordItem& wi) { return wi.word == word; });

                if (it != vecForWordItems.end()) {
                    // Word found in vector, update count
                    auto docIt = std::find_if(it->documentDetails.begin(), it->documentDetails.end(),
                        [&fileName](const DocumentItem& di) { return di.documentName == fileName; });
                    if (docIt != it->documentDetails.end()) {
                        docIt->count++;
                    }
                    else {
                        it->documentDetails.emplace_back(fileName, 1);
                    }
                }
                else {
                    // Word not found, add new WordItem
                    WordItem newItem;
                    newItem.word = word;
                    newItem.documentDetails.emplace_back(fileName, 1);
                    vecForWordItems.push_back(newItem);
                }
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

    auto start = std::chrono::high_resolution_clock::now();
    clock_t begin_time = clock();
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
    //double endBST = float(clock() - begin_time);
    auto endBST = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    // Performance comparison for Hash Table
    //begin_time = clock();
    std::map<std::string, std::map<std::string, int>> docWordCountMap2;
    start = std::chrono::high_resolution_clock::now();
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
    //double endHT = float(clock() - begin_time);
    auto endHT = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    // Performance comparison for Binary Search
    std::vector<WordItem> vecForBS = vecForWordItems;
    std::sort(vecForBS.begin(), vecForBS.end(), [](const WordItem& a, const WordItem& b) {
        return a.word < b.word;
        });

    // Prepare a map similar to docWordCountMap2 for binary search results
    std::map<std::string, std::map<std::string, int>> docWordCountMapBinarySearch;

    // Performance comparison for binary search in vecForWordItems
    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();

    for (const auto& word : queryWords) {
        int index = binarySearch(vecForWordItems, word);
        if (index != -1) {
            for (const auto& docItem : vecForWordItems[index].documentDetails) {
                docWordCountMapBinarySearch[docItem.documentName][word] = docItem.count;
            }
        }
    }

    // for k, run search to benchmark
    for (int i = 0; i < k; i++) {
        for (const auto& word : queryWords) {
            int index = binarySearch(vecForWordItems, word);
            // if (index != -1) {
            //     for (const auto &docItem : vecForWordItems[index].documentDetails) {
            //         docWordCountMapBinarySearch[docItem.documentName][word] = docItem.count;
            //     }
            // }
        }
    }
    //double endBS = float(clock() - begin_time);
    auto endBS = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);


    bool foundAnyInBST = false;
    bool foundAnyInHashTable = false;
    bool foundAnyInBS = false;

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

    // Display the results for bs
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
            foundAnyInBS = true;
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

    if (!foundAnyInBS)
    {
        std::cout << "No document contains the given query\n";
    }

    //sortings

    // Timing for Quick Sort with Median Pivot
    std::vector<WordItem> vecForSorting = vecForWordItems;
    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();
    auto sortedByQuickSortMedian = quickSortMedianPivot(vecForSorting);
    //double durationQuickSortMedian = double(clock() - begin_time);
    auto durationQuickSortMedian = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();
    auto sortedByQuickSortRandom = quickSortRandomPivot(vecForSorting);
    //double durationQuickSortRandom = double(clock() - begin_time);
    auto durationQuickSortRandom = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();
    auto sortedByQuickSortFirst = quickSortFirstPivot(vecForSorting);
    //double durationQuickSortFirst = double(clock() - begin_time);
    auto durationQuickSortFirst = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();
    std::vector<WordItem> vecForMergeSort = vecForWordItems;
    mergeSortInPlace(vecForMergeSort, 0, vecForMergeSort.size() - 1);
    //double durationMergeSort = double(clock() - begin_time);
    auto durationMergeSort = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();
    auto sortedByHeapSort = heapSort(vecForSorting);
    //double durationHeapSort = double(clock() - begin_time);
    auto durationHeapSort = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    begin_time = clock();
    start = std::chrono::high_resolution_clock::now();
    auto sortedByInserionSort = insertionSort(vecForSorting);
    //double durationInserionSort = double(clock() - begin_time);
    auto durationInserionSort = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);


    // Output the results and performance data
    std::cout << std::endl << "Binary Search Tree Time: " << endBST.count() / k << std::endl;
    std::cout << "Hash Table Time: " << endHT.count() << std::endl;
    std::cout << "Binary Search Time: " << endBS.count() << std::endl;
    std::cout << std::endl;
    std::cout << "Quick Sort(median) Time: " << durationQuickSortMedian.count() << std::endl;
    std::cout << "Quick Sort(random) Time: " << durationQuickSortRandom.count() << std::endl;
    std::cout << "Quick Sort(first) Time: " << durationQuickSortFirst.count() << std::endl;
    std::cout << "Merge Sort Time: " << durationMergeSort.count() << std::endl;
    std::cout << "Heap Sort Time: " << durationHeapSort.count() << std::endl;
    std::cout << "Insertion Sort Time: " << durationInserionSort.count() << std::endl;
    std::cout << std::endl;
    // Calculate and display speedup
    double speedUp1 = double(endBST.count()) / endHT.count();
    std::cout << "Speed Up BST/HT: " << speedUp1 << std::endl;
    double speedUp2 = double(durationMergeSort.count()) / durationQuickSortMedian.count();
    std::cout << "Speed Up Merge Sort/Quick Sort(Median): " << speedUp2 << std::endl;
    double speedUp3 = double(durationHeapSort.count()) / durationQuickSortMedian.count();
    std::cout << "Speed Up Heap Sort/Quick Sort(Median): " << speedUp3 << std::endl;
    double speedUp4 = double(durationInserionSort.count()) / durationQuickSortMedian.count();
    std::cout << "Speed Up Insertion Sort/Quick Sort(Median): " << speedUp4 << std::endl;
    std::cout << std::endl;
    double speedUp5 = double(endBS.count()) / endBST.count();
    std::cout << "Speed Up Binary Search / Binary Search Tree Time: " << speedUp5 << std::endl;
    double speedUp6 = double(endBS.count()) / endHT.count();
    std::cout << "Speed Up Binary Search / Hash Table Time: " << speedUp6 << std::endl;
    return 0;
}
