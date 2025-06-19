#pragma once
#define FILE_BUFFER true
#define ADD_BUFFER false

#include <list>
#include <string>

struct PieceDescriptor{
    bool isFile;
    uint startIndex;
    size_t length;
};

class PieceTable {
    public:
        std::string file, add; // write only file buffer, append-only add buffer
        std::list<PieceDescriptor> pieceDescriptors;

        void insertAtEdge(std::string str, std::list<PieceDescriptor>::iterator it);
        void insert(std::string str, int cursorPos);
        void remove(int cursorPos, int length);
        void stringify(std::string &ret);
        char* print();
        int rpos(int cursor, PieceDescriptor current);
        PieceDescriptor at(int cursor);

        PieceTable(std::string str);
        ~PieceTable();
};
