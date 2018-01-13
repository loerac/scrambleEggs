#include<iostream>
#include<vector>
#include<string>
#include<stdio.h>
#include<fstream>
#include<algorithm>
#include<iomanip>
#include<climits>
using namespace std;

// Functions
int scrambledEggs(string in, long long f);
string scramble(vector<string> l, string p);
bool binary(vector<string>list, string p);
bool intLinear(int arr[], int s, int a);
bool isString(char* str);
long long totalFactorial(string p);
long long factorial(int f);

int main(int argc, char* argv[]){
	if(argc < 2) {
		cout << "\nError: no phrase given: " << argv[0] << " [phrase] [n permutations (optional)]\n" << endl;
		return 2;
	}

	// Variables
	long long count, total;
	string temp;
	string phrase = "";
	ofstream fout;
	
	// Taking the phrase from the arguement and making it uppercase
	for(int i = 1; i < argc; i++) {
		if(!isString(argv[i])) {
			temp = argv[i];
			for(int j = 0; j < temp.size(); j++) { phrase += toupper(temp[j]); }
		}
	}

	// Checking if the user has a valid n permutations
	if((atoi(argv[argc - 1]) < 1) && argc > 3) {
		cout << "\nError: input has to be greater or equal to 1\n" << endl;
		return 2;
	}

	// Finding and display the total possibilities for the phrase
	total = (argc < 3 ? totalFactorial(phrase) : atoi(argv[argc - 1]));
	count = totalFactorial(phrase);
	if(count < total) {
		cout << "\nError: " << total << " is greater than total factorial\n" << endl;
		return 2;
	}
	cout << "Total Possibilites: " << total << "  -  " << phrase << endl;
	cout << "Please wait, this might take a while..." << endl;
	cout << "==================================================" << endl;

	count = scrambledEggs(phrase,total);

	cout << "\nName: " << phrase << endl;
	cout << "Found: " << count << " combinations out of " << total << endl;
	cout << "Check " << temp << " for name ciphers." << endl;
	cout << "==================================================" << endl;

	cout << "           __" << endl;
	cout << "         /  (|" << endl;
	cout << "         \\   \\" << endl;
	cout << "     ___  \\   \\ _________" << endl;
	cout << "   (_____)                  " << endl;
	cout << "  (_______)    Mission" << endl;
	cout << "   (_____)       Complete	" << endl;
	cout << "    (___) _________________\n" << endl;
	//system("PAUSE");
	return 0;
}

