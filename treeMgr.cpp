#include "treeMgr.h"
#include <climits>
#include <random>
#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;

void
TreeMgr::insertNode(Block* newBlock){
    blockList.push_back(newBlock);

    Block* thisBlock = root;
    if(root == 0) {
        root = newBlock;
        return;
    }
    Block* nextBlock;
    while(true){
        //// << "ji" << endl;
        if(newBlock -> getSize() < thisBlock -> getSize()){
            nextBlock = thisBlock -> getLeftChild();
            if(nextBlock == 0){
                thisBlock -> setLeftChild(newBlock);
                newBlock -> setParent(thisBlock);
                break;
            }
        }
        else{
            nextBlock = thisBlock -> getRightChild();
            if(nextBlock == 0){
                thisBlock -> setRightChild(newBlock);
                newBlock -> setParent(thisBlock);
                break;
            }
        }
        thisBlock = nextBlock;
    }
}
void
TreeMgr::resetCtl(){
    ContourLine* thisCtl = currentCtl -> getPrevSeg();
    ContourLine* tmpCtl;

    while(thisCtl != 0){
        tmpCtl = thisCtl -> getPrevSeg();
        delete thisCtl;
        thisCtl = tmpCtl;
        //// << "p" << endl;
    }
     // << "n" << endl;
    while(currentCtl != 0){
        tmpCtl = currentCtl -> getNextSeg();
        delete currentCtl;
        currentCtl = tmpCtl;
    }
    // << "finish" << endl;
    currentCtl = new ContourLine(0, INT_MAX, 0, 0, 0);
    chipHeight = 0;
    chipWidth = 0;
}

void 
TreeMgr::packing(){
    root -> set_x_cdn(0);
    root -> set_y_cdn(0);
    count = 0;
    setCdn(root);
}

void
TreeMgr::setCdn(Block* thisBlock){
    //cout << count << " " << thisBlock << " " << thisBlock -> getLeftChild() << " " << thisBlock -> getRightChild()<< endl ;
    ++count;
    while(thisBlock -> get_x_cdn() > currentCtl -> getHBound()){
        currentCtl = currentCtl -> getNextSeg();
    }

   
    //// << thisBlock -> get_x_cdn() << " " <<  currentCtl -> getLBound() << endl;
    while(thisBlock -> get_x_cdn() < currentCtl -> getLBound()){
        currentCtl = currentCtl -> getPrevSeg();
    }
    int y = currentCtl -> getHeight();
    
    ContourLine* newCtl;
    newCtl = new ContourLine(thisBlock -> get_x_cdn(), thisBlock -> get_x_cdn() + thisBlock -> getWidth() , y, 0, currentCtl -> getPrevSeg());
    if(currentCtl -> getPrevSeg() != 0) currentCtl -> getPrevSeg() -> setNextSeg(newCtl);

    ContourLine* prev = currentCtl -> getPrevSeg();
    ContourLine* cutCtl = 0;
    if(currentCtl -> getLBound() < thisBlock -> get_x_cdn()){
        cutCtl = new ContourLine(currentCtl -> getLBound(), thisBlock -> get_x_cdn(), currentCtl -> getHeight(), newCtl, currentCtl -> getPrevSeg());   
       // currentCtl -> setLBound() 
        newCtl -> setPrevSeg(cutCtl);
        if(currentCtl -> getPrevSeg() != 0) currentCtl -> getPrevSeg() -> setNextSeg(cutCtl);
    }

    ContourLine* tmpCtl;
    while(currentCtl -> getHBound() < thisBlock -> get_x_cdn() + thisBlock -> getWidth()){
        tmpCtl = currentCtl;
        currentCtl = currentCtl -> getNextSeg();
        currentCtl -> setPrevSeg(newCtl);
        // << tmpCtl << endl;
        delete tmpCtl;
        if(currentCtl -> getHeight() > y) y = currentCtl -> getHeight();
    }
    thisBlock -> set_y_cdn(y);
    newCtl -> setHeight(y + thisBlock -> getHeight());
    
    if(currentCtl -> getHBound() == thisBlock -> get_x_cdn() + thisBlock -> getWidth()){
        tmpCtl = currentCtl;
        currentCtl = currentCtl -> getNextSeg();
        ////  << tmpCtl << endl;
        delete tmpCtl;
    }
    else currentCtl -> setLBound(thisBlock -> get_x_cdn() + thisBlock -> getWidth());
    newCtl -> setNextSeg(currentCtl);
    currentCtl -> setPrevSeg(newCtl);
   // cout << "6" << endl;
    if(thisBlock -> getLeftChild() != 0 ){
        //cout << thisBlock << endl;
        thisBlock -> getLeftChild() -> set_x_cdn(thisBlock -> get_x_cdn() + thisBlock -> getWidth());
        //cout << "61" << endl;
       // cout << thisBlock << endl;
        setCdn(thisBlock -> getLeftChild());
       
    }
    //cout << "7" << endl;
    if(thisBlock -> getRightChild() != 0){
       //cout << "rightChild" << endl;
        thisBlock -> getRightChild() -> set_x_cdn(thisBlock -> get_x_cdn());
        setCdn(thisBlock -> getRightChild());
      
    }

    // Update chipWidth and chipHeight
    if(thisBlock -> get_x_cdn() + thisBlock -> getWidth() > chipWidth) chipWidth = thisBlock -> get_x_cdn() + thisBlock -> getWidth();
    if(thisBlock -> get_y_cdn() + thisBlock -> getHeight() > chipHeight) chipHeight = thisBlock -> get_y_cdn() + thisBlock -> getHeight();
}

