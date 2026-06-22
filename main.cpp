#include <iostream>
#include <vector>
#include <string>


using namespace std;

struct Position    
{
	int r;  
	int c;

	Position()  // needed this to get it to compile
	{
		r = 0;
		c = 0;
	}

	Position(int row, int col)
	{
		r = row;
		c = col;
	}

	bool operator==(const Position other)
	{
		if (r == other.r and c == other.c)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void print()
	{
		cout << "row: " << r << " col: " << c << endl;
	}
};



class Level
{
private: 
	string name;
	vector<vector<char>> board;
	Position goalPos;
	Position spyPos;
	vector<Position> guardList;


public:
	Level()
	{
		name = "Error";
	}

	Level(int difficulty)
	{
		if (difficulty == 1)
		{
			name = "A New Enemy";
			board = {
				{'#', '#', '#', '#', '#', '#', '#', '#'},
				{'#', ' ', ' ', ' ', ' ', 'v', ' ', '#'},
				{'#', '>', ' ', ' ', '#', ' ', ' ', '#'},
				{'#', ' ', ' ', ' ', '#', ' ', ' ', '#'},
				{'#', '@', ' ', ' ', '#', ' ', '$', '#'},
				{'#', '#', '#', '#', '#', '#', '#', '#'},
			};
			goalPos = Position(4, 6);
			spyPos = Position(4, 1);
			Position p1 = Position(2, 1);
			Position p2 = Position(1, 5);
			guardList = { p1, p2 };

		}
		else
		{
			name = "The Truth of the Weapon";
			board = {
				{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},  //0
				{'#', ' ', '#', ' ', ' ', 'v', ' ', ' ', ' ', ' ', '#'},  //1
				{'#', '>', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},  //2
				{'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'},  //3
				{'#', ' ', '#', ' ', '#', ' ', '$', ' ', ' ', ' ', '#'},  //4
				{'#', ' ', '@', '#', '#', '#', '#', '#', ' ', ' ', '#'},  //5
				{'#', ' ', 'v', ' ', '#', ' ', ' ', ' ', ' ', '<', '#'},  //6
				{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},  //7
				{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},  //8
			};
			goalPos = Position(4, 6);
			spyPos = Position(5, 2);
			Position p1 = Position(2, 1);
			Position p2 = Position(6, 2);
			Position p3 = Position(1, 5);
			Position p4 = Position(6, 9);
			guardList = { p1, p2, p3, p4 };
		}
	}
	

	string getName()
	{
		return name;
	}
	Position getSpyPos()
	{
		return spyPos;
	}
	Position getGoalPos()
	{
		return goalPos;  
	}
	void setSpyPos(Position next)
	{
		spyPos = next;
	}

	void printBoard()
	{
		for (const auto& row : board)
		{
			for (char c: row)
			{
				cout << c;
			}
			cout << endl;
		}
		
	}
	
	bool moveSpy(char direction)
	{
		Position next;
		
		if (direction == 'W' || direction == 'w')  // up
		{
			next = Position(spyPos.r - 1, spyPos.c);
		}
		else if (direction == 'A' || direction == 'a')  // left)
		{
			next = Position(spyPos.r, spyPos.c - 1);
		}
		else if (direction == 'S' || direction == 's')  // down
		{
			next = Position(spyPos.r + 1, spyPos.c);
		}
		else if (direction == 'D' || direction == 'd')  // right
		{
			next = Position(spyPos.r, spyPos.c + 1);
		}
		else
		{
			cout << "Invalid letter." << endl;  // should never get to this statement because of isValidLetter already checked.  Can delete.
		}

		bool hitGuard = isGuard(next);
		if (next == goalPos)
		{
			board[spyPos.r][spyPos.c] = ' ';
			board[next.r][next.c] = '@';
			setSpyPos(next);
			cout << "You got to the goal without getting caught!  You are the Ultimate Spy... You win!" << endl;
			return false;  //game over - set isRunning to false
		}
		else if (hitGuard)
		{
			board[spyPos.r][spyPos.c] = ' '; // delete the spy symbol at death
			cout << "Busted!  You ran right into a guard.  Game over... you are NOT the Ultimate Spy." << endl;
			return false;
		}
		else  // next is an empty space
		{
			board[spyPos.r][spyPos.c] = ' ';
			board[next.r][next.c] = '@';
			setSpyPos(next);
			bool inLineOfSight = isInGuardLineOfSight();
			if (!inLineOfSight)
			{
				return true;  // isRunning stays true
			}
			else
			{
				return false;  // in line of sight and isRunning becomes false
			}
		}
		
		
	}

