#ifndef TREEMGR_H
#define TREEMGR_H

#include <climits>
#include <vector>
#include <iostream>
#include "block.h"
#include "contour.h"
using namespace std;

enum Neighbor{
    ROTATE = 1,
    DELETENINSERT = 2,
    SWAP = 3,
    TURN = 4
};

class TreeMgr{
    public:
        TreeMgr(size_t t): root(0), chipWidth(0), chipHeight(0) {currentCtl = new ContourLine(0, INT_MAX, 0, 0); blockList.reserve(t); }
        ~TreeMgr(){}

        size_t getAreaCost() {
            return chipHeight*chipWidth;
        }
        int getChipWidth() const {return chipWidth;}
        int getChipHeight() const {return chipHeight;}
        void insertNode(Block*);
        void resetCtl();
        void packing();
        void setCdn(Block*);
        Neighbor findNeighbor();
        void reverseNeighbor(Neighbor);
        void rotate(Block*);
        void deleteNInsert(size_t, size_t);
        void reverseDeleteNInsert();
        void turn(Block*);
        void swapNode(Block&, Block&);

        void outputBlock(fstream&);
        void outputTree(fstream&);
        void outputTreeNode(fstream&, Block*);

    private:
        int chipWidth;
        int chipHeight;
        size_t tmpBlock_1;
        size_t tmpBlock_2;
        size_t tmpBlock_3;
        Block* root;
        ContourLine* currentCtl;
        vector<Block*> blockList;
        int count;

};

#endif
