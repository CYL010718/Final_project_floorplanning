#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "SA.h"
using namespace std;

SAMgr* mySA = 0;

void inputBlock(fstream& fin){
    string name, junk;
    int width, height;

    fin >> name >> width >> height;

    mySA -> setOutLine(width,height);

    int blockNum, terminalNum;
    fin >> name >> blockNum;
    fin >> name >> terminalNum;

    mySA -> initTree(blockNum);
    mySA -> initNet();

    Block* newBlock;

    //cout << blockNum << endl;
    for(int i = 0; i < blockNum; ++i){
        fin >> name  >> width >> height;
        newBlock = new Block(name,width,height);
        mySA -> insertBlock(newBlock, name);
    }

    
    mySA -> packing();

    Terminal* newTerminal;
    for(int i = 0; i < terminalNum; ++i){
        fin >> name >> junk >> width >> height;
        newTerminal = new Terminal(name,width,height);
        mySA -> insertTerminal(newTerminal, name);
    }
}

void inputNet(fstream& fin){
    int netSize, netDegree;
    string name;
    Net* newNet;

    fin >> name >> netSize;
    mySA -> setNetSize(netSize);
    for(int i = 0; i < netSize; ++i){
        fin >> name >> netDegree;
        newNet = new Net(netDegree);
        mySA -> insertNet(newNet);
        for(int j = 0; j < netDegree; ++j){
            fin >> name;
            newNet -> insertTerminal(name);
        }
    }
    mySA -> setWireCost();
    mySA -> resetCdnLimit();
}

void output(fstream& fout, clock_t start){
    double alpha = mySA -> getAlpha();
    fout << alpha * mySA -> getAreaCost() + (1 - alpha) * mySA -> getWireCost() << endl;
    fout << mySA -> getWireCost() << endl;
    fout << mySA -> getAreaCost() << endl;
    fout << mySA -> getChipWidth() << " " << mySA -> getChipHeight() << endl;
    clock_t end = clock();
    fout << double((end - start) / CLOCKS_PER_SEC) << endl;
    mySA -> outputBlock(fout);

    fstream treeStruct;
    treeStruct.open("treeStruct", ios::out);
    mySA -> outputTree(treeStruct);
}

int main(int argc, char** argv){
    clock_t start = clock();
    if(argc != 5) {cerr << "Error! Invalid input arguments!" << endl; return 0;}

    fstream block_in;
    fstream net_in;
    fstream out;

    block_in.open(argv[2]);
    if(!block_in) {cerr << "Error! Cannot open input block file!" << endl; return 0;}

    net_in.open(argv[3]);
    if(!net_in) {cerr << "Error! Cannot open input net file!" << endl; return 0;}

    double alpha = stod(argv[1]);
    if(alpha > 1 || alpha < 0) {cerr << "Error! Invalid cost function ratio! Should be in range [0,1]." << endl; return 0;}
    
    out.open(argv[4],ios::out);

    mySA = new SAMgr(alpha);

    inputBlock(block_in);
   // cout << "hi1" << endl;
    inputNet(net_in);
   // cout << "hi2" << endl;

    mySA -> SA();
   // cout << "hi3" << endl;

    output(out, start);

    return 0;
}