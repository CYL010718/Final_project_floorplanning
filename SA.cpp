#include "SA.h"
#include <random>
#include <cmath>
using namespace std;

void
SAMgr::SA(){
    double thisCost;
    double nextCost;
    Neighbor _neighbor;
    random_device rd;
    default_random_engine gen = default_random_engine(rd());
    int thisOutLineDistance;
    int nextOutLineDistance;

    while(temperature > 10){
        for(int i = 0; i < p; ++i){

            if(count < 100) setAvg( myTreeMgr -> getAreaCost(), myNetMgr -> getWireCost());
            thisCost = (100 * alpha) * (double)myTreeMgr -> getAreaCost() / (double)avgAreaCost + (100 * (1 - alpha)) * (double)myNetMgr -> getWireCost() / (double)avgWireCost;
            //cout << "thisCost "<< thisCost << endl;
            thisOutLineDistance = ((myTreeMgr -> getChipWidth() - outLineWidth) < 0 ? 0 :(myTreeMgr -> getChipWidth() - outLineWidth)) + ((myTreeMgr -> getChipHeight() - outLineHeight) < 0 ? 0 : (myTreeMgr -> getChipHeight() - outLineHeight));
            if(thisOutLineDistance <= 0) inOutLine = true;
           // cout << "tOLD " << thisOutLineDistance << " " << myTreeMgr -> getChipWidth() << " " << myTreeMgr -> getChipHeight() << " " << outLineWidth << "  " << outLineHeight << endl;

            _neighbor = myTreeMgr -> findNeighbor();
           // //cout << "heoo" << endl;
            myTreeMgr -> resetCtl();
            ////cout << i << endl;
            //cout << "forward" << endl;
            myTreeMgr -> packing();
            nextOutLineDistance = ((myTreeMgr -> getChipWidth() - outLineWidth) < 0 ? 0 :(myTreeMgr -> getChipWidth() - outLineWidth)) + ((myTreeMgr -> getChipHeight() - outLineHeight) < 0 ? 0 : (myTreeMgr -> getChipHeight() - outLineHeight));
            myNetMgr -> setWireCost();

            myNetMgr -> resetCdnLimit();
            
            if(inOutLine == true &&  (myTreeMgr -> getChipHeight() > outLineHeight || myTreeMgr -> getChipWidth() > outLineWidth)){
                myTreeMgr -> reverseNeighbor(_neighbor);
                //cout << "reverse" << endl;
                myTreeMgr -> resetCtl();
                myTreeMgr -> packing();
                myNetMgr -> setWireCost();
                myNetMgr -> resetCdnLimit();
                continue;
            }
            else if(inOutLine == false){
                
                if(nextOutLineDistance > thisOutLineDistance){
                    ++failCount;
                    bernoulli_distribution dis(exp(-(50 - failCount)));
                    if(!dis(gen)){
                        myTreeMgr -> reverseNeighbor(_neighbor);
                        myTreeMgr -> resetCtl();
                        myTreeMgr -> packing();
                        myNetMgr -> setWireCost();
                        myNetMgr -> resetCdnLimit();
                    }
                    else failCount = 0;         
                }
                else failCount = 0;
               continue;
            }
            //cout << "success" << endl;
            if(count < 100) setAvg( myTreeMgr -> getAreaCost(), myNetMgr -> getWireCost());
            nextCost = (100 * alpha) * (double)myTreeMgr -> getAreaCost() / (double)avgAreaCost + (100 * (1 - alpha)) * (double)myNetMgr -> getWireCost() / (double)avgWireCost;
            
            if(nextCost > thisCost){
                bernoulli_distribution dis(exp(0 - double(nextCost-thisCost)/temperature));
                if(!dis(gen)){
                    myTreeMgr -> reverseNeighbor(_neighbor);
                    myTreeMgr -> resetCtl();
                    myTreeMgr -> packing();
                    myNetMgr -> setWireCost();
                    myNetMgr -> resetCdnLimit();
                }
            }
        }
        if(inOutLine) {temperature *= r;}
    
    }
}