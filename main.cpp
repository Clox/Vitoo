#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

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


int main(int argc, char* argv[]) {
    string startTime;
    string duration;
    for (int argIterator=1; argIterator<=argc;++argIterator) {
    string command=string("ffprobe -print_format json -select_streams v -show_frames ")+argv[argIterator]+string(" 2>&1");

    string result=exec ((char *)command.c_str());
    std::ofstream out("output.txt");
    out << result;
    out.close();

        cout << "Enter start-time:";
        getline(cin, startTime);
        cout << "Enter duration:";
        getline(cin, duration);
    }

    std::cin.get();
    return 0;
}

