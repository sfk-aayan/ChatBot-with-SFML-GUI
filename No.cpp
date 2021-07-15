#include "header.h"
#include<bits/stdc++.h>
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
#include<sstream>
#include <SFML/Graphics.hpp>

using namespace std;



//SFML
class Button
{
public:
    Button() {}

    Button(string t, sf::Vector2f size,int charSize, sf::Color bgcolor, sf::Color textColor)
    {
        text.setString(t);
        text.setFillColor(textColor);
        text.setCharacterSize(charSize);

        button.setSize(size);
        button.setFillColor(bgcolor);
    }

    void setFont(sf::Font &font)
    {
        text.setFont(font);
    }

    void setBackColor(sf::Color color)
    {
        button.setFillColor(color);
    }

    void setTextColor(sf::Color color)
    {
        text.setFillColor(color);
    }

    void setPosition(sf::Vector2f pos)
    {
        button.setPosition(pos);

        float xPos = (pos.x + button.getLocalBounds().width / 6) - (text.getLocalBounds().width / 2);
        float yPos = (pos.y + button.getLocalBounds().height / 6) - (text.getLocalBounds().height / 2);

        text.setPosition({ xPos, yPos });
    }

    void drawTo(sf::RenderWindow &window)
    {
        window.draw(button);
        window.draw(text);
    }

    bool isMouseOver(sf::RenderWindow &window)
    {
        float mouseX = sf::Mouse::getPosition(window).x;
        float mouseY = sf::Mouse::getPosition(window).y;

        float btnPosX = button.getPosition().x;
        float btnPosY = button.getPosition().y;

        float btnPosWidth = button.getPosition().x + button.getLocalBounds().width;
        float btnPosHeight = button.getPosition().y + button.getLocalBounds().height;

        if (mouseX <btnPosWidth && mouseX > btnPosX && mouseY < btnPosHeight && mouseY > btnPosY)
            return true;

        return false;
    }

private:
    sf::RectangleShape button;
    sf::Text text;
};

class Textbox{
public:
	Textbox()
	{

	}

	Textbox(int size, sf::Color color, bool sel)
	{
		textbox.setCharacterSize(size);
		textbox.setFillColor(color);
		isSelected = sel;

		if (sel)
		{
			textbox.setString("_");
		}
		else
		{
			textbox.setString("");
		}
	}

	void setFont(sf::Font &font)
	{
		textbox.setFont(font);
	}

	void setPosition(sf::Vector2f pos)
	{
		textbox.setPosition(pos);
	}

	void setLimit(bool ToF)
	{
		hasLimit = ToF;
	}

	void setLimit(bool ToF, int lim)
	{
		hasLimit = ToF;
		limit = lim;
	}

	void setSelected(bool sel)
	{
		isSelected = sel;
		if (!sel)
		{
			string t = text.str();
			string newT = "";
			for (int i = 0; i < t.length(); i++)
			{
				newT += t[i];
			}
			textbox.setString(newT);
		}
	}

	string getText()
	{
		return text.str();
	}

	void drawTo(sf::RenderWindow &window)
	{
		window.draw(textbox);
	}

	void typedOn(sf::Event input)
	{
		if (isSelected)
		{
			int charTyped = input.text.unicode;
			if (charTyped < 128)
			{
				if (hasLimit)
				{
					if (text.str().length() <= limit)
					{
						inputLogic(charTyped);
					}
					else if(text.str().length() > limit && charTyped == DELETE_KEY)
					{
						deleteLastChar();
					}
				}
				else
				{
					inputLogic(charTyped);
				}
			}
		}
	}

private:
	sf::Text textbox;
	ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit;

	void inputLogic(int charTyped)
	{
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
		{
			text << static_cast<char>(charTyped);
		}
		else if(charTyped == DELETE_KEY)
		{
			if (text.str().length() > 0)
			{
				deleteLastChar();
			}
		}
		textbox.setString(text.str() + "_");
	}

