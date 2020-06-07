//
// Created by Aaron Kampmeier on 6/6/20.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>

// My macros
#define SUB_MACRO(a, b) ((a) - (b))
#define CUBE_MACRO(a) ((a) * (a) * (a))
#define MIN_MACRO(a, b) ((a) <= (b) ? (a) : (b))
#define ODD_MACRO(a) ((a) % 2)

using namespace std;

int selectFromMenu();

void playMiniGames(char* nameFileName, int numOfNames);
int randomInRange(int min, int max);
bool playEvenOrOdd(char* opponentName);
bool playRockPaperScissors(char* opponentName);
bool playGuessTheNumber(char* opponentName);
bool playDiceRollShowdown(char* opponentName);

void showScoreboard(int player1Score, int player2Score, char* opponentName);
bool scanIntSafe(int *outputVal, char *inputStr = nullptr);
bool isEven(int number);

void callFuncsAndMacros();
void resetNums(int* num1, int* num2);
int subf(int a, int b);
int cubef(int a);
int minf(int a, int b);
int oddf(int a);

int main(int argc, char** argv) {
    char* fileName = new char[20];
    int numOfNames;
    // Check command line arguments, first should be executable name, then the name filename, then the number of names
    if (argc == 3) {
        // Use the input to get the name file filename
        fileName = *(argv + 1);
        char* numOfNamesInput = *(argv + 2);
        
        bool success = scanIntSafe(&numOfNames, numOfNamesInput);

        if (!success) {
	        cerr << "Error getting num of names from command line arg, please try again with an integer as the second"
				 " arg" << endl;
	        exit(1);
        }
    } else {
        fileName = "random_names.txt";
        numOfNames = 100;
    }
    
    // Seed the random number generator for this program
    srand(time(nullptr));

    //Option stores the menu selection made by user
    int option = -1;
    do {
	    option = selectFromMenu();
	    switch (option) {
		    case 1:
			    playMiniGames(fileName, numOfNames);
			    break;
		    case 2:
		    	callFuncsAndMacros();
			    break;
		    default:
			    break;
	    }
    } while (option != 0); // Exit the loop on a 0 option
    return 0;
}

/**
 * Presents the main menu to the user and verifies input choice is valid
 * @return The menu choice
 */
int selectFromMenu() {
    cout << "Welcome to the CSE240 Assignment 2!\n"
            "Menu:\n"
            "1 – CSE240 Mini-Game Collection\n"
            "2 – Macros vs Functions Throw-down\n"
            "0 - Exit\n"
            "Choose an option:" << endl;

    int option;
    
    // Grab in the option input
    bool success = scanIntSafe(&option);
    
    //Check validity
    if (!success || option < 0 || option > 2) {
    	// There was invalid input
	    cout << "Invalid input, please only input 0, 1, or 2." << endl;
	    // Ask for menu input again
	    return selectFromMenu();
    } else {
        return option;
    }
}

/**
 * Reads a random opponent name from a text file and sets up mini games the user can play against that opponent.
 * @param nameFileName The filename of the file containing all the computer names.
 * @param numOfNames The number of names in the name file.
 */
