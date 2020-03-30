#ifndef CONTOUR_H
#define CONTOUR_H

using namespace std;

class ContourLine{
    public:
        ContourLine(int b1, int b2, int h, ContourLine* n = 0, ContourLine* p = 0): l_bound(b1), h_bound(b2), height(h), nextSeg(n), prevSeg(p){}
        ~ContourLine(){}

        int getLBound() const {return l_bound;}
        int getHBound() const {return h_bound;}
        int getHeight() const {return height;}
        void setLBound(int b) {l_bound = b;}
        void setHBound(int b) {h_bound = b;}
        void setHeight(int h) {height = h;}

        ContourLine* getNextSeg() {return nextSeg;}
        ContourLine* getPrevSeg() {return prevSeg;}
        void setNextSeg(ContourLine* n) {nextSeg = n;}
        void setPrevSeg(ContourLine* n) {prevSeg = n;}
    private:
        int l_bound;
        int h_bound;
        int height;

        ContourLine* nextSeg;
        ContourLine* prevSeg;
};

#endif