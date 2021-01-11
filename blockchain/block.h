#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>

using namespace std;

class Block{
    long index;
    long timestamp;
    string prevhash;
    long nonce;

    string hash;

    public:
    Block(long _index, long _timestamp, string _prevhash, long _nonce){
        this->index = _index;
        this->timestamp = _timestamp;
        this->prevhash = _prevhash;
        this->nonce = _nonce;
        this->hash = calHash();
    }

    private:
    string calHash(){
        return "";//index, timestamp, hash,prevhash
    }
};
#endif // __BLOCK_H__