// This algorithm works, trust me
// It is messy but that is what I call 04:00 coding
// I will clean it up soon
int scrambledEggs(string in, long long fact){
	int i = 0;
	int index = 0;
	float total = 0;
	float progress;
	ofstream fout;
	string begPhrase = "";
	string endPhrase = "";
	string comPhrase = in;
	string file = in + ".txt";
	string line = "";
	vector<string> foundWords;
	string first, second;
	bool r = true;
	
	// Checking if there is any existing files
	// Add the existing files to the vector and add the total amount found
	ifstream myfile(file.c_str());
	if (myfile.good()) {
		cout << "\n\tLoading other found phrases -- Please wait...\n" << endl;
		while (getline(myfile, line)) {
			foundWords.push_back(line);
			total++;
			comPhrase = line;
		} myfile.close();
	}

	// If the total amount found in the existing file is greater than what the user asked,
	// let user decide if they want to delete or not
	if (total >= fact) {
		cout << file << " already has " << total << endl;
		string resp;
		cout << "Delete existing file and continue [y/n] >> ";
		cin >> resp;
		resp = toupper(resp[0]);

		if(resp != "Y") { return false; }
		if(remove(file.c_str()) != 0) { 
			cout << "\nError: couldn't delete " << file;
			perror(" - Failure\n"); 
		} else {
			cout << "\nDeleted: " << file;
			puts(" - Success\n");
			total = 0;
			comPhrase = in;
		}
	}
	fout.open(file.c_str());

	if (total != 0) { for (i = 0; i < total; i++) { fout << foundWords[i] << endl; }}

	// This where it gets messy
	while (total != fact) {
		endPhrase = ""; 
		if (index % 2 == 0) {
			first = comPhrase[0]; second = comPhrase[1];
			for (i = 2; i < comPhrase.size(); i++) { endPhrase += comPhrase[i]; }
			comPhrase = first + second + endPhrase;
		} else {
			first = comPhrase[1]; second =comPhrase[2];
			for (i = 3; i < comPhrase.size(); i++) { endPhrase += comPhrase[i]; }
			begPhrase = comPhrase[0];
			comPhrase = begPhrase + second + first + endPhrase;
		}
	
		if (binary(foundWords, comPhrase) && (total <= fact)) {
			comPhrase = scramble(foundWords, comPhrase);
			r = false;
		}
		foundWords.push_back(comPhrase);
		sort(foundWords.begin(),foundWords.end());
		fout << comPhrase << endl;
		total++;

		if ((index % 2 == 0) && r) { comPhrase = second + first + endPhrase; }
		else if(r) { comPhrase = second + begPhrase + first + endPhrase; }

		if (binary(foundWords, comPhrase) && r) {
			comPhrase = scramble(foundWords, comPhrase);
			r = false;
		}
		foundWords.push_back(comPhrase);
		sort(foundWords.begin(), foundWords.end());
		fout << comPhrase << endl;
		total++;
	
		if ((index > 0) && (index % 2 != 0) && r) { 
			comPhrase = second + first + begPhrase + endPhrase; }
	
		progress = total / fact;
		cout << "Found: " << comPhrase << " ------------ Completed[" << fixed << setprecision(3) << (progress*100.0) << "%]\r";
		
		if ((foundWords.size() % 6 == 0) && r) {
			endPhrase = "";
			for (i = 0; i < (comPhrase.size() - 2); i++) { endPhrase += comPhrase[i]; }
			begPhrase = comPhrase[comPhrase.size() - 2];
			begPhrase += comPhrase[comPhrase.size() - 1];
			comPhrase = begPhrase + endPhrase;
			index--;
		}
	
		if (r) { index++; }
		r = true;
	} return total;
}

// If there already exists phrase in the vector,
// scramble the phrase into a new phrase
string scramble(vector<string> l, string in){
	string f, s, ep;
	string bp = in;
	string cp = in;
	int c = 0;

	while (binary(l, cp)) {
		if (c >= (cp.size() * 2))
			random_shuffle(cp.begin() , cp.end());
		else {
			ep = "";
			for (int i = 0; i < (cp.size() - 1); i++) { ep += cp[i]; }
			cp = cp[cp.size() - 1];
			cp += ep;
			if (cp == bp) {
				f = cp[cp.size() - 3 ];
				s = cp[cp.size() - 2];
				ep = "";
				for (int i = 0; i < (cp.size() - 3); i++) { ep += cp[i]; }
				cp = cp[cp.size() - 1];
				cp += ep + s + f;
				if (!binary(l, cp)) { return cp; }
				bp = cp;
			}
		} c++;
	} return cp;
}

// Checking if the phrase is already in the vector already
// Used for function: scrambleEggs() and scramble()
bool binary(vector<string>list,string p){
	if (binary_search(list.begin(), list.end(), p)) { return true; }
	return false;
}

// Checking if the integer is already in the array
// Used for function: totalFactorial()
bool intLinear(int arr[], int s, int a) {
	for(int i = 0; i < s; i++) { if(arr[i]==a) { return true; } }
	return false;
}

// Checking if the arguement is a string
// Used for seeing if the user wants a certain amount of scramble eggs 
bool isString(char* str) {
	while(*str != 0) {
		if(!isdigit(*str++)) { return false; }
	} return true;
}

// Checking if there are any multiple characters in phrase
// If there is multiple characters, send it to factorial()
// Used for: main()
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
			if(a==b && !intLinear(arr,c,j)) {
			temp++;
			arr[c]=j;
			c++;
		}
	}
	if(temp!=1) { denominator *= factorial(temp); }
	temp=1;
	} return (factorial(p.size()) / denominator);
}

// Find the total factorial
// Used for function: totalFactorial()
long long factorial(int f) {
	if(f <= 1) { return 1; }
	return f * factorial(f - 1);
}