Neighbor
TreeMgr::findNeighbor(){
    random_device rd;
    default_random_engine gen  = default_random_engine(rd());
    uniform_int_distribution <int> dis(0,blockList.size() - 1);
    // << blockList.size() << endl;
    int randNum = dis(gen);
    int targetNum = dis(gen);
   // randNum = 1;
    if(randNum % 4 == 0 && blockList[targetNum] != root){
        rotate(blockList[targetNum]);
        //cout << "hi" << blockList[targetNum] <<  endl;
        return ROTATE;
    }
    else if(randNum % 4 == 1 && blockList.size() > 1){
        int insertNum;
        do{
            insertNum = dis(gen);
        } while(insertNum == targetNum);
        deleteNInsert(targetNum, insertNum);
        return DELETENINSERT;
    }
    else if(randNum % 4 == 2){
       //cout << "swap" << endl;
        int swapNum;
        do{
            swapNum = dis(gen);
        } while(swapNum == targetNum);
        swapNode(*blockList[targetNum], *blockList[swapNum]);
        return SWAP;
    }
    else{
        turn(blockList[targetNum]);
        return TURN;
    }
}

void
TreeMgr::reverseNeighbor(Neighbor n){
    if(n == ROTATE){
        rotate((Block*)tmpBlock_1);
    }
    else if(n ==  DELETENINSERT){
        reverseDeleteNInsert();
    }
    else if(n == SWAP){
        swapNode(*(Block*)tmpBlock_2, *(Block*)tmpBlock_1);
    }
    else{
        turn((Block*)tmpBlock_1);
    }
}

void
TreeMgr::rotate(Block* thisBlock){
    Block* parentBlock = thisBlock -> getParent();
    Block* grandParent = parentBlock -> getParent();
    tmpBlock_1 = (size_t)parentBlock;
    if(thisBlock == parentBlock -> getLeftChild()){
        if(thisBlock -> getRightChild() != 0) thisBlock -> getRightChild() -> setParent(parentBlock);
        parentBlock -> setLeftChild(thisBlock -> getRightChild());
        if(grandParent != 0){
            if(parentBlock ==  grandParent -> getLeftChild())
                grandParent -> setLeftChild(thisBlock);
            else
                grandParent -> setRightChild(thisBlock);   
        }
        thisBlock -> setParent(grandParent);
        parentBlock -> setParent(thisBlock);
        thisBlock -> setRightChild(parentBlock);
    }

    else{
        if(thisBlock -> getLeftChild() != 0) thisBlock -> getLeftChild() -> setParent(parentBlock);
        parentBlock -> setRightChild(thisBlock -> getLeftChild());
         if(grandParent != 0){
            if(parentBlock ==  grandParent -> getLeftChild())
                grandParent -> setLeftChild(thisBlock);
            else
                grandParent -> setRightChild(thisBlock);   
        }
        thisBlock -> setParent(grandParent);
        parentBlock -> setParent(thisBlock);
        thisBlock -> setLeftChild(parentBlock);
    }

    if(parentBlock == root) root = thisBlock;
}

