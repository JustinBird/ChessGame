#include "ofApp.h"
/**
 * Code for button handling and text field taken from ofxUIUtils examples.
 * https://github.com/DomAmato/ofxUIUtils
 */

//--------------------------------------------------------------
void ofApp::setup() {
	state_ = MAINMENU;

	square_size_ = kSmallSquareSize;

	click_ = kNotSet;
	movefrom = kNotSet;
	moveto = kNotSet;

	//Set up text field.
	ipaddress_.setup();
	
	//Loads main menu images
	mainmenu_.load("resources/mainmenu/title.png");
	waiting_.load("resources/waiting/waiting.png");
	joining_.load("resources/joining/joining.png");

	//Loads buttons and creates action listeners.
	offline_button_.setup("resources/mainmenu/button1.png");
	ofAddListener(offline_button_.buttonEvent, this, &ofApp::OffButtonPressed);

	host_button_.setup("resources/mainmenu/button2.png");
	ofAddListener(host_button_.buttonEvent, this, &ofApp::HostButtonPressed);

	join_button_.setup("resources/mainmenu/button3.png");
	ofAddListener(join_button_.buttonEvent, this, &ofApp::JoinButtonPressed);
}

//--------------------------------------------------------------
void ofApp::update(){
	//Checks if server is set up
	if (state_ == CONNECTING) {
		if (client_.isConnected()) {
			std::cout << "Client connected to server" << std::endl;
			ong = onlinegame(BLACK);
			state_ = JOIN;
		}
	}

	//Checks if client has connected to server.
	if (state_ == WAITING) {
		if (server_.isClientConnected(0)) {
			std::cout << "Client connected to server" << std::endl;
			ong = onlinegame(WHITE);
			state_ = HOST;
		}
	}

	//Sends and recieves messages to client.
	if (state_ == HOST) {
		if (server_.isClientConnected(0)) {
			if (movefrom != kNotSet && moveto != kNotSet) {
				server_.send(0, std::to_string(movefrom));
				server_.send(0, std::to_string(moveto));
				std::cout << "Server sent message to client" << std::endl;
				movefrom = kNotSet;
				moveto = kNotSet;
			}

			if (opponent_move1 == "") {
				opponent_move1 = server_.receive(0);
			}

			if (opponent_move1 != "" && opponent_move2 == "") {
				opponent_move2 = server_.receive(0);
			}

			if (opponent_move1 != "" && opponent_move2 != "") {
				std::cout << "server received message from client" << std::endl;
				ong.OpponentMove(std::stoi(opponent_move1), std::stoi(opponent_move2));
				opponent_move1 = "";
				opponent_move2 = "";
			}
		}
		//If client is not connected then return to the main menu.
		else {
			state_ = MAINMENU;
		}
	}

	//Sends and receives messages to the server.
	if (state_ == JOIN) {
		if (client_.isConnected()) {
			if (opponent_move1 == "") {
				opponent_move1 = client_.receive();
			}

			if (opponent_move1 != "" && opponent_move2 == "") {
				opponent_move2 = client_.receive();
			}

			if (opponent_move1 != "" && opponent_move2 != "") {
				std::cout << "server received message from client" << std::endl;
				ong.OpponentMove(std::stoi(opponent_move1), std::stoi(opponent_move2));
				opponent_move1 = "";
				opponent_move2 = "";
			}

			if (movefrom != kNotSet && moveto != kNotSet) {
				client_.send(std::to_string(movefrom));
				client_.send(std::to_string(moveto));
				std::cout << "Client sent message to server" << std::endl;
				movefrom = kNotSet;
				moveto = kNotSet;
			}
		}
		//If client is not connected then return to the main menu.
		else {
			state_ = MAINMENU;
		}
	}
}

//--------------------------------------------------------------

