#include "ioutil.h"

namespace printing {
    std::string toBitString(BitBoard bb) {
        std::string out = "";
        for (int i = 0; i < 64; i++) {
            out += std::to_string(bb & 1);
            bb >>= 1;
        }

        return out;
    }

    std::string print(BitBoard bb) {
        std::string bs = toBitString(bb);
        std::string result = "";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                result += " ";
                result += bs[(7 - i) * 8 + j];
            }

            result += "\n";
        }

        return result;
    }

    std::string print(Square sq) { return print(toBB(sq)); }

    std::string printBoard(const std::string charBB) {
        std::string result = "";

        for (int i = 7; i >= 0; i--) {
            result += rankChar(Rank(i));
            result += " | ";

            for (int j = 0; j < 7; j++) {
                result += charBB[8 * (7 - i) + j];
                result += " ";
            }

            result += charBB[8 * (7 - i) + 7];
            result += "\n";
        }

        result += "   ----------------\n    ";

        for (auto _ = 0; _ < 7; _++) {
            result += fileChar(File(_));
            result += " ";
        }

        result += fileChar(File(7));
        result += "\n";

        return result;
    }
}