void playMiniGames(char* nameFileName, int numOfNames) {
    // Generate random number to select name
    int selectedNameIndex = randomInRange(1, numOfNames);
    
    //Open file
    FILE* nameFile;
    nameFile = fopen(nameFileName, "r");
    
    // Check to make sure the file exists
    if (nameFile == nullptr) {
    	// The file was unable to be opened, throw error
    	cerr << "The name file specified was unable to be opened. Try again with a correct file." << endl;
    	exit(1);
    }
    
    //Iterate through file until the row we want
    char* opponentName = new char[30];
    for (int i=1; i <= selectedNameIndex; i++) {
//    	fgets(opponentName, 30, nameFile);
        fscanf(nameFile, "%s", opponentName);
    }
    
    bool validGameNum;
	int numOfGames;
    do {
	    // Welcome the user and introduce games
	    cout << "Welcome to CSE240 Mini-Game Collection!\n";
	    cout << "I am your opponent " << opponentName << "\n\n";
	    cout << "How many rounds should we play? (Choose an odd number)\n>>" << endl;
	
	    // Pull in and verify num of games. numOfGames must be odd.
	    bool success = scanIntSafe(&numOfGames);
	    if (!success || numOfGames < 0 || numOfGames % 2 != 1) {
	    	validGameNum = false;
	    	cout << "Invalid input. Make sure the number you enter is odd.\n" << endl;
	    } else {
	    	validGameNum = true;
	    }
    } while (!validGameNum);
    
    cout << "Playing " << numOfGames << " games." << endl;
    
    // Play some random games. Once a majority victory is achieved, end and present winner. Show a scoreboard after
    // each game.
    int criticalGames = (numOfGames + 1) / 2; // The critical number of won games needed to win the competition.
    int userScore = 0; // For each game won, the players get one point.
    int computerScore = 0;
    
    int gameChoice;
    while (userScore < criticalGames && computerScore < criticalGames) {
	    gameChoice = randomInRange(1,4);
	    bool userWon;
    	switch (gameChoice) {
    		case 1:
    			userWon = playEvenOrOdd(opponentName);
    			break;
    		case 2:
    			userWon = playRockPaperScissors(opponentName);
    			break;
    		case 3:
    			userWon = playGuessTheNumber(opponentName);
    			break;
    		case 4:
    			userWon = playDiceRollShowdown(opponentName);
    			break;
    		default:
    			cerr << "Unexpected State. Undefined game." << endl;
    			exit(1);
    	}
    	
    	//Add the scores
    	userScore += userWon;
    	computerScore += !userWon;
    	
    	//Show the scoreboard
    	showScoreboard(userScore, computerScore, opponentName);
    }
    
    if (userScore > computerScore) {
    	cout << "You Win!! Congratulations!!\n";
    } else {
    	cout << opponentName << " Wins! So sad, try harder next time.\n";
    }
	cout << "Play again sometime soon!\n" << endl;
}

/**
 * Presents the scoreboard of players scores.
 * @param player1Score
 * @param player2Score
 */
void showScoreboard(int player1Score, int player2Score, char* opponentName) {
	cout << "\nThe score is: \n"
		 "Human: " << player1Score << "\n"
		 << opponentName << ": " << player2Score << "\n" << endl;
}

/// Mini-Games
/**
 * Plays the Even or Odd game.
 * @param opponentName
 * @return True if the user won, false if the computer won.
 */
bool playEvenOrOdd(char *opponentName) {
	const int MAX_GUESS = 100;
	
	cout << "Even/Odd Game!\n" << "Guess even or odd: (0 for even, 1 for odd)\n>>" << endl;
	
	int userParityGuess;
	bool inputSuccess = scanIntSafe(&userParityGuess);
	while (!inputSuccess || !(userParityGuess == 0 || userParityGuess == 1)) {
		cout << "Invalid Input.\n>>" << endl;
		inputSuccess = scanIntSafe(&userParityGuess);
	}
	
	cout << "Enter a number (between 0 and " << MAX_GUESS << ")\n>>" << endl;
	
	int userNumber;
	while (!scanIntSafe(&userNumber) || userNumber < 0 || userNumber > MAX_GUESS) {
		cout << "Invalid input.\n>>" << endl;
	}
	
	// Generate a random int for the opponent
	int opponentNumber = randomInRange(0, MAX_GUESS);
	cout << opponentName << " picked " << opponentNumber << endl;
	
	int sum = opponentNumber + userNumber;
	
	cout << userNumber << " + " << opponentNumber << " = " << sum << " - ";
	if (isEven(sum)) {
		// EVEN
		cout << "EVEN!" << endl;
	} else {
		cout <<"ODD!" << endl;
	}
	
	bool userWon = (sum%2 == userParityGuess);
	
	if (userWon) {
		cout << "You win the round!" << endl;
	} else {
		cout << opponentName << " wins the round!" << endl;
	}
	return userWon;
}

/**
 * Figures out if a number is even. Really unecessary function but oh well.
 * @param number
 * @return If number is even, then true.
 */
bool isEven(int number) {
	return number % 2 == 0;
}

/**
 * Plays good ole' rock, paper, scissors with the human.
 * @param opponentName
 * @return
 */
bool playRockPaperScissors(char *opponentName) {
	cout << "Rock, Paper, Scissors Game!" << endl;
	
	cout << "Choose a throw!\n"
	        "1. Rock\n"
	        "2. Paper\n"
	        "3. Scissors\n>>" << endl;
	
	int userThrow;
	while (!scanIntSafe(&userThrow) || userThrow < 1 || userThrow > 3) {
		cout << "Invalid Input. Try again.\n>>" << endl;
	}
	
	int computerThrow = randomInRange(1,3);
	const char weapons[3][10] = {"Rock", "Paper", "Scissors"};
	
	cout << opponentName << " threw " << weapons[computerThrow - 1] << endl;
	
	if (userThrow == computerThrow) {
		// Tie
		cout << "Tie. Play again." << endl;
		return playRockPaperScissors(opponentName);
	} else if ((computerThrow - 1) == (userThrow % 3)) { // Uses the unique trait in this ordering of rock, paper,
		// scissors, that each item beats the one directly below it in the list (looping/mod).
		// Computer won
		cout << opponentName << " wins!" << endl;
		return false;
	} else if ((userThrow - 1) == (computerThrow % 3)) {
		// User won!
		cout << "You win!" << endl;
		return true;
	} else {
		cerr << "Unexpected state." << endl;
		exit(1);
	}
}