	void deleteLastChar()
	{
		string t = text.str();
		string newT = "";
		for (int i = 0; i < t.length() - 1; i++)
		{
			newT += t[i];
		}
		text.str("");
		text << newT;

		textbox.setString(text.str());
	}

};


//Coding the bot
transpose transList[] =
{
    {" MYSELF ", " YOURSELF "},
    {" DREAMS ", " DREAM "},
    {" WEREN'T ", " WASN'T "},
    {" AREN'T ", " AM NOT "},
    {" I'VE ", " YOU'VE "},
    {" MINE ", " YOURS "},
    {" MY ", " YOUR "},
    {" WERE ", " WAS "},
    {" MOM ", " MOTHER "},
    {" I AM ", " YOU ARE "},
    {" I'M ", " YOU'RE "},
    {" DAD ", " FATHER "},
    {" MY ", " YOUR "},
    {" AM ", " ARE "},
    {" I'D ", " YOU'D "},
    {" I ", " YOU "},
    {" ME ", " YOU "}
};

size_t transposeListSize = sizeof(transList) / sizeof(transList[0]);

void CBot::loadDatabase()
{
    fstream fin("BOT.txt",ios::in); //Opening database file for reading
    if(fin.fail())
    {
        cout<<"Unable to load the database: script.txt"<<endl;
    }

    CResponse respObj;

    vector<CResponse> ListOfRespObj;

    string buffer;

    vstring keyList;

    while(getline(fin, buffer))
    {
        char cSymbol = buffer[0]; //storing the first character to distinguish between keywords,contexts & responses
        buffer.erase(0, 1); //erasing the first character in the database
        switch(cSymbol)
        {
        case 'K':
            keyList.push_back(buffer);
            break;
        case 'C':
            respObj.addContext(buffer);
            break;
        case 'R':
            respObj.addResp(buffer);
            break;
        case '%':
            ListOfRespObj.push_back(respObj);
            respObj.clear();
            break;
        case '#':
        {
            if(ListOfRespObj.empty())
            {
                ListOfRespObj.push_back(respObj);
            }

            vstring::const_iterator iter = keyList.begin();
            for( ; iter != keyList.end(); ++iter )
            {
                KnowledgeBase[*iter] = ListOfRespObj;  //maps responses and contexts with the keyword
            }
            keyList.clear();
            ListOfRespObj.clear();
            respObj.clear();
        }
        break;
        }
    }

    fin.close();
}

void CBot::unknown_input_load()
{
    fstream fout("unknown_input.txt",ios::out); //opening the file for writing
    if(fout.fail())
    {
      cout<<"Unable to save Unknown Input List"<<endl;
    }
    vstring::const_iterator it = unknown_input_list.begin();
    for( ; it != unknown_input_list.end(); ++it )
    {
        fout << *it << endl; //writes unknown inputs that bot didn't understand in a text file for future purpose
    }
    fout.flush();
    fout.close();
}

void CBot::Save_log()
{
	time_t ltime;
	time(&ltime);
	Log_file.open("log.txt", ios::out | ios::app); //opening the file for writing and appending
	if(Log_file.fail())
	{
		cout<<"Can't save conversation log"<<endl;
	}
	Log_file << "\n\nConversation log - " << ctime(&ltime) << endl; //writes log time in the log file
}

void CBot::Save_log(string str)
{
	if(str == "CHATBOT")
	{
		Log_file << botResponse << endl; //writes responses of the bot in the file
	}
	else if(str == "USER")
	{
		Log_file << ">" << botInput << endl; //writes inputs of the user in the file
	}
}

//Opening message
void CBot::welcome()
{
    handle_event("SIGNON**");
    select_response();
    Save_log();
    Save_log("CHATBOT");
    print_response();
}

void CBot::get_input() //FUNCTION 28
{
    cout<<"User: ";

    save_prev_input();  //saving the previous input for repeating input state //1
    getline(cin, botInput);

    preprocess();   //getting rid of unnecessary parts of string. //2
}

