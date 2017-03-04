#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "SingleQuerySearcher.h"

using namespace std;

typedef chrono::system_clock Clock;
string rel_path_to_target_dir1 = "./";

int calculateLength(string str){
	if (str.length()==0)
		return 0;
	int res=1;
	for(int i=0;i<str.length();i++){
		if(str[i]==' ')
			res++;
	}
	return res;
}

vector<string> splitIntoWords(string sentence){
	vector<string> strWords;
	short counter = 0;
	strWords.push_back("");
	for(short i=0;i<sentence.length();i++){
	    if(sentence[i] == ' '){
	    	strWords.push_back("");
	        counter++;
	        i++;
	    }
	    strWords[counter] += sentence[i];
	}
	return strWords;
}
// NOTE: the query_file contains the cleaned, stemmed data
int main(int argc, char** argv){
	if(argc<=2){
		cout << "Illegal num of args. Format: ./batch_query_search <query_file> <top_k>" << endl;
		exit(1);
	}
	string query_file_name = string(argv[1]);
	int top_k = atoi(argv[2]);

	ifstream fin;
	string line;
	vector<string> query_words_arr;
	bool first_time = true;
	int num_words_in_query=0, curr_query_num=1;

	// added by Nimisha
    chrono::time_point<Clock> start, end;
    chrono::duration<double> elapsed_seconds;
    start = Clock::now();  // start ticking
    // added by Nimisha

	SingleQuerySearcher* single_searcher = new SingleQuerySearcher();
	single_searcher->top_k = top_k;

    fin.open(rel_path_to_target_dir1 + query_file_name);
    
    while (std::getline(fin, line)){
    	cout << "####################Processing query number: " << curr_query_num << "####################" << endl;
    	
	    single_searcher->full_query = line;
	    num_words_in_query = calculateLength(line);
	    single_searcher->num_words_in_query = num_words_in_query;
    	single_searcher->query_words_arr = splitIntoWords(line);

    	if(first_time){
    		single_searcher->runSearch_without_preprocess();
    		first_time = false;
    	}else{
    		single_searcher->searchAgain_without_preprocess();
    	}

    	cout << "#################### Done with query number: " << curr_query_num << "####################" << endl;
    	curr_query_num ++;
    }
    delete single_searcher;

    // added by Nimisha
    end = Clock::now();
    elapsed_seconds = end - start;
    cout << "The batch query took: " << elapsed_seconds.count() << endl;
    // added by Nimisha
}