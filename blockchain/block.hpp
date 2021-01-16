#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>
#include <cstring>
#include "sha256.hpp"
#include "uint256.h"
#include "../rapidjson/prettywriter.h" // for stringify JSON
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/document.h"

using namespace std;
using namespace rapidjson;

class Block;
uint256 calculateHash( Block* const block);

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

    Block(string jsonStr){
        Document d;
        d.Parse(jsonStr.c_str());

        index = d["index"].GetInt64();
        timestamp = d["timestamp"].GetInt64();
        prevhash = uint256(d["prevhash"].GetString());
        nonce = d["nonce"].GetInt64();
        hash = uint256(d["hash"].GetString());
    }

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        // This base class just write out name-value pairs, without wrapping within an object.
        writer.String("index");
        writer.Int64(index);

        writer.String("timestamp");
        writer.Int64(timestamp);

        writer.String("prevhash");
        writer.String(hash_to_string(prevhash));

        writer.String("nonce");
        writer.Int64(nonce);

        writer.String("hash");
        writer.String(hash_to_string(hash));

        writer.EndObject();
    }    

    string GetJSON(){
        StringBuffer buffer;
        // PrettyWriter<StringBuffer> writer(buffer);
        Writer<StringBuffer> writer(buffer);
        Serialize(writer);
        return buffer.GetString();
    }
};


uint256 calculateHash( Block* const block){
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