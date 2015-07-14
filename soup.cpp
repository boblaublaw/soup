#include <iostream>
#include <string>       // string
#include <ctype.h>      // tolower
#include <map>          // map

#include <unistd.h>

using namespace std;
#define NUMCHARS 26
#define MAXCHARS 100

map<unsigned char,string> initialNames;
map<unsigned char,string> numberNames;
int counter;

string convert_number_names(unsigned char input) 
{
    string output = "";
    for(map<unsigned char, string>::reverse_iterator iterator = initialNames.rbegin(); iterator != initialNames.rend(); iterator++) {
        unsigned char key=iterator->first;
        if (key < input) {
            //cout << "submatched " << to_string(input) << " against " << to_string(key) << endl;
            return iterator->second + " " + convert_number_names(input - key);
        }
        else if (key == input) {
            //cout << "matched " << to_string(input) << " against " << to_string(key) << " yields " << iterator->second << endl;
            return iterator->second;
        }
    }
    cerr << "cannot convert " << to_string(input) << endl;
    exit(EXIT_FAILURE);
    return "This never returns.";
}

void init_number_names(void)
{
    unsigned char index;

    initialNames[90] = "ninety";
    initialNames[80] = "eighty";
    initialNames[70] = "seventy";
    initialNames[60] = "sixty";
    initialNames[50] = "fifty";
    initialNames[40] = "forty";
    initialNames[30] = "thirty";
    initialNames[20] = "twenty";
    initialNames[19] = "nineteen";
    initialNames[18] = "eighteen";
    initialNames[17] = "seventeen";
    initialNames[16] = "sixteen";
    initialNames[15] = "fifteen";
    initialNames[14] = "fourteen";
    initialNames[13] = "thirteen";
    initialNames[12] = "twelve";
    initialNames[11] = "eleven";
    initialNames[10] = "ten";
    initialNames[9] = "nine";
    initialNames[8] = "eight";
    initialNames[7] = "seven";
    initialNames[6] = "six";
    initialNames[5] = "five";
    initialNames[4] = "four";
    initialNames[3] = "three";
    initialNames[2] = "two";
    initialNames[1] = "one";
    initialNames[0] = "zero";

    for (index=0; index < MAXCHARS; index++) {
        numberNames[index] = convert_number_names(index);
        //cout << "initialized " << to_string(index) << " to " << numberNames[index] << endl;
    }
}

string gen_string(unsigned char counts[NUMCHARS])
{
    string output = "";
    char a = 'a';
    unsigned char i;

    for (i=0; i<NUMCHARS-1; i++) {
        if (counts[i] >= MAXCHARS) {
            cerr << "count of " << counts[i] << " has exceeded " << MAXCHARS << endl;
            exit(EXIT_FAILURE);
        }
        output.append(numberNames[counts[i]]);
        output.append(" ");
        output.append(1,(a + i));
        output.append("'s, ");
    }
    output.append("and ");
    i=NUMCHARS-1;
    output.append(numberNames[counts[i]]);
    output.append(" ");
    output.append(1,(a + i));
    output.append("'s.");
    
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
    counter=2;

    init_number_names();

    do {
        testString = prefix + gen_string(alphaCount);
        cout << testString << endl;
    } while (count_chars(testString, alphaCount));

    exit(EXIT_SUCCESS);
}
