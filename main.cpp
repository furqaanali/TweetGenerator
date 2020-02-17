//  TweetGenerator
//     Generate somewhat random tweets based on a datafile.
//
//  Open up a data file and find a starting point for some set of consecutive words
//  and print the starting word.
//
//  Next find all occurrences in the datafile of those consecutive words, and collect the set
//  of words that immediately follow each of those occurrences.  Choose one of the words
//  in this newly created set and add it to the set of consecutive words, shifting them
//  over by one so the first word gets overwritten by the second, and so on.
//
//  Repeat this process until >=30 words are displayed and an end-of-sentence word is found,
//  or until a displayed word has a newline or a return character at the end of it.
//
#include <iostream>
#include <cctype>       // For the letter checking functions
#include <fstream>      // For file input
#include <vector>
#include <cstring>      // For strlen()
#include <cstdlib>      // For rand()
using namespace std;

//---------------------------------------------------------------------
void getInputWords( vector<string> &allWords)   // List of all the words in the selected input file
{
    // Prompt for and get user choice for which data file to use
    cout << "Menu Options: \n"
         << "  1. Use Trump tweets \n"
         << "  2. Use Dalai Lama tweets \n"
         << "  3. Use Mixed Trump / Dalai Lama tweets \n"
         << "  4. Use small test file \n"
         << "  5. Use tiny test file \n"
         << "Your choice: ";
    int dataSelectionOption;
    cin >> dataSelectionOption;
    
    ifstream inputFileStream;  // declare the input file stream
    // open input file and verify
    switch( dataSelectionOption) {
        case 1: inputFileStream.open("TrumpTweets.txt"); break;
        case 2: inputFileStream.open("DalaiLamaTweets.txt"); break;
        case 3: inputFileStream.open("TrumpLama.txt"); break;
        case 4: inputFileStream.open("Test.txt"); break;
        case 5: inputFileStream.open("tiny.txt"); break;
        default: cout << "Invalid option chosen, exiting program. ";
            exit( -1);
            break;
    }
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file.  Exiting..." << endl;
        exit( -1);
    }
    
    char c = ' ';
    char inputWord[ 81];
    
    // Read a character at a time from the input file, separating out individual words.
    inputWord[ 0] = '\0';
    int index = 0;
    while( inputFileStream.get(c)) {
        if( ! isprint( c) && c != '\r' && c != '\n') {
            continue;   // Skip non-printable characters and get the next one
        }
        if( c != ' ') {
            // If it is not a return or newline, add it to the string.
            // If it is a return or newline character, only add it if there
            //    are already other characters in the string.
            if( (c != '\r' && c != '\n') ||
                ( (c == '\r' || c == '\n') && index > 0)
              ) {
                inputWord[ index++] = c;
            }
        }
        // End the word when encountering a space or a return character.
        if( c == ' ' || c == '\r' || c == '\n'){
            // Null terminate the input word. Store it if its length is > 0 and it is printable.
            inputWord[ index] = '\0';
            if( strlen( inputWord) > 0 && isprint( inputWord[0])) {
                allWords.push_back( string( inputWord));
            }
            // Check for special case where there is a space at the end of the line.  We don't want
            // to lose the end of line character ('\n' or '\r'), so we concatenate it to the
            // last word that was previously stored on the list.  First check to see if this is the case:
            if( (c == '\r' || c == '\n') && strlen( inputWord) == 0) {
                long indexOfLastWord = allWords.size() - 1;
                allWords.at( indexOfLastWord).append("\r");
            }
            
            index = 0;    // Restart the index for the next word
        }
    }//end while( inputFileStream...)
}


//---------------------------------------------------------------------
void checkWords( vector<string> startWords,  // List of first words in sentences
                 vector<string> allWords,    // All words from input file
                 vector<string> wordList,    // List of all words following search phrase
                 string nextWord)            // Nexts word found after search phrase
{
    int debugMenuOption = 0;
    
    do {
        // Prompt for and get user choice for which debug option to use
        cout << " \n"
             << "    >>> Debug menu options: <<<\n"
             << "        1. Display a start word \n"
             << "        2. Display one of all words \n"
             << "        3. Display wordWindow and next words \n"
             << "        4. Exit debugging \n"
             << "    Your choice -> ";
        cin >> debugMenuOption;
        
        int wordIndexValue = 0;    // Used to read user input for debug options below
        
        switch( debugMenuOption) {
            case 1: cout << "    Enter an index value from 0 to " << startWords.size()-1 << " to display a start word: ";
                    cin >> wordIndexValue;
                    cout << "    " << startWords.at( wordIndexValue) << endl;
                    break;
            case 2: cout << "    Enter an index value from 0 to " << allWords.size()-1 << " to display one of all words: ";
                    cin >> wordIndexValue;
                    cout << "    " << allWords.at( wordIndexValue) << endl;
                    break;
            case 3: cout << "    WordWindow and next words are: ";
                    for( int i=0; i<wordList.size(); i++) {
                        cout << wordList.at( i) << " ";
                    }
                    cout << nextWord << endl;
                    break;
        }//end switch(...)
    }while( debugMenuOption != 4);
    
}//end debug()


