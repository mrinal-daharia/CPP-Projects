#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>

#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#define SLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#define CLEAR "clear"
#define SLEEP(ms) sleep((ms)/1000)
#endif

using namespace std;

// Hangman ASCII states (indexed from 0 to 7)
const string HANGMAN_PICS[] = {
    R"( 
  +---+
  |   |
      |
      |
      |
      |
=========
)",
    R"(
  +---+
  |   |
  O   |
      |
      |
      |
=========
)",
    R"(
  +---+
  |   |
  O   |
  |   |
      |
      |
=========
)",
    R"(
  +---+
  |   |
  O   |
 /|   |
      |
      |
=========
)",
    R"(
  +---+
  |   |
  O   |
 /|\  |
      |
      |
=========
)",
    R"(
  +---+
  |   |
  O   |
 /|\  |
 /    |
      |
=========
)",
    R"(
  +---+
  |   |
  O   |
 /|\  |
 / \  |
      |
=========
)",
    R"(
  +---+
  |   |
 [O]  |
 /|\  |
 / \  |
      |
=========
)"
};

// Word list (shortened for example; you can keep the full list)
const vector<string> WORD_LIST = {
    "castle", "journey", "diamond", "network", "science", "holiday", "justice", "teacher",
    "library", "monster", "organic", "picture", "revenue", "station", "uniform", "volcano",
    "weather", "freedom", "capital", "pattern", "penguin", "physics", "project", "regular",
    "sandbox", "silicon", "student", "subject", "success", "summary", "surface", "virtual"
};

// Display word with guessed letters
void displayWord(const string& word, const vector<char>& guessed) {
    for (char c : word) {
        if (find(guessed.begin(), guessed.end(), c) != guessed.end()) {
            cout << c << " ";
        } else {
            cout << "_ ";
        }
    }
    cout << endl;
}

// Check if the word is completely guessed
bool isWordGuessed(const string& word, const vector<char>& guessed) {
    for (char c : word) {
        if (find(guessed.begin(), guessed.end(), c) == guessed.end()) {
            return false;
        }
    }
    return true;
}

// Reveal random letters at the start
void revealRandomCharacters(const string& word, vector<char>& guessedLetters) {
    int length = word.length();
    int hints = 0;

    if (length <= 6)
        hints = 2;
    else if (length <= 10)
        hints = 3;
    else
        hints = 5;

    set<char> revealed;
    while (revealed.size() < hints) {
        char c = word[rand() % length];
        revealed.insert(c);
    }

    for (char c : revealed)
        guessedLetters.push_back(c);
}

// Main game function
void hangmanGame() {
    srand(time(0));
    string word = WORD_LIST[rand() % WORD_LIST.size()];
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    vector<char> guessedLetters;
    revealRandomCharacters(word, guessedLetters);

    int maxAttempts = 7;
    int wrongGuesses = 0;

    while (wrongGuesses < maxAttempts) {
        system(CLEAR);
        cout << "HANGMAN GAME\n";
        cout << HANGMAN_PICS[wrongGuesses] << endl;
        cout << "Word: ";
        displayWord(word, guessedLetters);
        cout << "Guessed letters: ";
        for (char c : guessedLetters) cout << c << " ";
        cout << "\nWrong attempts left: " << (maxAttempts - wrongGuesses) << endl;

        cout << "Enter a letter: ";
        char guess;
        cin >> guess;
        guess = tolower(guess);

        if (!isalpha(guess)) {
            cout << "Invalid input. Please enter a letter.\n";
            SLEEP(1000);
            continue;
        }

        if (find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            cout << "You already guessed that letter.\n";
            SLEEP(1000);
            continue;
        }

        guessedLetters.push_back(guess);

        if (word.find(guess) == string::npos) {
            cout << "Incorrect guess.\n";
            wrongGuesses++;
            SLEEP(1000);
        }

        if (isWordGuessed(word, guessedLetters)) {
            system(CLEAR);
            cout << "HANGMAN GAME\n";
            cout << HANGMAN_PICS[wrongGuesses] << endl;
            cout << "Word: ";
            displayWord(word, guessedLetters);
            cout << "\nCongratulations! You guessed the word: " << word << endl;
            return;
        }
    }

    system(CLEAR);
    cout << "HANGMAN GAME\n";
    cout << HANGMAN_PICS[7] << endl;
    cout << "Out of attempts! The word was: " << word << endl;
}

// Entry point
int main() {
    char playAgain;
    do {
        hangmanGame();
        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;
    } while (tolower(playAgain) == 'y');

    cout << "Thanks for playing Hangman!" << endl;
    return 0;
}
