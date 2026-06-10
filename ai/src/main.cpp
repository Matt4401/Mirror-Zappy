#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <string>

int main(int argc, char *argv[]) {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) return 1;
    path[count] = '\0';
    
    std::string dir = dirname(path);
    std::string scriptPath = "\"" + dir + "/ai/src/zappy_ai.py\"";
    std::string command = "python3 " + scriptPath;
    for (int i = 1; i < argc; ++i) {
        command += " ";
        command += argv[i];
    }
    
    return std::system(command.c_str());
}