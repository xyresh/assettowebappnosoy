/*
*   Author: Mrayo
*
*   the non soyware js version of laptimes.js
*
*   use as you like, modify as you like, don't come at me if it doesnt work, if it breaks something etc. etc.
*   your problem if its not functional.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>


// Function to format milliseconds into minutes, seconds, and milliseconds
std::string formatTime(int milliseconds) {
    int minutes = milliseconds / (1000 * 60);
    milliseconds %= (1000 * 60);
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    std::stringstream formattedTime;
    formattedTime << minutes << " min, " << seconds << " sec, " << milliseconds << " ms";
    return formattedTime.str();
}

// Function to extract lap times from the log file
std::vector<std::string> extractLapTimes(const std::string& logFileName) {
    std::vector<std::string> lapTimes;

    std::ifstream logFile(logFileName);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file: " << logFileName << std::endl;
        return lapTimes;
    }

    std::string line;
    while (std::getline(logFile, line)) {
        // Check if the line contains lap time information
        if (line.find("Lap completed by") != std::string::npos && line.find("cuts, laptime") != std::string::npos) {
            // Extract username and lap time
            std::istringstream iss(line);
            std::string username, token;
            int laptime;
            while (iss >> token) {
                if (token == "by") {
                    iss >> username;
                } else if (token == "laptime") {
                    iss >> laptime;
                    break;
                }
            }

            // Format lap time and add to the vector
            lapTimes.push_back(username + ": " + formatTime(laptime));
        }
    }

    logFile.close();
    return lapTimes;
}

void cutStringsFromFile(const std::string& fileName, const std::string& searchString, std::vector<std::string>& cutStrings) {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return;
    }

    std::string line;
    bool found = false;

    while (std::getline(inFile, line)) {
        if (!found && line.find(searchString) != std::string::npos) {
            found = true;
        }
        if (found) {
            cutStrings.push_back(line);
        }
    }

    inFile.close();
}

void pasteStringsToFile(const std::string& fileName, const std::vector<std::string>& pasteStrings) {
    std::ofstream outFile(fileName, std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return;
    }

    for (const std::string& s : pasteStrings) {
        outFile << s << std::endl;
    }

    outFile.close();
}


void updateHTMLWithLapTimes(const std::string& htmlFileName, const std::vector<std::string>& lapTimes) {
    std::ifstream htmlFile(htmlFileName);
    if (!htmlFile.is_open()) {
        std::cerr << "Error opening HTML file: " << htmlFileName << std::endl;
        return;
    }

    std::ofstream updatedHTMLFile(htmlFileName + ".temp");
    if (!updatedHTMLFile.is_open()) {
        std::cerr << "Error creating temporary HTML file" << std::endl;
        htmlFile.close();
        return;
    }
    

    std::string line;
    bool logListFound = false;
    bool stopHereFound = false;
    bool logListProcessed = false;
    
    
    while (std::getline(htmlFile, line)) {
        // Check if the line contains the marker for stopping point
        if (line.find("<!-- STOP HERE -->") != std::string::npos) {
            stopHereFound = true;
        }

        // Check if the line contains the marker for logList section
        if (line.find("<!-- Log entries will be added here -->") != std::string::npos) {
            logListFound = true;
            // Write the marker line
            updatedHTMLFile << line << std::endl;
            // Skip existing lap times until </ul> marker is found
            while (std::getline(htmlFile, line) && line.find("</ul>") == std::string::npos) {}
            // Write new lap times to the HTML file
            for (const auto& lapTime : lapTimes) {
                updatedHTMLFile << "<li>" << lapTime << "</li>" << std::endl;
            }
            logListProcessed = true;
        }

        // Skip the old lap times
        if (!stopHereFound && logListFound) {
            continue;
        }

        // Write other lines to the output file
        updatedHTMLFile << line << std::endl;
    }

    htmlFile.close();
    updatedHTMLFile.close();

    if (std::rename((htmlFileName + ".temp").c_str(), htmlFileName.c_str()) != 0) {
        std::cerr << "Error replacing HTML file with updated content" << std::endl;
    } else {
        std::cout << "Lap times updated in " << htmlFileName << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <logFileName> <htmlFileName>" << std::endl;
        return 1;
    }

    std::string logFileName = argv[1];
    std::string htmlFileName = argv[2];

    // Extract lap times from the log file
    std::vector<std::string> lapTimes = extractLapTimes(logFileName);

    std::vector<std::string> cutStrings;

    cutStringsFromFile( htmlFileName, "<!-- STOP HERE -->", cutStrings);

    // Update the HTML file with new lap times
    updateHTMLWithLapTimes(htmlFileName, lapTimes);

    pasteStringsToFile(htmlFileName, cutStrings);


    return 0;
}