#ifndef __BLOCKCHAIN_H__
#define __BLOCKCHAIN_H__

#include <iostream>
#include <vector>
#include "block.hpp"
#include "../rapidjson/prettywriter.h" // for stringify JSON
#include "../rapidjson/stringbuffer.h"

using namespace std;
class Blockchain{
    vector<Block> chain;

    void createGenesisBlock();

    public:
    Blockchain();
    Block getLastBlock();
    void addBlock(Block& block);
    bool isChainValid();
    string GetJSON();

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("chains_count");
        writer.Int64(chain.size());

        writer.String("chains");
        writer.StartArray();
        for(auto block : chain)
            block.Serialize(writer);
        writer.EndArray();

        writer.EndObject();
    }       

};

void Blockchain::createGenesisBlock(){    
    chain.push_back(Block(0, 0, uint256(0), 0));
}

Blockchain::Blockchain() 
{
    createGenesisBlock();
}

Block Blockchain::getLastBlock() 
{
    return chain.back();
}

void Blockchain::addBlock(Block& block) 
{
    auto last_block = getLastBlock();
    // auto new_block = Block(chain.size(), chain.size(), last_block.hash, chain.size());
    if(last_block.hash != block.prevhash){
        cout<<"invalid block"<<endl;
        return;
    }
    chain.push_back(block);    
}

bool Blockchain::isChainValid() 
{
    for(int i=0; i<chain.size(); i++){

    }

    return true;
}

string Blockchain::GetJSON() 
{
    StringBuffer buffer;
    // PrettyWriter<StringBuffer> writer(buffer);
    Writer<StringBuffer> writer(buffer);
    Serialize(writer);
    return buffer.GetString();
}


#endif // __BLOCKCHAIN_H__