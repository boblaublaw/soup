#include <iostream>
#include <string>
#include <map>

#include <unistd.h>

using namespace std;
#define NUMCHARS 26

map<int,string> numberNames;

void init_number_names(void)
{
    numberNames[90]="ninety";
    numberNames[80]="eighty";
    numberNames[70]="seventy";
    numberNames[60]="sixty";
    numberNames[50]="fifty";
    numberNames[40]="forty";
    numberNames[30]="thirty";
    numberNames[20]="twenty";
    numberNames[19]="nineteen";
    numberNames[18]="eighteen";
    numberNames[17]="seventeen";
    numberNames[16]="sixteen";
    numberNames[15]="fifteen";
    numberNames[14]="fourteen";
    numberNames[13]="thirteen";
    numberNames[12]="twelve";
    numberNames[11]="eleven";
    numberNames[10]="ten";
    numberNames[9]="nine";
    numberNames[8]="eight";
    numberNames[7]="seven";
    numberNames[6]="six";
    numberNames[5]="five";
    numberNames[4]="four";
    numberNames[3]="three";
    numberNames[2]="two";
    numberNames[1]="one";
    numberNames[0]="zero";
}

string convert_number_names(unsigned char input) 
{
    return to_string(input);
}

string gen_string(unsigned char counts[NUMCHARS])
{
    string output="";
    char a='a';

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
    return false;
}

int main(void)
{
    string prefix="Joe Boyle's challenge is to construct a sentence which contains ";
    unsigned char alphaCount[NUMCHARS] = { 0 };
    string testString;

    init_number_names();

    do {
        testString = prefix + gen_string(alphaCount);
    } while (count_chars(testString, alphaCount));

    cout << testString << endl;

    exit(EXIT_SUCCESS);
}
