#include "PieceTable.h"
#include <cstring>
#include <iostream>

PieceTable::PieceTable(std::string str){
    file = str;
    pieceDescriptors.push_back({true, 0, str.length()});
}

void PieceTable::stringify(std::string &ret){
    std::string out = "";
    for(auto desc : pieceDescriptors){
        if(desc.isFile){
            // printf("start: %i, length: %i\n", desc.startIndex, desc.length);
            out += file.substr(desc.startIndex, desc.length);
            // std::cout << "cake\n";
        } else{
            // std::cout << "add: " << add.length() << "\n";
            out += add.substr(desc.startIndex, desc.length);
            // std::cout << "cake\n";
        }
    }
    ret = out;
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
            if(++it1 == pieceDescriptors.end()){
                pieceDescriptors.push_back(newDesc);
            }else{
                pieceDescriptors.insert(it1, newDesc);
            }
            return;
        } else if(offset > cursorPos){
            uint startPos = it1->startIndex;
            //HACK kinda bad solution, and i dont think it'll keep working if we optimize the insert in blocks rather than by characters
            int fileBufferPos = cursorPos;
            for(auto desc : pieceDescriptors){
                if(offset > desc.startIndex && !desc.isFile){
                    fileBufferPos -= desc.length;
                }
            }
            uint newLength1 = fileBufferPos - it1->startIndex; 
            uint newLength2 = it1->startIndex + it1->length - fileBufferPos;
            // printf("startPos: %i, newLength1: %i, newLength2: %i\n", startPos, newLength1, newLength2);
            it1->length = newLength1;
            it1++;
            if(it1 == pieceDescriptors.end()){
                pieceDescriptors.push_back(newDesc);
                pieceDescriptors.push_back({FILE_BUFFER, startPos + newLength1, newLength2});
                // print();
            }else{
                pieceDescriptors.insert(it1, newDesc);
                it1++;
                pieceDescriptors.insert(it1, {FILE_BUFFER, startPos + newLength1, newLength2});
                // print();
            }
            return;
        }

    }
}

void PieceTable::remove(int cursorPos, int length){
}

void PieceTable::print(){
    printf("isFile        start        length\n");
    for(auto desc : pieceDescriptors){
        printf("%i\t\t%i\t\t%i\n", desc.isFile, desc.startIndex, desc.length);
    }
}

PieceTable::~PieceTable(){}


