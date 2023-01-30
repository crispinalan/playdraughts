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

	numBMAN=12;
    numWMAN=12;
	numBKING=0;
    numWKING=0;
	searchDepth=8; //search depth

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
	return;
	}

	else {
		x2=xpos;
		y2=ypos;

		Move m_tmp {};
		m_tmp.start.x=x1;
		m_tmp.start.y=y1;
		m_tmp.end.x=x2;
		m_tmp.end.y=y2;

		QString x1Str =	QString::number(x1);
		QString y1Str =	QString::number(y1);
		QString x2Str =	QString::number(x2);
		QString y2Str =	QString::number(y2);


		gameInfo.clear();
		gameInfo.append(" Player move:  (");
		gameInfo.append(x1Str);
		gameInfo.append(",");
		gameInfo.append(y1Str);
		gameInfo.append(")  -> (");
		gameInfo.append(x2Str);
		gameInfo.append(",");
		gameInfo.append(y2Str);
		gameInfo.append(") ");

		bool status =makeMove(theBoard, m_tmp, WHITE);


		if(status ==false) //check move status
		{
			gameInfo.append(" Player move not possible: check for jumps");
			firstClick=true;
			playerFinished=false;
			emit outgoingText(gameInfo);
			return;
		}


		displayBoard(theBoard);

		//check for win
		if(checkForWin(theBoard,WHITE)) {
			//qDebug()<<"White Wins\n";
			gameInfo.clear();
			gameInfo ="White wins. ";
			emit outgoingText(gameInfo);
			//messagebox win
			QString text = "White wins.";
			QMessageBox::information(this, "Draughts", text);
			firstClick=true;
			currentPlayer=WHITE;
			return;
		}

		//check for another jump

		if( isJump(theBoard, m_tmp.start.x, m_tmp.start.y, m_tmp.end.x, m_tmp.end.y) )
		{
			if( canJump(theBoard, m_tmp.end.x, m_tmp.end.y, WHITE) )
			{
				gameInfo.append("Player can jump again");
				firstClick=true;
		        playerFinished=false;
		        emit outgoingText(gameInfo);
			}
			else
			{
				//can't jump again so AI to play
				gameInfo.append("AI player to move");
				firstClick=true;
		        playerFinished=true;
		        emit outgoingText(gameInfo);
				makeAIMove();

			}
		}
		else
		{
			gameInfo.append("AI player to move");
			firstClick=true;
			playerFinished=true;
			emit outgoingText(gameInfo);
			makeAIMove();
		}



		firstClick=true;
		playerFinished=true;
		emit outgoingText(gameInfo);

	} //else first click


}


void BoardWidget::makeAIMove(){

	Move mov = minimaxStart(theBoard, BLACK, true);

	//qDebug() <<"minimax returned AI move  ..\n";
	//qDebug()<<"("<<mov.start.x<<","<<mov.start.y<<") -> ("<<mov.end.x<<","<<mov.end.y<<") ";


	QString x1Str =	QString::number(mov.start.x);
	QString y1Str =	QString::number(mov.start.y);
	QString x2Str =	QString::number(mov.end.x);
	QString y2Str =	QString::number(mov.end.y);

	gameInfo.clear();
	gameInfo.append(" AI move:  (");
	gameInfo.append(x1Str);
	gameInfo.append(",");
	gameInfo.append(y1Str);
	gameInfo.append(")  -> (");
	gameInfo.append(x2Str);
	gameInfo.append(",");
	gameInfo.append(y2Str);
	gameInfo.append(") ");

    bool status =makeMove(theBoard, mov, BLACK);
	//qDebug()<<"AI move status = "<<status;

	if(status ==false) {
		qDebug()<<"makeAIMove failed status: "<<status;
	}

	displayBoard(theBoard);

	//check for another jump

	if( isJump(theBoard, mov.start.x, mov.start.y, mov.end.x, mov.end.y) )
	{

		if( canJump(theBoard, mov.end.x, mov.end.y, BLACK) )
		{
			gameInfo.append("AI can jump again");
			//qDebug()<<"AI can jump again\n";
			// recurse
			makeAIMove();
		}
		else
		{
			//can't jump again so white to play
			gameInfo.append("White player to move");
		}
	}
	else
	{
		gameInfo.append("White player to move");
	}

	//check for win
	if(checkForWin(theBoard, BLACK)) {
		//qDebug()<<"White Wins\n";
		gameInfo.clear();
		gameInfo ="Black wins. ";
		emit outgoingText(gameInfo);
		//messagebox win
		QString text = "Black wins.";
		QMessageBox::information(this, "Draughts", text);
		firstClick=true;
		currentPlayer=WHITE;
		return;
	}

	emit outgoingText(gameInfo);



}

