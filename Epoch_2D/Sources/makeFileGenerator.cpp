
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
using namespace std;

/*
* Return the target it it exist
* return NULL otherwise
*/
const char* containTarget(char *str){
    string s(str);
    unsigned int pos=s.find(":");
    if(pos<s.length()){
        return s.substr(0,pos).c_str();
    }
    return NULL;
}

/*
* Return the name (with the extension attached) of the cpp file in the string 
*/
string extractCPP(char * str){
    string s(str);
    unsigned int pos=s.find(":");
    unsigned int end=s.find(".cpp");
    return s.substr(pos+1,end+4-pos);
}

/*
* Return the string freed from the backslash that might have been at the end of it
* If there is no backslash, the string is return unmodified
*/
string removeBackslash(char * str){
    string s(str);
    unsigned int pos=s.find("\\");
    if(pos<s.length()){
        return s.substr(0,pos);
    }
    return s;
}

int main(int argc, char** args){
    
    string libLinking;

    if(argc>1){
        libLinking=*new string(args[1]);
    }

    FILE* file=popen("g++ -MM *.cpp", "r");
    FILE* makefile=fopen("makefile", "w+");
    FILE* temp=fopen("temp", "w+");
    vector<string*> targets;
    vector<string*>::iterator it;
    char actualLine[164];
    const char* target;
    string compilCPP;
    string* sTarget;

    fputs("ALL = Run.exe\n\n\0",makefile);
    fputs("# Targets\n\n\0",makefile);

    //Write the rules returned by the command in a temporary file
    while(fgets(actualLine,164,file)){

        target=containTarget(actualLine);
        if(target!=NULL){
            sTarget=new string(target);
            //We are not interested in the makefileGenerator rule so we don't add it to our targets vector
            if(sTarget->find("makeFileGenerator")>sTarget->length()){
                //write compilCPP
                if(!compilCPP.empty()){
                    fputs(compilCPP.c_str(), temp);
                }
                targets.push_back(new string(target));
                compilCPP= "\n\tg++ -c "+extractCPP(actualLine)+"\n";
                fputs("\n\n\0",temp);
            }
            delete sTarget;
        }
        //We don't want to write anything about the makeFilegenerator
        if(target==NULL || (new string(target))->find("makeFileGenerator")==string::npos){
            fputs(removeBackslash(actualLine).c_str(),temp);
        }        
        
    }
    if(!compilCPP.empty()){
        fputs(compilCPP.c_str(), temp);
    }

    //Write the Run.exe rule a the top of the file
    fputs("\n\nRun.exe : ", makefile);
    for(it=targets.begin();it!=targets.end();++it){
        fputs((*it)->c_str(),makefile);
        fputs(" ",makefile);
    }
    //Write the compilation command right below it
    fputs("\n\tg++", makefile);
    if(argc>1){
        fputs(" -l", makefile);
        fputs(libLinking.c_str(), makefile);
    }
    fputs(" -o Run.exe ", makefile);
    for(it=targets.begin();it!=targets.end();++it){
        fputs((*it)->c_str(),makefile);
        fputs(" ",makefile);
    }

    fclose(temp);
    temp=fopen("temp","r");

    //Write the rest of the rules (from the temp file) into the makefile
    while(fgets(actualLine,164,temp)){
        fputs(actualLine,makefile);
    }
    //Add the clean rule
    fputs("\n\nclean : \n\trm *.o *.exe\0", makefile);

    fclose(temp);
    remove("temp");
    fclose(makefile);
    pclose(file);

}