void CBot::extract_RespList(vector<CResponse> objList)  //gives the response list deciding if there is a context to relate or not
{
    vector<CResponse>::const_iterator it = objList.begin();
    for( ; it != objList.end(); ++it )
    {
        vstring ListContext = it->getContextList();
        if(ListContext.empty())                     //giving normal response list
        {
            response_list = it->getRespList();
        }
        else if(find(ListContext.begin(),ListContext.end(), prevResponse) != ListContext.end()) //giving response list based on context
        {
            response_list = it->getRespList();
            break;
        }
    }
}

void CBot::prepareTemplate()
{
    if(botResponse.find("*") != string::npos)
    {
        find_subject();
        transpose(subject);

        replace(botResponse, "*"," " + subject);
    }
}

void CBot::find_subject()
{
    subject.erase();
    trimRight(botInput, " "); // We will remove the space added during the preprocess of the data.
    trimBoth(tempkeyWord,"_");
    size_t pos = botInput.find(tempkeyWord);
    if(pos != string::npos)
    {
        subject = botInput.substr(pos+tempkeyWord.length()+1, botInput.length()-1);
    }
}

string CBot::sub_phrase(vstring wordList, size_t start_pos, size_t end_pos)  // tries to find sub phrases of the input so that it can find best keyword in the database
{
    string buffer;
    for(int i = start_pos; i < end_pos; ++i)
    {
        buffer += wordList[i];  //extracting word from wordlist to the buffer
        if(i != end_pos - 1)
        {
            buffer += " ";      //adding space between the words
        }
    }
    return buffer;
}

void CBot::preProcess_KeyWord(string &str, size_t start_pos, size_t end_pos, size_t iSize) // edit the keywords to match with the database
{
    if(start_pos == 0)
    {
        str.insert(0, "_");
    }
    if(end_pos == iSize - 1)
    {
        str.insert(str.length(), "_");
    }
}

void CBot::transpose(string &str)   //changing the pronoun to make sense of the output
{
    bool isTransposed = false;
    string buffer =" " + str + " ";

    for(int i=0; i<transposeListSize; i++)  //transposing from left to right from the transpose list
    {
        string prevPronoun = transList[i].first;
        string transposedPronoun = transList[i].second;
        size_t pos = replace(buffer, prevPronoun, transposedPronoun);
        if(pos != string::npos)
        {
            isTransposed = true;
        }

    }

    if(!isTransposed)                   //If pronoun not matched then transposing from right to left
    {
        for(int i=0; i<transposeListSize; i++)
        {
            string prevPronoun = transList[i].first;
            string transposedPronoun = transList[i].second;
            size_t pos = replace(buffer, transposedPronoun, prevPronoun);
        }
    }
    trimBoth(buffer," ");
    str=buffer;
}

void CBot::tokenize(const string str, vstring &v)   //extracting only the words from the input into the vector.
{
    string buffer;
    for(int i = 0; i < str.length(); ++i)
    {
        if(!ispunct(str[i]) && !isspace(str[i]) && str[i] != '.')
        {
            buffer += str[i];
        }
        else if(!buffer.empty())
        {
            v.push_back(buffer);
            buffer.erase();
        }
    }
    if((v.empty() && !buffer.empty()) || !buffer.empty())
    {
        v.push_back(buffer);
    }
}

string CBot::findBestKey(vstring v)       // finds best keyword from the database that matches with the input
{
    string buffer;
    int Size = v.size();
    bool best_KeyFound = 0;
    for( int i = 0, j = Size; i < Size && j >= 1; ++i, --j )
    {
        for( int k = 0; (k + j) <= Size; ++k )
        {
            buffer = sub_phrase(v, k, k + j);       //go up damn it
            if(KnowledgeBase.find(buffer) != KnowledgeBase.end())
            {
                best_KeyFound = 1;
            }
            else
            {
                preProcess_KeyWord(buffer, k, k + j, Size); //go up again -_-
                if(KnowledgeBase.find(buffer) != KnowledgeBase.end())
                {
                    best_KeyFound = 1;
                }
            }
            if(best_KeyFound)
            {
                return buffer;
            }
        }
    }
    return buffer;
}

