#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "includes/rapidjson/stringbuffer.h"
#include "includes/rapidjson/document.h"
#include "includes/rapidjson/writer.h"

using namespace rapidjson;

using namespace std;

void cutVideo(std::vector<std::string> &inputFiles);
std::string exec(char* cmd);
vector <string> split(const string& str, const string& delimiter);
int separate_console(void);
void explorerInterface (int argc, char* argv[]);
void consoleInterface (int argc, char* argv[]);
void cutVideo(int numFiles, char* files[]);

int main(int argc, char* argv[]) {
    if (separate_console()) {
        explorerInterface(argc, argv);
    } else {
        consoleInterface(argc, argv);
    }
    return 0;
}

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

vector <string> split(const string& str, const string& delimiter) {
    vector <string> tokens;

    string::size_type lastPos = 0;
    string::size_type pos = str.find(delimiter, lastPos);

    while (string::npos != pos) {
        // Found a token, add it to the vector.
        cout << str.substr(lastPos, pos - lastPos) << endl;
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.size();
        pos = str.find(delimiter, lastPos);
    }

    tokens.push_back(str.substr(lastPos, str.size() - lastPos));
    return tokens;
}


#include <windows.h>
/** Detects if the program has been executed from a separate console(FALSE), or from explorer(TRUE)
IMPORTANT: This function needs to be called before any cout is done.
http://stackoverflow.com/a/513574**/
int separate_console(void){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE), &csbi)){
        printf( "GetConsoleScreenBufferInfo failed: %lu\n", GetLastError());
        return FALSE;
    }
    return ((!csbi.dwCursorPosition.X) && (!csbi.dwCursorPosition.Y));
}

void explorerInterface (int argc, char* argv[]) {
    std::vector<std::string> inputFiles;
    if (argc>1) {
        inputFiles.reserve(argc-1);
        for (int filei=1; filei<argc; ++filei) {
            inputFiles[filei-1]=argv[filei];
        }
    }
    cout << "Select action to perform on ";
    if (argc==2) {
        char fname[_MAX_FNAME];
        char ext[_MAX_EXT];
        _splitpath(argv[1],NULL,NULL,fname,ext);
        cout <<'"'<< fname<<ext<<"\"."<<endl;
    } else if (argc>2) {
        cout  << argc-1 << " files."<<endl;
    }
    string choiseString;
    int choise;
    do {
        cout << "Options are:"<<endl<<"1: Cut video"<<endl;
        std::getline(std::cin,choiseString);
        choise = choiseString[0]-'0';
    } while (choise<1||choise>1);

    switch (choise) {
        case 1:
            cutVideo(inputFiles);
        break;
    }
}

void consoleInterface (int argc, char* argv[]) {
}

void cutVideo(std::vector<std::string> &inputFiles) {
    double startTime=0;
    double duration=5.2;

    //calculated times for passing to ffmpeg.
    //the time between mark1&mark2 is the part in the beginning of the cut that needs to be re encoded.  the time between mark2
    //and mark 3 is the part that doesn't need to be re encoded, and finally between mark3 and mark4 is the end portion that
    //needs re encoding.
    //if no re encoding is needed in the beginning because startTime is a keyframe then mark1 is set to -1 to indicate that,
    //likewise mark4 may be -1 to indicate endTime is on a keyframe.
    //mark3&4 may be -1 if the whole clip needs to be re encoded because it doesn't even contain 2 keyframes.
    double mark1;
    double mark2;
    double mark3;
    double mark4;

    /*
    cout << "Enter start-time:";
    getline(cin, startTime);
    cout << "Enter duration:";
    getline(cin, duration);
    */

    for (int filei=inputFiles.size(); filei>=0; --filei) {
        string command=string("ffprobe -loglevel quiet -print_format json -select_streams v -show_frames ")
            +inputFiles[filei]+string(" 2>&1");


        //cout << command << endl;
        string result=exec ((char *)command.c_str());

        //debug start
        std::ofstream out("output.txt");
        out << result;
        out.close();
        //debug end

        Document document;
        document.Parse(result.c_str());
        assert(document.IsObject());
        assert(document.HasMember("frames"));
        const Value& frames = document["frames"];
        assert(frames.IsArray());
        for (SizeType i = 0; i < frames.Size(); i++) {
            const Value& frame = frames[i];
            //cout << frames[i]pkt_pos
        }


    }

}
