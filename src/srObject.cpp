#include "srObject.hpp"

srObject::srObject() {
    queCnt = 0;
    queCntBig = 0;
    queCntBigMax = rand() % 150 + 100;

    handle = 0;
    type = 0;
    owner = 0;
    status = 0;
    name = "";
    x = 0;
    y = 0;
}

srObject::~srObject() {
    //dtor
}

/// add cargo
void srObject::cargoAdd(string pType, int pAmount) {
    for( cargoIterator=cargo.begin(); cargoIterator!=cargo.end(); ++cargoIterator) {
        if( (*cargoIterator).type == pType ) {
            (*cargoIterator).type += pAmount;
        }
    }
}

/// get cargo
int srObject::cargoGet(string pType) {
    for( cargoIterator=cargo.begin(); cargoIterator!=cargo.end(); ++cargoIterator) {
        if( (*cargoIterator).type == pType ) {
            return (*cargoIterator).amount;
        }
    }
    return 0;
}

