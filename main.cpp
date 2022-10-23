#include<bits/stdc++.h>
#include <fstream>
#include <filesystem>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <sstream>
#include<pthread.h>
#include <string.h>
namespace fs = std::filesystem;
using namespace std;
typedef unsigned char byte;

map<string,char*>fileHash;

char* sha1(string filePath){
    const int DataLen = 30;
    SHA_CTX shactx;
    unsigned char digest[SHA_DIGEST_LENGTH];
    int i;
    const char* fPath=filePath.c_str();
    char * buffer = 0;
    long length;
    FILE * f = fopen (fPath, "rb");
    
	if (f)
	{
  		fseek (f, 0, SEEK_END);
  		length = ftell (f);
  		fseek (f, 0, SEEK_SET);
  		buffer = (char*)malloc (length);
  		if (buffer)
  		{
    			fread (buffer, 1, length, f);
  		}
  	fclose (f);
	}
    SHA1_Init(&shactx);
    SHA1_Update(&shactx, buffer, DataLen);
    SHA1_Final(digest, &shactx);
    
    char *sha1_hash=new char[(sizeof digest) + 1];
    memcpy(sha1_hash, digest, sizeof digest);
    sha1_hash[sizeof digest] = 0; 

    return sha1_hash;
}
void init(){
    cout<<"Do you want to create an empty repository? Enter 1 if yes and 0 if No";
    int choice; cin>>choice;
    if(choice==1){
        cout<<"Enter repo name: ";
        string fileName; cin>>fileName;
        fileName+="_v1";
        const char* fName=fileName.c_str();
        mkdir(fName,0777);
    }
    else{
        cout<<"Enter path of directory: ";
        string dirName; cin>>dirName;
        fs::copy(dirName,dirName+"_v1");
        ofstream file1;
        file1.open("FileHashes.txt");
        for (const auto & entry : fs::directory_iterator(dirName)){
            string fileName=entry.path();
            char*hash=sha1(fileName);
            fileHash[fileName]=hash;
            file1<<fileName+" ";
            string fileHash=hash;
            file1<<fileHash+"\n";
        }
        file1.close();
    }
}
void status(){
    cout<<"Enter the directory whose status you want to know: ";
    string dirName; cin>>dirName;
    vector<string>modified,untracked;
    ifstream infile("/home/hussain/Desktop/GitVersionControl/FileHashes.txt");
    for(const auto & entry: fs::directory_iterator(dirName)){
        string fileName=entry.path();
        string fName,fHash;
        infile>>fName>>fHash;
        char* hash=sha1(fileName);
        if(fileName!=fName){
            untracked.push_back(fileName);
        }
        else if(hash!=fHash){
            modified.push_back(fileName);
        }
    }
    cout<<"Modified files:\n";
    for(auto it:modified) cout<<it<<"\n";
    cout<<endl; cout<<"Untracked:\n";
    for(auto it:untracked) cout<<it<<"\n";

    std::fstream myfile;
    myfile.clear();
    ofstream file1;
    file1.open("FileHashes.txt");
    for (const auto & entry : fs::directory_iterator(dirName)){
            string fileName=entry.path();
            char*hash=sha1(fileName);
            fileHash[fileName]=hash;
            file1<<fileName+" ";
            string fileHash=hash;
            file1<<fileHash+"\n";
    }
}
void add(){

}
void commit(){
    cout<<"Enter the directory name that you want to commit: ";
    string dirName; cin>>dirName;
    fs::copy(dirName,dirName+"_latestVersion");
    cout<<"Commit successfull";
}
void diff(){
    cout<<"Enter the filenames between which you want to find the difference:";
    cout<<"Enter filename1: ";
    string file1; cin>>file1;
    cout<<"Enter filename2: ";
    string file2; cin>>file2;
    const char*f1=file1.c_str();
    const char*f2=file2.c_str();
    FILE *fp1=fopen(f1,"r");
    FILE *fp2=fopen(f2,"r");
    char ch1 = getc(fp1);
    char ch2 = getc(fp2);
  
    int error = 0, pos = 0, line = 1;
  
    while (ch1 != EOF && ch2 != EOF)
    {
        pos++;
        if (ch1 == '\n' && ch2 == '\n')
        {
            line++;
            pos = 0;
        }
        if (ch1 != ch2)
        {
            error++;
            printf("Line Number : %d \tChange"
               " Position : %d \n", line, pos);
        }
  
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }
    printf("Total changes : %d\t", error);
    cout<<endl;
}
void push(){
    cout<<"Enter which files you want to push:\n";
    int cnt; cout<<"Enter no of files: "; cin>>cnt;
    for(int i=1;i<=cnt;i++){
        cout<<"Enter file1 path: "; 
        string filePath; cin>>filePath; cout<<endl;
        char cmd[100]; const char* fPath=filePath.c_str();
        strcpy(cmd,"mv ");
        strcat(cmd,fPath);
        strcat(cmd," ");
        strcat(cmd,"/home/hussain/Desktop/GitVersionControl_latestVersion");
        strcat(cmd,"");
        system(cmd);
    }
}
void pull(){
    cout<<"Enter which files you want to pull:\n";
    int cnt; cout<<"Enter no of files: "; cin>>cnt;
    for(int i=1;i<=cnt;i++){
        cout<<"Enter file1 path: "; 
        string filePath; cin>>filePath; cout<<endl;
        const char* fPath=filePath.c_str();
        remove(fPath);
    }
}
int main(int argc,char** argv){
    cout<<"Welcome to Mini Git Version Control System!!!\n";
    while(true){
    cout<<"Enter command: ";
    string command; cin>>command;
        if(command=="init"){
            cout<<"yes";
            init();
        }
        else if(command=="status"){
            status();
        }
        else if(command=="add"){
            add();
        }
        else if(command=="commit"){
            commit();
        }
        else if(command=="diff"){
            diff();
        }
        else if(command=="push"){
            push();
        }
        else if(command=="pull"){
            pull();
        }
        else break;
    }
}
//g++ -std=c++17 main.cpp -Wall -lcrypto -pthread -o git