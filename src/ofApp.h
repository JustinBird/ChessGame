#pragma once

#include "ofMain.h"
#include "offline_game.h"
#include "online_game.h"
#include "ofxTCPServer.h"
#include "ofxTCPClient.h"
#include "ofxUIUtils.h"

class ofApp : public ofBaseApp{
	const int kSmallSquareSize = 80;

	const int kNotSet = -1;
	const int kPort = 4444;
	const int kMaxCoord = 7;

	//Main menu spacing.
	const int kTitleYOffset = 50;
	const int kButtonXOffset = 160;
	const int kOffButtonYOffset = 280;
	const int kHostButtonYOffset = 400;
	const int kJoinButtonYOffset = 520;

	//Join game spacings.
	const int kTextFieldXOffset = 160;
	const int kTextFieldYOffset = 300;
	const int kTextFieldLen = 320;
	const int kTextFieldHeight = 40;
	const int kTextFieldPadding = 10;
	const int kIPAddressYOffset = 160;

	//Waiting spacings.
	const int kWaitImageYOffset = 180;

	//Winner Spacings
	const int kWinImageXOffset = 160;
	const int kWinImageYOffset = 220;

	//Color definitions.
	const int kDarkSquareColor = 232;
	const int kLightSquareColor = 122;

	//Size of board squares, pixels.
	int square_size_;

	//Possible game states.
	enum gamestate {
		MAINMENU = 0,
		WAITING,
		CONNECTING,
		OFFLINE,
		HOST,
		JOIN
	};
	
	//offline game object.
	offlinegame offg;

	//online game object, defaults to white. Is changed if needed.
	onlinegame ong = onlinegame(WHITE);

	//Server and client objects for online play.
	ofxTCPServer server_;
	ofxTCPClient client_;

	//Stores users previous click.
	int click_;

	//Stores local player's moves for sending to opponent.
	int movefrom;
	int moveto;

	//Stores opponent moves that are sent over the net.
	std::string opponent_move1;
	std::string opponent_move2;

	public:
		gamestate state_;

		//Images
		ofImage mainmenu_;
		ofImage waiting_;
		ofImage joining_;

		//Buttons
		ofxImgButton offline_button_;
		ofxImgButton host_button_;
		ofxImgButton join_button_;

		//Textfield
		ofxTextInputField ipaddress_;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Draw functions
		void drawBoard();
		void drawPieces();
		void drawMainMenu();
		void drawWaiting();
		void drawConnecting();
		void drawWinLose();

		//Button action listeners.
		void ofApp::OffButtonPressed(const pair<bool, int> & state);
		void ofApp::HostButtonPressed(const pair<bool, int> & state);
		void ofApp::JoinButtonPressed(const pair<bool, int> & state);
};
