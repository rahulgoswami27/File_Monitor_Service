#ifndef DOING_UNIT_TESTS
#include <iostream>
#include <chrono>
#include <filesystem>
#include "Service_Helper.cpp"
#include "MonitorHelper.h"

int main() {
    std::cout << "Directory-monitoring & Data collection service..." << std::endl;
    //Example: C:\Users\Rahul\Desktop\Project\Folders
    std::string searchPath;
    do {
        std::cout << "Enter a valid existing path to monitor: ";
        std::cin >> searchPath;
    }while(!std::filesystem::exists(searchPath));//repeat if path entered is invalid or non-existent

    std::string basePath= searchPath;
    //Example: C:\Users\Rahul\Desktop\Project\Output
    std::string outputPath;
    do {
        std::cout << "Enter a different path for storing output (cannot be subpath of monitoring path): ";
        std::cin >> outputPath;
    }while(outputPath == searchPath || outputPath.find(searchPath)!=std::string::npos); //outputPath needs to be different from searchPath
    if(!std::filesystem::exists(outputPath)) {
            //create the output directory if it does not exist
            std::cout << "Creating directory at specified location for storing output" << std::endl;
            std::filesystem::create_directory(outputPath);
    }
    std::cout << "Monitoring service is starting..." << std::endl;

    // A MonitorHelper instance which checks the current folder for changes every 3 seconds
    MonitorHelper fw{searchPath, std::chrono::milliseconds(3000)};

    // Monitor the required folder for changes
    fw.start([&basePath, &outputPath] (std::string searchPath, Status status) -> void {
        if(!std::filesystem::is_regular_file(std::filesystem::path(searchPath)) && status != Status::erased) {//watch only regular files
            return;
        }

        switch(status) {//switch for different file change status-es
            case Status::created:
                std::cout << "File has been CREATED: " << searchPath << '\n';
                {
                    std::string base_filename = searchPath.substr(searchPath.find_last_of("/\\") + 1);
                    if(Service_Helper::isTriggerName(base_filename)) {//if created file is a trigger file
                        std::cout << base_filename <<" is a trigger file."<< std::endl; //notify user
                        Service_Helper::getMetaData(basePath); //get current disk usage information for all files
                        Service_Helper::createTarFile(basePath, outputPath); //create tar with the metadata info and directory tree
                    }
                }
                break;
            case Status::modified:
                std::cout << "File has been MODIFIED: " << searchPath << std::endl;
                break;
            case Status::erased:
                std::cout << "File has been DELETED: " << searchPath << std::endl;
                break;
            default:
                std::cout << "INVALID case !!!" << std::endl; //none of the cases match, invalid change
        }
    });
}
#endif