void
TreeMgr::deleteNInsert(size_t _delete, size_t insert){
    Block* deleteBlock = blockList[_delete];
    Block* insertBlock = blockList[insert];
    Block* replaceBlock = deleteBlock;
    bool check = false;
    
    tmpBlock_1 = (size_t)deleteBlock;
    while(!(replaceBlock -> getLeftChild() == 0 && replaceBlock -> getRightChild() == 0)){
       //cout << "deleteLoop" << endl;
        while(replaceBlock -> getLeftChild())
            replaceBlock = replaceBlock -> getLeftChild();
        while(replaceBlock -> getRightChild())
            replaceBlock = replaceBlock -> getRightChild();
    } 
    //cout << "lalala " << deleteBlock << " " << insertBlock << " " << replaceBlock << endl;
    if(replaceBlock == deleteBlock -> getLeftChild()){
        check = true;
    }


    if(replaceBlock -> getParent() != 0) {
        if(replaceBlock == replaceBlock -> getParent() -> getLeftChild()){
            if(replaceBlock -> getParent() -> getRightChild() != 0){
                tmpBlock_3 = (size_t)replaceBlock -> getParent();
            }
            else tmpBlock_3 = 0;
            replaceBlock -> getParent() -> setLeftChild(0);
            tmpBlock_2 = (size_t)replaceBlock;
        }         
        else{
            if(replaceBlock -> getParent() -> getLeftChild() != 0){
                tmpBlock_3 = (size_t)replaceBlock -> getParent() + 1;
            }
            else tmpBlock_3 = 0;
            replaceBlock -> getParent() -> setRightChild(0);
            tmpBlock_2 = (size_t)replaceBlock + 1;
           // cout << "set " << tmpBlock_2  <<endl;
        }       
    }

    if(replaceBlock != deleteBlock){
        *replaceBlock = *deleteBlock;
        if(deleteBlock -> getParent()){
            if(deleteBlock == deleteBlock -> getParent() -> getLeftChild())
                deleteBlock -> getParent() -> setLeftChild(replaceBlock);
            else
                deleteBlock -> getParent() -> setRightChild(replaceBlock);
        }
        if(deleteBlock -> getLeftChild())
            deleteBlock -> getLeftChild() -> setParent(replaceBlock);
        if(deleteBlock -> getRightChild())
            deleteBlock -> getRightChild() -> setParent(replaceBlock);       
        if(deleteBlock == root) root = replaceBlock;
    }
    else{
        //cout << "same" << endl;
        if(tmpBlock_2 % 2 == 0)
            tmpBlock_2 = (size_t)replaceBlock -> getParent() + 2;
        else
            tmpBlock_2 = (size_t)replaceBlock -> getParent() + 3; 
    }

    if(check == true) tmpBlock_2 += 4;
   

    deleteBlock -> setParent(insertBlock -> getParent());
    if(insertBlock == root) {
        root = deleteBlock;
        deleteBlock -> setLeftChild(insertBlock);
        deleteBlock -> setRightChild(0);
        insertBlock -> setParent(deleteBlock);
    }
    else if(insertBlock == insertBlock -> getParent() -> getLeftChild()){
        deleteBlock -> setLeftChild(insertBlock);
        deleteBlock -> setRightChild(0);
        insertBlock -> getParent() -> setLeftChild(deleteBlock);
        insertBlock -> setParent(deleteBlock);
    }
    else{
        deleteBlock -> setRightChild(insertBlock);
        deleteBlock -> setLeftChild(0);
        insertBlock -> getParent() -> setRightChild(deleteBlock);
        insertBlock -> setParent(deleteBlock);
    }

}

