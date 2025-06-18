#include "PieceTable.h"
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

            //FIX: this won't work when you delete a piece of the original file
            uint relativePos = cursorPos;
            uint fileBufferPos = cursorPos;
            uint lastEnd = pieceDescriptors.begin()->startIndex;
            for(auto desc : pieceDescriptors){
                if(offset > desc.startIndex + desc.length){
                    relativePos -= desc.length;
                }
                if(offset > desc.startIndex){
                    if(desc.isFile != it1->isFile){
                        fileBufferPos -= desc.length;
                    } else {
                        //add the deleted part of the buffer back
                        if(lastEnd != desc.startIndex){
                            fileBufferPos += desc.startIndex - lastEnd;
                        }
                        lastEnd = desc.startIndex + desc.length;
                    }
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
    int offset = 0;
    std::list<PieceDescriptor>::iterator it1;
    for(it1 = pieceDescriptors.begin(); it1 != pieceDescriptors.end(); it1++){
        offset += it1->length;
        if(offset - it1->length == cursorPos){
            //its at the beginning of a descriptor
            if(it1->length > length){
                it1->length -= length;
                it1->startIndex += length;
                return;
            } else {
                //if the delete length is larger or equal to the descriptor length
                //then delete the descriptor and calculate the previous desc
                while(length >= it1->length){
                    length -= it1->length;
                    pieceDescriptors.erase(it1);
                }
                if(length > 0){
                    it1->length -= length;
                    it1->startIndex += length;
                }
                return;
            }
        } else if (offset > cursorPos){
            //its in the middle of a descriptor
            uint relativePos = cursorPos; // this is the position relative to the current descriptor
            uint bufferPos = cursorPos; // this will be the position relative to whatever buffer we are in
            uint lastEnd = pieceDescriptors.begin()->startIndex;
            for(auto desc : pieceDescriptors){
                if(offset > desc.startIndex + desc.length){
                    relativePos -= desc.length;
                }
                if(offset > desc.startIndex){
                    if(desc.isFile != it1->isFile){
                        bufferPos -= desc.length;
                    } else {
                        //add the deleted part of the buffer back
                        if(lastEnd != desc.startIndex){
                            bufferPos += desc.startIndex - lastEnd;
                        }
                        lastEnd = desc.startIndex + desc.length;
                    }
                }
            }
            // std::cout << "relativePos: " << relativePos << "\n";
            if(it1->length > length){
                //split the descriptor into 2 pieces
                std::cout << "fish\n";
                uint length2 = it1->length - relativePos - length; // calulcate the length of the second piece descriptor
                std::cout << "cake\n";
                // we don't need to calculate lenght1 because it's just relativePos                                                   
                // startPos for newDesc should be the bufferPos of the cursor

                auto newDesc = PieceDescriptor{it1->isFile, bufferPos + length, length2};
                std::cout << "bufPos: " << bufferPos << ", relative: " << relativePos << "\n";
                it1->length = relativePos;
                pieceDescriptors.insert(++it1, newDesc);
                return;
            } else {
                //if the delete length is larger or equal to the descriptor length
                //then delete the descriptor and calculate the previous desc
                if(length >= it1->length - relativePos){
                    it1->length = relativePos;
                    std::cout << "rice\n";
                    length -= it1->length - relativePos;
                    std::cout << "cake\n";
                    it1++;
                }
                //now we know the "delete pointer" is at the beginning of the descriptor
                while(length >= it1->length){
                    length -= it1->length;
                    pieceDescriptors.erase(it1);
                }
                if(length > 0){
                    it1->length -= length;
                    it1->startIndex += length;
                }
                return;
            }
        }
    }
}

char* PieceTable::print(){
    std::string res;
    char *out;
    sprintf(out, "isFile        start        length\n");
    for(auto desc : pieceDescriptors){
        sprintf(out, "%i\t\t%i\t\t%zu\n", desc.isFile, desc.startIndex, desc.length);
    }
    std::cout << out;
    return out;
}

PieceTable::~PieceTable(){}


