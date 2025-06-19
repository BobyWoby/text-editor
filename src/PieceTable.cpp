#include "PieceTable.h"
#include "SDL3/SDL_loadso.h"
#include <cassert>
#include <iostream>
#include <random>

PieceTable::PieceTable(std::string str){
    file = str;
    pieceDescriptors.push_back({true, 0, str.length()});
}

void PieceTable::stringify(std::string &ret){
    std::string out = "";
    for(auto desc : pieceDescriptors){
        if(desc.isFile){
            if(desc.startIndex >= file.length()){
                print();
            }
            assert((desc.startIndex < file.length()));
            out += file.substr(desc.startIndex, desc.length);
        } else{
            if(desc.startIndex >= add.length()){
                print();
            }
            assert((desc.startIndex < add.length()));
            out += add.substr(desc.startIndex, desc.length);
        }
        out += "|";
    }
    ret = out;
}

//TODO: Implement this function
void PieceTable::insert(std::string str, int cursorPos){
}
void PieceTable::insertAtEdge(std::string str, std::list<PieceDescriptor>::iterator it){
    pieceDescriptors.insert(it, {false, (uint)add.length(), str.length()});
    add += str;
}

//TODO: Implement this function
void PieceTable::remove(int cursorPos, int length){
}

/**
 * Finds the position of the cursor relative to the beginning of the current
 * piece descriptor. If you need the bufpos, just add rpos to current.startIndex
 *
 * @param cursor The position of the cursor within the text document
 * @param current The descriptor that the cursor is in
 * @return the position of the cursor relative to the current descriptor
 * */
int PieceTable::rpos(int cursor, PieceDescriptor current){
    uint lastEnd = pieceDescriptors.begin()->startIndex;
    for(auto desc : pieceDescriptors){
        if(desc.isFile == current.isFile && desc.startIndex == current.startIndex){
            break;
        }
        if(desc.isFile != current.isFile){
            cursor -= desc.length;
        }else{
            if(lastEnd < desc.startIndex){
                cursor += desc.startIndex - lastEnd;
            }
            lastEnd = desc.startIndex + desc.length;
        }
    }
    return cursor - current.startIndex;
}

PieceDescriptor PieceTable::at(int cursor){
    std::list<PieceDescriptor>::iterator it1 = pieceDescriptors.begin();
    uint offset = it1->startIndex + it1->length; 
    while(offset < cursor){
        it1++;
        offset += it1->length;
    }
    return *it1;
}

char* PieceTable::print(){
    std::string res;
    char *out;
    sprintf(out, "isFile        start        length\n");
    res += out;
    for(auto desc : pieceDescriptors){
        sprintf(out, "%i\t\t%i\t\t%zu\n", desc.isFile, desc.startIndex, desc.length);
        res += out;
    }
    std::cout << res;
    return out;
}

PieceTable::~PieceTable(){}