	bool isValidLetter(char let)
	{
		if (let == 'A' || let == 'a' || let == 'W' || let == 'w' || let == 'D' || let == 'd' || let == 'S' || let == 's')
		{
			return true;
		}
		cout << "Invalid letter.  Try again." << endl;
		return false;
	}

	bool isValidMove(char direction)
	{

		if (direction == 'W' || direction == 'w')  // up
		{
			if (board[spyPos.r - 1][spyPos.c] == '#')
			{
				
				cout << "Can't move up.  There is a wall there.  Try again." << endl;
				return false;
			}
		}
		else if (direction == 'A' || direction == 'a')  // left)
		{
			if (board[spyPos.r][spyPos.c - 1] == '#')
			{
				cout << "Can't move left.  There is a wall there.  Try again." << endl;
				return false;
			}
		}
		else if (direction == 'S' || direction == 's')  // down
		{
			if (board[spyPos.r + 1][spyPos.c] == '#')
			{
				cout << "Can't move down.  There is a wall there.  Try again." << endl;
				return false;
			}
		}
		else if (direction == 'D' || direction == 'd')  // right
		{
			if (board[spyPos.r][spyPos.c + 1] == '#')
			{
				cout << "Can't move right.  There is a wall there.  Try again." << endl;
				return false;
			}
		}
		return true;
	}
	


	bool isGuard(Position p)
	{
		for (int i = 0; i < guardList.size(); i++)
		{
			if (guardList[i] == p)
			{
				return true;
			}
		}
		return false;
	}





	bool isInGuardLineOfSight()
	{
		for (int g = 0; g < guardList.size(); g++)  
		{
			Position temp = guardList[g];
			char symbol = board[temp.r][temp.c];
			if (symbol == '>')  // traverse cols ++
			{
				int i = temp.c + 1;
				while (board[temp.r][i] == ' ')
				{
					i++;
				}
				if (board[temp.r][i] == '@')
				{
					cout << "One of the guards can see you!" << endl;
					return true;
				}
			}
			else if (symbol == '<')  // travers cols --
			{
				int i = temp.c - 1;
				while (board[temp.r][i] == ' ')
				{
					i--;
				}
				if (board[temp.r][i] == '@')
				{
					cout << "One of the guards can see you!" << endl;
					return true;
				}
			}
			else if (symbol == '^')  // travers rows --
			{
				int i = temp.r - 1;
				while (board[i][temp.c] == ' ')
				{
					i--;
				}
				if (board[i][temp.c] == '@')
				{
					cout << "One of the guards can see you!" << endl;
					return true;
				}
			}
			else if (symbol == 'v')  // travers rows ++
			{
				int i = temp.r + 1;
				while (board[i][temp.c] == ' ')
				{
					i++;
				}
				if (board[i][temp.c] == '@')
				{
					cout << "One of the guards can see you!" << endl;
					return true;
				}
			}
		}
		return false;
	}
	


