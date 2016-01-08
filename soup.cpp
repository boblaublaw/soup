#include <iostream>
#include <string>       // string
#include <ctype.h>      // tolower
#include <map>          // map
#include <vector>       // vector
#include <unistd.h>     // exit
#include <assert.h>     // assert
using namespace std;

#include <algorithm>
#include <functional>

// http://stackoverflow.com/questions/20181940/most-efficient-way-to-solve-a-system-of-linear-equations

template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(),
       std::back_inserter(result), std::minus<T>());
    return result;
}

template <typename T>
std::vector<T> operator+(const vector<T>& a, const vector<T>& b) {
    assert(a.size() == b.size());

    vector<T> result;
    result.reserve(a.size());

    transform(a.begin(), a.end(), b.begin(), 
                   back_inserter(result), plus<T>());
    return result;
}

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
charvec ones(NUMCHARS,1);
charvec zero(NUMCHARS);

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

charvec make_hist(string input)
{
    string::iterator sit = input.begin();
    char charIndex;
    unsigned charCount=0;
    charvec result(NUMCHARS);

    for (charvec_it cit=result.begin(); cit != result.end(); cit++)
        *cit=0;

    for (sit = input.begin() ; sit < input.end(); ++sit) {
        charIndex = tolower(*sit) - 'a';

        if ((charIndex >= 0) && (charIndex < NUMCHARS)) {
            result[charIndex]++;
            charCount++;
        }
    }

    int total=0;
    for (charvec_it cit=result.begin(); cit != result.end(); cit++)
        total+=*cit;

    if (total != charCount) {
        cerr << "input is " << to_string(charCount);
        cerr << " but array total is " << to_string(total) << endl;
        exit(EXIT_FAILURE);
    }
    return result;
}

string convert_number_names(unsigned char input) 
{
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
    unsigned char i,j;

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
        cv[i] = make_hist(numberNames[i]);
        // we add a count for 's' here because every count
        // includes an 's'. ie - "one X's"
        //                              ^
        //                             /|\
        //                              |
        //                              |
        cv[i]['s' - 'a']++; 
#if 1
        cout << to_string(i) << ":";
        cout << dumpvec(cv[i]) << ":" << numberNames[i] << endl;
#endif
    }
}

inline string gen_string(charvec counts)
{
    string output = "";
    charvec_it it;
#ifdef DEBUG
    for (it=counts.begin(); it!=counts.end(); it++) {
        cout << "index:     " <<             distance(counts.begin(), it) << endl;
        cout << "str index: " << numberNames[distance(counts.begin(), it)] << endl;
        cout << "count:     " <<           + *it  << endl; 
        cout << "str count: " << numberNames[ + *it] << endl; 
        string cs="";
        cs.append(1,'a' + distance(counts.begin(), it));
        cout << "character: " << cs << endl;
        cout << endl;
    }
#endif

    for (it=counts.begin(); it != counts.end()-1; it++) {
        if (*it >= MAXCHARS) {
            cerr << "count of " << *it << " has exceeded " << MAXCHARS << endl;
            exit(EXIT_FAILURE);
        }
        output.append(numberNames[ + *it]);
        output.append(" ");
        output.append(1,'a' + distance(counts.begin(), it));
        output.append("'s, ");
    }

    it = counts.end();
    output.append("and ");
    output.append(numberNames[ + *it]);
    output.append(" z's.");
    return output;
}

void solve(string prefixString)
{
    charvec attempt(NUMCHARS);
    charvec diffHist(NUMCHARS);
    charvec prefixHist(NUMCHARS);
    charvec resultHist(NUMCHARS);
    charvec solutionHist(NUMCHARS);
    string sentence;
    string attemptString;
    string resultString;

    cout << "the prefix string is \"" << prefixString << "\"" << endl;

    prefixHist = make_hist(prefixString);
    cout << "the prefix histogram is " << dumpvec(prefixHist) << endl;

    // make a sentence and determine histogram
    attemptString = gen_string(prefixHist + solutionHist);

    do {
        // modify the attempt
        
        solutionHist = solutionHist + ones;;
        
        //cout << attemptString << endl;
        
        // measure the attempt
        sentence = prefixString + attemptString;
        resultHist = make_hist(sentence);

        diffHist = resultHist - prefixHist - solutionHist;
        
        cout << endl;
        cout << "   result   " << dumpvec(resultHist) << endl;
        cout << " - prefix   " << dumpvec(prefixHist) << endl;
        cout << " - solution " << dumpvec(solutionHist) << endl;
        cout << "            ____________________________________________________________________" << endl;
        cout << " = diff     " << dumpvec(diffHist) << endl;
        cout << endl;

    } while (diffHist != zero);
}

int main(void)
{
    init_number_data();

    string prefix="Joe Boyle's challenge is to construct a sentence which contains ";
    //string prefix = "abcee";
    solve(prefix);

    exit(EXIT_SUCCESS);
}
