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


#include "boardwidget.h"

BoardWidget::BoardWidget(QWidget *parent) :QWidget(parent)
{

    //Constructor

    const int height = ((QApplication::desktop()->height())/6)*5;

    boardWidgetWidth =(height/10)*9;
    boardWidgetHeight=(height/10)*9;

    this->setFixedSize(boardWidgetWidth,boardWidgetHeight);

    squareSize = (boardWidgetHeight/8);


    //board pixmap
    boardPixmap.load("://board/board.png"); //load resource
    boardPixmap = boardPixmap.scaled(boardWidgetHeight,boardWidgetHeight);

    whitePawn.load("://pieces/whitepiece.png");
    whiteKing.load("://pieces/whiteking.png");


    whitePawn = whitePawn.scaled(boardWidgetHeight/8,boardWidgetHeight/8,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    whiteKing = whiteKing.scaled(boardWidgetHeight/8,boardWidgetHeight/8,Qt::KeepAspectRatio, Qt::SmoothTransformation);

    blackPawn.load("://pieces/blackpiece.png");
    blackKing.load("://pieces/blackking.png");

    blackPawn = blackPawn.scaled(boardWidgetHeight/8,boardWidgetHeight/8,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    blackKing = blackKing.scaled(boardWidgetHeight/8,boardWidgetHeight/8,Qt::KeepAspectRatio, Qt::SmoothTransformation);

	newGame();

}

void BoardWidget::newGame(){

	setupBoard(theBoard);
    displayBoard(theBoard);
    currentPiece=WMAN; // white is the player
	gameInfo ="White to move";
	emit outgoingText(gameInfo);
    firstClick =true;
	x1=0;
	y1=0;
	x2=0;
	y2=0;
	currentPlayer =WHITE;
	aiColour=BLACK;
	playerFinished = false;

}

void BoardWidget::resetPlayer(){

	firstClick =true;
	x1=0;
	y1=0;
	x2=0;
	y2=0;

	gameInfo.clear();
	QString playerStr="";
	if (currentPlayer ==WHITE) playerStr="WHITE";
	else playerStr="BLACK";
	gameInfo.append(" Current player: ");
    gameInfo.append(playerStr);
	gameInfo.append(".  ");
	emit outgoingText(gameInfo);
}

void BoardWidget::setupBoard(int board[8][8])
{

   	for (int y = 0; y < 8; y++){
		for (int x = 0; x < 8; x++){
				board[x][y] = EMPTY;
			}
	}

	//White pieces (x,y)
	board[0][1] =BMAN;
	board[1][0] =BMAN;
	board[1][2] =BMAN;
	board[2][1] =BMAN;
	board[3][0] =BMAN;
	board[3][2] =BMAN;
	board[4][1] =BMAN;
	board[5][0] =BMAN;
	board[5][2] =BMAN;
	board[6][1] =BMAN;
	board[7][0] =BMAN;
	board[7][2] =BMAN;

	//black pieces
	board[0][7] = WMAN;
	board[0][5] = WMAN;
	board[1][6] = WMAN;
	board[2][7] = WMAN;
	board[2][5] = WMAN;
	board[3][6] = WMAN;
	board[4][7] = WMAN;
	board[4][5] = WMAN;
	board[5][6] = WMAN;
	board[6][7] = WMAN;
	board[6][5] = WMAN;
	board[7][6] = WMAN;

}


void BoardWidget::displayBoard(int board[8][8]) {

    int i=0;
    for (int y = 0; y < 8; y++){
		for (int x = 0; x < 8; x++){
				boardArray[i] =board[x][y];
                i=i+1;
			}
	}

	repaint();
}

void BoardWidget::paintEvent(QPaintEvent *event)
{

    QRect boardSize(QPoint(0,0),QPoint(boardWidgetHeight,boardWidgetHeight));
    QRect pieceSize(QPoint(0,0),QPoint(boardWidgetHeight/8,boardWidgetHeight/8));

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(boardSize,boardPixmap,boardSize); //draw board

	 QRect positionRect;
    int player;

   for(int i=0;i<=63;i++){

        player=boardArray[i];
        positionRect = positionToRectangle(i+1);


        switch(player){
        case(WMAN):
            painter.drawPixmap(positionRect,whitePawn,pieceSize);
            break;
        case(WKING):
           painter.drawPixmap(positionRect,whiteKing,pieceSize);
            break;
        case(BMAN):
            painter.drawPixmap(positionRect,blackPawn,pieceSize);
            break;
        case(BKING):
           painter.drawPixmap(positionRect,blackKing,pieceSize);
            break;
        case(EMPTY):
            break;

        } //switch
    } //for i

}

QRect BoardWidget::positionToRectangle(int position){

    int xPos,yPos,x2Pos,y2Pos;
    xPos = ((position-1)%8)* squareSize;
    yPos = ((position-1)/8)*squareSize;
    x2Pos = xPos + squareSize;
    y2Pos = yPos + squareSize;
    QRect rectangle(QPoint(xPos,yPos),QPoint(x2Pos,y2Pos));
    return rectangle;
}


void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    const int xpos = event->pos().x() / squareSize;
    const int ypos = event->pos().y() / squareSize;

    if(firstClick){
	x1=xpos;
	y1=ypos;
	firstClick=false;

    QString x1Str =	QString::number(x1);
    QString y1Str =	QString::number(y1);
    gameInfo.clear();
    gameInfo.append(" Player moving peice at:  (");
    gameInfo.append(x1Str);
    gameInfo.append(",");
    gameInfo.append(y1Str);
    gameInfo.append(") ");
    emit outgoingText(gameInfo);

	return;
	}
	else {
	x2=xpos;
	y2=ypos;
	//currentPiece=getPiece(theBoard,x1,y1); //set gobally

	if (findMakeWhiteMove(theBoard,x1,y1,x2,y2))
	{
		displayBoard(theBoard);
		firstClick=true;
		playerFinished=true;

		if(checkForWin(theBoard,WHITE)) {
			//qDebug()<<"White Wins\n";
			gameInfo.clear();
		    gameInfo ="White wins. ";
		    emit outgoingText(gameInfo);
			//messagebox win
			QString text = "White wins.";
            QMessageBox::information(this, "Draughts", text);
			firstClick=true;
			playerFinished=false;
			return;
		}
	}
	else {
		//qDebug()<<"Illegal move (wrong position or check for jumps)\n";
		gameInfo ="Illegal move (wrong position or check for jumps)";
		emit outgoingText(gameInfo);
		firstClick=true;
		playerFinished=false;
		return;
	}

	if (playerFinished)
	{

		//computer player
		makeAIMove(theBoard,BLACK);
		displayBoard(theBoard);
		playerFinished=false;
		//qDebug()<<"White to move\n";
		gameInfo.clear();
		gameInfo ="White to move. ";
		emit outgoingText(gameInfo);

		//check for win
		if(checkForWin(theBoard,BLACK)) {
		//qDebug()<<"black wins\n";
		gameInfo.clear();
		gameInfo ="Black wins. ";
		emit outgoingText(gameInfo);
		QString text = "Black wins.";
		QMessageBox::information(this, "Draughts", text);
		firstClick=true;
		playerFinished=false;
		return;
		}

	} //playerfinished

	} //else first click


}



//-------------------------------------------------------------------------------
// Helpers
//-------------------------------------------------------------------------------

bool BoardWidget::isValidPos(Pos pos)
{
	//check if position is on the board
    if(pos.x>=0 && pos.x<8 && pos.y >=0 && pos.y<8) return true;
	else return false;
}

int BoardWidget::isKing(int piece)
{
	bool king =false;
    if  (piece==WKING || piece ==BKING) king =true;
    return king;
}

int BoardWidget::isOpposite(COLOUR player, int piece)
{
	if (player == WHITE && (piece == BMAN || piece == BKING)) return 1;
	if (player == BLACK && (piece == WMAN || piece == WKING)) return 1;
	return 0;
}

COLOUR BoardWidget::getOppositeColour(COLOUR player)
{
	// Return the opposite colour to the argument
   if(player == WHITE) return BLACK;
	else return WHITE;
}

int BoardWidget::isEndOfBoard(Pos piece, COLOUR player)
{
	if (player == WHITE) return piece.y == BOARDSIZE - 1;
	else return piece.y == 0;
}


Pos BoardWidget::getPrevDiag(Pos from, Pos to)
{
	Pos res;
	if (from.x - to.x > 0) res.x = to.x + 1;
	else res.x = to.x - 1;
	if (from.y - to.y > 0) res.y = to.y + 1;
	else res.y = to.y - 1;
	return res;
}
Pos BoardWidget::getNextDiag(Pos from, Pos to)
{
	Pos res;
	if (from.x - to.x > 0) res.x = to.x - 1;
	else res.x = to.x + 1;
	if (from.y - to.y > 0) res.y = to.y - 1;
	else res.y = to.y + 1;
	return res;
}

bool BoardWidget::destinationsValid(std::vector<Pos> dests)
{
	//check destinations are on board
	for(unsigned int i=0; i<dests.size(); i++)
	{
		Pos p =dests.at(i);
		if(!isValidPos(p)) return false;
	}

	return true;
}


bool BoardWidget::checkForWin(int board[8][8], COLOUR player)
{
	if(player ==BLACK)	{
	//check for black win
	int whiteNumber =numberWhitePiecesOnBoard(board);
    if(whiteNumber==0) return true;
	}
	if(player ==WHITE) {
		//check for white win
    int blackNumber=numberBlackPiecesOnBoard(board);
    if(blackNumber==0) return true;
	}

	return false;
}

int BoardWidget::numberWhitePiecesOnBoard(int board[8][8])
{
	int numberWhitePieces=0;

	for(int y=0; y<8; y++){
		for(int x=0; x<8; x++){
			if(board[x][y]==WMAN|| board[x][y]==WKING ){
				numberWhitePieces++;
			}
		}
	}
	return numberWhitePieces;
}

int BoardWidget::numberBlackPiecesOnBoard(int board[8][8])
{
	int numberBlackPieces=0;

	for(int y=0; y<8; y++){
		for(int x=0; x<8; x++){
			if(board[x][y]==BMAN || board[x][y]==BKING ){
				numberBlackPieces++;
			}
		}
	}
	return numberBlackPieces;
}
//------------------------------------------------------------------------------------
// Movements
//-------------------------------------------------------------------------------------

void BoardWidget::printMovement(Movement m){

	//used for debugging
	//qDebug()<<"movement \n";

	Pos spos =m.start;
	std::vector<Pos> dests = m.dests;
	int numberOfCaptures = m.numberOfCaptures;

	qDebug()<<"start (x,y) ("<<spos.x<<","<<spos.y<<") ";
	qDebug()<<"destinations: ";
	for (long unsigned int i = 0; i < dests.size(); i++) {
	Pos p =dests.at(i);
	qDebug()<<" ("<<p.x<<","<<p.y<<") ";
	}
	//qDebug()<<"\n";
	qDebug()<<"numberOfCaptures = "<<numberOfCaptures<<"\n";

}

void BoardWidget::addMove(Pos piece, std::vector<Pos> destinationsList, int captureCount)
{
	Movement m_tmp {};
	m_tmp.start=piece;
	m_tmp.dests=destinationsList;
	m_tmp.numberOfCaptures=captureCount;

	if (destinationsValid(destinationsList))
	moves.push_back(m_tmp);
}

void BoardWidget::duplicateBoard(int board1[8][8], int board2[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board2[i][j] = board1[i][j];
}

bool BoardWidget::findMakeWhiteMove(int board[8][8],int x1, int y1, int x2, int y2)
{
	//Get black moves for board
	int copyBoard[BOARDSIZE][BOARDSIZE];
	duplicateBoard(board,copyBoard);

	std::vector<Movement> whiteMoves = getAllMoves(board,WHITE);

	//qDebug()<<"findMakeWhiteMove: whitemoves size = "<<whiteMoves.size()<<"\n";

	if(whiteMoves.empty()) return false;

	duplicateBoard(copyBoard,board);

	for(unsigned int i=0; i<whiteMoves.size(); i++)
	{
		Movement m =whiteMoves.at(i);

		if (m.start.x ==x1 && m.start.y == y1)
		{
			if(m.dests.at(m.dests.size()-1).x ==x2 &&
				m.dests.at(m.dests.size()-1).y ==y2)	{
					//found move
					//duplicateBoard(copyBoard,board);
					makeMove(board,m);
					return true;
				}

		}
	}
	duplicateBoard(copyBoard,board);
	return false;
}

void BoardWidget::makeMove(int board[8][8], Movement move)
{

	Pos cur;
	Pos cap;  //capture

	cur.x=move.start.x;
	cur.y=move.start.y;

	//qDebug()<<"makeMove: start ("<<cur.x<<","<<cur.y<<") piece_type = "<<board[cur.x][cur.y]<<"\n";

	if(move.dests.empty()) return;

	for(unsigned int i =0; i<move.dests.size(); i++)
	{

		board[move.dests[i].x][move.dests[i].y] = board[cur.x][cur.y];

// 		//check for kings
		if((move.dests[i].y == 0) && (board[cur.x][cur.y] == WMAN)) {
 			board[move.dests[i].x][move.dests[i].y] = WKING;
 		    currentPiece=WKING;
 		}

 		if((move.dests[i].y == BOARDSIZE - 1) && (board[cur.x][cur.y] == BMAN)) {
 			board[move.dests[i].x][move.dests[i].y] = BKING;
 			currentPiece=BKING;
 		}


		board[cur.x][cur.y] = EMPTY;

		if (move.numberOfCaptures > 0){
			 cap = getPrevDiag(cur, move.dests[i]);
			 //cout<<"Capture pos = ("<<cap.x<<","<<cap.y<<")"<<endl;
			 board[cap.x][cap.y] = EMPTY;
		}

	cur.x = move.dests[i].x;
	cur.y = move.dests[i].y;

	}
}


std::vector<Movement> BoardWidget::getAllMoves(int board[8][8], COLOUR player)
{


	Pos p;
	vector<Movement> allMoves{};
	vector<Movement> legalMoves{};
	char currPiece;

	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			if (!isOpposite(player, board[x][y]) && board[x][y] != EMPTY){
		p.y = y;
		p.x = x;
		currPiece = board[x][y];

		//qDebug()<<"getAllMoves(): testing currPiece ("<<x<<","<<y<<") = "<<currPiece<<"\n";
		if (isKing(currPiece)){
			if(player==WHITE) currentPiece=WKING; //globals
			if(player==BLACK) currentPiece=BKING;
			vector<Movement> king_moves =getKingMoves(board, player, p);
            allMoves.insert(allMoves.end(), king_moves.begin(),king_moves.end());
			}
		else {
			if(player==WHITE) currentPiece=WMAN; //globals
			if(player==BLACK) currentPiece=BMAN;
			vector<Movement> pawn_moves = getPawnMoves(board, player, p);

			//qDebug()<<"getAllMoves(): pawnMoves Size = "<<pawn_moves.size()<<"\n";

			//print out pawn_moves

// 			for (long unsigned int i=0; i < pawn_moves.size(); i++) {
// 				Movement m =pawn_moves.at(i);
// 				printMovement(m);
// 			}


			allMoves.insert(allMoves.end(), pawn_moves.begin(),pawn_moves.end());
			}
		}

	int count = getBestCaptureCount(allMoves);
	//qDebug()<<"getAllMoves(): bestCaptureCount = "<<count<<"\n";

	if (count == 0) return allMoves; //don't change allmoves as no capture
	else {
	for(unsigned int i=0; i<allMoves.size(); i++)
	{
		Movement m =allMoves.at(i);
		int captures = m.numberOfCaptures;
		if(captures >0)
		{
			legalMoves.push_back(m);
		}
	}
	}
	return legalMoves;
}


//------------------------------------------------------------------------------------
// Pawn moves
//-------------------------------------------------------------------------------------

std::vector<Movement> BoardWidget::getPawnMoves(int board[8][8], COLOUR player, Pos pos1)
{
	//Pass board by value as it should be copied for each call so that the
	//previous state is retained in the call function
	vector<Movement> filteredMoves{};
	vector<Pos> destPositions{}; //destination positions for jump moves

	moves.clear(); //clear global moves vector

	//pos array of the 4 possible diagonals
	Pos pos[4] = {  { pos1.x - 1, pos1.y - 1 },
					{ pos1.x + 1, pos1.y - 1 },
					{ pos1.x - 1, pos1.y + 1 },
					{ pos1.x + 1, pos1.y + 1 } };

	int direction = 1;
	int foundFront = -1;


	//if (player == BLACK) direction = -1;

	if (player == BLACK) direction = 1;
	if (player == WHITE) direction = -1;

	for (int i = 0; i < 4; i++){

		if ((isValidPos(pos[i])) &&
				(pos[i].y == pos1.y + direction) &&
				(board[pos[i].x][pos[i].y] == EMPTY) )
		{
			//---------------------------------------
			//normal single move
			//----------------------------------------
			destPositions.clear();	//clear
			destPositions.push_back(pos[i]); //push current move
			addMove(pos1,destPositions,0);
			//return moves;
			destPositions.clear();

		}

		else if ((isValidPos(pos[i])) &&
				(pos[i].y == pos1.y + direction) &&
				(isOpposite(player, board[pos[i].x][pos[i].y])))
		{
			//-------------------------------------
			//now in single jump terrirory
			//-------------------------------------


			char tmp = board[pos[i].x][pos[i].y];	//opposite colour
			board[pos[i].x][pos[i].y] = EMPTY;
			board[pos1.x][pos1.y] = EMPTY;
			Pos pos2 = getNextDiag(pos1, pos[i]);
			//pos2 = getNextDiag(pos1, pos[i]);


			destPositions.push_back(pos2);

			//&&(pos[i].y == pos2.y + direction)

			if (isValidPos(pos2)  && board[pos2.x][pos2.y] == EMPTY)
			{

				//-----------------------------------------------------
				//now in multiple jumps terriory (recursion)
				//-----------------------------------------------------

				//dest_positions.push_back(pos2);

				if (!isEndOfBoard(pos2, player)) {

					//recursively adds capture moves to moves
					foundFront = getPawnCaptureMoves(pos1, pos2,
													board, player, 1, destPositions);
				}
			   if (foundFront == 0 || isEndOfBoard(pos2, player))
				{
					addMove(pos1,destPositions,1);
				}

			}
			board[pos[i].x][pos[i].y] = tmp;
			board[pos1.x][pos1.y] = currentPiece; //put piece back on board e.g.curr_piece= BLACK_M
			destPositions.clear();
		}
	}
	return filteredMoves=filterMovesOnCaptureCount(moves);
}

int BoardWidget::getPawnCaptureMoves(Pos pos1, Pos pos2, int board[8][8], COLOUR player,
										int countCaptures, std::vector<Pos> destPositions)
{
	//Pass the current board to recursive function by value.
	//It should be copied for each call so that the previous state is retained
	//in the call function (captures).

	Pos pos[4] = { 	{ pos2.x - 1, pos2.y - 1 },
					{ pos2.x + 1, pos2.y - 1 },
					{ pos2.x - 1, pos2.y + 1 },
					{ pos2.x + 1, pos2.y + 1 } };
	int direction = 1;
	int foundNow = 0;
	char tmp =board[pos1.x][pos1.y];
	int foundFront = -1;
	//if (player == BLACK) direction = -1;
	if (player == BLACK) direction = 1;
	if (player == WHITE) direction = -1;

	for (int i = 0; i < 4; i++)

		//add direction checking
		//&&(pos[i].y == pos2.y + direction)
		if (isValidPos(pos[i])
		&&(pos[i].y == pos2.y + direction)
		&& isOpposite(player, board[pos[i].x][pos[i].y])){

			Pos newPiece = getNextDiag(pos2, pos[i]);

		if (isValidPos(newPiece) && board[newPiece.x][newPiece.y] == EMPTY){

			foundNow++;

			tmp = board[pos[i].x][pos[i].y]; //copy board piece to tmp
			board[pos[i].x][pos[i].y] = EMPTY; //now make pos position empty
			//vector<Pos> newDests(destPositions.begin(), destPositions.end());
			vector<Pos> newDests;
			newDests.insert(newDests.end(),destPositions.begin(), destPositions.end());
			newDests.push_back(newPiece);

			//if (!(isEndOfBoard(newPiece, player) && !isKing(currentPiece)))
			if (!(isEndOfBoard(newPiece, player)) && !isKing(currentPiece))

				foundFront = getPawnCaptureMoves(pos1, newPiece, board, player, countCaptures + 1, newDests);

			if (foundFront == 0 || (isEndOfBoard(newPiece, player) && !isKing(currentPiece))) {
				addMove(pos1,newDests,countCaptures+1);
				newDests.clear();

			}
			newDests.clear();
			board[pos[i].x][pos[i].y] = tmp;
		}
	}
	return foundNow;
}

//----------------------------------------------------------------------------------------
// filter moves on capture count
//-----------------------------------------------------------------------------------------

std::vector<Movement> BoardWidget::filterMovesOnCaptureCount(std::vector<Movement> moves)
{
	//filter moves for valid moves only
	vector<Movement> filteredMoves {};
	//filteredMoves.insert(filteredMoves.end(), moves.begin(), moves.end());

	int bestCaptureCount = getBestCaptureCount(moves);

	if (bestCaptureCount>0)
	{
		//only add moves with capture count >zero

	for(unsigned int i=0; i<moves.size(); i++)
	{
		Movement m =moves.at(i);
		int captures = m.numberOfCaptures;
		if(captures>0) filteredMoves.push_back(m);
	}
	return filteredMoves;
	}

	return moves; //else just return original moves
}

int BoardWidget::getBestCaptureCount(std::vector<Movement> moves)
{
	//Each position can have a set of moves
	// stored in a vector<Movement> list

	int bestCaptureCount=0;

	for(unsigned int i=0; i< moves.size(); i++)
	{
		Movement m =moves.at(i);
		int captures = m.numberOfCaptures;
		if(captures>bestCaptureCount)
		{
			bestCaptureCount=captures;
		}
	}
	return bestCaptureCount;
}


//----------------------------------------------------------------------------------------
// king moves
//-----------------------------------------------------------------------------------------

std::vector<Movement> BoardWidget::getKingMoves(int board[8][8], COLOUR player, Pos pos1)
{
	vector<Pos> destPositions{}; //destination positions for jump moves
	vector<Movement> filteredMoves {};
	moves.clear(); //clear global moves vector


	//pos array of the 4 possible diagonals
	Pos pos[4] = {  { pos1.x - 1, pos1.y - 1 },
					{ pos1.x + 1, pos1.y - 1 },
					{ pos1.x - 1, pos1.y + 1 },
					{ pos1.x + 1, pos1.y + 1 } };

	Pos curr, newPiece;
	int foundFront = -1;


	for (int i = 0; i < 4; i++){
		curr=pos[i];
		if ((isValidPos(curr)) &&
				(board[curr.x][curr.y] == EMPTY) )
		{
			//---------------------------------------
			//normal single move
			//----------------------------------------
			destPositions.clear();	//clear
			destPositions.push_back(curr); //push current move
			addMove(pos1,destPositions,0);


		}

		else if ((isValidPos(curr)) &&
				(isOpposite(player, board[curr.x][curr.y])))
		{
			//-------------------------------------
			//now in jumping terrirory
			//-------------------------------------

			char tmp = board[curr.x][curr.y];	//opposite colour
			board[curr.x][curr.y] = EMPTY;
			board[pos1.x][pos1.y] = EMPTY;
			//Pos pos2 = getNextDiag(pos1, pos[i]);
			newPiece=getNextDiag(pos1,curr);
			destPositions.push_back(newPiece);

			if (isValidPos(newPiece) && board[newPiece.x][newPiece.y] == EMPTY)
			{

				foundFront = getKingCaptureMoves(pos1, newPiece,board, player, 1, destPositions);

			   if (foundFront == 0)
				{

					addMove(pos1,destPositions,1);
				}

			}

			destPositions.clear();
			board[pos[i].x][pos[i].y] = tmp;
		}

	}
		return filteredMoves=filterMovesOnCaptureCount(moves);
}


int BoardWidget::getKingCaptureMoves(Pos pos1, Pos pos2, int board[8][8], COLOUR player, int countCaptures, std::vector<Pos> destPositions)
{

	Pos pos[4] = { 	{ pos2.x - 1, pos2.y - 1 },
					{ pos2.x + 1, pos2.y - 1 },
					{ pos2.x - 1, pos2.y + 1 },
					{ pos2.x + 1, pos2.y + 1 } };

	int foundNow = 0;
	char tmp =board[pos1.x][pos1.y];
	int foundFront = -1;

	for (int i = 0; i < 4; i++)

		if (isValidPos(pos[i]) && isOpposite(player, board[pos[i].x][pos[i].y])){

			Pos newPiece = getNextDiag(pos2, pos[i]);

		if (isValidPos(newPiece) && board[newPiece.x][newPiece.y] == EMPTY){

			foundNow++;

			tmp = board[pos[i].x][pos[i].y]; //copy board piece to tmp
			board[pos[i].x][pos[i].y] = EMPTY; //now make pos position empty

			vector<Pos> newDests(destPositions.begin(), destPositions.end());
			newDests.push_back(newPiece);


			if (!(isEndOfBoard(newPiece, player)))

				foundFront = getKingCaptureMoves(pos1, newPiece, board, player, countCaptures + 1, newDests);

			if (foundFront == 0) {
				addMove(pos1,newDests,countCaptures+1);
			}

			newDests.clear();
			board[pos[i].x][pos[i].y] = tmp;
		}
	}

	return foundNow;
}



//----------------------------------------------------------------------------------------
// AI (to do)
//-----------------------------------------------------------------------------------------


void  BoardWidget::makeAIMove(int board[8][8], COLOUR player) {

	vector<Movement> moveList = getAllMoves(board, player); //filtered on capture count
	Movement best_movement;

	if(!moveList.empty())
	{
		best_movement=moveList.at(0);
	}

	makeMove(board,best_movement);

}

