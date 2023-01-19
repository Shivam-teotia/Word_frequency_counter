/*
	|---------------|         |-----------------------------------------------|
	|G R O U P - 'D'|         |             Word Frequency Counter            |
	|---------------|         |-----------------------------------------------|
*/

//header files
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <pthread.h>
using namespace std;

unordered_map<string, int> word_count;

unordered_set<string> excluded_words;

pthread_mutex_t word_count_mutex;

//function - checking words of length less than or equal to one
bool is_simple_word(string& word) {
   return word.length() <= 1;
}
//function - to count the words line by line in the text file
void* count_words(void* file_name)
{
try{
    string line;
    ifstream file(static_cast<char*>(file_name));
    while (getline(file, line)){
        istringstream iss(line);
        string word;
        while (iss >> word){
         if(excluded_words.find(word) == excluded_words.end() && !is_simple_word(word))//condition to exclude words while counting the words
	  {
             pthread_mutex_lock(&word_count_mutex);
                ++word_count[word];
             pthread_mutex_unlock(&word_count_mutex);
          }
        }
      }
}catch(exception& e){
	cout<<e.what()<<endl;   //return null termination char sequence 
	}
    return nullptr;
}

void output(){
	for (const auto& word : word_count) {
        	cout << word.first << " : " << word.second <<endl; 
            }
}

int main(int argc, char* argv[])
{
    if (argc < 3){
    cout<<"Note : Give atleast two file names as arguments! \n[1st file should have the words which you don't want to count followed by other files in which words are to be counted.]"<<endl;
    exit(0);
    }
    string file_name;

    pthread_t threads[argc - 2];

    pthread_mutex_init(&word_count_mutex, nullptr);

    string exclude_file_name = argv[1];

    ifstream exclude_file(exclude_file_name); 

    string word;
     while (exclude_file >> word) {
       	excluded_words.insert(word);
       	}
       	exclude_file.close();

    		for (int i = 2; i < argc; ++i){
		sleep(2);
        	pthread_create(&threads[i-2], nullptr, count_words, argv[i]);
			}

		    for (int i = 2; i < argc; ++i) {
			sleep(2);
       			pthread_join(threads[i - 2], nullptr);
       			file_name = argv[i];
       			ifstream file_name(argv[i]);
       				if(!file_name){
        				cout<<argv[i]<<" : File Doesn't Exist...!\n";
       					}
       				if (file_name.is_open()){
					file_name.seekg(0,ios::end);
					size_t size = file_name.tellg();
					if(size == 0){
        	 			cout<<argv[i]<<" : File is Empty!\n";
					}
	}
}
output();
pthread_mutex_destroy(&word_count_mutex); 
return 0;
}
