#include "PieceTable.h"

PieceTable::PieceTable(std::string str){
    file = str;
    pieceDescriptors.push_back({true, 0, str.length()});
}

void PieceTable::stringify(std::string &ret){
}

void PieceTable::insert(std::string str, int cursorPos){
    //calculate which descriptor and where in said descriptor needs to be split
    int offset = 0, vectorPos = 0;
    add += str;
    if(cursorPos == 0){
        pieceDescriptors.push_front({});
        return;
    }
    std::list<PieceDescriptor>::iterator it1;
    for(it1 = pieceDescriptors.begin(); it1 != pieceDescriptors.end(); it1++){
        offset += it1->length;
        if(offset == cursorPos){
            // its at and edge and should be simpler
            return;
        } else if(offset > cursorPos){
            return;
        }

    }
}

void PieceTable::remove(int cursorPos, int length){
}

PieceTable::~PieceTable(){}