	bool updateGuards()
	{		
		for (int i = 0; i < guardList.size(); i++)  // for each guard, check if about to hit wall or goal and flip.
		{
			Position& p = guardList[i];
			if (board[p.r][p.c] == '<')
			{
				//check left
				if (board[p.r][p.c - 1] == '#' || board[p.r][p.c - 1] == '$')
				{
					board[p.r][p.c] = '>';
				}
			}
			else if (board[p.r][p.c] == '>')
			{
				//check right
				if (board[p.r][p.c + 1] == '#' || board[p.r][p.c - 1] == '$')
				{
					board[p.r][p.c] = '<';
				}
			}
			else if (board[p.r][p.c] == '^')
			{
				//check up
				if (board[p.r - 1][p.c] == '#' || board[p.r][p.c - 1] == '$')
				{
					board[p.r][p.c] = 'v';
				}
			}
			else if (board[p.r][p.c] == 'v')
			{
				//check down
				if (board[p.r + 1][p.c] == '#' || board[p.r][p.c - 1] == '$')
				{
					board[p.r][p.c] = '^';
				}
			}
		}

		// all have been flipped if needed. So now all guards move forward one: update Position, update chars in board, and check collision

		for (int i = 0; i < guardList.size(); i++)
		{
			Position& p = guardList[i];
			if (board[p.r][p.c] == '<')
			{
				p.c--;
				board[p.r][p.c] = '<';
				board[p.r][p.c + 1] = ' ';
				if (p == getSpyPos())
				{
					cout << "Guard captured you! Game over.  You are NOT the Ultimate Spy." << endl;
					return false;
				}
			}
			else if (board[p.r][p.c] == '>')
			{
				p.c++;
				board[p.r][p.c] = '>';
				board[p.r][p.c - 1] = ' ';
				if (p == getSpyPos())
				{
					cout << "Guard captured you! Game over.  You are NOT the Ultimate Spy." << endl;
					return false;
				}
			}
			else if (board[p.r][p.c] == '^')
			{
				p.r--;
				board[p.r][p.c] = '^';
				board[p.r + 1][p.c] = ' ';
				if (p == getSpyPos())
				{
					cout << "Guard captured you! Game over.  You are NOT the Ultimate Spy." << endl;
					return false;
				}
			}
			else if (board[p.r][p.c] == 'v')
			{
				p.r++;
				board[p.r][p.c] = 'v';
				board[p.r - 1][p.c] = ' ';
				if (p == getSpyPos())
				{
					cout << "Guard captured you!  Game over. You are NOT the Ultimate Spy." << endl;
					return false;
				}
			}
			bool inLineOfSight = isInGuardLineOfSight();
			if (inLineOfSight)
			{
				cout << "Game over.  You are NOT the Ultimate Spy." << endl;
				return false;  // set isRunning to false - game over.
			}
		}
		return true;
	}

};


int main()
{

	cout << "Welcome to Ultra - Spy!" << endl;
	cout << "Select a level: " << endl;
	cout << "\n1. A New Enemy" << endl;
	cout << "2. The Truth of the Weapon" << endl;

	string input;
    getline(cin, input);
	Level level;
	
	if (input.length() == 1)
	{
		if (input.compare("1") == 0)
		{
			cout << "You have chosen Level 1: A New Enemy." << endl;
			level = Level(1);
			level.printBoard();
		}
		else if (input.compare("2") == 0)
		{
			cout << "You have chosen Level 2: The Truth of the Weapon." << endl;
			level = Level(2);
			level.printBoard();
		}
		else
		{
			cout << "Invalid title." << endl;
		}
	}
	else  // length > 1 so user entered name of level
	{
		for (char& c : input)
		{
			c = tolower(c);
		}
		if (input.compare("a new enemy") == 0)
		{
			cout << "You have chosen Level 1: A New Enemy." << endl;
			level = Level(1);
			level.printBoard();
		}
		else if (input.compare("the truth of the weapon") == 0)
		{
			cout << "You have chosen Level 2: The Truth of the Weapon." << endl;
			level = Level(2);
			level.printBoard();
		}
	}

	

	bool isRunning = true;
	char dir;
	bool validLetter = false;
	bool validMove = false;
	while (isRunning == true)
	{
		
		do
		{
			cout << "Which way do you want to move? A/W/S/D: " << endl;
			
			cin >> dir;
			validLetter = level.isValidLetter(dir);
			if (validLetter)
			{
				validMove = level.isValidMove(dir);
			}
			
		}while(!(validLetter && validMove));
		
		isRunning = level.moveSpy(dir);
		cout << "You moved: " << endl;
		level.printBoard();
		cout << "\n";

		if (isRunning)
		{
			cout << "Gurads moved: " << endl;
			isRunning = level.updateGuards();
			level.printBoard();
			cout << "\n\n";
		}
		
	}
	
	
}