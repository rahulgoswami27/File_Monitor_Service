#ifndef DOING_UNIT_TESTS
#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <thread>
#include "Service_Helper.cpp"
#include "MonitorHelper.h"

int main() {
    std::cout << "Directory-monitoring & Data collection service started..." << std::endl;
    //C:\Users\Rahul\Desktop\Project\Folders
    std::string searchPath;
    do {
        std::cout << "Enter a valid existing path to monitor: ";
        std::cin >> searchPath;
    }while(!std::filesystem::exists(searchPath));//repeat if path entered is invalid or non-existent

    std::string basePath= searchPath;
    //C:\Users\Rahul\Desktop\Project\Output
    std::string outputPath;
    std::cout << "Enter path for storing output: ";
    std::cin >> outputPath;
    if(!std::filesystem::exists(outputPath)) {
            //create the output directory if it does not exist
            std::cout << "Creating directory at specified location for storing output" << std::endl;
            std::filesystem::create_directory(outputPath);
    }
    std::cout << "Monitoring service is starting..." << std::endl;

    // Create a MonitorHelper instance that will check the current folder for changes every 3 seconds
    MonitorHelper fw{searchPath, std::chrono::milliseconds(3000)};

    // Monitor the required folder for changes, watch only regular files
    fw.start([&basePath, &outputPath] (std::string searchPath, Status status) -> void {
        if(!std::filesystem::is_regular_file(std::filesystem::path(searchPath)) && status != Status::erased) {
            return;
        }

        switch(status) {
            case Status::created:
                std::cout << "File has been CREATED: " << searchPath << '\n';
                {
                    std::string base_filename = searchPath.substr(searchPath.find_last_of("/\\") + 1);
                    if(Service_Helper::isTriggerName(base_filename)) {
                        std::cout << base_filename <<" is a trigger file."<< std::endl;
                        Service_Helper::getMetaData(basePath);
                        Service_Helper::createTarFile(basePath, outputPath);
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
                std::cout << "INVALID case !!!" << std::endl;
        }
    });
}
#endif