#include "PieceTable.h"
#include <numbers>

PieceTable::PieceTable(std::string str){
    file = str;
    pieceDescriptors.push_back({true, 0, str.length()});
}

void PieceTable::stringify(std::string &ret){
}

void PieceTable::insert(std::string str, int cursorPos){
    //calculate which descriptor and where in said descriptor needs to be split
    int offset = 0, listPos = 0;

    uint start = add.length();
    add += str;
    PieceDescriptor newDesc{ADD_BUFFER, start, str.length()};

    if(cursorPos == 0){
        pieceDescriptors.push_front(newDesc);
        return;
    }
    std::list<PieceDescriptor>::iterator it1;
    for(it1 = pieceDescriptors.begin(); it1 != pieceDescriptors.end(); it1++){
        offset += it1->length;
        if(offset == cursorPos){
            // its at an edge and should be simpler
            pieceDescriptors.insert(++it1, newDesc);
            return;
        } else if(offset > cursorPos){
            uint startPos = it1->startIndex;
            size_t newLength1 = cursorPos - it1->startIndex;
            size_t newLength2 = it1->startIndex + it1->length - cursorPos;
            it1->length = newLength1;
            pieceDescriptors.insert(++it1, newDesc);
            pieceDescriptors.insert(++it1, {FILE_BUFFER, startPos + (uint)newLength1, newLength2});
            return;
        }

    }
}

void PieceTable::remove(int cursorPos, int length){
}

PieceTable::~PieceTable(){}


