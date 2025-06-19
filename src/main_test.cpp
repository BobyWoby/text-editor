#include <iostream>
#include "PieceTable.h"

//Test file
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
    std::list<PieceDescriptor>::iterator it1 = ptable.pieceDescriptors.begin();
    while(it1 != ptable.pieceDescriptors.end()){
        ptable.insertAtEdge("test ", it1);
        if(it1 != ptable.pieceDescriptors.end()){
            it1++;
        }
    }
    ptable.stringify(out);
    std::cout << out << "\n";
    return 0;
}
