#include <iostream>
#include <string>       // string
#include <ctype.h>      // tolower
#include <map>          // map
#include <vector>       // vector
#include <unistd.h>
using namespace std;

// consts
#define NUMCHARS 26
#define MAXCHARS 100

// typedefs
typedef vector<unsigned char> charvec;
typedef charvec::iterator charvec_it;

// globals
map<unsigned char,string> initialNames;
map<unsigned char,string> numberNames;

string convert_number_names(unsigned char input) 
{
    string output = "";
    for(map<unsigned char, string>::reverse_iterator it = initialNames.rbegin(); it != initialNames.rend(); it++) {
        unsigned char key=it->first;
        if (key < input) 
            return it->second + " " + convert_number_names(input - key);
        else if (key == input) 
            return it->second;
    }
    cerr << "cannot convert " << to_string(input) << endl;
    exit(EXIT_FAILURE);
    return "This part never returns but I am suppressing a warning with it.";
}

void init_number_names(void)
{
    unsigned char index;

    // human readable name parts:
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

    // fill in the blanks
    for (unsigned char index=0; index < MAXCHARS; index++) 
        numberNames[index] = convert_number_names(index);
}

inline string gen_string(charvec counts)
{
    string output = "";
    charvec_it it;

    for (it=counts.begin(); it != counts.end()-1; it++) {
        if (*it >= MAXCHARS) {
            cerr << "count of " << *it << " has exceeded " << MAXCHARS << endl;
            exit(EXIT_FAILURE);
        }
        output.append(numberNames[counts[*it]]);
        output.append(" ");
        output.append(1,'a' + distance(counts.begin(), it));
        output.append("'s, ");
    }
    it=counts.end();
    output.append("and ");
    output.append(numberNames[counts[*it]]);
    output.append(" ");
    output.append(1,'z');
    output.append("'s.");
    return output;
}

inline void count_chars(string input, charvec counts)
{
    string::iterator sit = input.begin();
    unsigned char index = 0;
    char charIndex;

    for (charvec_it cit=counts.begin(); cit != counts.end(); cit++) 
        *cit=0;
    for (sit = input.begin() ; sit < input.end(); ++sit) {
        charIndex = tolower(*sit) - 'a';
        if ((charIndex >= 0) && (charIndex < NUMCHARS)) 
            counts[charIndex]++;
    }
}

inline string dumpvec(charvec x)
{
    string result="";
    charvec_it it;
    for (it=x.begin(); it != x.end()-1; it++) {
        result.append(to_string(*it) + ",");
    }
    result.append(to_string(*(x.end())));
    return result;
}

void solve(string prefix)
{
    charvec seed(NUMCHARS);
    charvec result(NUMCHARS);
    charvec zero(NUMCHARS);
    string testString;

    //count_chars(prefix, seed);

    do {
        testString = prefix + gen_string(seed);
        count_chars(testString, result);
        cout << dumpvec(seed) << ":" << testString << endl;
    } while (seed != result);
}

int main(void)
{
    string prefix="Joe Boyle's challenge is to construct a sentence which contains ";
    init_number_names();
    solve(prefix);
    exit(EXIT_SUCCESS);
}
