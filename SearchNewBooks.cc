#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

class Book{

private:
    int ISBN;
    string type;

public:
    int get_isbn() const{
        return ISBN;
    }
    string get_type(){
        return type;
    }
    void set_isbn(int i){
        ISBN = i;
    }
    void set_type(string str){
        type = str;
    }
};

class Clock{

private:
    chrono::high_resolution_clock::time_point start;

public:
    void Reset(){
        start = chrono::high_resolution_clock::now();
    }
    double CurrentTime(){
        auto end = chrono::high_resolution_clock::now();
        double elapsed_us = chrono::duration<double, micro>(end - start).count();
        return elapsed_us;
    }
};
ostream &operator<<(ostream &stream, vector<Book> &book_vector){
    int i;
    for(i = 0; i < book_vector.size(); i++){
        stream<<book_vector[i].get_isbn()<<'\n';
        stream<<book_vector[i].get_type()<<'\n';
    }
    return stream;
}


int file_to_vector(string str, vector<Book> *book_vector){
    int i;
    string line;
    ifstream book_file;
    book_file.open(str);
    while(!book_file.eof()){
        Book *ptr;
        getline(book_file, line, ',');
        ptr = new Book;
        stringstream convertToInteger(line);
        convertToInteger >> i;
        ptr -> set_isbn(i);
        getline(book_file, line);
        ptr -> set_type(line);
        book_vector -> push_back(*ptr);
    }
    return true;
}


bool l_search(vector<Book> l_vector, int f, string type){
    int i = 0;
    for(i = 0; i < l_vector.size(); i++){
        if(f == l_vector[i].get_isbn()){
            if(type == l_vector[i].get_type()){
                return true;
            }
        }
    }
    return false;
}

int sorting (Book b1, Book b2){
    if(b1.get_isbn() != b2.get_isbn()){
        return b1.get_isbn() < b2.get_isbn();
    }
    else{
        return b1.get_type() < b2.get_type();
    }
}

bool b_search(vector<Book> b_vector, int isbn, string type){
    int m;
    int l = 0;
    int h = b_vector.size() - 1;
    while(l < h){
        m = (l+h)/2;
        if(isbn == b_vector[m].get_isbn()){
            if(type == b_vector[m].get_type()){
                return true;
            }
            else if(type > b_vector[m].get_type()){
                l = m + 1;
            }
        }
        else if(isbn > b_vector[m].get_isbn()){
            l = m + 1;
        }
        else{
            h = m -1;
        }
    }
    return false;
}

void print_output(string output,int time_taken, int time){
    ofstream output_file(output);
    if (!output_file.is_open()){
        cerr<<"Error: cannot open file "<<output<<"\n";
    }
    output_file<<time_taken<<endl;
    cout<<"CPU time: "<<time<<" microseconds"<<endl;

}

int main(int argc, char *argv[]){
    if(argc != 4){
        cerr<<"Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>\n";
        exit(0);
    }
    ifstream b_file(argv[1]);
    ifstream r_file(argv[2]);
    if(!b_file.is_open()){
        cerr<<"Error: cannot open file "<<argv[1]<<"\n";
        exit(0);
    }
    if(!r_file.is_open()){
        cerr<<"Error: cannot open file "<<argv[2]<<"\n";
        exit(0);
    }
    ofstream output(argv[3]);
    if(!output.is_open()){
        cerr<<"Error: cannot open file "<<argv[3]<<"\n";
    }
    bool flag = true;
    Clock ct;
    char ch;
    int out;
    vector<Book> new_book;
    vector<Book> requested_book;
    file_to_vector(argv[1], &new_book);
    file_to_vector(argv[2], &requested_book);
    cout<<"Choice of search method ([l]inear, [b]inary)?\n";
    cin>>ch;
    while(cin){
        ct.Reset();
        switch (ch){
            case 'l':
                for (int i = 0; i < requested_book.size(); i++){
                    if(l_search(new_book, requested_book[i].get_isbn(), requested_book[i].get_type()) == flag){
                        out++;
                    }
                }
                print_output(argv[3], out, ct.CurrentTime());
                break;
            case 'b':
                sort(new_book.begin(), new_book.end(), sorting);
                for (int i = 0; i < requested_book.size(); i++){
                    if(b_search(new_book, requested_book[i].get_isbn(), requested_book[i].get_type()) == flag){
                        out++;
                    }
                }
                output<<out<<endl;
                cout<<"CPU time: "<<ct.CurrentTime()<<" microseconds"<<endl;
                break;
            default:
                cout<<"Incorrect choice\n";
                cout<<"Choice of search method ([l]inear, [b]inary)?\n";
                cin>>ch;
        }
    }
}