void
TreeMgr::reverseDeleteNInsert(){
    Block* deleteBlock = (Block*) (tmpBlock_1);
    Block* replaceBlock = (Block*) (tmpBlock_2 & ~0x7);

    

    Block* insertBlock = deleteBlock -> getLeftChild() == 0 ? deleteBlock -> getRightChild() : deleteBlock -> getLeftChild();
    //cout << "rev "<< deleteBlock  << " " << insertBlock << "  " << replaceBlock << endl;
    if(deleteBlock == root){
        root = insertBlock;
        insertBlock -> setParent(0);
    }
    else{
        insertBlock -> setParent(deleteBlock -> getParent());
        if(insertBlock == deleteBlock -> getLeftChild())
            deleteBlock -> getParent() -> setLeftChild(insertBlock);
        else
            deleteBlock -> getParent() -> setRightChild(insertBlock);    
    }


    if(tmpBlock_2 % 4 > 1){
        if(tmpBlock_2 % 2 == 0){
            replaceBlock -> setLeftChild(deleteBlock);
            deleteBlock -> setLeftChild(0);
            deleteBlock -> setRightChild(0);
            deleteBlock -> setParent(replaceBlock);
        }      
        else{
            replaceBlock -> setRightChild(deleteBlock);
            deleteBlock -> setLeftChild(0);
            deleteBlock -> setRightChild(0);
            deleteBlock -> setParent(replaceBlock);
        }
        tmpBlock_3 = 0;
        return;
    }
    //cout  << replaceBlock << endl;
    *deleteBlock = *replaceBlock;
    

    if(replaceBlock -> getParent()){
        if(replaceBlock == replaceBlock -> getParent() -> getLeftChild())
            replaceBlock -> getParent() -> setLeftChild(deleteBlock);
        else
            replaceBlock -> getParent() -> setRightChild(deleteBlock);
    }
    if(replaceBlock -> getLeftChild())
        replaceBlock -> getLeftChild() -> setParent(deleteBlock);
    if(replaceBlock -> getRightChild())
        replaceBlock -> getRightChild() -> setParent(deleteBlock);       
    if(replaceBlock == root) root = deleteBlock;
    
    if(tmpBlock_2 % 8 == 4){
        //cout << "hihi" << endl;
        replaceBlock -> setLeftChild(0);
        replaceBlock -> setRightChild(0);
        replaceBlock -> setParent(deleteBlock);
        deleteBlock -> setLeftChild(replaceBlock);
        tmpBlock_3 = 0;
        return;
    }
    Block* leafBlock = replaceBlock;

    if(tmpBlock_3){
        leafBlock = (Block*)(tmpBlock_3 & ~0x1);
        replaceBlock -> setLeftChild(0);
        replaceBlock -> setRightChild(0);
        replaceBlock -> setParent(leafBlock);
        if(tmpBlock_3 % 2 == 0){
            leafBlock -> setLeftChild(replaceBlock);
        }
        else{
            leafBlock -> setRightChild(replaceBlock);
        }
        tmpBlock_3 = 0;
        return;
    }
   
    
    
    while(!(leafBlock -> getLeftChild() == 0 && leafBlock -> getRightChild() == 0)){
         //cout << "loop" << endl;
        while(leafBlock -> getLeftChild()) 
            leafBlock = leafBlock -> getLeftChild();
        while(leafBlock -> getRightChild())
            leafBlock = leafBlock -> getRightChild();
       
    } 
    

    if(leafBlock == replaceBlock){
        if(tmpBlock_2 % 2 == 0)
            deleteBlock -> setLeftChild(replaceBlock);
        else
            deleteBlock -> setRightChild(replaceBlock);
        replaceBlock -> setParent(deleteBlock);
        return;
           
    }
    //cout << "leaf " << leafBlock << endl; 
    replaceBlock -> setLeftChild(0);
    replaceBlock -> setRightChild(0);
    replaceBlock -> setParent(leafBlock);
    
    //cout  << "rev " << tmpBlock_2 << endl;
    if(tmpBlock_2 % 2 == 0)
        leafBlock -> setLeftChild(replaceBlock);
    else{
        leafBlock -> setRightChild(replaceBlock);
        //cout << "jjjj" << endl;
    }
        
    
}

