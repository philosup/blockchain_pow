#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>
#include <cstring>
#include "sha256.hpp"
#include "uint256.h"

using namespace std;

class Block;
uint256 calculateHash(const Block* block);
string hash_to_string(const uint256& hash){
    char buf[2*SHA256::DIGEST_SIZE+1];
    buf[2*SHA256::DIGEST_SIZE] = 0;
    for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
        sprintf(buf+i*2, "%02x", hash.begin()[SHA256::DIGEST_SIZE - i - 1]);
    return std::string(buf);    
}

class Block{
    public:
    long index;
    long timestamp;
    uint256 prevhash;
    long nonce;

    uint256 hash;

    Block(long _index, long _timestamp, uint256 _prevhash, long _nonce){
        this->index = _index;
        this->timestamp = _timestamp;
        this->prevhash = _prevhash;
        this->nonce = _nonce;
        this->hash = calculateHash(this);
    }
};


uint256 calculateHash(const Block* block){
    uint256 digest;

    SHA256 ctx;
    ctx.init();

    string prevhash = hash_to_string(block->prevhash);
    ctx.update((unsigned char*)&block->index, sizeof(block->index));
    ctx.update((unsigned char*)&block->timestamp, sizeof(block->timestamp));
    ctx.update((unsigned char*)prevhash.c_str(), prevhash.size());
    ctx.update((unsigned char*)&block->nonce, sizeof(block->nonce));

    ctx.final(digest.begin());

    return digest;
}
#endif // __BLOCK_H__