/**
 * Plays the guess the number game with the user. They have 5 guesses to guess a random number within two bounds
 * @param opponentName
 * @return
 */
bool playGuessTheNumber(char *opponentName) {
	cout << "Number Guessing Game!" << endl;
	
	// Generate a diff between 30 and 100
	int diff = randomInRange(30, 100);
	
	//Generate a lower bound
	int lowBound = randomInRange(0, 900);
	
	//Generate a random
	int computerNumber = randomInRange(lowBound, lowBound + diff);
	
	cout << "Welcome puny human, " << opponentName << " is thinking of a number between " << lowBound << " and " <<
	lowBound + diff
	<< "!\n";
	cout << "You have 5 guesses, good luck!" << endl;
	
	int userGuess;
	for (int g=0; g < 5; g++) {
		cout << "Guess #" << (g + 1) << ": " << endl;
		while(!scanIntSafe(&userGuess) || userGuess < 0 || userGuess > 1000) {
			cout << "Invalid input." << endl;
		}
		
		if (userGuess < computerNumber) {
			cout << opponentName << "'s number is higher!" << endl;
		} else if (computerNumber < userGuess) {
			cout << opponentName << "'s number is lower!" << endl;
		} else if (userGuess == computerNumber) {
			cout << "You guessed it! Congrats!" << endl;
			return true;
		}
	}
	
	cout << "You ran out of guesses :/. " << opponentName << "'s number was " << computerNumber << ".\nBetter luck "
																								"next time." << endl;
	return false;
}

/**
 * Play a game where the user and computer roll a user input number of dice of user determined size. The player with
 * the highest sum of the values on the rolled dice wins.
 * @param opponentName
 * @return If the user won.
 */
bool playDiceRollShowdown(char *opponentName) {
	cout << "Dice Roll Showdown!" << endl;
	
	int dieSize;
	int numOfDice;
	
	// Pull in the user's game settings
	cout << "How many sides do the dice have (input positive integer)?" << endl;
	while (!scanIntSafe(&dieSize) || dieSize < 1) {
		cout << "Invalid input." << endl;
	}
	
	cout << "How many dice do you want to roll?" << endl;
	while (!scanIntSafe(&numOfDice) || numOfDice < 1) {
		cout << "Invalid input." << endl;
	}
	
	int playerSum;
	int computerSum;
	do {
		playerSum = 0;
		computerSum = 0;
		// Roll the dice for the player
		cout << "Puny human Rolled: " << endl;
		for (int d = 0; d < numOfDice; d++) {
			int rolled = randomInRange(1,dieSize);
			playerSum += rolled;
			cout << rolled << "\n";
		}
		cout << "Total: " << playerSum << endl;
		
		// Now the computer
		cout << "Magnificent " << opponentName << " Rolled: " << endl;
		for (int d = 0; d < numOfDice; d++) {
			int rolled = randomInRange(1, dieSize);
			computerSum += rolled;
			cout << rolled << "\n";
		}
		cout << "Total: " << computerSum << "\n" << endl;
		
		if (playerSum == computerSum) {
			cout << "There was a tie, re-rolling...\n" << endl;
		}
		
	} while (playerSum == computerSum); // Keep re-rolling if there is a tie
	
	bool playerWon = playerSum > computerSum;
	
	if (playerWon) {
		cout << "You Win. Woohoo..." << endl;
	} else {
		cout << opponentName << " Wins! Haha, try to actually beat me next time ;)." << endl;
	}
	
	return playerWon;
}

/**
 * Scans an input string or the std cin and converts it to an int if possible, will fail gracefully if any chars
 * other than digits are present.
 * @param outputVal A pointer to the int where the scanned value should be stored.
 * @param inputStr Can be NULL. If provided, input will be scanned from this char array instead of cin.
 * @return Successful or not
 */
