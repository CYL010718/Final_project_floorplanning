#include "netMgr.h"
using namespace std;

void
NetMgr::setWireCost(){
    Net* thisNet;
    string terminalName;
    unordered_map<string, Block*>::iterator blockIt;
    unordered_map<string, Terminal*>::iterator terminalIt;
    Block* thisBlock;
    Terminal* thisTerminal;
    double x_cdn;
    double y_cdn;
    wireCost = 0;

    for(size_t i = 0; i < netList.size(); ++i){
        thisNet = netList[i];
        for(size_t j = 0; j < thisNet -> getTerminalSize(); ++j){
            terminalName = thisNet -> getTerminal(j);
            blockIt = blockHash.find(terminalName);
            if(blockIt != blockHash.end()){
                thisBlock = blockIt -> second;
                x_cdn = thisBlock -> getTerminal_x_cdn();
                y_cdn = thisBlock -> getTerminal_y_cdn();
            }
            else{
                terminalIt = terminalHash.find(terminalName);
                thisTerminal = terminalIt -> second;
                x_cdn = thisTerminal -> getTerminal_x_cdn();
                y_cdn = thisTerminal -> getTerminal_y_cdn();
            }
            if(x_cdn < min_x) min_x = x_cdn;
            if(x_cdn > max_x) max_x = x_cdn;
            if(y_cdn < min_y) min_y = y_cdn;
            if(y_cdn > max_y) max_y = y_cdn;
        }
        wireCost += (max_x - min_x) + (max_y - min_y);
        resetCdnLimit();
    }
}