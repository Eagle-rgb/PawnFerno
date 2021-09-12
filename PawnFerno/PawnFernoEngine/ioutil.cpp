#include "ioutil.h"

namespace printing {
    string toBitString(BitBoard bb) {
        string out = "";
        for (int i = 0; i < 64; i++) {
            out += to_string(bb & 1);
            bb >>= 1;
        }
        return out;
    }

    void print(BitBoard bb) {
        string bs = toBitString(bb);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << " " << bs[(7 - i) * 8 + j];
            }
            cout << endl;
        }
    }

    void print(Square sq) { print(toBB(sq)); }

    void printBoard(const string charBB) {
        for (int i = 7; i >= 0; i--) {
            cout << rankChar(Rank(i)) << " | ";

            for (int j = 0; j < 7; j++) {
                cout << charBB[8 * i + j] << " ";
            }

            cout << charBB[8 * i + 7] << endl;
        }

        cout << "   ----------------" << endl << "    ";

        for (auto _ = 0; _ < 7; _++)
            cout << fileChar(File(_)) << " ";

        cout << fileChar(File(7)) << endl;
    }
}