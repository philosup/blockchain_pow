#include <iostream>

#include "blockchain/block.h"
#include "blockchain/uint256.h"
#include "httplib.h"

using namespace std;

int main(int argc, char** argv){

    if(argc != 2){
        cout<< "need argument to port !!" << endl;
        return -1;
    }

    int port = strtol(argv[1], NULL, 10);

    cout<< "Start !!" << endl;

    // HTTP
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
    });


    svr.Get("/mine", [](const httplib::Request &, httplib::Response &res) {
        auto block = new Block(0, 1, uint256(0), 1);
        cout<< hash_to_string(block->hash) << endl;

        uint256 UINT256_MAX = 0;
        UINT256_MAX--;

        uint256 difficult = UINT256_MAX >>= 16;

        auto prevhash = block->hash;
        for(long index = 1; index < 11; index++){

            for(long i = 0; i< 1000'0000; i++)
            {
                long timestamp = 1;

                auto block2 = new Block(index, timestamp, prevhash, i);
                auto hash = calculateHash(block2);

                if( hash < difficult){
                    cout<< hash_to_string(block2->hash) << ", " << block2->nonce << endl;
                    prevhash = block2->hash;
                    break;
                }
            }
        }
        res.set_content("Mined!", "text/plain");
    });

    svr.listen("0.0.0.0", port);

    return 0;
}

/*
Start !!
307593ad9aa3ee5045094826ff828f3f9311f221eae6714c892a28b393e16161
000002069abe95924dd2afb969fd126f6b39deee162bf5ea50da6c1a04a3a2c7, 1126881
000005cbb73f12f6271027ffe884d9b26712e118f48a03dee70aa552d65eaa72, 233490
0000041468c1f7294ac954e921e021198ab3c29b896818f570add618deb37916, 149770
00000639c5beef17ebb60718b57725658050b6df77cd9b22d2457c4b023bcf7d, 1344099
0000065ff2b3e8c4e823f9642615d8bd2ee1f2002b4c8b8c192fab8288fdbf72, 470274
000003fa8bda3193cf0d527bba516b2d2ac477d1b1db69ca32718163da1c7b62, 1492262
000004f7ec21351242ce17c8f08cdbb84c42d060c22a2976414bc3b83b43105d, 1728749
0000003d3a3be370dabb2378b68f4328192deb924c7fed320f87432a56ac37f0, 898725
000006a1edf5df4d9c234d93f7999c9255c3cf3948d0b1a15be21becdc530ee6, 2736228
0000002d631bc220f929bb6a66aa1a1ec5fc11c5182c884f7ba3558e3dd98b70, 2087979
*/