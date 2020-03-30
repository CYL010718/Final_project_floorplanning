#include <iostream>
#include "treeMgr.h"
#include "netMgr.h"
using namespace std;

class SAMgr{
    public:
         SAMgr(double a): alpha(a), temperature(500), r(0.9), p(10), inOutLine(false), count(0), avgAreaCost(0), avgWireCost(0), failCount(0){};
         ~SAMgr(){};
         void SA();

         void setOutLine(int w, int h) {outLineWidth = w; outLineHeight = h;}
         void initTree(size_t n){myTreeMgr = new TreeMgr(n);}
         void initNet(){myNetMgr = new NetMgr;}
         void setNetSize(size_t n) {myNetMgr -> reserveList(n);} 
         void insertBlock(Block* b, string& n){myTreeMgr -> insertNode(b); myNetMgr -> insertBlock(n,b);}
         void insertTerminal(Terminal* t, string& n){myNetMgr -> insertTerminal(n,t);}
         void insertNet(Net* n) {myNetMgr -> insertNet(n);}

         void packing() {myTreeMgr -> packing();}
         void setWireCost () {myNetMgr -> setWireCost();}
         void resetCdnLimit() {myNetMgr -> resetCdnLimit();}

         double getAlpha() const {return alpha;}
         size_t getAreaCost() {return myTreeMgr -> getAreaCost();}
         size_t getWireCost() {return myNetMgr -> getWireCost();}
         int getChipWidth() {return myTreeMgr -> getChipWidth();}
         int getChipHeight() {return myTreeMgr -> getChipHeight();}
         void outputBlock(fstream& fout) {myTreeMgr -> outputBlock(fout);}
         void outputTree(fstream& fout) {myTreeMgr -> outputTree(fout);}

         void setAvg(size_t newAreaCost, size_t newWireCost) {avgAreaCost = (avgAreaCost*count + newAreaCost) / (count + 1); avgWireCost = (avgWireCost*count + newWireCost) / (count + 1); ++count;}
        

    private:
        int outLineWidth;
        int outLineHeight;
        double temperature;
        double r;
        int p;
        double alpha;
        TreeMgr* myTreeMgr;
        NetMgr* myNetMgr;
        bool inOutLine;
        int count;
        int failCount;
        size_t avgAreaCost;
        size_t avgWireCost;
};