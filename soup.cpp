#include <iostream>
#include <string>       // string
#include <ctype.h>      // tolower
#include <map>          // map

#include <unistd.h>

using namespace std;
#define NUMCHARS 26

map<unsigned char,string> numberNames;
int counter;

void init_number_names(void)
{
    numberNames[90] = "ninety";
    numberNames[80] = "eighty";
    numberNames[70] = "seventy";
    numberNames[60] = "sixty";
    numberNames[50] = "fifty";
    numberNames[40] = "forty";
    numberNames[30] = "thirty";
    numberNames[20] = "twenty";
    numberNames[19] = "nineteen";
    numberNames[18] = "eighteen";
    numberNames[17] = "seventeen";
    numberNames[16] = "sixteen";
    numberNames[15] = "fifteen";
    numberNames[14] = "fourteen";
    numberNames[13] = "thirteen";
    numberNames[12] = "twelve";
    numberNames[11] = "eleven";
    numberNames[10] = "ten";
    numberNames[9] = "nine";
    numberNames[8] = "eight";
    numberNames[7] = "seven";
    numberNames[6] = "six";
    numberNames[5] = "five";
    numberNames[4] = "four";
    numberNames[3] = "three";
    numberNames[2] = "two";
    numberNames[1] = "one";
    numberNames[0] = "zero";
}

string convert_number_names(unsigned char input) 
{
    string output = "";
    for(map<unsigned char, string>::reverse_iterator iterator = numberNames.rbegin(); iterator != numberNames.rend(); iterator++) {
        unsigned char key=iterator->first;
        //cout << "checking " << to_string(input) << " against " << to_string(key) << endl;
        if (key < input) {
            //cout << "submatched " << to_string(input) << " against " << to_string(key) << endl;
            return iterator->second + " " + convert_number_names(input - key);
        }
        else if (key == input) {
            //cout << "matched " << to_string(input) << " against " << to_string(key) << endl;
            return iterator->second;
        }
    }
    cerr << "cannot convert " << to_string(input) << endl;
    exit(EXIT_FAILURE);
    return "This never returns.";
}

string gen_string(unsigned char counts[NUMCHARS])
{
    string output = "";
    char a = 'a';

    for (unsigned char i=0; i<NUMCHARS; i++) {
        output.append(convert_number_names(counts[i]));
        output.append(" ");
        output.append(1,(a + i));
        output.append("'s, ");
    }
    return output;
}

bool count_chars(string input, unsigned char counts[NUMCHARS])
{
    string::iterator it = input.begin();
    int index = 0;
    char charIndex;

    for (index=0; index < NUMCHARS; index++) 
        counts[index]=0;

    for (it = input.begin() ; it < input.end(); ++it) {
        charIndex = tolower(*it) - 'a';
        if ((charIndex >= 0) && (charIndex <= 25 )) {
            counts[charIndex]++;
            //cout << to_string(charIndex) << " " << *it << endl;
        }
    }

    return (--counter);
}

int main(void)
{
    string prefix="Joe Boyle's challenge is to construct a sentence which contains ";
    unsigned char alphaCount[NUMCHARS] = { 0 };
    string testString;
    counter=10;

    init_number_names();

    do {
        testString = prefix + gen_string(alphaCount);
        cout << testString << endl;
    } while (count_chars(testString, alphaCount));

    exit(EXIT_SUCCESS);
}
