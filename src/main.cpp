#include<iostream>
#include<string>
#include "huffman.h"

using namespace std;

int main(int argc,char* argv[]){

  if(argc!=4){
    cerr << "Error: Incorrect number of arguments .\n";
    cerr << "Usage:" << argv[0] << " <compress/decompress> <inputfile> <outputfile>\n";
    return 1;
  }

  string mode = argv[1];
  string inputfile = argv[2];
  string outputfile = argv[3];

  Huffman huffmanTool;

  if(mode == "compress"){
    huffmanTool.compress(inputfile, outputfile);
  } else if(mode == "decompress"){
    huffmanTool.decompress(inputfile, outputfile);
  } else {
   cerr << "Error: Unknown mode '" << mode << "'. Use 'compress' or 'decompress'.\n";
    return 1;
  }

  return 0;
}