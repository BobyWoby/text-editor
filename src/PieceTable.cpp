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
        // out += "|";
    }
    ret = out;
}

//TODO: Implement this function
void PieceTable::insert(std::string str, int cursorPos){
    std::list<PieceDescriptor>::iterator it = iterat(cursorPos);
    int relativePos = rpos(cursorPos, *it);
    int bufferPos = it->startIndex + relativePos;
    if(relativePos == 0){
        insertAtEdge(str, it);
        return;
    }else{
        //create a new descriptor to insert beforet he iterator and change it 
        //to be correct 
        PieceDescriptor newDesc{it->isFile, it->startIndex, (size_t)relativePos};
        it->length = it->length - relativePos;
        it->startIndex = bufferPos;
        pieceDescriptors.insert(it, newDesc);
        insertAtEdge(str, it);
        return;
    }
}
void PieceTable::insertAtEdge(std::string str, std::list<PieceDescriptor>::iterator it){
    pieceDescriptors.insert(it, {false, (uint)add.length(), str.length()});
    add += str;
}

//TODO: Implement this function
void PieceTable::remove(int cursorPos, int length){
    std::list<PieceDescriptor>::iterator it = iterat(cursorPos);
    int relativePos = rpos(cursorPos, *it);


    if(relativePos + length == it->length){
        it->length = relativePos;
        if(!relativePos) pieceDescriptors.erase(it);
        return;
    } else if (relativePos + length < it->length){
        if(!relativePos){
            it->startIndex += length;
            it->length -= length;
            return;
        }
        PieceDescriptor newDesc{it->isFile, it->startIndex, (size_t)relativePos};
        it->startIndex += relativePos + length;
        it->length -= (relativePos + length);
        pieceDescriptors.insert(it, newDesc);
        return;
    } else{
        // //TODO: Kinda wanna try and do it without recursion, but this should work?
        length -= (it->length - relativePos);
        it->length = relativePos;
        if(it->length == 0){
            pieceDescriptors.erase(it);
        }
        if(!cursorPos) remove(cursorPos, length);
        else remove(cursorPos, length);
    }
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
        if(desc.isFile != current.isFile){
            cursor -= desc.length;
        }else{
            if(lastEnd < desc.startIndex){
                cursor += desc.startIndex - lastEnd;
            }
            lastEnd = desc.startIndex + desc.length;
        }
        if(desc.isFile == current.isFile && desc.startIndex == current.startIndex){
            break;
        }
    }
    return cursor - current.startIndex;
}

PieceDescriptor PieceTable::at(int cursor){
    return *iterat(cursor);
}

std::list<PieceDescriptor>::iterator PieceTable::iterat(int cursor){
    std::list<PieceDescriptor>::iterator it1 = pieceDescriptors.begin();
    uint offset = it1->startIndex + it1->length; 
    while(offset <= cursor){
        it1++;
        offset += it1->length;
    }
    return it1;
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


