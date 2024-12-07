#include <bits/stdc++.h>
using namespace std;

bool ischar(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool doubleoperators(string s)
{
    string operators[] = {"<<", ">>", "==", "&&", "||", "++", "+=", "-=", "*=", "/=", "!=", ">=", "<="};
    return find(begin(operators), end(operators), s) != end(operators);
}

bool ispunctuation(char c)
{
    string punctuations = ",.!;:'\"()[]{}&_/$#@|~^`";
    return punctuations.find(c) != string::npos;
}

bool isoperator(char c)
{
    string operators = "+-*/%=><&|!^?:.";
    return operators.find(c) != string::npos;
}

bool iskeyword(string s)
{
    string keywords[] =
    {
        "auto", "bool", "break", "case", "catch", "char", "class", "const",
        "continue", "default", "delete", "do", "double", "else", "enum", "explicit",
        "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline",
        "int", "long", "mutable", "namespace", "new", "operator", "private", "protected",
        "public", "register", "return", "short", "signed", "sizeof", "static", "struct",
        "switch", "template", "this", "throw", "true", "try", "typedef", "typeid",
        "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "while",
        "cout", "cin", "endl"
    };

    return find(begin(keywords), end(keywords), s) != end(keywords);
}

pair<bool,string> isvalidvariable(string s)
{
    string reason;

    if(iskeyword(s))
    {
        reason = "Variable name cannot be a keyword name";
        return make_pair(false,reason);
    }
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i]=='*' || s[i]=='+' || s[i]=='*' || s[i]=='/')
        {
            reason = "variable name can not contains operator ";
            return make_pair(false,reason);
        }
        else if(((s[i]>='A' && s[i] <= 'Z')==false && (s[i]>='a' && s[i]<='z')==false) &&
                ((s[i]>='0' && s[i] <= '9'))==false && s[i] != '_')
        {
            reason = "Special Character Found ";
            return make_pair(false,reason);
        }
    }
    if((s[0]>='a' && s[0] <= 'z')==false && (s[0]>='A' && s[0]<='Z')==false)
    {
        reason = "Number can't be in the starting point";
        return make_pair(false,reason);
    }
    else return make_pair(true,reason);
}

bool isconstant(const string& str)
{
    return (str[0] == '"' && str[str.size() - 1] == '"') || (str[0] == '\'' && str[str.size() - 1] == '\'') ||
           (all_of(str.begin(), str.end(), ::isdigit));
}

int main()
{
    ifstream file("sourceFile.txt");

    if (!file.is_open())
    {
        cerr << "Error (Can not open the file)" << endl;
        return 1;
    }

    string datatypes [] = {"int", "double", "float", "char", "long", "short"};
    string line;
    map<string, int> variables;
    map<string, int> identifiermap;
    vector<string> keywords;
    vector<string> identifiers;
    vector<string> constants;
    vector<char> punctuations;
    vector<string> operators;
    vector<string> errorMessages;

    while (getline(file, line))
    {
        istringstream iss(line);

        string word;
        vector<string> words;

        while(iss >> word)
        {
            words.push_back(word);
        }

        for(int i = 0; i < words.size(); i++)
        {
            if(words[i]=="#include" || words[i]=="using" || words[i]=="#define" || words[i].size()==0)
            {
                words.clear();
                break;
            }

            if(ischar(words[i][0]))
            {
                bool datatypeflag = false;
                for(auto &datatype : datatypes)
                {

                    if(datatype == words[i])
                    {
                        datatypeflag = true;
                        int sizee = words[i+1].size();

                        if(words[i+1][sizee-1] == ')')
                        {
                            words[i+1].pop_back();
                            words[i+1].pop_back();
                        }
                        auto isvalidvar = isvalidvariable(words[i+1]);

                        if(isvalidvar.first == true)
                        {
                            variables[words[i+1]]++;
                            i++;
                            break;
                        }
                        else
                        {
                            errorMessages.push_back(isvalidvar.second + ": " + words[i+1]);
                        }
                    }
                }
                if(iskeyword(words[i]))break;

                if(variables[words[i]]>0)break;

                else if(!datatypeflag)
                {
                    if (words[i] == "itn") {
                        errorMessages.push_back("Invalid Identifier: " + words[i]);
                        continue;
                    }
                    errorMessages.push_back("Undefined Element: " + words[i+1]);
                }
            }
            else if(iskeyword(words[i]))continue;
        }
    }

    ifstream file2("sourceFile.txt");

    if (!file2.is_open())
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    while (getline(file2, line))
    {
        istringstream iss(line);
        string word;

        while (iss >> word)
        {
            if (word == "#include" || word == "using" || word == "#define" || word.size() == 0)
            {
                break;
            }

            if(word.back()==';')
            {
                punctuations.push_back(word.back());
                word.pop_back();
            }

            if (doubleoperators(word))
            {
                operators.push_back(word);
            }
            else if (iskeyword(word))
            {
                keywords.push_back(word);
            }
            else if (ischar(word[0]))
            {
                if(identifiermap[word]<1)
                {
                    identifiers.push_back(word);
                    identifiermap[word]++;
                    if(word.back() == ')')
                    {
                        punctuations.push_back(word[word.size()-2]);
                        punctuations.push_back(word.back());
                    }
                }

            }
            else if (isconstant(word))
            {
                constants.push_back(word);
            }
            else
            {
                for (char c : word)
                {
                    if (ispunctuation(c))
                    {
                        punctuations.push_back(c);}
                    else if (isoperator(c))
                    {
                        string op(1, c);
                        operators.push_back(op);
                    }
                }
            }
        }
    }

    cout << "Errors:" << endl;
    for (const auto& errorMsg : errorMessages)
    {
        cout << errorMsg << endl;
    }

    cout << "Identifiers:" << endl;
    for (const auto& id : identifiers)
    {
        cout << id << endl;
    }

    cout << "Keywords:" << endl;
    for (const auto& keyword : keywords)
    {
        cout << keyword << endl;
    }

    cout << "Constants:" << endl;
    for (const auto& constant : constants)
    {
        cout << constant << endl;
    }

    cout << "Punctuations:" << endl;
    for (const auto& punctuation : punctuations)
    {
        cout << punctuation << endl;
    }

    cout << "Operators:" << endl;
    for (const auto& op : operators)
    {
        cout << op << endl;
    }

    getchar();
}
