#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include "Service_Helper.h"

using std::filesystem::recursive_directory_iterator;

bool Service_Helper::isCorrectExtension(std::string name, std::string fileExt) {
    if(name.substr(name.find_last_of(".") + 1) == fileExt) {
        return true;
    }
    else {
        return false;
    }
}

bool Service_Helper::isCorrectStartName(std::string name, std::string startString) {
    if(name.substr(0,name.find_first_of(".")) == startString) {
        return true;
    }
    else {
        return false;
    }
}

bool Service_Helper::hasHexId(std::string name) {
    std::string separator = ".";
    int pos = 0;
    bool found=false;
    std::string token;
    while ((pos = name.find(separator)) != std::string::npos) {
        token = name.substr(0, pos);
        if (std::all_of(token.begin(), token.end(), ::isxdigit)) {
            found=true;
        }
        name.erase(0, pos + separator.length());
    }
    return found;
}

bool Service_Helper::isTriggerName(std::string name) {
    //decides if the filename is valid to create a trigger
    if(isCorrectExtension(name, "lz4") && isCorrectStartName(name, "core") && hasHexId(name)) {
        return true;
    }
    else {
        return false;
    }
}

std::string Service_Helper::generateFileName() {
    time_t timeNow = time(nullptr);
    std::string timeString=std::to_string(timeNow);
    std::string base_name="log_";
    std::string fileExt=".tar ";
    std::string result=base_name.append(timeString);
    result=result.append(fileExt);
    return result;
}

void Service_Helper::createTarFile(std::string searchPath, std::string outputPath) {
    //creates a tar file of the searchPath tree and stores in user-specified outputPath
    std::string fileName=generateFileName(); //get unique name part according to epoch time
    std::string baseCommand1 = "tar cvf "; //system command part to create a tar file
    std::string command = baseCommand1.append(outputPath).append("\\").append(fileName).append(searchPath); //form the command
    std::cout << command << std::endl; //show user what is being executed
    system(command.c_str()); //execute tar file creation
}

void Service_Helper::getMetaData(std::string searchPath) {
    std::string basePath = searchPath;
    std::string logPath=searchPath.append("\\Log_file");
    std::filesystem::path path{logPath};
    path /= "log_file.txt";
    std::filesystem::create_directories(path.parent_path());//add directories based on the object path
    std::ofstream ofs(path);
    for (const auto & file : recursive_directory_iterator(basePath))
       {
           if (file.is_regular_file() && file.path()!=path)
        {
            int size = file.file_size();
            ofs << "File Path: " << file.path() << " ; Size: " << size << " bytes" << std::endl;
        }
    }
    ofs.close();
}
