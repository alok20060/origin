#include <bits/stdc++.h>
using namespace std;

vector<string> files;                  
vector<string> all_words;            
unordered_map<string,int> freq;      
queue<string> history;                  
stack<string> undoStack;              

void extractWordsFromFile(const string &filename,vector<string> &all_words,unordered_map<string,int> &freq);
vector<string> searchKeyword(const string &keyword,const vector<string> &files);
vector<string> autoSuggest(const string &prefix,const vector<string> &all_words,const unordered_map<string,int> &freq);

void extractWordsFromFile(const string &filename,vector<string> &all_words,unordered_map<string,int> &freq)
{
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        
        string clean = "";
        for (char c : line) {
            if (isalpha(c))
                clean += tolower(c);
            else
                clean += ' ';
        }

        string word = "";
        for (char c : clean) {
            if (c != ' ')
                word += c;
            else {
                if (!word.empty()) {
                    all_words.push_back(word);
                    freq[word]++;
                    word = "";
                }
            }
        }

        if (!word.empty()) {
            all_words.push_back(word);
            freq[word]++;
        }
    }

    fin.close();
}

vector<string> searchKeyword(const string &keyword,const vector<string> &files)
{
    vector<string> results;
    string key = keyword;

    for (char &c : key)
        c = tolower(c);

    for (const string &fname : files) {
        ifstream fin(fname);
        if (!fin.is_open()) continue;

        string content((istreambuf_iterator<char>(fin)),istreambuf_iterator<char>());

        for (char &c : content)
            c = tolower(c);

        if (content.find(key) != string::npos)
            results.push_back(fname);

        fin.close();
    }

    return results;
}

vector<string> autoSuggest(const string &prefix,const vector<string> &all_words,const unordered_map<string,int> &freq)
{
    vector<string> suggestions;
    string pre = prefix;

    for (char &c : pre)
        c = tolower(c);

    for (const string &word : all_words) {
        if (word.size() >= pre.size() &&word.substr(0, pre.size()) == pre)
        {
            suggestions.push_back(word);
        }
    }

    sort(suggestions.begin(), suggestions.end(),[&](const string &a, const string &b) {
             return freq.at(a) > freq.at(b);
         });

    sort(suggestions.begin(), suggestions.end());
    suggestions.erase(unique(suggestions.begin(),suggestions.end()),suggestions.end());

    if (suggestions.size() > 5)
        suggestions.resize(5);

    return suggestions;
}

int main() {
    int choice;

    while (true) {
        cout<< "\n============================\n";
        cout<< "      MINI SEARCH ENGINE\n";
        cout<< "============================\n";
        cout<< "1. Load and index files\n";
        cout<< "2. Search a keyword\n";
        cout<< "3. Auto-suggest words\n";
        cout<< "4. View search history\n";
        cout<< "5. Undo last search\n";
        cout<< "6. Exit\n";
        cout<< "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input!\n";
            continue;
        }

        switch (choice) {

        case 1: {
            int n;
            cout << "How many files do you want to load? ";
            cin >> n;

            files.clear();       
            all_words.clear();
            freq.clear();

            for (int i = 0; i < n; i++) {
                string fname;
                cout<< "Enter file " << i + 1 << ": ";
                cin>> fname;

                files.push_back(fname);
                extractWordsFromFile(fname, all_words, freq);
            }

            cout<< "Files loaded and indexed successfully!\n";
            break;
        }

            case 2: {
            string keyword;
            cout<< "Enter keyword to search: ";
            cin>>keyword;

            vector<string> results = searchKeyword(keyword, files);

        
            if (results.empty()) {
                cout<< "Keyword not found in any file.\n";
            } else {
                cout<< "Keyword found in:\n";
                for (const string &f : results)
                    cout<< "- " << f << endl;
            }

            
            history.push(keyword);
            undoStack.push(keyword);

            break;
        }

            case 3: {
            string prefix;
            cout<< "Enter prefix: ";
            cin>>prefix;

            vector<string> sug = autoSuggest(prefix, all_words, freq);

            if (sug.empty()) {
                cout<< "No suggestions found.\n";
            } else {
                cout<< "Suggestions:\n";
                for (const string &word : sug)
                    cout<< "- " << word << endl;
            }

            break;
        }

            case 4: {
            if (history.empty()) {
                cout<< "No search history.\n";
            } else {
                cout<< "Search History:\n";
                queue<string> temp = history;

                while (!temp.empty()) {
                    cout<< "- " << temp.front() << endl;
                    temp.pop();
                }
            }
            break;
        }

            case 5: {
            if (undoStack.empty()) {
                cout<< "Nothing to undo.\n";
            } else {
                cout<< "Undoing last search: " << undoStack.top() << endl;
                undoStack.pop();
            }
            break;
        }

            case 6: {
            cout<< "Exiting program...\n";
            return 0;   
        }

        default:
            cout << "Invalid choice. Try again.\n";
        } 
    } 
}
