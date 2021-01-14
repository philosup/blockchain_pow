#ifndef __BLOCKCHAIN_H__
#define __BLOCKCHAIN_H__

#include <vector>
#include "block.h"

class Blockchain{
    vector<Block> chain;

    void createGenesisBlock();

    public:
    Blockchain();
    Block getLastBlock();
    void addBlock();
    bool isChainValid();

};
#endif // __BLOCKCHAIN_H__