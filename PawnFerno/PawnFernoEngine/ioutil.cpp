#include "ioutil.h"
#include <string>

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
    print(BB_RANK1);

    return 0;
}