void ofApp::draw(){
	if (state_ == MAINMENU) {
		drawMainMenu();
	}
	else if (state_ == OFFLINE || state_ == HOST || state_ == JOIN) {
		drawBoard();
		drawPieces();
		drawWinLose();
	}
	else if (state_ == CONNECTING) {
		drawConnecting();
	}
	else if (state_ == WAITING) {
		drawWaiting();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//Handles entering the ip address in connecting screen.
	if (key == OF_KEY_RETURN) {
		if (state_ == CONNECTING) {
			client_.setup(ipaddress_.getText(), kPort);
			ipaddress_.clear();
		}
	}
	
	//Returns to the main menu if not there already.
	if (key == 'm') {
		if (state_ != MAINMENU) {
			state_ = MAINMENU;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (state_ == OFFLINE) {
		int x_ = (x - (x % square_size_)) / square_size_;
		int y_ = (y - (y % square_size_)) / square_size_;
		if (click_ == kNotSet) {
			if (offg.get_board().at(ToIndex(x_, y_))) {
				click_ = ToIndex(x_, y_);
				movefrom = ToIndex(x_, y_);
			}
		}
		else {
			offg.MovePiece(click_, ToIndex(x_, y_));
			moveto = ToIndex(x_, y_);
			click_ = kNotSet;
		}
	}
	else if (state_ == HOST || state_ == JOIN) {
		int x_ = (x - (x % square_size_)) / square_size_;
		int y_ = (y - (y % square_size_)) / square_size_;
		if (ong.get_color() == WHITE) {
			if (click_ == kNotSet) {
				if (ong.get_board().at(ToIndex(x_, y_))) {
					click_ = ToIndex(x_, y_);
					movefrom = ToIndex(x_, y_);
				}
			}
			else {
				ong.PlayerMove(click_, ToIndex(x_, y_));
				moveto = ToIndex(x_, y_);
				click_ = kNotSet;
			}
		}
		else {
			if (click_ == kNotSet) {
				if (ong.get_board().at(ToIndex(kMaxCoord - x_, kMaxCoord - y_))) {
					click_ = ToIndex(kMaxCoord - x_, kMaxCoord - y_);
					movefrom = ToIndex(kMaxCoord - x_, kMaxCoord - y_);
				}
			}
			else {
				ong.PlayerMove(click_, ToIndex(kMaxCoord - x_, kMaxCoord - y_));
				moveto = ToIndex(kMaxCoord - x_, kMaxCoord - y_);
				click_ = kNotSet;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

/**
 * Draws checkerboard pattern.
 */
void ofApp::drawBoard() {
	for (int y = 0; y < kBoardLen; y++) {
		for (int x = 0; x < kBoardLen; x++) {
			if ((x + y) % 2 == 0) {
				ofSetColor(kDarkSquareColor);
			}
			else {
				ofSetColor(kLightSquareColor);
			}
			ofDrawRectangle(x * square_size_, y * square_size_, square_size_, square_size_);
		}
	}
}

void ofApp::drawPieces() {
	if (state_ == OFFLINE) {
		std::vector<ChessPiece*> bv = offg.get_board();
		for (int index = 0; index < kBoardSquares; index++) {
			if (bv.at(index)) {
				ofImage piece;
				piece.load(bv.at(index)->GetImagePath());
				piece.draw(ToXCoord(index) * square_size_, ToYCoord(index) * square_size_);
			}
		}
	}
	else if (state_ == HOST || state_ == JOIN) {
		if (ong.get_color() == WHITE) {
			std::vector<ChessPiece*> bv = ong.get_board();
			for (int index = 0; index < kBoardSquares; index++) {
				if (bv.at(index)) {
					ofImage piece;
					piece.load(bv.at(index)->GetImagePath());
					piece.draw(ToXCoord(index) * square_size_, 
						ToYCoord(index) * square_size_);
				}
			}
		}
		else {
			std::vector<ChessPiece*> bv = ong.get_board();
			for (int index = 0; index < kBoardSquares; index++) {
				if (bv.at(index)) {
					ofImage piece;
					piece.load(bv.at(index)->GetImagePath());
					piece.draw((kMaxCoord - ToXCoord(index)) * square_size_,
						(kMaxCoord - ToYCoord(index)) * square_size_);
				}
			}
		}
		
	}
}

/**
 * Draws main menu.
 */
void ofApp::drawMainMenu() {
	drawBoard();

	//Title image.
	mainmenu_.draw(0, kTitleYOffset);

	//draws buttons.
	offline_button_.draw(kButtonXOffset, kOffButtonYOffset);
	host_button_.draw(kButtonXOffset, kHostButtonYOffset);
	join_button_.draw(kButtonXOffset, kJoinButtonYOffset);
}

/**
 * Draws connecting to host screen.
 */
void ofApp::drawConnecting() {
	drawBoard();

	//Text field.
	ipaddress_.setPlaceholderText("Ip address");
	ipaddress_.setBounds(kTextFieldXOffset, 
		kTextFieldYOffset,
		kTextFieldLen, 
		kTextFieldHeight);
	ipaddress_.setVerticalPadding(kTextFieldPadding);
	ipaddress_.draw();

	joining_.draw(0, kIPAddressYOffset);
}

/**
 * Draws waiting for opponent screen.
 */
void ofApp::drawWaiting() {
	drawBoard();
	waiting_.draw(0, kWaitImageYOffset);
}

/**
 * Draws winning and losing screens if the game has been won or lost.
 */
void ofApp::drawWinLose() {
	if (state_ == OFFLINE) {
		if (offg.get_checkmate()) {
			ofImage winner;
			if (offg.get_turn() == WHITE) {
				winner.load("resources/winlose/bwon.png");
			}
			else {
				winner.load("resources/winlose/wwon.png");
			}
			winner.draw(kWinImageXOffset, kWinImageYOffset);
		}
		else if (offg.get_stalemate()) {
			ofImage stalemate;
			stalemate.load("resources/winlose/stalemate.png");
			stalemate.draw(kWinImageXOffset, kWinImageYOffset);
		}
	}

	if (state_ == HOST || state_ == JOIN) {
		if (ong.get_checkmate()) {
			ofImage winner;
			if (ong.get_turn() == ong.get_player()) {
				winner.load("resources/winlose/lost.png");
			}
			else {
				winner.load("resources/winlose/won.png");
			}
			winner.draw(kWinImageXOffset, kWinImageYOffset);
		}
		else if (ong.get_stalemate()) {
			ofImage stalemate;
			stalemate.load("resources/winlose/stalemate.png");
			stalemate.draw(kWinImageXOffset, kWinImageYOffset);
		}
	}
}

/**
 * Called when offline game button is clicked and changes game state.
 */
void ofApp::OffButtonPressed(const pair<bool, int> & state) {
	if (state_ == MAINMENU) {
		state_ = OFFLINE;
		offg = offlinegame();
	}
}

/**
* Called when host game button is clicked and changes game state.
*/
void ofApp::HostButtonPressed(const pair<bool, int> & state) {
	if (state_ == MAINMENU) {
		state_ = WAITING;
		server_.setup(kPort);
	}
}

/**
* Called when join game button is clicked and changes game state.
*/
void ofApp::JoinButtonPressed(const pair<bool, int> & state) {
	if (state_ == MAINMENU) {
		state_ = CONNECTING;
	}
}