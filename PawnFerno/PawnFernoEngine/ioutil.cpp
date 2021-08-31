#include "ioutil.h"

string toBitString(BitBoard bb) {
    string out = "";
    for (int i=0; i < 64; i++){
        out += to_string(bb & 1);
        bb >>= 1;
    }
    return out;
}

void print(BitBoard bb){
    string bs = toBitString(bb);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            cout << " " << bs[(7 - i) * 8 + j];
        }
        cout << endl;
    }
}

int main() {
    BitBoardInit();
    print(PAWN_CAPTURES[0][SQb2]);

    cout << endl;

    BitBoard b = BB_RANK7;

    print(BB_RANK7 & SQa7); cout << endl;
    print(b &= SQa7);

    return 0;
}