//-------------------------------------------------------------------------------
// Helpers
//-------------------------------------------------------------------------------


Pos BoardWidget::getMidSquare(Move m) {
    Pos middle = {(m.start.x + m.end.x) / 2, (m.start.y + m.end.y) / 2};
    return middle;
}

bool BoardWidget::isOpponentPiece(COLOUR player, const int piece) {

	bool result=false;

	if (player == BLACK) {
		if (piece == WMAN || piece == WKING) result =true;
	}

	if (player == WHITE) {
		if (piece == BMAN || piece == BKING) result =true;
	}

	return result;
}

bool BoardWidget::isOwnPiece(int board[8][8], int x, int y, COLOUR player) {

	bool result =false;
	const int  piece = board[x][y];

	if (player == BLACK) {
		if (piece == BMAN || piece == BKING) result =true;
	}

	if (player == WHITE) {
		if (piece == WMAN || piece == WKING) result =true;
	}

	return result;
}


//-------------------------------------------------------------------------------
// Jumps
//-------------------------------------------------------------------------------

void BoardWidget::getValidJumpMoves(int board[8][8], int x, int y, COLOUR side, vector<Move>& moves)
{

	//single jump moves

	Pos start{x,y};
	vector<Pos> capture_pos;

	if (side == WHITE && board[x][y] == WMAN) {

		capture_pos.push_back({x - 2, y - 2});
        capture_pos.push_back({x + 2, y - 2});

	}

	else if (side == BLACK && board[x][y] == BMAN) {

		capture_pos.push_back({x - 2, y + 2});
        capture_pos.push_back({x + 2, y + 2});
    }

    else if (board[x][y] == BKING || board[x][y] == WKING) {
        capture_pos.push_back({x + 2, y + 2});
        capture_pos.push_back({x + 2, y - 2});
        capture_pos.push_back({x - 2, y + 2});
        capture_pos.push_back({x - 2, y - 2});
    }

	 int n = capture_pos.size();
	 for (int i = 0; i < n; i++) {
		 Pos temp = capture_pos[i];
		 Move m = {start, temp};
		 Pos mid = getMidSquare(m);

		 if (temp.x>= 0 && temp.x < 8 && temp.y >= 0 && temp.y < 8

			 && board[temp.x][temp.y] == EMPTY

			 && isOpponentPiece(side, board[mid.x][mid.y]))
			 {
				 moves.push_back(m);

			 }
	 }

}

bool BoardWidget::isJump(int board[8][8], int x1, int y1, int x2, int y2)
{

	if (qAbs(x1 - x2) == 2 && qAbs(x1 - x2) == 2 ) //jump check
    {
		return true;
	}

	return false;
}

bool BoardWidget::canJump(int board[8][8], int x, int y, COLOUR side)
{
	bool jump =false;

	Pos start{x,y};
	vector<Pos> capture_pos;

	if (side == WHITE && board[x][y] == WMAN) {

		capture_pos.push_back({x - 2, y - 2});
        capture_pos.push_back({x + 2, y - 2});

	}

	else if (side == BLACK && board[x][y] == BMAN) {

		capture_pos.push_back({x - 2, y + 2});
        capture_pos.push_back({x + 2, y + 2});
    }

    else if (board[x][y] == BKING || board[x][y] == WKING) {
        capture_pos.push_back({x + 2, y + 2});
        capture_pos.push_back({x + 2, y - 2});
        capture_pos.push_back({x - 2, y + 2});
        capture_pos.push_back({x - 2, y - 2});
    }

	 int n = capture_pos.size();
	 for (int i = 0; i < n; i++) {
		 Pos temp = capture_pos[i];
		 Move m = {start, temp};
		 Pos mid = getMidSquare(m);

		 if (temp.x>= 0 && temp.x < 8 && temp.y >= 0 && temp.y < 8

			 && board[temp.x][temp.y] == EMPTY

			 && isOpponentPiece(side, board[mid.x][mid.y]))
			 {
				jump=true;

			 }
	 }

	return jump;
}

//-------------------------------------------------------------------------------
// Moves
//-------------------------------------------------------------------------------

