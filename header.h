#include<bits/stdc++.h>

using namespace std;

//We have numbered the functions to make it easier to follow them

typedef vector<string> vstring;

class CResponse; //defining our response class

typedef map<string,vector<CResponse>> mapString;

struct transpose //transpose list
{
    string first;
    string second;
};

class CBot
{
public:
    CBot(string str)    //constructors with parameters
        :botName(str), botInput("")//initialization list
    {
        loadDatabase();
        srand((unsigned) time(NULL)); //26
    }
    ~CBot() {};//empty body in destructor

    void get_input(string input);
    string respond();
    string welcome();

//variable names
public:
    string	botName;
    string	userName;
    string	botInput;
    string	botResponse;
    string	prevInput;
    string	prevResponse;
    string	event;
    string	prevEvent;
    string	botInputBackup;
    string  subject;       //subject for transposing
    string  tempkeyWord;
    string Context;
    string prevContext;


    vstring			response_list;
    vstring         unknown_input_list; //contains all the inputs the bot doesn't understand
    mapString		KnowledgeBase;      //database
    stack<string>   Response_Log;       //stores responses of the bot
    fstream         Log_file;           //saves our conversations

public:
    void find_match();
    void handle_repetition();
    void handle_user_repetition();
    void handle_event(string str);

    void select_response() //FUNCTION 19 //chooses a response randomly
    {
        int index;
        for(int i=0; i<response_list.size(); i++)
        {
            index = rand()%response_list.size();
            swap(response_list[i], response_list[index]); //shuffles responses
        }
        botResponse = response_list[0];
    }

    void save_prev_input()//FUNCTION 1
    {
        prevInput = botInput;
    }

    void save_prev_response()//FUNCTION 3
    {
        prevResponse = botResponse;
    }

    void save_prev_event()//FUNCTION 7
    {
        prevEvent = event;
    }

    void save_input() //FUNCTION 9 //saving our input as backup to handle repetition issues
    {
        botInputBackup = botInput;
    }

    void set_input(string str) //FUNCTION 10
    {
        botInput = str;
    }

    void set_event(string str)//FUNCTION 4 FUNCTION 8
    {
        event = str;
    }

    void restore_input() //FUNCTION 12
    {
        botInput = botInputBackup;
    }

    string print_response() const //FUNCTION 24
    {
        if(botResponse.size()>0)
        {
            string phase=botResponse;
            return botResponse;
        }
    }

    void Text_to_Speech()
    {
        if(botResponse.size()>0)
        {
            string phase=botResponse;
            string command="espeak -v +f2 \""+phase+"\""; //Voice recognition feature using eSpeak
            const char* char_command=command.c_str();
            system(char_command);
        }
    }

    void preprocess()//FUNCTION 2
    {
        cleanString(botInput);//removes extra spaces and punctuation marks
        trimRight(botInput,". ");
        uppercase(botInput);

    }
    bool bot_repeat()  //checks bot repetition
    {
        int pos = find_Response_Pos(botResponse);  // finds the position of the selected response in the response log of the bot
                                                  //it returns true if the response was said by the bot recently
        if(pos >=0)
        {
            return ((pos + 1) <= response_list.size());
        }
        return false;

    }
    bool user_repeat()  //FUNCTION 14  //checks user repetition
    {
        string temp1=" "+prevInput+" ";
        string temp2=" "+botInput+" ";
        return (temp1.length() > 0 && ((temp1 == temp2) || (temp2.find(temp1) != string::npos) || (temp1.find(temp2) != string::npos)));
    }

    bool bot_understand() const //FUNCTION 18 //checks whether response is present in the list
    {
        return response_list.size()>0;
    }

    bool null_input() const //FUNCTION 5
    {
        return (botInput.length() == 0 && prevInput.length()!=0);
    }
    bool null_input_repetition() const //FUNCTION 13
    {
        return(botInput.length() == 0 && prevInput.length()==0);
    }
    bool same_event() const //FUNCTION 11
    {
        return(event.size()>0 && event == prevEvent);
    }

    bool same_input() const //FUNCTION 25
    {
        return(botInput.size()>0 && botInput == prevInput); //if current input is exactly same to previous input
    }

    bool no_response() const //FUNCTION 23
    {
        return response_list.size() == 0;
    }
    bool similar_input() const //FUNCTION 16 //if current input is similar to previous input
    {
        return(botInput.size()>0 &&(botInput.find(prevInput)!=string::npos || prevInput.find(botInput)!=string::npos));
    }

    void save_response() ///
    {
        if(!botResponse.empty())
            Response_Log.push(botResponse);
    }

     void update_unkown_input_list()       // stores the list of unknown inputs that bot didn't understand
    {
        unknown_input_list.push_back(botInput);

    }

    //Function Prototyping
    void cleanString(string &str);
    void uppercase(string &str);
    void copy(char *arr[], vstring &v, size_t arrSize);
    size_t replace(string &str, string substr1, string substr2);
    void trimRight(string &str, string delim);
    void trimLeft(string &str, string delim);
    void trimBoth(string &str, string characters);

    //For template response
    void find_subject();
    void transpose(string &str);
    void prepareTemplate();

    //ALL NEW FUNCTIONS
    void tokenize(const string str, vstring &v);
    string sub_phrase(vstring wordList, size_t start_pos, size_t end_pos);
    string findBestKey(vstring v);                       // finds best keyword from the database that matches with the input
    void preProcess_KeyWord(string &str, size_t start_pos, size_t end_pos, size_t Size);
    void loadDatabase();
    void extract_RespList(vector<CResponse> objList);
    int find_Response_Pos(string str);                // finds the position of the response in the response log
    void unknown_input_load();                       // saves the unknown inputs in a file
    void Save_log();                                // saves the log time
    void Save_log(string str);                     // saves the log

};

//Response Class
class CResponse
{
public:
    CResponse() {}
    ~CResponse() {}

    void addContext(string str)
    {
        ListOfContext.push_back(str);
    }

    void addResp(string str)
    {
        ListOfResp.push_back(str);
    }

    vstring getContextList() const
    {
        return ListOfContext;
    }

    vstring getRespList() const
    {
        return ListOfResp;
    }

    void clear()
    {
        ListOfResp.clear();
        ListOfContext.clear();
    }

public:
    vstring ListOfContext;
    vstring ListOfResp;
};



