#include "ioutil.h"

void print(BitBoard bb){
    for (int i=0; i < 64; i++){
        cout << " " << (bb & 1);
        bb >>= 1;
        if (i % 8 == 7) {
            cout << endl;
        }
    }

}

int main() {
    BitBoardInit();
    print(PAWN_CAPTURES[0][9]);
    return 0;
}