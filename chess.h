#ifndef _CHESS_H_
#define _CHESS_H_
#include <iostream>
#include <vector>
#include "game.h"
#include "player.h"
#include <utility>
#include "chess_instruction.h"
#include "piece.h"
class ChessISA;
class ChessPieceCreator;
class ChessAI;
class ChessSupervisor;
class Chess: public Game<ChessISA> {
    std::vector<std::vector<Piece<ChessISA>*>> setupGrid;
    bool defaultSetup = true;
    bool stalemateRequest = false;
    void castlingExecute(const ChessISA&);
    void enPassantExecute(const ChessISA&);
    void promotionExecute(const ChessISA&);
    void movingExecute(const ChessISA&);


    void reverseCastlingExecute(const ChessISA&);
    void reverseEnPassantExecute(const ChessISA&);
    void reversePromotionExecute(const ChessISA&, bool);
    void reverseMovingExecute(const ChessISA&, bool);

    void resignExecute(const ChessISA&);
    void stalemateExecute(const ChessISA&);
    void undoExecute();
    void historyExecute();

    void setupBackup();
    void setupRecover();
    public:
    Chess();
    ~Chess();
    bool execute(const ChessISA&, bool validationCheck = true) override;
    bool isFinish() override;
    void init() override;
    void setup() override;
    void clear() override;

};
#endif

