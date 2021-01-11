#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>
#include <cstring>
#include "sha256.hpp"
#include "uint256.h"

using namespace std;

class Block{
    public:
    long index;
    long timestamp;
    string prevhash;
    long nonce;

    string hash;

    Block(long _index, long _timestamp, string _prevhash, long _nonce){
        this->index = _index;
        this->timestamp = _timestamp;
        this->prevhash = _prevhash;
        this->nonce = _nonce;
        this->hash = calculateHash();
    }

    private:
    string calculateHash(){


        unsigned char digest[SHA256::DIGEST_SIZE];
        memset(digest,0,SHA256::DIGEST_SIZE);

        SHA256 ctx;
        ctx.init();

        ctx.update((unsigned char*)&index, sizeof(index));
        ctx.update((unsigned char*)&timestamp, sizeof(timestamp));
        ctx.update((unsigned char*)prevhash.c_str(), prevhash.size());
        ctx.update((unsigned char*)&nonce, sizeof(nonce));

        ctx.final(digest);

        char buf[2*SHA256::DIGEST_SIZE+1];
        buf[2*SHA256::DIGEST_SIZE] = 0;
        for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
            sprintf(buf+i*2, "%02x", digest[SHA256::DIGEST_SIZE - i - 1]);
        return std::string(buf);

        // return prevhash + to_string(index) + to_string(timestamp) + to_string(nonce);
    } 
};
#endif // __BLOCK_H__