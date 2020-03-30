#include "nets.h"
#include "terminal.h"
#include <vector>
#include <unordered_map>
#include <climits>
using namespace std;

class NetMgr{
    public:
        NetMgr(){min_x = INT_MAX; min_y = INT_MAX; max_y = 0; max_x = 0;}
        ~NetMgr();

        void reserveList(size_t s) {netList.reserve(s);}
        void insertNet(Net* n) {netList.push_back(n);}
        void insertBlock(string& s, Block* b) {blockHash.insert(make_pair(s,b));}
        void insertTerminal(string& s, Terminal* t) {terminalHash.insert(make_pair(s,t));}
        void setWireCost();
        double getWireCost(){return wireCost;};
        void resetCdnLimit(){min_x = INT_MAX; min_y = INT_MAX; max_y = 0; max_x = 0;}

    private:
        vector<Net*> netList;
        unordered_map<string, Block*> blockHash;
        unordered_map<string, Terminal*> terminalHash;
        double min_x;
        double min_y;
        double max_x;
        double max_y;
        double wireCost;
};