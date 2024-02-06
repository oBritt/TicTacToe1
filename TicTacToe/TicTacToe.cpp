#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <unordered_map>
#include <thread> 
#include <chrono> 
#include <conio.h>
#include <fstream>


using namespace std;


// erstelle Class Spiel 
class Game {
public:

	vector<vector<char>> grid;
	vector<vector<int>> grid_for_calculations;
	bool playersTurn;
	char playerSign = 'X';
	bool isLost = false;
	int wins = 0; // nur um zu ärgern 
	int loses = 0;
	int drawes = 0;
	int counter = 0;


	//funktion die abliest die Statistik
	void get_info() {
		ifstream file("data.txt");
		string temp;
		file >> temp;
		loses = stoi(temp);
		file >> temp;
		drawes = stoi(temp);
		file.close();

	}

	//funktion die schreibt die Statistik auf
	void update_info() {
		ofstream file("data.txt");
		string line = to_string(loses) + " " + to_string(drawes);
		file << line;
		file.close();
	}

	Game(bool players = true) {
		//erstelle Spielfeld so soll es am Ende aussehen
		// dies ist alles in vector der vectoren gespeichert (vector ist ein dynamischer array )
		//    |    |   
		//    |    |   
		//    |    |   
		// ————————————
		//    |    |   
		//    |    |   
		//    |    |   
		// ————————————		
		//    |    |   
		//    |    |   
		//    |    | 

		playersTurn = players;
		vector<vector<char>> temp(11, vector<char>(11, ' '));
		grid = temp;
		for (int y = 0; y < 11; y++) {
			for (int x = 0; x < 11; x++) {
				if (x == 3 or x == 7) {
					grid[y][x] = '|';
				}
				if (y == 3 or y == 7) {
					grid[y][x] = '-';
				}
			}
		}
		vector<vector<int>> g(3, vector<int>(3));
		grid_for_calculations = g;

		get_info(); // ablesen von Statistik
	}


	//funktion die prüft ob Bot mit "X" oder "0" spielt
	char get_bots_char() {
		return playerSign == 'X' ? '0' : 'X';
	}

	// delay funktion in milisekunden
	void delay(int time) {
		this_thread::sleep_for(chrono::milliseconds(time));
	}


	// funktion die Spielfeld ausgibt
	void draw() {
		string t = "          ";
		cout << "   wins: " << wins << "   loses: " << loses << "   draws: " << drawes << endl << endl;
		cout << t << "You play as: " << playerSign << endl << endl;
		for (const auto& row : grid) {
			cout << t;
			for (const auto& e : row) {
				cout << e;
			}
			cout << endl;
		}
		cout << endl << "input collomn and row like that '1 3' " << endl;
	}

	// funktion die überprüft ob input korrekt ist
	bool isValid(string cords) {

		if (cords[0] - '0' > 3 or cords[0] - '0' < 1 or cords[2] - '0' > 3 or cords[2] - '0' < 1) {
			return false;
		}

		if (grid_for_calculations[cords[2] - '0' - 1][cords[0] - '0' - 1] != 0) {
			return false;
		}
		grid_for_calculations[cords[2] - '0' - 1][cords[0] - '0' - 1] = 1; // Eingabe stimmt und wird aufgeschrieben
		return true;
	}

