#include "ioutil.h"

namespace printing {
    std::string toBitString(BitBoard bb) {
        std::string out = "";
        for (Square sq = SQa1; sq <= SQh8; ++sq) {
            out += std::to_string(bb & 1);
            bb >>= 1;
        }

        return out;
    }

    std::string print(BitBoard bb) {
        std::string bs = toBitString(bb);
        std::string result = "";
        for (Rank i = RANK1; i <= RANK8; ++i) {
            for (File j = FILEA; j <= FILEH; ++j) {
                result += " ";
                result += bs[(7 - i) * 8 + j];
            }

            result += "\n";
        }

        return result;
    }

    std::string print(Square sq) {
        std::string x = "";
        char fileRank[2];
        x += fileChar(toFile(sq));
        x += rankChar(toRank(sq));

        return x;
    }

    std::string print(Move m, Color player) {
        if (move::isPromotion(m)) {
            char promotionPiece = toChar(move::getPromotionPiece(m), player);
            return print(move::originSquare(m)) + print(move::destinationSquare(m)) + promotionPiece;
        }

        return print(move::originSquare(m)) + print(move::destinationSquare(m));
    }

    std::string print(std::vector<Move> moves, Color who) {
        std::vector<Move>::iterator it;
        std::string result = "";

        for (it = moves.begin(); it <= moves.end(); ++it) {
            result += print(*it, who);
        }

        return result;
    }

    std::string printSquareBB(Square sq) { return print(toBB(sq)); }

    std::string printBoard(const std::string charBB) {
        std::string result = "";

        for (Rank i = RANK8; i >= RANK1; --i) {
            result += rankChar(i);
            result += " | ";

            for (File j = FILEA; j <= FILEG; ++j) {
                result += charBB[8 * (7 - i) + j];
                result += " ";
            }

            result += charBB[8 * (7 - i) + FILEH];
            result += "\n";
        }

        result += "   ----------------\n    ";

        for (auto _ = FILEA; _ < FILEH; ++_) {
            result += fileChar(_);
            result += " ";
        }

        result += fileChar(FILEH);
        result += "\n";

        return result;
    }
}