void BoardWidget::getAllMoves(int board[8][8], COLOUR side, vector<Move>& moves) {


	int pawn=EMPTY;
	int king=EMPTY;

	if(side == BLACK) {

		pawn=BMAN;
		king=BKING;
	}

	if(side == WHITE) {

		pawn=WMAN;
		king=WKING;
	}

	for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            const int  piece = board[x][y];
            if (piece == pawn|| piece == king) {
                // genValidMoves(i, j, side, moves);
                getValidJumpMoves(board, x, y, side, moves);
            }
        }
    }
    if (moves.empty()) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                const int  piece = board[x][y];
                if (piece == pawn || piece == king) {
                    // genValidMoves(i, j, side, moves);
                    getValidMoves(board, x, y, moves);
                }
            }
        }
    }
}


bool BoardWidget::movesEqual(Move one, Move two) {

	bool result=false;

	if(one.start.x==two.start.x
		&& one.start.y ==two.start.y
		&& one.end.x ==two.end.x
		&& one.end.y ==two.end.y) {

		//qDebug()<<"moves equal\n";

		result =true;
	}

   return result;
}

void BoardWidget::getValidMoves(int board[8][8], int x, int y, vector<Move> &moves) {

	const int  piece = board[x][y];


	if (piece == EMPTY) {
        cout << "EMPTY PIECE\n";
        return;
    }

    //man moves
	if (piece == WMAN || piece == BMAN) {

		//int xchange =0;
		int ychange=0;

		if (piece==WMAN) ychange = -1; //WMAN going up board
		if (piece ==BMAN) ychange= +1; //BMAN going down board

		int ynew = y+ychange;


		if (ynew >= 0 && ynew < 8) {
            int xnew = x + 1;
            if (xnew < 8 && board[xnew][ynew] == EMPTY) {
                moves.push_back({x, y, xnew, ynew});
            }
            xnew = x - 1;
            if (xnew >= 0 && board[xnew][ynew] == EMPTY) {
                moves.push_back({x, y, xnew, ynew});
            }
        }


	//king move
    } else {
        int ynew = y + 1;
        if (ynew < 8) {
            int xnew = x + 1;
            if (xnew < 8 && board[xnew][ynew] == EMPTY) {
                moves.push_back({x, y, xnew, ynew});
            }
            xnew = x - 1;
            if (xnew >= 0 && board[xnew][ynew] == EMPTY) {
                moves.push_back({x, y, xnew, ynew});
            }
        }
        ynew= y - 1;
        if (ynew >= 0) {
            int xnew = x + 1;
            if (xnew < 8 && board[xnew][ynew] == EMPTY) {
                moves.push_back({x, y, xnew, ynew});
            }
            xnew = x - 1;
            if (xnew >= 0 && board[xnew][ynew] == EMPTY) {
                moves.push_back({x, y, xnew, ynew});
            }
        }
    }
}

//-------------------------------------------------------------------------------
// Make move
//-------------------------------------------------------------------------------

bool BoardWidget::makeMove(int board[8][8], Move move, COLOUR side) {

    Pos start = move.start;
    int startx = start.x;
    int starty = start.y;

	Pos end = move.end;
    int endx = end.x;
    int endy = end.y;

    const int current_piece = board[startx][starty];

    if (!isOwnPiece(board, startx, starty, side) || board[startx][starty] == EMPTY) {
        return false;
    }

    vector<Move> possible_moves;

	getAllMoves(board, side, possible_moves);

	bool found = false;

    int n = possible_moves.size();

	for (int i = 0; i < n && !found; i++) {

        found = movesEqual(possible_moves[i], move);
    }

    if (found) {
		//qDebug()<<"move found ";
        //bool jump = false;

		if (starty + 1 == endy || starty - 1 == endy) { //pawn move

			//qDebug()<<"moving pawn ";

			board[startx][starty] = EMPTY;
            board[endx][endy] = current_piece;
        }

        else {
            //jump = true;
			//qDebug()<<"jump move ";

			board[startx][starty] = EMPTY;

			board[endx][endy] = current_piece;

			Pos mid_sqr = getMidSquare(move);

            const int midpiece =  board[mid_sqr.x][mid_sqr.y];


            if (midpiece == BMAN) {
                numBMAN--;
            } else if (midpiece == WMAN) {
                numWMAN--;
            } else if (midpiece == BKING) {
                numBMAN--;
                numBKING--;
            } else if (midpiece == WKING) {
                numWMAN--;
                numWKING--;
            }

            board[mid_sqr.x][mid_sqr.y] = EMPTY;
        }


        //check for kings
        if (endy == 0 && side == WHITE) {
            board[endx][endy] = WKING;
            numWKING++;
        }

        else if (endy == 7 && side == BLACK) {
            board[endx][endy] = BKING;
            numBKING++;
        }
        return true;
    }
    else {
        return false;
    }
}