bool scanIntSafe(int* outputVal, char* inputStr) {
	char* end; // strtol will put the pointer of where it ended scanning on the input string in here
	
	// If it was not called with an input string then allocate one and read in from cin.
	bool readFromCin = false;
	if (inputStr == nullptr) {
		inputStr = (new char[20]);
		cin >> inputStr;
		readFromCin = true;
	}
	
	// Originally I used this sscanf to convert a str to an int, but after doing some reading I decided strtol is the
	// widely accepted choice for safe conversion.
	// Update: Okay I reached the end of the assignment doc where it listed "Important Notes" and said we didn't have
	// to worry about bizarre input like text into an integer field right now which I didn't see until now when I'm
	// all done. I quite like my implementation now, I'm proud of it, so even if it is overkill for right now oh well
	// whatever. Put text into my integer fields all you like, see what it does. Who needs scanf.
	int val = (int) strtol(inputStr, &end, 0);
	
	// Check if it succeeded, if the end ptr points to something other than \0, then the conversion didn't go
	// through the entire arg string meaning some characters other than digits got in the way. Consider this
	// invalid input. Thus if end is not pointing to \0 then the conversion failed.
	bool success = *end == '\0';
	
	//Clean up the memory allocated for the array if we created it
	if (readFromCin) {
		delete[] inputStr;
	}
	
	// Set the read integer into the variable passed in
	*outputVal = val;
	
	return success;
}

/**
 * Returns a random number within a range, inclusive
 * @param min
 * @param max
 * @return
 */
int randomInRange(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}


/// MACRO FUN
/**
 * Calls some functions and their macro counterparts to showcase the difference between the two and the dangers of
 * macros. Some of the calls will yield different results because macros are performing text replacement which can
 * cause duplications of inline increment or decrement calls.
 */
void callFuncsAndMacros() {
	int num1, num2;
	resetNums(&num1, &num2);
	
	cout << "Making calls to functions and macros using the same variables.\n"
		 "Result of function call is first, then the result of the macro." << endl;
	
	// Sub first
	cout << "\nSub:\n";
	cout << subf(num1, num2) << "\n";
	resetNums(&num1, &num2);
	cout << SUB_MACRO(num1, num2) << "\n";
	resetNums(&num1, &num2);
	cout << "With increment/decrement operators:\n";
	cout << subf(num1++, num2--) << "\n";
	resetNums(&num1, &num2);
	cout << SUB_MACRO(num1++, num2--) << endl;
	
	resetNums(&num1, &num2);
	
	// Next cube
	cout << "\nCube:\n";
	cout << cubef(num1) << "\n";
	resetNums(&num1, &num2);
	cout << CUBE_MACRO(num1) << "\n";
	resetNums(&num1, &num2);
	cout << "With increment/decrement operators:\n";
	cout << cubef(--num1) << "\n";
	resetNums(&num1, &num2);
	cout << CUBE_MACRO(--num1) << endl;
	
	resetNums(&num1, &num2);
	
	//Min
	cout << "\nMin:\n";
	cout << minf(num1, num2) << "\n";
	resetNums(&num1, &num2);
	cout << MIN_MACRO(num1, num2) << "\n";
	resetNums(&num1, &num2);
	cout << "With increment/decrement operators:\n";
	cout << minf(--num1, --num2) << "\n";
	resetNums(&num1, &num2);
	cout << MIN_MACRO(--num1, --num2) << endl;
	
	resetNums(&num1, &num2);
	
	// Odd
	cout << "\nOdd:\n";
	cout << oddf(num1) << "\n";
	resetNums(&num1, &num2);
	cout << ODD_MACRO(num1) << "\n";
	resetNums(&num1, &num2);
	cout << "With increment/decrement operators:\n";
	cout << oddf(num1++) << "\n";
	resetNums(&num1, &num2);
	cout << ODD_MACRO(num1++) << endl;
}

/**
 * Resets the integers used in callFuncsAndMacros to their original values for clean usage.
 * @param num1
 * @param num2
 */
void resetNums(int* num1, int* num2) {
	*num1 = 10;
	*num2 = 17;
}

/**
 * Subtracts a - b
 * @param a
 * @param b
 * @return
 */
int subf(int a, int b) {
	return a - b;
}

/**
 * Cubes a
 * @param a
 * @return
 */
int cubef(int a) {
	return a * a * a;
}

/**
 * Retunrs the min of a and b
 * @param a
 * @param b
 * @return
 */
int minf(int a, int b) {
	return a <= b ? a : b;
}

/**
 * Returns if a is odd or not
 * @param a
 * @return 1/True if a is odd.
 */
int oddf(int a) {
	return a % 2;
}
