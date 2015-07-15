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
typedef vector<char> charvec;
typedef charvec::iterator charvec_it;
typedef vector< charvec > charDist;

// globals
map<unsigned char,string> simpleNames;
map<unsigned char,string> numberNames;
charDist cv(MAXCHARS, charvec(NUMCHARS));

inline string dumpvec(charvec x)
{
    string result="";
    charvec_it it;
    for (it=x.begin(); it != x.end(); it++) {
        result.append(to_string(*it) + ",");
    }
    result.pop_back();
    return result;
}

inline void count_chars(string input, charvec &counts)
{
    string::iterator sit = input.begin();
    unsigned char index = 0;
    char charIndex;
    unsigned charCount=0;

    for (charvec_it cit=counts.begin(); cit != counts.end(); cit++)
        *cit=0;
    for (sit = input.begin() ; sit < input.end(); ++sit) {
        charIndex = tolower(*sit) - 'a';
        if ((charIndex >= 0) && (charIndex < NUMCHARS)) {
            counts[charIndex]++;
            //cout << "char " << to_string(distance(input.begin(),sit)) << ":" << *sit << "(" << to_string(charIndex) << ") = " << to_string(counts[charIndex]) << endl;
            charCount++;
        }
    }
    int total=0;
    for (charvec_it cit=counts.begin(); cit != counts.end(); cit++) 
        total+=*cit;

    if (total != charCount) {
        cerr << "input is " << to_string(charCount);
        cerr << " but array total is " << to_string(total) << endl;
        exit(EXIT_FAILURE);
    }
}

string convert_number_names(unsigned char input) 
{
    string output = "";
    for(map<unsigned char, string>::reverse_iterator it = simpleNames.rbegin(); it != simpleNames.rend(); it++) {
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

void init_number_data(void)
{
    unsigned char i;

    // human readable name parts:
    simpleNames[90] = "ninety";
    simpleNames[80] = "eighty";
    simpleNames[70] = "seventy";
    simpleNames[60] = "sixty";
    simpleNames[50] = "fifty";
    simpleNames[40] = "forty";
    simpleNames[30] = "thirty";
    simpleNames[20] = "twenty";
    simpleNames[19] = "nineteen";
    simpleNames[18] = "eighteen";
    simpleNames[17] = "seventeen";
    simpleNames[16] = "sixteen";
    simpleNames[15] = "fifteen";
    simpleNames[14] = "fourteen";
    simpleNames[13] = "thirteen";
    simpleNames[12] = "twelve";
    simpleNames[11] = "eleven";
    simpleNames[10] = "ten";
    simpleNames[9] = "nine";
    simpleNames[8] = "eight";
    simpleNames[7] = "seven";
    simpleNames[6] = "six";
    simpleNames[5] = "five";
    simpleNames[4] = "four";
    simpleNames[3] = "three";
    simpleNames[2] = "two";
    simpleNames[1] = "one";
    simpleNames[0] = "zero";

    // generate compound names
    for (i=0; i< MAXCHARS; i++) 
        numberNames[i] = convert_number_names(i);

    // figure out what each count contributes as a vector
    for (i=0; i < MAXCHARS; i++) {
        count_chars(numberNames[i],cv[i]);
        // we add a count for 's' here because every count
        // includes an 's'. ie - "one X's"
        //                              ^
        //                             /|\
        //                              |
        //                              |
        cv[i]['s' - 'a']++; 
        cout << to_string(i) << ":";
        cout << dumpvec(cv[i]) << ":" << numberNames[i] << endl;
    }
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

void solve(string prefix)
{
    charvec attempt(NUMCHARS);
    charvec result(NUMCHARS);
    charvec prefixCount(NUMCHARS);
    charvec ones(NUMCHARS);
    string sentence;

    for (charvec_it cit=ones.begin(); cit != ones.end(); cit++)
        *cit=1;
    
    count_chars(prefix, prefixCount);
    cout << "prefix:" << dumpvec(prefixCount) << endl;
    sentence = prefix + gen_string(prefixCount);
    count_chars(sentence, attempt);

    do {
        // modify the attempt
       
        // generate the sentence 
        sentence = prefix + gen_string(attempt);

        // gen the histogram
        count_chars(sentence, result);

        cout << dumpvec(attempt) << "-" << dumpvec(result);
        cout << endl; 
        cout << sentence;
        cout << endl; 

    } while (attempt == result);
}

int main(void)
{
    string prefix="Joe Boyle's challenge is to construct a sentence which contains ";
    init_number_data();
    solve(prefix);
    exit(EXIT_SUCCESS);
}