//--------------------------------------------------------------------------
// AI Minimax
//--------------------------------------------------------------------------

int BoardWidget::getScore(int board[8][8], COLOUR player){

	const int KINGWEIGHT=4;
	const int PAWNWEIGHT=1;

	int score =0;

	if(player==BLACK) {

		score =(KINGWEIGHT*numBKING + PAWNWEIGHT*numBMAN) -(KINGWEIGHT*numWKING + PAWNWEIGHT*numWMAN);
		return score;
	}

	score =(KINGWEIGHT*numWKING + PAWNWEIGHT*numWMAN) -(KINGWEIGHT*numBKING + PAWNWEIGHT*numBMAN);

	return score;

}


void BoardWidget::cloneBoard(int board1[8][8], int board2[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board2[i][j] = board1[i][j];
}


Move BoardWidget::minimaxStart(int board[8][8], COLOUR player, bool maximizing)
{
	int alpha = -1000, beta = 1000;

	vector<Move> possible_moves;

	getAllMoves(board, player, possible_moves);

	vector<int> scores;

	if (possible_moves.empty()) {
		qDebug()<<"No possible moves";
		return {-1};
	}

	int tempBoard[8][8];
	int num_moves = possible_moves.size();

	for (int i = 0; i < num_moves; i++) {

		cloneBoard(board,tempBoard);
		makeMove(tempBoard, possible_moves[i], player);

// 		bool status = makeMove(tempBoard, possible_moves[i], player);
// 		if(status ==false) {
// 			qDebug()<<"Minimax start: makeMove failed status: "<<status;
// 		}


		int score =minimax(tempBoard, player, !maximizing, searchDepth-1, alpha, beta);
		scores.push_back(score);

	}

	int max_score=-1000;
	int num_scores=scores.size();


	for (int i = num_scores - 1; i >= 0; i--) {
		if (scores[i] >= max_score) {
			max_score = scores[i];
		}
	}

	for (int i = 0; i < num_scores; i++) {

		if (scores[i] < max_score) {
			scores.erase(scores.begin() + i);
			possible_moves.erase(possible_moves.begin() + i);
			i--;
			num_scores--;
		}
	}

	num_moves = possible_moves.size();
	Move final_move = possible_moves[rand() % num_moves];
	return final_move;
}


//Side = COLOUR
int BoardWidget::minimax(int board[8][8], COLOUR player, bool maximizing, int searchDepth, int alpha, int beta) {

	if (searchDepth == 0) {
		return getScore(board, player);
	}

	vector<Move> possible_moves;

	getAllMoves(board, player, possible_moves);

	int n = possible_moves.size();
	int initial = 0;
	//Board tempBoard;
	int tempBoard[8][8];
	if (maximizing) {
		for (int i = 0; i < n; i++) {
			cloneBoard(board,tempBoard);

			makeMove(tempBoard, possible_moves[i], player);

// 			bool status = makeMove(tempBoard, possible_moves[i], player);
// 			if(status ==false) {
// 			qDebug()<<"Minimax maximising: makeMove failed status: "<<status;
// 			}


			//switch player
			if (player == BLACK) player=WHITE;
			else if(player==WHITE) player=BLACK;

			int result = minimax(tempBoard, player, !maximizing, searchDepth-1, alpha, beta);
			initial = max(result, initial);
			alpha = max(initial, alpha);

			if (alpha >= beta) {
				break;
			}
		}
	} else {
		initial = 1000;
		for (int i = 0; i < n; i++) {

			cloneBoard(board,tempBoard);

			makeMove(tempBoard, possible_moves[i], player);

// 			bool status = makeMove(tempBoard, possible_moves[i], player);
// 			if(status ==false) {
// 			qDebug()<<"Minimax minimising: makeMove failed status: "<<status;
// 			}

			//switch player
			if (player == BLACK) player=WHITE;
			else if(player==WHITE) player=BLACK;

			int result = minimax(tempBoard, player, !maximizing, searchDepth - 1, alpha, beta);

			initial = min(result, initial);
			beta = min(beta, initial);

			if (alpha >= beta) {
				break;
			}
		}
	}
	return initial;
}



//--------------------------------------------------------------------------
// Check for win
//--------------------------------------------------------------------------


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
    if(blackNumber==0) return true;	}

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



