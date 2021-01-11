#include <iostream>

#include "blockchain/block.h"

using namespace std;

int main(){
    cout<< "Start !!" << endl;

    auto block = new Block(0, 1, "", 1);
    cout<< block << endl;


    return 0;
}