//handles bot response and checks the various conditions to satisfy
void CBot::respond() //FUNCTION 29
{
    save_prev_response(); //3
    set_event("BOT UNDERSTAND**");  //4

    if(null_input()) //5
    {
        handle_event("NULL INPUT**");
    }
    else if(null_input_repetition()) //13
    {
        handle_event("NULL INPUT REPETITION**");
    }
    else if(user_repeat()) //14
    {
        handle_user_repetition(); //15
    }
    else
    {
        find_match();
    }


    if(user_want_to_quit()) //17
    {
        quitProgram = 1;
    }

    if(!bot_understand()) //18
    {
        handle_event("BOT DONT UNDERSTAND**");
        update_unkown_input_list();
    }

    if(response_list.size()>0)
    {
        select_response(); //19
        prepareTemplate();

        //makes bot look smarter as it doesn't repeat outputs
        if(bot_repeat()) //21
        {
            handle_repetition(); //22
        }
        Save_log("CHATBOT");
        print_response(); //24
        save_response();
    }
}

//handles bot repetition
void CBot::handle_repetition() ///
{
    map<int,string>PrevResponse_list;
    if(response_list.size()>1)
    {
        vstring::const_iterator it1=response_list.begin();
        for(;it1!=response_list.end(); ++it1)
        {
            int pos=find_Response_Pos(*it1); //finds position of a response in the response log (stack) of the bot , it means we start checking from the end response
            PrevResponse_list[pos]=*it1; // stores the response in the previous response list (map) at the position that we found before
        }
    }
    map<int,string> :: const_iterator it2=PrevResponse_list.end();   //pointing at the end  of previous responses list
    --it2;
    botResponse=it2->second;     //we store the response which was not used recently
}

void CBot::handle_user_repetition()  // FUNCTION 15
{

    if(same_input()) //25
    {
        handle_event("REPETITION T1**");
    }
    else if(similar_input()) //16
    {
        handle_event("REPETITION T2**");
    }
}

//handles all events
void CBot::handle_event(string str) //FUNCTION 6
{
    save_prev_event(); //7
    set_event(str); //8

    save_input(); //9


    set_input(str); //10 //input is set as str

    if(!same_event()) //11
    {
        find_match();
    }

    restore_input(); //12 //restores original input
}

void CBot::uppercase(string &str)
{
    int len = str.size();

    for(int i=0; i<len; i++)
    {
        str[i] = toupper(str[i]);
    }
}

void CBot::cleanString(string &str )
{
    int l=str.length();
    string temp=""; //edited string is stored here
    char prev_char=0;
    for(int i=0; i<l; i++)
    {
        if(str[i]==' ' && prev_char==' ') //skipping double or more spaces
            continue;
        else if(!ispunct(str[i])) //if the string is okay, the character will be stored in the temp variable
            temp+=str[i];
        else if(ispunct(str[i])) //if punctuation is present, it will be converted to a space in the edited string
        {
            temp+=" ";
            str[i]=' ';
        }
        prev_char=str[i];
    }
    str=temp; //edited string is copied to original string
}


//used to copy data from our inputs and responses structure to a string vector
void CBot::copy(char *arr[], vstring &v, size_t arrSize)
{
    for(int i=0; i<arrSize; i++)
    {
        //if array is not empty, it will be stored in the string vector
        if(arr[i]!=NULL)
            v.push_back(arr[i]);
        else
            break;
    }
}

size_t CBot::replace(string &str, string substr1, string substr2)
{
    size_t pos = str.find(substr1);
    if(pos!=string::npos)
    {
        str.erase(pos, substr1.length());
        str.insert(pos, substr2);           //replacing the substring.
    }
    return pos;

}

void CBot::trimRight(string &str, string delim)
{
    size_t pos = str.find_last_not_of(delim);   //returns the index from last of the first unmatched character.
    if(pos != string::npos)
    {
        str.erase(pos + 1, str.length()); //erasing that unmatched character
    }
}

