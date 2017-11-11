#include<iostream>
#include<vector>
#include<string>
#include<stdio.h>
#include<fstream>
#include<algorithm>
#include<iomanip>

using namespace std;

long long totalFactorial(string p);
long long factorial(int f);
int scrambledEggs(string in, long long f);
string scramble(vector<string> l, string p);
bool binary(vector<string>list, string p);
bool linear(int arr[], int s, int a);

int main(){
    string temp; // = "supercalifragilisticexpialidocious";	//catrovacer
    string phrase = "";
    vector<string> list;

    int count;
    long long fact;

    cout << "Enter phrase >> ";
    getline(cin,temp);
    for (int i = 0; i < temp.size(); i++)
	phrase += toupper(temp[i]);
	
    fact = totalFactorial(phrase);
    cout << "There are " << fact << " possibilities for " << phrase << "." << endl;
    cout << "Please wait, this might take a while..." << endl;
    cout << "==================================================" << endl;
    
    count = scrambledEggs(phrase, fact);
    cout << "\nName: " << phrase << endl;
    cout << "Found: " << count << " combinations out of " << fact << endl;
    cout << "Check " << phrase << ".txt for name ciphers." << endl;
    cout << "==================================================" << endl;
    if (count == fact) {
	cout << "           __" << endl;
	cout << "         /  (|" << endl;
	cout << "         \\   \\" << endl;
	cout << "     ___  \\   \\ _________" << endl;
	cout << "   (_____)                  " << endl;
	cout << "  (_______)    Mission" << endl;
	cout << "   (_____)       Complete	" << endl;
	cout << "    (___) _________________\n" << endl;
    } else {
	cout << "    _________________" << endl;
	cout << "  (                  " << endl;
	cout << "  (    Mission" << endl;
	cout << "  (      Incomplete  " << endl;
	cout << "  ( __       ______" << endl;
	cout << "      \\    /" << endl;
	cout << "      /   /" << endl;
	cout << "     /   /" << endl;
	cout << "    (___/" << endl;
    } return 0;
}

// Finding the total factorial of the phrase
long long totalFactorial(string p) {
    char a, b;
    int c = 0;
    int temp = 1;
    int arr[p.size()];
    int denominator = 1;
    
    for(int i = 0; i < p.size(); i++) {
        a = p[i];
        for(int j = (i+1); j < p.size(); j++) {
            b = p[j];
            if(a==b && !linear(arr,c,j)) {
                temp++;
                arr[c]=j;
                c++;
            }
        }
        if(temp!=1)
            denominator *= factorial(temp);
        temp=1;
    } return (factorial(p.size()) / denominator);
}

long long factorial(int f){
    if(f<=1) return 1;
    return f * factorial(f - 1);
}

// Scramble the word around
// The code works but it is really messy
// I don't like it and will fix it soon
int scrambledEggs(string in,long long fact){
    int i = 0;
    int c = 0;
    float total = 0;
    float progress;
    ofstream fout;
    string at = "";
    string bt = "";
    string t = in;
    string file = in + ".txt";
    string line = "";
    vector<string>foundWords;
    string a, b;
    bool r = true;

    ifstream myfile(file.c_str());
    if (myfile.good()) {
        cout << "\n\tLoading other found phrases -- Please wait...\n" << endl;
	while (getline(myfile, line)) {
            foundWords.push_back(line);
            total++;
	    t = line;
        }
	myfile.close();
    }
    fout.open(file.c_str());
    if (total != 0)
	for (i = 0; i < total; i++)
	    fout << foundWords[i] << endl;
	if (total >= fact) return total;
        
	while (total != fact) {
	    bt = ""; 
	    if (c % 2 == 0) {
		a = t[0]; b = t[1];
		for (i = 2; i < t.size(); i++)
		    bt += t[i];
		t = a + b + bt;
	    } else{
		a = t[1]; b=t[2];
		for (i = 3; i < t.size(); i++)
		    bt += t[i];
		at = t[0];
		t = at + b + a + bt;
	    }
	    if (total >= fact) return total;
	    if (!binary(foundWords, t) && (total <= fact)) {
                foundWords.push_back(t);
                sort(foundWords.begin(),foundWords.end());
	        fout << t << endl;
		total++;
	    } else if(foundWords.size()<=fact){
		t = scramble(foundWords, t);
                foundWords.push_back(t);
		sort(foundWords.begin(), foundWords.end());
		fout << t << endl;
		total++;
	        r = false;
	    }
	    if ((c % 2 == 0) && r)
	        t = b + a + bt;
	    else if(r)
		t = b + at + a + bt;
	    if (total >= fact) return total;
	    if (!binary(foundWords, t) && r) {
		foundWords.push_back(t);
		sort(foundWords.begin(), foundWords.end());
		fout << t << endl;
		total++;
	    }
	    else if ((total <= fact) && r) {
		t = scramble(foundWords, t);
		foundWords.push_back(t);
		sort(foundWords.begin(), foundWords.end());
		fout << t << endl;
		total++;
		r = false;
	    }
	    if ((c > 0) && (c % 2 != 0) && r)
		t = b + a + at + bt;
	    progress = total / fact;
	    cout << "Found: " << t << " ------------ Completed[" << fixed << setprecision(3) << (progress*100.0) << "%]\r";
	    if (total >= fact) return total;
	    if ((foundWords.size() % 6 == 0) && r) {
		bt = "";
	        for (i = 0; i < (t.size() - 2); i++)
		    bt += t[i];
	        at = t[t.size() - 2];
	        at += t[t.size() - 1];
	        t = at + bt;
	        c--;
	    }
	    if (r)
		c++;
	    r = true;
    } return total;
}

// If the string is already in the list,
// shift the string to the right once.
// The last char will carry over to the beginning.
string scramble(vector<string> l, string in){
    string a, b, bt;
    string at = in;
    string t = in;
    int c = 0;

    while (binary(l, t)) {
	// If string has been shifted all the possiblitlies,
	// randomly shuffle the string around
 	if (c >= (t.size()))
	    random_shuffle(t.begin() , t.end());
	else {
    	    bt = "";
	    for (int i = 0; i < (t.size() - 1); i++)
		bt += t[i];
	    t = t[t.size() - 1];
	    t += bt;
	    if (t == at) {
		a = t[t.size() - 3 ];
		b = t[t.size() - 2];
		bt = "";
		for (int i = 0; i < (t.size() - 3); i++)
		    bt += t[i];
		t = t[t.size() - 1];
		t += bt + b + a;
		if (!binary(l, t)) return t;
		at = t;
		}
	}
		c++;
    } return t;
}

bool binary(vector<string>list,string p){
    if (binary_search(list.begin(), list.end(), p))
        return true;
    return false;
}

bool linear(int arr[], int s, int a) {
    for(int i = 0; i < s; i++)
        if(arr[i]==a) 
            return true;
    return false;
}
