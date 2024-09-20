#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


//hunter nelson, csci 299 Wordle

void printGreen(const std::string &text) {
  std::cout << "\033[92m" << text << "\033[0m";
}
/*
void printYellow(const std::string& text) {
    std::cout << "\033[93m" << text << "\033[0m";
}
*/
//make words lowercase to fix error
std::string to_lowerc(const std::string &str) {
  std::string lcStr = str;
  for (size_t i = 0; i < lcStr.length(); ++i) {
    lcStr[i] = tolower(lcStr[i]);
  }
  return lcStr;
}

//load wordlist
std::vector<std::string> loadList(const std::string &filename) {
  std::vector<std::string> words;
  std::ifstream file("wordlist.txt");
  std::string word;

  while (std::getline(file, word)) {
    word = to_lowerc(word);
    words.push_back(word);
  }
  file.close();
  std::sort(words.begin(), words.end());
  return words;
}

//check if word is in list
bool isValid(const std::vector<std::string> &words,const std::string &guess) {
  return std::binary_search(words.begin(), words.end(), guess);
}

//process the guess to replace letters
void processGuess(const std::string &target, const std::string &guess,std::string &boardRow) {
  std::string tempTarget = target;
  std::string result(5, '*');
  std::vector<bool> matched(5, false);

  //check green (changed from print green due to errors)
  for (int i = 0; i < 5; ++i) {
    if (guess[i] == target[i]) {
      result[i] = guess[i];
      result.replace(i, 1,"\033[92m" + std::string(1, guess[i]) + "\033[0m"); //replace method with green if it is correct
      tempTarget[i] = '_';
      matched[i] = true;
    }
  }

  //check for yellow
  for (int i = 0; i < 5; ++i) {
    if (result[i] == '*') {
      size_t pos = tempTarget.find(guess[i]);
      if (pos != std::string::npos) { //if letter is still in word
        result[i] = guess[i];
        result.replace(i, 1,"\033[93m" + std::string(1, guess[i]) +"\033[0m"); //replace with yellow if right but wrong place
        tempTarget[pos] = '_';
      }
    }
  }

  boardRow = result;
}

//load the wordle board
void gameBoard(const std::vector<std::string> &board) {
  for (const auto& row : board) { //used to not make multiple copies and instead just change the single copy
    std::cout << row << std::endl;
  }
}

int main() {
  srand(time(0));

  std::vector<std::string> words = loadList("wordlist.txt");
  if (words.empty()) {
    std::cout << "Error loading wordlist, please try again." << std::endl;
    return 1;
  }

  std::string userInput;
  bool playAgain = true;

  std::cout << "Welcome to Wordle!" << std::endl;

  while (playAgain) {
    std::string target = words[rand() % words.size()]; //pick word
    std::vector<std::string> board(6, "*****");
    int attempts = 0;
    bool gameWon = false;

    while (attempts < 6) {
      gameBoard(board);
      std::cout << "Enter your guess (or 'quit' to quit, and 'new' to start a new game): ";
      std::cin >> userInput;

      if (userInput == "quit") {
        std::cout << "Thanks for playing!" << std::endl;
        return 0;
      } else if (userInput == "new") {
        break;
      } else if (userInput.length() != 5) {
        std::cout << "Please enter a 5-letter word." << std::endl;
        continue; //added so loop can be skipped
      } else if (!isValid(words, userInput)) {
        std::cout << "Invalid word, please try again." << std::endl;
        continue;
      }

      processGuess(target, userInput, board[attempts]);
      //attempts++;

      if (userInput == target) {
        std::cout << "Congratulations! You guessed the word: ";
        printGreen(target);
        std::cout << std::endl;
        gameWon = true;
        break;
      } else {
        attempts++;
      }
    }

    if (!gameWon && attempts > 5) {
      std::cout << "Sorry, you ran out of guesses. The correct word was: ";
      printGreen(target);
      std::cout << std::endl;
    }

    std::cout << "Would you like to play again? (y/n): ";
    std::cin >> userInput;
    if (userInput != "y") {
      playAgain = false;
      std::cout << "Thanks for playing!" << std::endl;
    }
  }

  return 0;
}
