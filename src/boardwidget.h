/***************************************************************************
 *   Author Alan Crispin                 *
 *   crispinalan@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                  *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/


#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H


#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>
#include <QMessageBox>


#include <iostream>
#include <vector>
#include <string>


using namespace std;

const int BOARDSIZE=8;


const int LEGALMOVE = 1;
const int ILLEGALMOVE = 2;
const int INCOMPLETEMOVE = 3;

const int EMPTY=0;
const int WMAN=1;
const int BMAN=2;
const int WKING=3;
const int BKING=4;

typedef enum {WHITE =0, BLACK=1} COLOUR;



struct Pos{
public:
	int x;
	int y;
};

struct Move{
public:
	Pos start;
	Pos end;
};


class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);


	void newGame();
	void resetPlayer();

    
signals:
    void outgoingText(const QString &text); //communication with mainwindow
    
public slots:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


private:

	int theBoard[8][8];
	int boardArray[64];

	void setupBoard(int board[8][8]);
	void displayBoard(int board[8][8]);


	//player clicks
	bool firstClick =true;
	int x1=0;
	int y1=0;
	int x2=0;
	int y2=0;


	int currentPiece=WMAN;
	COLOUR currentPlayer =WHITE;
	COLOUR aiColour=BLACK;
	bool playerFinished = false;



	bool makeMove(int board[8][8], Move move, COLOUR side);
	bool movesEqual(Move one, Move two);
	void makeAIMove();

	//helpers new
	int numBMAN;
    int numWMAN;
    int numBKING;
    int numWKING;


	Pos getMidSquare(Move m);
	bool isOpponentPiece(COLOUR player, const int piece);
	bool isOwnPiece(int board[8][8], int x, int y, COLOUR player);

	bool canJump(int board[8][8], int x, int y, COLOUR side);
	bool isJump(int board[8][8], int x1, int y1, int x2, int y2);

	void getValidMoves(int board[8][8], int x, int y, vector<Move> &moves);
	void getValidJumpMoves(int board[8][8], int x, int y, COLOUR side, vector<Move>& moves);
	void getAllMoves(int board[8][8], COLOUR side, vector<Move>& moves);


    void cloneBoard(int board1[8][8], int board2[8][8]);

	int getScore(int board[8][8], COLOUR player);

    int searchDepth; //search depth
	Move minimaxStart(int board[8][8], COLOUR player, bool maximizing); //returns final move
	int minimax(int board[8][8], COLOUR player, bool maximizing, int depth, int alpha, int beta); //returns score

	bool checkForWin(int board[8][8], COLOUR player);
	int numberWhitePiecesOnBoard(int board[8][8]);
	int numberBlackPiecesOnBoard(int board[8][8]);


	// Drawing
	QRect positionToRectangle(int position);
	int squareSize;
	int boardWidgetWidth;
	int boardWidgetHeight;

	//bitmaps
	QPixmap boardPixmap;
	QPixmap whitePawn;
	QPixmap whiteKing;
	QPixmap blackPawn;
	QPixmap blackKing;
	QString gameInfo="";

};

#endif // BOARDWIDGET_H
