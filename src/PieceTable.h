#pragma once
#define FILE_BUFFER true
#define ADD_BUFFER false

#include <list>
#include <string>
#include <vector>

struct PieceDescriptor{
    bool isFile;
    uint startIndex;
    size_t length;
};

class PieceTable {
    public:
        std::string file, add; // write only file buffer, append-only add buffer
        std::list<PieceDescriptor> pieceDescriptors;

        void insert(std::string str, int cursorPos);
        void remove(int cursorPos, int length);
        void stringify(std::string &ret);

        PieceTable(std::string str);
        ~PieceTable();
};
