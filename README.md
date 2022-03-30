# File_Monitor_Service
## Description
This is an implementation of a service that collects disk usage information of a specified directory tree upon reception of a trigger event.
The trigger event is raised when a file with a certain name in the specified directory is detected.

The file that raises a trigger on creation has the following properties
- Starts with `core`
- Ends with `lz4`
- Contains one or more hexadecimal id separated by dots

The collected data is
  * Individual files, as well as whole directory tree contents 
  * Disk usage information of the files, written to a log_file.txt file in a Log_file folder.

The collected data is then stored as a tar acrhive file in a directory specified by the user. 

This is implemented mostly with the filesystem library introduced in C++17. Sample unit tests have been written to check for file names. 
Gtest has been used to implement the example unit tests. This library is uploaded in an archive format under lib/ and needs to be extracted.

This has been compiled using cmake on my Windows Pc using:
- cd build
- cmake .. -G "MinGW Makefiles"
- make

The executables, once built, are then stored in:
- build/src/File_Monitor_service.exe --> The binary for running the service
- build/tst/File_Monitor_txt.exe --> The binary for running the tests