//---------------------------------------------------------------------
void getStartWords( vector<string> &destinationVector, vector<string> sourceVector) {
  
    destinationVector.push_back(sourceVector.at(0));    // Append first word in allWords to startWords
  
    // Append word from allWords to startWords if the word prior to it ends with a return or newline
    for (int i = 1; i < sourceVector.size(); ++i) {
          
        if ( ( sourceVector.at( i).at( sourceVector.at( i).size() - 1) == '\n' ||
               sourceVector.at( i).at( sourceVector.at( i).size() - 1) == '\r') &&
               ( ( i + 1) != sourceVector.size() )) {

            destinationVector.push_back( sourceVector.at( i + 1));

        }

    }
}
//---------------------------------------------------------------------
void getWordWindow( vector<string> &allWords, vector<string> &wordList, int wordWindowWidth) {
  
    for ( int i = 0; i < allWords.size(); ++i) {
          
        if ( allWords.at( i) == wordList.at( 0)) {
            
            for ( int j = 1; j < wordWindowWidth; ++j) {
              
                wordList.push_back( allWords.at( i + j));
              
            }
          
            break;    // Exit function after the consecutive words have been stored into wordList vector
          
        }
    }
  
}
//---------------------------------------------------------------------
// Stores all words that appear after the words stored in wordList (word window) into nextWords vector

void getNextWords( vector<string> &allWords, vector<string> &wordList, vector<string> &nextWords) {
  
    bool isNextWord;    // Stores whether all nextWord conditions have been met
  
    for ( int i = 0; i < allWords.size(); ++i) {
              
        if ( ( allWords.at( i) == wordList.at( 0)) && ( i + wordList.size() < allWords.size() )) {
          
            isNextWord = true;
            
            for ( int j = 1; j < wordList.size(); ++j) {
              
                if ( allWords.at( i + j) != wordList.at( j) ) {
                  
                    isNextWord = false;
                  
                }
              
            }
          
            if ( isNextWord) {
              
                nextWords.push_back( allWords.at( i + wordList.size() ));
              
            }
            
                    
        }
    }
  
}
//---------------------------------------------------------------------
int main()
{
    vector<string> allWords;   // List of all input words
    vector<string> startWords; // List of words that start sentences, to use
                               //   in starting generated tweets.

    cout << "Welcome to the tweet generator. " << endl << endl;
    
  
    getInputWords( allWords);
  
    getStartWords( startWords, allWords);
    
  
    string nextWord;    // Stores the next word after the phrase (word window)
  
    vector<string> nextWords;    // Stores all nextWord options
    
  
    // Allow looping to give more than one tweet
    int wordWindowWidth = 0;
    char userInput = ' ';
    
    // Main loop
    while( userInput != 'X') {
        cout << endl;
        cout << "Enter x to exit, or d/c to debug/continue followed by word window size: ";
        cin >> userInput;
        userInput = toupper( userInput);    // fold user input into upper case
        
        if( userInput == 'X') {
            break;   // Exit program
        }

      
        // Read in the wordWindowWidth to be used in both the "continue" and the "debug" modes
        cin >> wordWindowWidth;

        vector< string> wordList;   // Stores the list of consecutive words that are searched for
      
        
        // Find a random starting word in the startWords list
        wordList.push_back( startWords.at( rand() % startWords.size()));
        
      
        // Find the words after the selected startWord in allWords and append them to wordList
        getWordWindow( allWords, wordList, wordWindowWidth);
      
      
        
        int wordCounter = 0;
      
        while( true) {
            
            nextWords.clear();    // Reset nextWord options from previous iteration
          
            
            getNextWords( allWords, wordList, nextWords);    // Find all nextWord options and store into nextWords vector
         
            nextWord = nextWords.at( rand() % nextWords.size());    // Store a random nextWord from nextWords
           
          
            // If in checkWordsMode then allow displaying single word values using the checkWords(...) function, then exit the program.
            
            if ( userInput == 'D') {
              
                checkWords( startWords, allWords, wordList, nextWord); 
                return 0;    // End program
              
            }
            
            // Display the first word in the wordWindow
            if ( userInput == 'C') {
              
                cout << " " << wordList.at( 0);
              
            }
          
            wordCounter++;
            
            // If the first word in the wordWindow ends in a return, then we're done with our output for this tweet.
            
            if ( wordList.at( 0).at( wordList.at( 0).size() - 1 ) == '\n' ||
                 wordList.at( 0).at( wordList.at( 0).size() - 1 ) == '\r' ) {
              
                break;    // End tweet
              
            }
            

            // If there have been >= 30 words and the word ends with '.', '.' or '!' or '?', then we're done with output for this tweet
          
            if ( wordCounter >= 30) {
                
                if ( wordList.at( 0).at( wordList.at(0).size() - 1 ) == '.' ||
                     wordList.at( 0).at( wordList.at(0).size() - 1 ) == '!' ||
                     wordList.at( 0).at( wordList.at(0).size() - 1 ) == '?' ) {
                   
                    break;    // End tweet
                }
            }
          
            // Shift words "left" to prepare for next round, shifting nextWord into the "right" of the wordWindow words.
            
            wordList.erase( wordList.begin());    // Delete first element of wordList
          
            wordList.push_back( nextWord);    // Append nextWord to wordList
            
            
        }//end while( true)
        
    }// end while( userInput != 'x')
    
    cout << endl << endl;
    
    return 0;
}//end main()
