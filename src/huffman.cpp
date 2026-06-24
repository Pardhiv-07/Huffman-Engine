#include "huffman.h"
#include<iostream>
#include<fstream>
#include<queue>
#include<vector>

using namespace std;

void Huffman::storeCodes(Node*root,string str){
  if(root == nullptr)
    return;

  if(!root->left && !root->right){
    huffmanCodes[root->ch] = str;
  }

  storeCodes(root->left, str + "0");
  storeCodes(root->right, str + "1");
}

void Huffman::compress(const string& inputfile, const string& outputfile) {
    // Implementing the compression logic here
    cout << "[C++ BACKEND]Analyzing file: " << inputfile << "...\n";

// 1. Reading the input file
    ifstream inFile(inputfile);
    if(!inFile){
      cerr << "Error: Could not open input file!\n";
        return;
    }
    
// Reading the whole file into a string
    string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    if(text.empty()){
      cerr << "File is empty!\n";
        return;
    }

// 2. Counting character frequencies
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

// 3. Creating a MinHeap and inserting all characters
    priority_queue<Node*, vector<Node*>, compare> minHeap;
    int idCounter = 0; // Unique identifier for each node
    for (auto pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second, idCounter++));
    }

// 4. Building the Huffman Tree
    while(minHeap.size()!= 1){
      Node* left = minHeap.top();
      minHeap.pop();
      Node* right = minHeap.top();
      minHeap.pop();

      Node* top = new Node('$', left->freq + right->freq, idCounter++);
      top->left = left;
      top-> right = right;

      minHeap.push(top);
    }   
    
// 5. Generating and printing Huffman Codes
    storeCodes(minHeap.top(), "");
    
    cout << "\n--- Huffman Dictionary Generated ---\n";
    for (auto pair : huffmanCodes){
      if(pair.first == '\n'){
        cout << "'\\n' : " << pair.second << "\n";
      }
      else 
      cout << "'" << pair.first << "'  : " << pair.second << "\n";
    }
    cout << "------------------------------------\n";

// 6. opening output file in binary mode
    ofstream outFile(outputfile,ios::binary);
    
    size_t mapSize = freqMap.size();
    outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

    for (auto pair : freqMap) {
        outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }

// 7. Converting the whole string into 0's and 1's
    string encodedString = "";
    for (char ch : text) {
        encodedString += huffmanCodes[ch];
    }
    
// 8. writing the exact number of bits
    size_t totalBits = encodedString.length();
    outFile.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));
    
// 9. Bit-packing loop
    unsigned char currentByte = 0;
    int bitCount = 0;

    for (char bit : encodedString) {
        currentByte = currentByte << 1; 
        if (bit == '1') {
            currentByte = currentByte | 1; 
        }
        bitCount++;

        if (bitCount == 8) {
            outFile.write(reinterpret_cast<const char*>(&currentByte), sizeof(currentByte));
            currentByte = 0;
            bitCount = 0;
        }
    }    

// 10. Writing any remaining bits
    if (bitCount > 0) {
        currentByte = currentByte << (8 - bitCount);
        outFile.write(reinterpret_cast<const char*>(&currentByte), sizeof(currentByte));
    }
    
    outFile.close();
    cout << "[C++ Backend] Compression complete. Saved to " << outputfile << "\n";
}

void Huffman::decompress(const string& inputfile, const string& outputfile) {
    // Implementing the decompression logic here
    cout << "[C++BACKEND]Decompressing " << inputfile  << "...\n";

    // 1. Opening the compressed binary file
    ifstream inFile(inputfile, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open compressed file!\n";
        return;
    }

    //2. Reading the Huffman Dictionary size
    size_t mapSize;
    inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

   // 3 & 4. Read the file header and directly rebuild the Min-Heap
    priority_queue<Node*, std::vector<Node*>, compare> minHeap;
    int idCounter = 0; // Unique identifier for each node
    
    for (size_t i = 0; i < mapSize; i++) {
        char ch;
        int freq;
        inFile.read(reinterpret_cast<char*>(&ch), sizeof(ch));
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        
        // Fix: Push directly into the heap to perfectly preserve the original insertion order!
        minHeap.push(new Node(ch, freq, idCounter++));
    }

    if(minHeap.empty()) return ;

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();
        Node* top = new Node('$', left->freq + right->freq, idCounter++);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    Node* root = minHeap.top();

    //5. Reading the total number of bits
    size_t totalBits;
    inFile.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

    //6. opening the output file to write the restored normal text
    ofstream outFile(outputfile);

    //7. Decoding the bits by traversing through the tree
       Node* current = root;
    size_t bitsRead = 0;
    unsigned char currentByte;

    while (inFile.read(reinterpret_cast<char*>(&currentByte), sizeof(currentByte))){
      // Loop through the 8 bits inside this byte (from left to right)
        for (int i = 7; i >= 0; i--) {
            
            if (bitsRead >= totalBits) break;

            // ExtractING the i-th bit
            int bit = (currentByte >> i) & 1;

            // TraversING the tree based on the bit
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            if (!current->left && !current->right) {
                outFile << current->ch;  // Writing the character to the text file
                current = root;          // Resetting back to the top of the tree for the next bit
            }
            bitsRead++;
    }
}

inFile.close();
outFile.close();
std::cout << "[C++ Backend] Decompression complete. Saved to " << outputfile << "\n";
}