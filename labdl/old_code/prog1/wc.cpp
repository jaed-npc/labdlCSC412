#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Boolean function to check manually for whitespace characters
bool isWhiteSpace(char ch){
    return ch == ' ' || ch == '\t' || ch== '\n' || ch == '\r';
}




// Void function to count lines, words, and bytes ina file or input stream

void wc (std:: istream& input, const std::string& name, int& total_lines, int& total_words,int& total_bytes){
    // Local counters/vars to store lines, words, and bytes
    int lines = 0;
    int words = 0;
    int bytes = 0;
    // Var to store each line of input
    std::string line;

    //Read the input line by line
    while (std::getline(input, line)){
    // Incream the line counter for each line processed and read
        lines++;

    // Add length of the line to the byte count, plus 1 for  the newline char
    bytes +=line.size()+ 1;
    // Track wetjer we are inside a word
    bool in_word = false;
    for (char ch: line){
        if (isWhiteSpace(ch)){
            if(in_word){
                //End of word
                words++;
                in_word = false;
            }

        }else{
            if(!in_word){
                //Start of new word
                in_word = true;
            }
        }

    }

    //If the line ends with a word, increment the word count
    if(in_word){
        words++;
    }

 }


    // Update each counter/var with the values from the current file
    total_lines += lines;
    total_words += words;
    total_bytes += bytes;

    // Prin the counter/var for the current file

    std::cout<<lines<< " " << words<< " "<< bytes<< " "<< name << '\n';

}


int main(int argc , char* argv[]){
    // Var to store the total counts across ALL files
     int total_lines = 0;
     int total_words = 0;
     int total_bytes = 0;

     // Vector to store the names of valid files that are processed completeley
     std::vector<std::string> valid_files;

     // Conditional to check if no arguments are provided

     if (argc ==1){
        // If no arg, read from stdin
        wc(std::cin, "(stdin)", total_lines, total_words, total_bytes);

     }else {
        //If no args are provided, process each one as a filename
        for (int i =1; i < argc; i++){
            // Try to open the file
            std::ifstream file(argv[i]);

            //Check if the file was succesfully opened
            if (!file){
                //If file cannt be opened, error message
                std::cerr << argv[0]<<": " << argv[i] << ": No such file or directory\n";

            }else {
                //If the file is valid, add its name to the list of valid files
                valid_files.push_back(argv[i]);
                //Process the file and update the totals

                wc(file, argv[i], total_lines, total_words, total_bytes);
                
            
            }
        }

        // If there are multiple valid files , print the total count
        if (argc >2){
            std::cout<< total_lines<< " " << total_words<< " "<<total_bytes<< " total\n";

        }
     }

     return 0;
}
