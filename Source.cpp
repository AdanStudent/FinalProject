//Adan Benitez
//C++ Final Project
//December 10, 2017

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

class Player {
public:
	unsigned int score = 0;
};

#pragma region Die Class

//Die Class
class Die {
public:
	Die(string color); //constructor
	string virtual DieColor() const;
	vector <string>& GetDieFaces();
	void DisplayVector();

protected:
	vector <string> dieFaces;
	string color;

};

Die::Die(string color) :
	color(color)
{}

string Die::DieColor() const
{
	return color;
}

vector<string>& Die::GetDieFaces() {

	return dieFaces;
}

void Die::DisplayVector() 
{
	for (unsigned int i = 0; i < dieFaces.size(); i++)
	{
		cout << "Element [" << i << "] = " << dieFaces[i] << endl;
	}
}

//GreenDie child/sub class
class GreenDie : public Die {

public:
	GreenDie(string color = "green");//constructor
	string virtual DieColor() const;
};

GreenDie::GreenDie(string color) :Die(color) {}

string GreenDie::DieColor() const {

	return color;
}

//YellowDie child/sub class
class YellowDie : public Die {

public:
	YellowDie(string color = "yellow");//constructor
	string virtual DieColor() const;
};

YellowDie::YellowDie(string color) :Die(color) {}

string YellowDie::DieColor() const {

	return color;
}

//RedDie child/sub class
class RedDie : public Die {

public:
	RedDie(string color = "red");//constructor
	string virtual DieColor() const;
};

RedDie::RedDie(string color) :Die(color) {}

string RedDie::DieColor() const {

	return color;
}
#pragma endregion

string die[] = { "bbffbs", "fsbbsf", "ssffsb" };
int brains, feet, shots, currentScore = 0;
const int PLAYERDICEMAX = 3;
const int WINNINGSCORE = 13;
int currentPlayer, prevPlayer = 0;
int nextPlayer = 0;


#pragma region Protypes
void Instructions();

//modifies Die
void FillDieCup(vector <Die>& gCup, vector <Die> &pCup);
int HowManyGreenDie(vector <Die> &pCup);
int HowManyYellowDie(vector <Die> &pCup);
int HowManyRedDie(vector <Die> &pCup);
bool isDiceCupEmpty(vector <Die> &dCup, int addedDice);
int HowManyDiceNeeded(vector <Die> &pCup);
void FillPlayerCup(vector <Die> &dCup, vector <Die> &pCup, int addedDice);
string RollDice(vector <Die> &pCup);
void RemoveDie(vector<Die>& pCup, string result);
void ClearPlayerCup(vector <Die> &pCup);

//Check Result after roll
bool CheckDieResult(char dieFace);
void PrintResults(string results);

//Modifies Players
void AddPlayers(vector <Player> &Players, int numOfPlayers);
void UpdatePlayers(vector<Player> &Players, vector <Die> &pCup);
void NextPlayerTurn(vector <Player> &Players, vector <Die> &pCup);
void HowManyShots(vector <Player> &Players, vector <Die> &pCup);
void UpdateScore(vector <Player> &Players);
bool CheckForWinner(vector<Player> *ptrPlayers);
#pragma endregion



int main() {

	srand(unsigned(time(0)));
	vector <Die> gameCup;
	vector <Die> playerCup;

	vector <Player> Players; //can have any number of players
	vector <Player> *ptrPlayer = &Players; //this will point to the current player

	string gameLoop = "y";

	while (gameLoop == "y")
	{
		Instructions();
		int input;
		std::cin >> input;
		AddPlayers(Players, input);

		FillPlayerCup(gameCup, playerCup, PLAYERDICEMAX);
		PrintResults(RollDice(playerCup));
		HowManyShots(Players, playerCup);

		bool won = false;
		while (!won)
		{
			printf("It is the %d players' turn with a Score of: %d\n", currentPlayer + 1, Players[currentPlayer].score);
			printf("You have %d green die, %d yellow die, and %d red die.\n", HowManyGreenDie(playerCup), HowManyYellowDie(playerCup), HowManyRedDie(playerCup));
			std::cout << "1. Roll again" << endl;
			std::cout << "2. Pass to Next Player" << endl;
			std::cin >> input;
			system("cls");

			switch (input)
			{
			case 1:
				FillPlayerCup(gameCup, playerCup, HowManyDiceNeeded(playerCup));
				PrintResults(RollDice(playerCup));
				HowManyShots(Players, playerCup);
				break;
			case 2:
				UpdatePlayers(Players, playerCup);
				break;
			}

			won = CheckForWinner(ptrPlayer);

		}

		std::cout << "Would you like to play again? y or n" << endl;
		std::cin >> gameLoop;

	}

	return 0;
}

void Instructions()
{
	std::cout << "On your turn you will roll 3 dice." << endl;
	std::cout << "If you get brains those will count towards points." << endl;
	std::cout << "If you get feet they will allow you to re-roll those dice." << endl;
	std::cout << "During your turn if you get 3 or more shots your turn ends \nand all the brains you rolled are lost from that turn" << endl;
	std::cout << "To Win you need 13 brains total." << endl;
	std::cout << "Please enter how many players will be playing?" << endl;
}

//Die Fuctions