void
TreeMgr::swapNode(Block& block_1, Block& block_2){
   
    Block tmpBlock;
    if(block_1.getParent() == block_2.getParent()){
        if(&block_1 == block_1.getParent() -> getRightChild()){
            swapNode(block_2,block_1);
            return;
        }
    }
    //cout << "swap: " << &block_1 << " " << &block_2 << endl;
    tmpBlock = block_1;
    block_1 = block_2;
    if(block_1.getLeftChild() != 0)  {
        if(block_1.getLeftChild() == &block_1){
             tmpBlock.setParent(&block_1);
             block_1.setLeftChild(&block_2);
        }
        else
            block_1.getLeftChild() -> setParent(&block_1);
            
    }
    if(block_1.getRightChild() != 0) {
        if(block_1.getRightChild() == &block_1){
            tmpBlock.setParent(&block_1);
            block_1.setRightChild(&block_2);
        }
        else
            block_1.getRightChild() -> setParent(&block_1);
    }
    if(block_1.getParent() != 0) {
        if(block_1.getParent() == &block_1){
            block_1.setParent(&block_2);
            if(tmpBlock.getLeftChild() == &block_2)
                tmpBlock.setLeftChild(&block_1);
            else
                tmpBlock.setRightChild(&block_1); 
        }
        else{
             if(&block_2 == block_1.getParent() -> getLeftChild())
                block_1.getParent() -> setLeftChild(&block_1);
            else
                block_1.getParent() -> setRightChild(&block_1); 
        }
       
    }
    block_2 = tmpBlock;
    if(block_2.getLeftChild() != 0 )  block_2.getLeftChild() -> setParent(&block_2);
    if(block_2.getRightChild() != 0)  block_2.getRightChild() -> setParent(&block_2);
    if(block_2.getParent() != 0 && block_2.getParent() != &block_1) {
        if(&block_1 == block_2.getParent() -> getLeftChild())
            block_2.getParent() -> setLeftChild(&block_2);
        else
            block_2.getParent() -> setRightChild(&block_2); 
    }

    if(&block_1 == root) root = &block_2;
    else if(&block_2 == root) root = &block_1; 

    tmpBlock_1 = (size_t)&block_1;
    tmpBlock_2 = (size_t)&block_2;
}

void
TreeMgr::turn(Block* thisBlock){
    thisBlock -> swapWH();
    tmpBlock_1 = (size_t) thisBlock;
    return ;
}

void
TreeMgr::outputBlock(fstream& fout){
    Block* thisBlock;
    for(size_t i = 0; i < blockList.size() ; ++i){
        thisBlock = blockList[i];
        fout << left << setw(6) << thisBlock -> getName() <<  setw(5) << thisBlock -> get_x_cdn() 
            << setw(5) << thisBlock -> get_y_cdn() << setw(5) << thisBlock -> get_x_cdn() + thisBlock -> getWidth()
            << setw(5) << thisBlock -> get_y_cdn() + thisBlock -> getHeight() << endl;
    }
}

void
TreeMgr::outputTree(fstream& fout){
    outputTreeNode(fout, root);
}

void
TreeMgr::outputTreeNode(fstream& fout, Block* thisBlock){
    fout << thisBlock -> get_x_cdn() + thisBlock -> getWidth()/2 << " " << thisBlock -> get_y_cdn() + thisBlock -> getHeight()/2 << " ";
    
    Block* leftChild = thisBlock -> getLeftChild();
    if(leftChild){
        fout << leftChild -> get_x_cdn() + leftChild -> getWidth()/2 << " " << leftChild -> get_y_cdn() + leftChild -> getHeight()/2 << " ";
    }
    else fout << 0 << " " << 0 << " ";

    Block* rightChild = thisBlock -> getRightChild();
    if(rightChild){
        fout << rightChild -> get_x_cdn() + rightChild -> getWidth()/2 << " " << rightChild -> get_y_cdn() + rightChild -> getHeight()/2 << " ";
    }
    else fout << 0 << " " << 0 << " ";

    fout << endl;

    if(leftChild) outputTreeNode(fout,leftChild);
    if(rightChild) outputTreeNode(fout,rightChild);
}