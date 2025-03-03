#include <iostream>
#include <fstream>
#include <deque>
#include <string>

int main(int argc, char* argv[]) 
{
    std::string line;
    std::deque<std::string> lastLines; // Store last 10 lines

    if (argc == 1) {
        // No files, reading from stdin
        while (std::getline(std::cin, line)) 
        {
            if (lastLines.size() == 10) 
            {
                lastLines.pop_front(); // Keep only the last 10 lines
            }
            lastLines.push_back(line);
        }

        // Output the last 10 lines
        for (const auto& l : lastLines) 
        {
            std::cout << l << std::endl;
        }

    } 
    else
    {
        // Process each file provided in the arguments
        bool firstFile = true; // Track if it is the first file to avoid extra newline

        for (int i = 1; i < argc; ++i) 
        {
            std::ifstream inputFile(argv[i]);

            if (inputFile.is_open()) 
            {
                // If it's not the first file, print a separator
                if (argc > 2 && !firstFile) 
                { // Only print the separator when more than one file
                    std::cout << std::endl;
                }
                firstFile = false;

                // Print the header only if there are multiple files
                if (argc > 2) 
                {
                    std::cout << "==> " << argv[i] << " <==" << std::endl;
                }

                // Read and store the last 10 lines of the file
                lastLines.clear();  // Clear previous file's lines
                while (std::getline(inputFile, line))
                {
                    if (lastLines.size() == 10) 
                    {
                        lastLines.pop_front();
                    }
                    lastLines.push_back(line);
                }

                // Output the last 10 lines
                for (const auto& l : lastLines) 
                {
                    std::cout << l << std::endl;
                }
                inputFile.close();
            } 
            else 
            {
                // Error handling for files that cannot be opened
                std::cerr << "tail: cannot open '" << argv[i] << "' for reading: No such file or directory" << std::endl;
            }
        }
    }

    return 0;
}
