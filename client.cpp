#define PORT 8083
#define MAX_SIZE 20480
#define MOD 23
#define G 9

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h> 
#include <vector>
#include "rc4.h"

using namespace std;

void processSpecialCommands(string msgRead);
vector <string> splitWord(string &s, char delimiter);

// This class implemented by me proivdes me a high level interface to network as a client:
class Client{
    int  sockfd;
    struct sockaddr_in saddr;

public:

    string name;
    int x;    
    int ckey;
    string private_key;
    
    Client(string server_address,int server_port){
        //Initialise the socket:
  
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sockfd < 0){
            cout << "Error creating the socket"<<endl;
            exit(EXIT_FAILURE);
        }
        
        //Set the address:
        memset(&saddr, '\0', sizeof(sockaddr));
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(server_port);

        if(inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr)<=0){
            cout << "Invalid Address"<< endl;
            exit(EXIT_FAILURE);
        }
    }

    void connectnow(){
        //Start Connection:
        if(connect(sockfd, (sockaddr *)&saddr, sizeof(sockaddr)) < 0){
            cout << "Connection Failed" << endl;
            exit(EXIT_FAILURE);
        }
    }


    string readMsg(){
        string msg = "";
        char last_character = '-';

        //This is a part of my protocol
        do{
            char buffer[2048] = {0};
            read( sockfd , buffer, 2048);
            string s(buffer);
            msg += s;
            last_character = s[s.size()-1];
        }while(last_character!='%');

        return msg.substr(0, msg.size()-1);
    }

    bool sendMsg(string msg){
        // char *msg_pointer = &msg[0];
        // send(sockfd, msg_pointer, sizeof(msg), 0);

        //This protocol is used to send the message as the message is not sent fully by the underlying layer:
        //The message is sent in parts:
        msg += "%";         //Special Terminating Character:
	 
	char buffer[msg.size()+1];
        strcpy(buffer, &msg[0]);
        int bytes_sent_total = 0;
        int bytes_sent_now = 0;
        int message_len = msg.length();
        while (bytes_sent_total < message_len)
        {
            bytes_sent_now = send(sockfd, &buffer[bytes_sent_total], message_len - bytes_sent_total, 0);
            if (bytes_sent_now == -1)
            {
                // Handle error:
                cout <<"Send Msg Error in Sending Protocol" << endl;
                return false;
            }
            bytes_sent_total += bytes_sent_now;
        }
        return true;
    }

    void closenow(){
        close(sockfd);
    }
};



//Global controller variables:
Client myclient = Client("127.0.0.1", PORT);
pthread_t recv_t, send_t;


//Main client logic:
void* receiveMsg(void *arg){
	 //cout << "readbegin\n";
    while(true){
        string msgread = myclient.readMsg();
        if(msgread.size() ==0 )
        continue;
        //cout << msgread << "\n";
        vector<string> parsedCommand = splitWord(msgread, ' ');
        //cout << parsedCommand[0] << "\n";
        
        if(parsedCommand.size()>0 && parsedCommand[0].compare("key:") == 0){
    	//cout << "%$\n";
        string s = parsedCommand[1];
        int id_key = stoi(s);
        int p_key = (int)pow(id_key,myclient.x) % MOD;
        s = to_string(p_key);
        //cout << " secret key : " << s << "\n";
        myclient.private_key = s;
        cout << "(Server): Connected, start chatting\n\n";
        continue;
        //cout << "$%\n";
        }
        
        else if(parsedCommand.size()>0 && parsedCommand[0] != "(server):"){
        
      	string cipher_text = msgread;
        //cout << "decrypting with private_key : " << myclient.private_key << "\n";
       //cout << "key is : " << myclient.private_key[0] << "\n";
       //cout << cipher_text << " ^^\n";
       //cout << myclient.private_key << " **\n";
        string plain_text = decrypt(cipher_text, myclient.private_key);
        msgread = plain_text;
        //cout << "**\n";
        }
      	
        cout << msgread << endl;
        processSpecialCommands(msgread);
    }
    
     //cout << "endbegin\n";
}

void* sendMsg(void *arg){
	//cout << "beginsend\n";
    while(true){
        string msg;
        getline(cin, msg);
        if(msg.size()==0)
        continue;
       	vector<string> parsedCommand = splitWord(msg, ' ');
 
 	if( parsedCommand.size()>0 && parsedCommand[0].compare("status") != 0 && parsedCommand[0].compare("close") != 0 && parsedCommand[0].compare("goodbye") != 0 && parsedCommand[0].compare("connect") != 0 && parsedCommand[0].compare("name+key") != 0){
	string plain_text = "\t(" + myclient.name + "): " + msg;
	//cout << "&&\n";
    //cout << plain_text << "$$\n";
    //cout << myclient.private_key << "\n";
	string cipher_text = encrypt(plain_text,myclient.private_key);
	msg = cipher_text;
	//cout << "**\n";
	}
        myclient.sendMsg(msg);
        //cout << "endsend\n";
        processSpecialCommands(msg);
    }

}

//Helper Functions:
void processSpecialCommands(string msgRead){
    vector<string> parsedCommand = splitWord(msgRead, ' ');
    if(parsedCommand.size()>0 && (parsedCommand[0].compare("close")==0 || parsedCommand[0].compare("\x1b[31mClose:")==0))
        exit(0);
        
}

//Utility Functions:
vector <string> splitWord(string &s, char delimiter){
    vector <string> res;
    string curr;
    for(auto x : s){
        if(x==delimiter){
            res.push_back(curr);
            curr = "";
        }else
        curr += x;
    }
    res.push_back(curr);
    return res;
}




//Signal Handlers:
void exit_handler(int sig){
    myclient.sendMsg("close");
    myclient.closenow();
    exit(0);
}



int main(){

    //Register signal handlers:
    signal(SIGINT, exit_handler); 

    myclient.connectnow();
    cout << "Connected to server..." << endl;
    //cout << "iamhere\n";
    cout << "Enter your username: ";
   
    string myName;
    getline(cin, myName);

    myclient.name = myName;
    
    cout << "Enter your private key from 1 to 10: ";
    cin >> myclient.x;
    
    myclient.ckey = (int)pow(G,myclient.x) % MOD;
    
    string head = "name+key ";
    string s = to_string(myclient.ckey);
    myName += " " + s;
    myName = head + myName;
    myclient.sendMsg(myName);
    
    //cout << myName<< "\n";
    
    //cout << "client name and client key sent\n";

    //Just to confirm that the connection is established:

    pthread_create(&recv_t, NULL, receiveMsg, NULL);
    pthread_create(&send_t, NULL, sendMsg, NULL);

    pthread_join(recv_t, NULL);
    pthread_join(send_t, NULL);

    myclient.closenow();
    return 0;
}
