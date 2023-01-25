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


const int WWINS=5;
const int BWINS=6;


struct Pos{
public:
	int x;
	int y;
};

struct Movement{
public:
	Pos start;
	std::vector<Pos> dests;
	int numberOfCaptures;
};


class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);


	void newGame();
	void resetPlayer();

    
signals:
    void outgoingText(const QString &text);
    
public slots:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


private:


	//int position[8][8];
	int theBoard[8][8];
	int boardArray[64];

	void setupBoard(int board[8][8]);
	void displayBoard(int board[8][8]);


	//globals
	bool firstClick =true;
	int x1=0;
	int y1=0;
	int x2=0;
	int y2=0;

	//int turn; //current player
	std::vector<Movement> moves; //global moves

	int currentPiece=WMAN;
	COLOUR currentPlayer =WHITE;
	COLOUR aiColour=BLACK;
	bool playerFinished = false;


	//movement
	void printMovement(Movement m);
	void addMove(Pos piece, std::vector<Pos> destinationsList, int captureCount);
	bool findMakeWhiteMove(int board[8][8],int x1, int y1, int x2, int y2);
	void makeMove(int board[8][8], Movement move);

	std::vector<Movement> getPawnMoves(int board[8][8], COLOUR player, Pos pos1);

	int getPawnCaptureMoves(Pos pos1, Pos pos2, int board[8][8], COLOUR player,int countCaptures, std::vector<Pos> destPositions);

    std::vector<Movement> getKingMoves(int board[8][8], COLOUR player, Pos pos1);

	int getKingCaptureMoves(Pos pos1, Pos pos2, int board[8][8], COLOUR player, int countCaptures, std::vector<Pos> destPositions);

	std::vector<Movement> filterMovesOnCaptureCount(std::vector<Movement> moves);

	int getBestCaptureCount(std::vector<Movement> moves);

	std::vector<Movement> getAllMoves(int board[8][8], COLOUR player);



	//helpers
	bool isValidPos(Pos pos);
	int isKing(int piece);
	int isOpposite(COLOUR player, int piece);
	COLOUR getOppositeColour(COLOUR player);

	int isEndOfBoard(Pos piece, COLOUR player);
	Pos getNextDiag(Pos from, Pos to);
	Pos getPrevDiag(Pos from, Pos to);
	int getDiagDirection(Pos from, Pos to);
	bool destinationsValid(std::vector<Pos> dests);

	bool checkForWin(int board[8][8], COLOUR player);
	int numberWhitePiecesOnBoard(int board[8][8]);
	int numberBlackPiecesOnBoard(int board[8][8]);


	//AI (more to do)
	void duplicateBoard(int board1[8][8], int board2[8][8]);
	void  makeAIMove(int board[8][8], COLOUR player); //testing


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