void FillDieCup(vector <Die> &gCup, vector <Die> &pCup)
{
	//creating each of the die that are needed to go into the gCup
	GreenDie gd;
	YellowDie yd;
	RedDie rd;

	//setting what each of their respective die faces should be before adding them to the gCup
	gd.GetDieFaces().push_back(die[0]);
	yd.GetDieFaces().push_back(die[1]);
	rd.GetDieFaces().push_back(die[2]);

	for (int i = 0; i < (6 - HowManyGreenDie(pCup)); i++)
	{
		gCup.push_back(gd);
	}

	for (int i = 0; i < (4 - HowManyYellowDie(pCup)); i++)
	{
		gCup.push_back(yd);
	}

	for (int i = 0; i < (3 - HowManyGreenDie(pCup)); i++)
	{
		gCup.push_back(rd);
	}

	random_shuffle(gCup.begin(), gCup.end());
}

int HowManyGreenDie(vector <Die> &pCup)
{
	int counter = 0;
	for each (Die s in pCup)
	{
		if (s.DieColor() == "green") {
			counter++;
		}
	}

	return counter;
}

int HowManyYellowDie(vector <Die> &pCup)
{
	int counter = 0;
	for each (Die s in pCup)
	{
		if (s.DieColor() == "yellow") {
			counter++;
		}
	}
	return counter;
}

int HowManyRedDie(vector <Die> &pCup)
{
	int counter = 0;
	for each (Die s in pCup)
	{
		if (s.DieColor() == "red") {
			counter++;
		}
	}
	return counter;
}

bool isDiceCupEmpty(vector <Die> &dCup, int addedDice)
{
	if (dCup.size() < addedDice)
	{
		return true;
	}

	return false;
}

int HowManyDiceNeeded(vector <Die> &pCup)
{
	return (PLAYERDICEMAX - pCup.size());
}

void FillPlayerCup(vector <Die> &dCup, vector <Die> &pCup, int addedDice)
{
	if (isDiceCupEmpty(dCup, addedDice))
	{
		FillDieCup(dCup, pCup);
	}

	for (int i = 0; i < addedDice; i++)
	{
		pCup.push_back(dCup[0]);
		dCup.erase(dCup.begin());

	}
}//end of FillPlayerCup

string RollDice(vector <Die> &pCup)
{
	string roll;
	feet = 0;

	for (int i = 0; i < PLAYERDICEMAX; i++)
	{
		roll += pCup[i].GetDieFaces()[0][rand()%6];
	}

	RemoveDie(pCup, roll);
	return roll;

}//end of RollDice

void RemoveDie(vector<Die>& pCup, string result)
{
	for (int i = 0; i < PLAYERDICEMAX; i++)
	{
		if (CheckDieResult(result[i]))
		{
			switch (i)
			{
			case 0:
				pCup.erase(pCup.begin());
				break;
			case 1:
				if (pCup.size() < 3)
				{
					pCup.erase(pCup.begin());
				}
				else
				{
					pCup.erase(pCup.begin() + 1);
				}

				break;
			case 2:
				if (pCup.size() > 1)
				{
					pCup.erase(pCup.begin() + 1);
				}
				else
				{
					pCup.erase(pCup.begin());
				}

				break;

			default:
				break;
			}

		}
	}

}//end of RemoveDie

bool CheckDieResult(char dieFace)
{
	if (dieFace == 'f') { // if the dieFace is s for shots or b for brains return true
		return false;
	}
	else
	{
		return true;
	}

}//end of CheckDieResult

void PrintResults(string results)
{
	for each (char c in results)
	{
		switch (c)
		{
		case 's':
			shots++;
			break;
		case 'f':
			feet++;
			break;
		case 'b':
			brains++;
			break;
		}
	}

	printf("Roll's results: shots: %d, feet: %d, brains: %d\n", shots, feet, brains);
}

void HowManyShots(vector <Player> &Players, vector <Die> &pCup)
{
	if (shots >= 3)
	{
		UpdatePlayers(Players, pCup);
	}

}

void ClearPlayerCup(vector <Die> &pCup)
{
	pCup.clear();
}

//Players Functions

void AddPlayers(vector <Player> &Players, int numOfPlayers)
{
	Player p;

	for (int i = 0; i < numOfPlayers; i++)
	{
		Players.push_back(p);
	}
	nextPlayer++;
}

void UpdatePlayers(vector<Player> &Players, vector <Die> &pCup)
{
	prevPlayer = currentPlayer;
	currentPlayer = nextPlayer;
	if (nextPlayer == Players.size() - 1)
	{
		nextPlayer = 0;
	}
	else
	{
		nextPlayer++;
	}
	NextPlayerTurn(Players, pCup);
}

void NextPlayerTurn(vector <Player> &Players, vector <Die> &pCup)
{
	if (shots <= 2)
	{
		UpdateScore(Players);
	}

	system("cls");
	brains = 0;
	shots = 0;
	feet = 0;

	ClearPlayerCup(pCup);
}

void UpdateScore(vector <Player> &Players)
{
	Players[prevPlayer].score += brains;
}

bool CheckForWinner(vector<Player> *ptrPlayers)
{
	vector <Player> &players = *ptrPlayers;
	int counter = 0;
	for each (Player p in players)
	{
		if (p.score >= 13)
		{
			printf("The Winner is %d \n", counter + 1);
			return true;
		}
		counter++;
	}

	return false;
}
