#ifndef BLOCK_H
#define BLOCK_H

#include <string>
using namespace std;

class Block{
    public:
        Block():leftChild(0), rightChild(0), parent(0){}
        Block(string& s, int& w, int& h): name(s), width(w), height(h), leftChild(0), rightChild(0), parent(0) {}
        ~Block(){}

        void setRightChild(Block* b) {rightChild = b;}
        void setLeftChild(Block* b) {leftChild = b;}
        void setParent(Block* b) {parent = b;}
        Block* getRightChild() const {return rightChild;}
        Block* getLeftChild() const {return leftChild;}
        Block* getParent() const {return parent;}

        size_t getSize() const {return width*height;}
        int getWidth() const {return width;}
        int getHeight() const {return height;}
        void swapWH() {swap(width,height);}

        string& getName() {return name;}

        void set_x_cdn(int x) {x_cdn = x;}
        void set_y_cdn(int y) {y_cdn = y;}
        int get_x_cdn() const {return x_cdn;}
        int get_y_cdn() const {return y_cdn;}

        double getTerminal_x_cdn() const {return double(x_cdn) + double(width)/2;}
        double getTerminal_y_cdn() const {return double(y_cdn) + double(height)/2;}

        void operator = (const Block& b) {leftChild = b.leftChild; rightChild = b.rightChild; parent = b.parent;}
    private:
        string name;
        size_t width;
        size_t height;
        int x_cdn;
        int y_cdn;
        Block* leftChild;
        Block* rightChild;
        Block* parent;
};

#endif