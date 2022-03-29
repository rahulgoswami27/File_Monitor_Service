#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include "Service_Helper.h"

using std::filesystem::recursive_directory_iterator;

//check if the given file name has the desired extension
bool Service_Helper::isCorrectExtension(std::string name, std::string fileExt) {
    if(name.substr(name.find_last_of(".") + 1) == fileExt) {//extract extension after last "."
        return true;
    }
    else {
        return false;
    }
}

//check if the given filename starts with the desired text
bool Service_Helper::isCorrectStartName(std::string name, std::string startString) {
    if(name.substr(0,name.find_first_of(".")) == startString) {//extract name before first "."
        return true;
    }
    else {
        return false;
    }
}

//Check if a given filename contains anz hexadecimal ID separated by "."s
bool Service_Helper::hasHexId(std::string name) {
    std::string separator = ".";
    int pos = 0;
    bool found=false;
    std::string token;
    while ((pos = name.find(separator)) != std::string::npos) {//search till end of string
        token = name.substr(0, pos);//tokenize till separator occurrence
        if (std::all_of(token.begin(), token.end(), ::isxdigit)) {//check if entire token is onlz Hex digits
            found=true;
        }
        name.erase(0, pos + separator.length());//move on to next part
    }
    return found;
}

/*checks if the given file name has the specifications to raise the trigger
    Trigger file name properties:
    -Starts with core
    -Ends with lz4
    -Contains one or more hexadecimal id separated by dots
*/
bool Service_Helper::isTriggerName(std::string name) {
    //decides if the filename is valid to create a trigger
    if(isCorrectExtension(name, "lz4") && isCorrectStartName(name, "core") && hasHexId(name)) {
        return true;
    }
    else {
        return false;
    }
}

/*function to generate a unique file name containing epoch time and with .tar extension
This function is called only when a trigger file is created in the monitoring directory tree
*/
std::string Service_Helper::generateFileName() {
    time_t timeNow = time(nullptr);
    std::string timeString=std::to_string(timeNow);
    std::string base_name="log_";
    std::string fileExt=".tar ";
    std::string result=base_name.append(timeString);
    result=result.append(fileExt);
    return result;
}

//function to create a tar file of the searchPath tree and stores in user-specified outputPath
void Service_Helper::createTarFile(std::string searchPath, std::string outputPath) {
    std::string fileName=generateFileName(); //get unique name part according to epoch time
    std::string baseCommand1 = "tar cvf "; //system command part to create a tar file
    std::string command = baseCommand1.append(outputPath).append("\\").append(fileName).append(searchPath); //form the command
    std::cout << command << std::endl; //show user what is being executed
    system(command.c_str()); //execute tar file creation
}

//function to obtain disk usage information for all files in the searchPath
void Service_Helper::getMetaData(std::string searchPath) {
    std::string basePath = searchPath;
    std::string logPath=searchPath.append("\\Log_file");//create folder for storing log_file.txt
    std::filesystem::path path{logPath};
    path /= "log_file.txt";//file where disk usage information is to be written
    std::filesystem::create_directories(path.parent_path());//add directories based on the object path
    std::ofstream ofs(path);//open file for writing
    for (const auto & file : recursive_directory_iterator(basePath))
       {
           if (file.is_regular_file() && file.path()!=path)
        {
            int size = file.file_size();
            ofs << "File Path: " << file.path() << " ; Size: " << size << " bytes" << std::endl; //write to file
        }
    }
    ofs.close();//close file after writing
}
