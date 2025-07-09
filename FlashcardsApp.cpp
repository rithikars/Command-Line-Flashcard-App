#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <dirent.h>
#include <conio.h>
#include <unordered_map>
#include <limits>

using namespace std;

//declaring required functions/classes/variables
unordered_map<int, string> listDeck();
void createDeck();
void studyDeck();
int countDeck();
void deleteDeck();

class flashcard;

void clrscr(){// clear screen function
    cout << "\033[2J\033[1;1H";
}

//--------------------MAIN PROGRAM-------------------------------
int main(){
    string enter;
    int choice = -1;
    while(true){
        if(countDeck() == 0){ // When zero decks exist
            cout<<"No decks have been created"<<endl;
            cout<<"Create a Deck? y/n ";
            getline(cin, enter);
            if(enter == "y"){
                createDeck();
            }if(enter == "n"){
                continue;
            }
        }else{// Choosing wanted option
            clrscr();
            cout<<"Would you like to:"<<endl;
            cout<<"1 - Create a new Deck"<<endl;
            cout<<"2 - Study a pre-existing deck"<<endl;
            cout<<"3 - Delete a Deck"<<endl;
            cout<<"4 - Quit?"<<endl<<endl;
            while (!(cin >> choice) || choice < 1 || choice > 4) {  
                cin.clear(); 
                cin.ignore(10000, '\n');
                cout << "Invalid choice. Please enter 1, 2, 3, or 4: ";
            }
            cin.ignore();
            switch(choice){
                case 1:
                    createDeck();
                    break;
                case 2:
                    studyDeck();
                    break;
                case 3:
                    deleteDeck();
                    break;
                case 4:
                    return 0;
            }
        }
    }
}
//--------------------MAIN PROGRAM-------------------------------

class flashcard {
    private:
        string front, back;
    public:
        void getData(string f, string b){
            front = f;
            back = b;
        }
        int showData(int count){// displaying the flash card returning l/r/q
            string side;
            char key;
            bool turn = true;
            while(true){
                side = turn?front:back;
                clrscr();
                cout<<"Card "<<count<<endl;
                cout<<"-------------------------"<<endl<<endl;
                for(int i = 0; i<side.size(); i++){
                    if(i!=0 && i<side.size() && i%24 == 0){
                        if(side[i] != ' ' && side[i+1] != ' '){
                            cout<<side[i]<<"-"<<endl;
                            continue;
                        }else{
                            cout<<side[i]<<endl;
                            continue;
                        }
                    }else if(i!=0 && i%25 == 0){
                        if(side[i]==' '){
                            continue;
                        }
                    }
                    cout<<side[i];
                }
                cout<<endl<<endl;
                cout<<"-------------------------"<<endl;
                
                key = _getch();
                if(key == 13){
                    turn = !turn;
                }
                if (key == 97) {
                    return 0; //a 
                } 
                if (key == 100){
                    return 1;// d
                }
                if (key == 113){
                    return 2;// q
                }
            }
        }
};

unordered_map<int, string> listDeck(){// listing text files in same folder 
    string path = "."; 
    DIR* dir;
    unordered_map<int, string> fileNames;
    struct dirent* entry;
    int count = 1;
    cout << "Available Decks: " << endl;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".txt") {
                cout <<count<<" - "<< filename.substr(0, filename.size() - 4) << endl;
                fileNames[count] = filename;
                count +=1;
            }
        }
        closedir(dir);
    } else {
        cerr << "Error opening directory." << endl;
    }
    return fileNames; // returns hash map index:fileName
}

void createDeck(){//defining the function to create a deck
    clrscr();
    string front, back, exit,quit,filename;
    while(true){ // verifying that deck name does not exist 
        try{
            cout<<"Enter the Name of the Deck: ";
            getline(cin, filename);
            filename += ".txt";
            ifstream file(filename);
            if(file){
                throw runtime_error("");
            }else{
                break;
            }
        }
        catch(const exception&){
                cout<<"Deck already exists, enter a different name."<<endl;
        }
    }
    ofstream Deck(filename);
    int count = 1;
    while (true) {// asking user to enter front/back and adding to text file 
        cout << "----------Card " << count << "----------" << endl;
        cout << "Front: ";
        getline(cin, front);
        cout << "Back: "; 
        getline(cin, back);
        cout << endl;
    
        Deck << front << endl;
        Deck << back << endl;
        Deck << endl;
    
        count += 1;
        quit = " ";
        cout << "Quit? y/n ";
        while (quit != "y" && quit != "n") {
            getline(cin, quit);
        }
        if (quit == "y") {
            clrscr();
            break;
        }
        clrscr();
    }
    Deck.close();
}
void studyDeck(){// defining the function to study(view) the deck
    clrscr();
    int count= 0, choice;
    string filename;
    unordered_map<int, flashcard> deck;// hash map index:flashcard 
    cout<<"Flashcards Commands: enter-switch | a-left | d-right | q-quit "<<endl;
    unordered_map<int, string> fileNames = listDeck(); 
    cout<<"Enter the number of the deck you want to study: ";// user chooses deck
    while (!(cin >> choice) || choice < 1 || choice > fileNames.size()) {  
        cin.clear(); 
        cin.ignore(10000, '\n');
        cout<<"Out of range, Enter an existing deck."<<endl;
        cout<<"Enter the number of the deck you want to study: ";
    }
    filename = fileNames[choice];
    ifstream file(filename); 
    string front, back, line;
    bool isFront = true;

    while (getline(file, line)) {// inserting data from file to flashcard class
        if (line.empty()) { 
            continue;
        }
        if (isFront) {
            front = line;
        } else {
            back = line;
            deck[count].getData(front,back);
            count +=1;
        }
        
        isFront = !isFront;
    }
    int i = 0;
    while(true){// handling the left/right/quit commands
        choice =deck[i].showData(i+1); 
        if(choice==1){
            if (i == count-1){
                continue;
            }
            i +=1;
        }if(choice == 0){
            if (i == 0){
                continue;
            }
            i -=1;
        }if(choice == 2){
            break;
        }
    }
    cout<<endl;
    file.close();
}

int countDeck(){// defining function to count number of decks in system(same folder)
    string path = "."; 
    DIR* dir;
    struct dirent* entry;
    int count = 0;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename.size() > 4 && filename.substr(filename.size()-4) == ".txt"){
                count +=1;
            }
        }
        closedir(dir);
    } else {
        cerr << "Error opening directory." << endl;
    }
    return count;
}
void deleteDeck(){// defining the function to delete decks(txt file)
    clrscr();
    string filename;
    int choice;
    char key;
    unordered_map<int, string> fileNames = listDeck();
    cout<<"Enter the number of the deck you want to delete: ";
    while (!(cin >> choice) || choice < 1 || choice > fileNames.size()) {  
        cin.clear(); 
        cin.ignore(10000, '\n');
        cout<<"Out of range, Enter an existing deck."<<endl;
        cout<<"Enter the number of the deck you want to delete: ";
    }
    filename = fileNames[choice];
    cout<<endl;
    if (remove(filename.c_str()) == 0) {
        cout << "Deck '"<<filename.substr(0, filename.size() - 4)<<"' deleted successfully."<<endl;
    } else {
        perror("Error deleting file\n");
    }
    cout<<"Press any key to continue...\n"<<endl;
    key = _getch();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}