	// funktion die zeichnet "X" bzw "0" ein
	void update_gird() {
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				if (grid_for_calculations[y][x] == 1) {
					grid[1 + 4 * y][1 + 4 * x] = playerSign;
				}
				else if (grid_for_calculations[y][x] == -1) {
					grid[1 + 4 * y][1 + 4 * x] = get_bots_char();
				}
				else {
					grid[1 + 4 * y][1 + 4 * x] = ' ';
				}
			}
		}
	}

	// funktion die löscht alte Feld aus der Konslole 
	void delete_old() {
		system("cls");
	}

	// überprüfung von allem
	int find_move(int search) {
		pair<int, int> compare = { -1, -1 };
		pair<int, int> cord;
		// diagonale 
		if (grid_for_calculations[1][1] == search) {
			if (grid_for_calculations[0][0] == search and grid_for_calculations[2][2] == 0) {
				return 9;
			}
			if (grid_for_calculations[2][2] == search and grid_for_calculations[0][0] == 0) {
				return 1;
			}
			if (grid_for_calculations[0][2] == search and grid_for_calculations[2][0] == 0) {
				return 7;
			}
			if (grid_for_calculations[0][2] == 0 and grid_for_calculations[2][0] == search) {
				return 3;
			}
		}
		// zeilen
		for (int y = 0; y < 3; y++) {
			cord = compare;

			int counter = 0;
			for (int x = 0; x < 3; x++) {
				if (grid_for_calculations[y][x] == search) {
					counter++;
				}
				if (grid_for_calculations[y][x] == 0) {
					cord = { y, x };
				}
			}
			if (counter == 2 and cord != compare) {
				return cord.first * 3 + cord.second + 1;
			}
		}
		// spalten 
		for (int x = 0; x < 3; x++) {
			cord = compare;

			int counter = 0;
			for (int y = 0; y < 3; y++) {
				if (grid_for_calculations[y][x] == search) {
					counter++;
				}
				if (grid_for_calculations[y][x] == 0) {
					cord = { y, x };
				}
			}
			if (counter == 2 and cord != compare) {
				return cord.first * 3 + cord.second + 1;
			}
		}
		return 0;
	}

	// die selbe wie find_move zählt aber mögliche Anzahl von 3 in der Linie 
	int find_moveV2(vector<vector<int>> temp_grid, int search = 1) {
		int counter = 0;

		bool zero = false;


		//diagonale
		if (grid_for_calculations[1][1] == search) {
			if (temp_grid[0][0] == search and temp_grid[2][2] == 0) {
				counter++;
			}
			if (temp_grid[2][2] == search and temp_grid[0][0] == 0) {
				counter++;
			}
			if (temp_grid[0][2] == search and temp_grid[2][0] == 0) {
				counter++;
			}
			if (temp_grid[0][2] == 0 and temp_grid[2][0] == search) {
				counter++;
			}
		}
		// zeilen
		for (int y = 0; y < 3; y++) {
			bool zero = false;

			int counter1 = 0;
			for (int x = 0; x < 3; x++) {
				if (temp_grid[y][x] == search) {
					counter1++;
				}
				if (temp_grid[y][x] == 0) {
					zero = true;
				}
			}
			if (counter1 == 2 and zero) {
				counter++;
			}
		}

		// spalten 
		for (int x = 0; x < 3; x++) {
			zero = false;

			int counter2 = 0;
			for (int y = 0; y < 3; y++) {
				if (temp_grid[y][x] == search) {
					counter2++;
				}
				if (temp_grid[y][x] == 0) {
					zero = true;
				}
			}
			if (counter2 == 2 and zero) {
				counter++;
			}
		}
		/*char t;
		cout << counter<<endl;
		for (const auto& e : temp_grid) {
			for (const auto& i : e) {
				cout << i << " ";
			}
			cout << endl;
		}
		cin >> t;
		*/
		return counter;

	}

	// funktion die danach sucht ob ein Zug exestiert der so tut dass spieler mit 100% gewinnt durch Gabel
	int test_all() {
		for (int i = 0; i < 9; i++) { // hier versuche ich alle mögliche Zuge von Spieler zu kalkulieren
			if (grid_for_calculations[i / 3][i % 3] != 0) {
				continue;
			}
			vector<vector<int>> temp_grid = grid_for_calculations;
			temp_grid[i / 3][i % 3] = 1;
			if (find_moveV2(temp_grid) >= 2) {
				return i;
			}

		}
		return 0;
	}


	// richtiger Eck wählen
	int find_diagonal() {
		if (!grid_for_calculations[0][0] and (grid_for_calculations[0][1] or grid_for_calculations[1][0])) {
			return 1;
		}
		if (!grid_for_calculations[0][2] and (grid_for_calculations[0][1] or grid_for_calculations[2][1])) {
			return 3;
		}
		if (!grid_for_calculations[2][0] and (grid_for_calculations[1][0] or grid_for_calculations[2][1])) {
			return 7;
		}
		if (!grid_for_calculations[2][2] and (grid_for_calculations[2][1] or grid_for_calculations[1][2])) {
			return 9;
		}
		return 0;
	}



	// den Zug zu finden der den Gegner blockiert und Gefährden bildet(Gegner hat eck)
	int find_not_easy_move() {
		if (grid_for_calculations[0][0] == 1) { //oben links
			if (!grid_for_calculations[0][1] and !grid_for_calculations[2][1]) {
				return 2;
			}
			if (!grid_for_calculations[1][0] and !grid_for_calculations[1][2]) {
				return 4;
			}
		}
		if (grid_for_calculations[2][0] == 1) { // unten links
			if (!grid_for_calculations[0][1] and !grid_for_calculations[2][1]) {
				return 8;
			}
			if (!grid_for_calculations[1][0] and !grid_for_calculations[1][2]) {
				return 4;
			}
		}
		if (grid_for_calculations[0][2] == 1) { // unten rechte
			if (!grid_for_calculations[0][1] and !grid_for_calculations[2][1]) {
				return 2;
			}
			if (!grid_for_calculations[1][0] and !grid_for_calculations[1][2]) {
				return 6;
			}
		}
		if (grid_for_calculations[2][2] == 1) { // unten rechts
			if (!grid_for_calculations[0][1] and !grid_for_calculations[2][1]) {
				return 8;
			}
			if (!grid_for_calculations[1][0] and !grid_for_calculations[1][2]) {
				return 6;
			}
		}

		return 0;

	}

	int otherMoves() {
		//ecken
		if (grid_for_calculations[0][0] == 0) {
			return 1;
		}
		if (grid_for_calculations[2][2] == 0) {
			return 9;
		}
		if (grid_for_calculations[2][0] == 0) {
			return 7;
		}
		if (grid_for_calculations[0][2] == 0) {
			return 3;
		}
		//rest
		if (grid_for_calculations[1][0] == 0) {
			return 4;
		}
		if (grid_for_calculations[0][1] == 0) {
			return 2;
		}
		if (grid_for_calculations[2][1] == 0) {
			return 8;
		}
		if (grid_for_calculations[1][2] == 0) {
			return 6;
		}
	}

	// bot kalkuliert besten zug für ihn
	void makeprediction() {
		int t;

		// überprüfung von gewinnen
		if (find_move(-1)) {
			t = find_move(-1) - 1;
			grid_for_calculations[t / 3][t % 3] = -1;
			isLost = true;
			return;
		}

		// überprüfung von gefärden
		if (find_move(1)) {
			t = find_move(1) - 1;
			grid_for_calculations[t / 3][t % 3] = -1;
			return;
		}

		// ob Zentrum leer ist 
		if (grid_for_calculations[1][1] == 0) {
			grid_for_calculations[1][1] = -1;
			return;
		}

		// entgegengesetzte ecke 
		if (((grid_for_calculations[0][0] == 1 and grid_for_calculations[2][2] == 1) or (grid_for_calculations[0][2] == 1 and grid_for_calculations[2][0] == 1)) and !grid_for_calculations[0][1]) {
			grid_for_calculations[0][1] = -1;
			return;
		}

		// Suche nach "Gabel" 
		if (test_all()) {
			t = test_all();
			grid_for_calculations[t / 3][t % 3] = -1;
			return;
		}

		// priorität neben Gegner horizontale
		if (find_not_easy_move()) {
			t = find_not_easy_move() - 1;
			grid_for_calculations[t / 3][t % 3] = -1;
			return;
		}
		if (find_diagonal()) {
			t = find_diagonal() - 1;
			grid_for_calculations[t / 3][t % 3] = -1;
			return;
		}

		t = otherMoves() - 1;
		grid_for_calculations[t / 3][t % 3] = -1;

	}

	//funktion in der gespielt wird 
	void play() {
		for (;true;) {
			update_gird();
			delay(200);
			delete_old();
			draw();
			//Zug von spiller
			if (playersTurn) {
				string cords;
				getline(cin, cords);
				cords.resize(3);

				if (!isValid(cords)) {
					delete_old();
					continue;
				}

				playersTurn = false;

			}
			// Zug von Maschine
			else {
				makeprediction();
				playersTurn = true;
				delay(200);
			}
			counter++;

			//ausgabe wenn verloren / unentschieden
			if (counter == 9 or isLost) {
				delete_old();
				update_gird();
				draw();
				if (isLost == 1) {
					loses++;

					cout << "You have lost. Press any button to continue";
					while (true) {
						if (_kbhit()) { // knopf ist gedruckt
							break;
						}
					}
					// aufschreiben von statistik änderung
					update_info();
				}
				else {
					drawes++;
					cout << "Good game, but still only draw. Press any button to continue";
					while (true) {
						if (_kbhit()) { // knopf ist gedruckt
							break;
						}
					}
					// aufschreiben von statistik änderung
					update_info();
				}
				// reset
				counter = 0;
				isLost = false;
				for (int y = 0; y < 3; y++) {
					for (int x = 0; x < 3; x++) {
						grid_for_calculations[y][x] = 0;
					}
				}
				//change player and machine 
				if (playerSign == 'X') {
					playerSign = '0';
					playersTurn = false;
				}
				else {
					playerSign = 'X';
					playersTurn = true;
				}
				// aufschreiben von statistik änderung
				update_info();
			}
		}

	}


};


int main() {
	Game g(true);
	g.play();

}