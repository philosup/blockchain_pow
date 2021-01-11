#include "blockchain.h"



void Blockchain::createGenesisBlock(){    
    chain.push_back(Block(0, 0, "", 0));
}

Blockchain::Blockchain() 
{
    createGenesisBlock();
}

Block Blockchain::getLastBlock() 
{
    return chain.back();
}

void Blockchain::addBlock() 
{
    auto last_block = getLastBlock();
    auto new_block = Block(chain.size(), chain.size(), last_block.hash, chain.size());
    chain.push_back(new_block);    
}

bool Blockchain::isChainValid() 
{
    for(int i=0; i<chain.size(); i++){

    }

    return true;
}

