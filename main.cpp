#include <iostream>

#include "blockchain/block.h"
#include "blockchain/uint256.h"

using namespace std;

int main(){
    cout<< "Start !!" << endl;

    auto block = new Block(0, 1, "", 1);
    cout<< block->hash << endl;


    uint256 difficult = 1;
    difficult <<= 235;

    string prevhash = block->hash;
    for(long index = 1; index < 11; index++){

        for(long i = 0; i< 1000'0000; i++)
        {
            long timestamp = 1;
            uint256 digest;

            SHA256 ctx;
            ctx.init();

            ctx.update((unsigned char*)&index, sizeof(index));
            ctx.update((unsigned char*)&timestamp, sizeof(timestamp));
            ctx.update((unsigned char*)prevhash.c_str(), prevhash.size());
            ctx.update((unsigned char*)&i, sizeof(i));

            auto pbegin = digest.begin();
            ctx.final( pbegin );

            if( digest < difficult){
                auto block2 = new Block(index, timestamp, prevhash, i);
                cout<< block2->hash << ", " << block2->nonce << endl;
                prevhash = block2->hash;
                break;
            }
        }
    }
    return 0;
}

/*
Start !!
3227c755330530eb57a676513886cd929f76dac5f99249dbe38d7d6b86acb1cc
00000542e58928571e6d53ea85f53a5da04138694eaad2575f9c7eb338948552, 3816149
000006cbd73eb3a511a061a491c6509663cc8e8c9f453148a0bf147af536cc03, 1222756
0000001f87d1384decd1f1781d62624720c7ade745a93922445986fa5f423695, 2677122
000003ab447ad864d3554b1374c467d2eef99c9af8a1801d730e3674b3de711e, 406803
0000003a29918072f1f24aaf8da1d2a9fcca48112d4b071600ed7ec60643d85c, 309030
0000040889f60f8b8a6173ad52324b5b6eb5f0c4d4d61ae0474df1e9f47e9ad9, 878929
0000079a30847f0715a86e8696f0f3bce92dcb4855c34eb3a487fdc3494893a2, 3756359
00000355e16476566e76f49013adeaa6ceb6d553ec499a44ccefd19795fc38fa, 4212142
0000009d5e4fa6d2dcaf906e2704f9ee0b60fb0714b9a77cd7716a9bf0a794c1, 3658307
000006afaea9c4e966fcf7c16985b82bfe202c6242c6ebe0f3a0ecd7466bd12b, 1265988
*/