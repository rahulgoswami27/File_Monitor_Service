#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>

//Possible file changes
enum class Status {
    created, modified, erased
};

//The following class is used to monitor a specified directory tree for modifications

class MonitorHelper {
public:

    std::chrono::duration<int, std::milli> interval;// the interval after which check is done for changes
    std::string searchPath; // the path where to search

    MonitorHelper(std::string searchPath, std::chrono::duration<int, std::milli> interval) : searchPath{searchPath}, interval{interval} {
        for(auto &file : std::filesystem::recursive_directory_iterator(searchPath)) {//recursive traversal
            //store the last modification time of files recursively in the directory tree
            mapPaths[file.path().string()] = std::filesystem::last_write_time(file);
        }
    }

    void start(const std::function<void (std::string, Status)> &userFunc) {
        // Inspect "searchPath" and execute "userFunc" whenever any change is done
        // userFunc parameters: path of file (string) & type of change (Status)
        while(isRunning) {
            // Wait for "interval"
            std::this_thread::sleep_for(interval);

            auto iter = mapPaths.begin(); //define an iterator
            while (iter != mapPaths.end()) {//iterate all available entries in mapPaths
                if (!std::filesystem::exists(iter->first)) {//DELETION (path does not exist)
                    userFunc(iter->first, Status::erased); //set status of current entry to erased
                    iter = mapPaths.erase(iter); //erase from mapPaths
                }
                else {
                    iter++; //move on to next entry
                }
            }

            // Monitor file CREATION / MODIFICATION
            for(auto &file : std::filesystem::recursive_directory_iterator(searchPath)) {//recursive search
                auto currentLastWriteTime = std::filesystem::last_write_time(file);

                // CREATION
                if(!contains(file.path().string())) {//if this path entry is not already present, it is NEW
                    mapPaths[file.path().string()] = currentLastWriteTime; //value is set for this new file
                    userFunc(file.path().string(), Status::created); //set Status to created
                // MODIFICATION
                } else {
                    //if a file is modified, then last write time value will change and this is checked here
                    if(mapPaths[file.path().string()] != currentLastWriteTime) {
                        mapPaths[file.path().string()] = currentLastWriteTime; //update last write time
                        userFunc(file.path().string(), Status::modified); //set Status to modified
                    }
                }
            }
        }
    }

private:
    /*    The above operation is obtained by:
        - maintaining a record of the current files in the directory tree
        - A hash table in the form of std::unordered_map is used
        - Keys --> path of the file
        - Values --> last modification time of that file
    */
    std::unordered_map<std::string, std::filesystem::file_time_type> mapPaths;
    bool isRunning = true; //bool to denote if the monitoring is active

    // Check if "mapPaths" contains a given key -> path of the file
    bool contains(const std::string &key) {
        auto k = mapPaths.find(key);
        return k != mapPaths.end();
    }
};