void CBot::trimLeft(string &str, string delim)
{

    size_t pos = str.find_first_not_of(delim);
    if(pos != string::npos)
    {
        str.erase(0, pos);
    }
}

void CBot::trimBoth(string &str, string characters)
{
    trimLeft(str, characters);
    trimRight(str, characters);
}

int CBot::find_Response_Pos(string str)        // finds the position of the response in the response log of the bot
    {
        int pos = -1;
        stack<string>s = Response_Log;         // all the responses that were given by the bot are stored in Response_log (as response log is a stack, most recent response of the bot will be stored at the top)
        while(!s.empty())                     //if response log is not empty we try to find the position of the string str in the response log (starting from the end)
        {                                     // it helps to handle repetition of the bot
            ++pos;
            if(s.top() == str)
            {
                break;
            }
            s.pop();
        }
        return pos;
    }

//used to find the keyword to get a suitable response, implementing keyword ranking
void CBot::find_match()
{
    response_list.clear(); // clears the list of responses which was stored earlier for another input
    string bestKeyWord; //for keyword ranking
    vstring ListOfWord;

    if(botInput.find("**") == string::npos) //checks if it is a special case (like null input,repetition etc) or not
    {
        trimRight(botInput, ".");
        uppercase(botInput);
        tokenize(botInput, ListOfWord);
        bestKeyWord = findBestKey(ListOfWord);
        tempkeyWord = bestKeyWord;
    }
    else
    {
        tempkeyWord = botInput;
    }
    if(KnowledgeBase.find(tempkeyWord) != KnowledgeBase.end())
    {
        //tempkeyword is the bot input
        //knowledgebase[tempkeyword] is the mapped value for the keyword (means context & responses for that specific keyword in the database)
        vector<CResponse> ListOfRespObj = KnowledgeBase[tempkeyWord];
        extract_RespList(ListOfRespObj); //tries to find if bot should give context related response or not
    }
}


/*int main()
{
    CBot bot("chatterbot"); //Parameter of constructor
    bot.welcome(); //Opening message
    while(!bot.quit()) //27
    {
        bot.get_input();//28
        bot.Save_log("USER"); // saves the conversation of user in the log file
        bot.respond(); //29
    }

    bot.unknown_input_load(); //updating the unknown input list
    return 0;
}*/



int main()
{
   sf::RenderWindow window(sf::VideoMode(600, 800), "SFML", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    window.setKeyRepeatEnabled(true);

    Textbox textbox1(30, sf::Color::White, true);


    sf::Font font;
    if (!font.loadFromFile("atwriter.ttf"))
    {
        cout << "You messed up in loading file" << endl;
        system("pause");
    }

    //sf::Text text;
    textbox1.setFont(font);
    textbox1.setPosition({ 100, 400 });
    //string yo = "Power!";
    //text.setString(yo);
    Button btn1("Why indeed", { 200, 50 }, 20, sf::Color::Red, sf::Color::White);
    btn1.setPosition({ 300,600 });
    btn1.setFont(font);

    sf::Texture texture;
    if (!texture.loadFromFile("bg.png"))
    {
        cout << "You messed up in loading Texture" << endl;
        system("pause");
    }

    sf::Sprite s(texture);

    while (window.isOpen())
    {
        sf::Event evt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            textbox1.setSelected(true);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            textbox1.setSelected(false);
        }

        while (window.pollEvent(evt))
        {
            if (evt.type == sf::Event::Closed)
                window.close();
            if(evt.type == sf::Event::TextEntered)
                textbox1.typedOn(evt);
            if (evt.type == sf::Event::MouseMoved)
            {
                if (btn1.isMouseOver(window))
                {
                    btn1.setBackColor(sf::Color::White);
                    btn1.setBackColor(sf::Color::Blue);
                }
                else {
                    btn1.setBackColor(sf::Color::Red);
                }
            }

        }

        window.clear();
        window.draw(s);
        btn1.drawTo(window);
        textbox1.drawTo(window);
        window.display();
    }
    return 0;
}







