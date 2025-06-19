#include <iostream>
#include "PieceTable.h"

int main(){
    PieceTable ptable{"Hello"};
    ptable.file = "Hello World!";
    ptable.add = "a ";
    ptable.pieceDescriptors = std::list<PieceDescriptor>{
        {true, 0, 1},
        {true, 2, 4},
        {false, 0, 2},
        {true, 6, 6}
    };
    ptable.print();
    std::string out;
    ptable.stringify(out);
    std::cout << out <<"\n";

    auto piece = ptable.at(8);
    // std::cout << piece.isFile << "\t\t" << piece.startIndex << "\t\t" 
    //     << piece.length << "\n";
    std::cout << ptable.rpos(8, ptable.at(8)) << "\n";
    return 0;
}
