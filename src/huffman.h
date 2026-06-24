#ifndef HUFFMAN_H
#define HUFFMAN_H

using namespace std;
#include<string>
#include<unordered_map>

struct Node {
    char ch;
    int freq;
    int id;
    Node *left, *right;

    Node(char character, int frequency,int identifier){
      ch = character;
      freq = frequency;
      id = identifier;
      left = right = nullptr;
    }
  };

  struct compare{
    bool operator()(Node *l, Node *r){
      if(l->freq == r->freq) {
        return l->id > r->id; // If frequencies are equal, compare by id
      }
      return (l->freq > r->freq);
    }
  };

class Huffman{
  private:
  unordered_map<char, string> huffmanCodes;

  void storeCodes(Node* root,string str);

  public:
  // To compress
  void compress(const string& inputfile, const string& outputfile);
  // To decompress
  void decompress(const string& inputfile, const string& outputfile);
};

#endif