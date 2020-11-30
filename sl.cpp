#include <iostream>
#include <vector>
#include<string.h>
#include<iterator>
#include<cmath>
#include <fstream>
#include <ctime>
#include <time.h>

using namespace std;

//defining struct student to contain four variables
struct Student {

  char firstName[20];
  char lastName[20];
  int studentId;
  float GPA;

};

//definig struct node to have a next node pointer and a student pointer
struct Node {

  Node* next = NULL;
  Student* s = NULL;
  
};

//method prototypes
bool addStudent(Node** &hashtable, int size, vector<int> &keys);
void printStudent(Node** &hashtable, int size);
void deleteStudent(Node** &hashtable, int size, vector<int> &keys);
int hashIt(int key, int size);
void rehash(Node** &hashtable, int size, vector<int> keys);
void errorMessage();
bool computerGenerate(int numFirst,char* first, int numLast,char* last, int &currentId, Node** &hashtable, int size, vector<int> &keys);

int main() {

  //set up variables that get used throughout the class
  int currentId = 1;
  int size = 101;

  //create the hash map of head pointers and make them all null pointers
  Node** ht = new Node*[size];
  for(int a = 0; a < 101; a++){
    ht[a]=nullptr;
  }
  
  //create the vector of keys that are contained in hash table, this is for rehashing
  vector<int> keys;
  
  //stream from the first names file and read everything in to a char array, add terminating null character at the end
  ifstream fn;
  fn.open("firstNames.txt");
  char* first = new char[200];
  for(int i = 0; i < 200; i++){
      if(fn.eof()){
	break;
      }
      first[i] = fn.get();
     }
  first[strlen(first)-1] = '\0';
  fn.close();

  //stream from the last name file and read everything in to a char array, add terminating null character at the end
  ifstream ln;
  ln.open("lastName.txt");
  char* last = new char[200];
  for(int i = 0; i < 200; i++){
      if(ln.eof()){
	break;
      }
      last[i] = ln.get();
     }
  last[strlen(last)-1] = '\0';
  ln.close();

  //count the number of first names
  int numFirst = 0;
  for(int a = 0; a < strlen(first); a++){
    if(first[a]==' '){
      numFirst++;
    }
  }
  numFirst++;

  //count the number of last names
  int numLast = 0;
  for(int a = 0; a < strlen(last); a++){
    if(last[a]==' '){
      numLast++;
    }
  }
  numLast++;

  //program cycle for user input
  while(true){
    cout << "Type add,print,delete, or quit" <<  endl;
    char input[20];
    cin >> input;
    cin.get();

    //if add, ask if they want computer generated studesnts
    if(input[0]=='a' || input[0]=='A'){
      cout << "do you want to computer generate list of students? (yes/no)" << endl;
      char input1[20];
      cin >> input1;
      cin.get();

      //create students manually or by computer, and check if it is full (more than 3 nodes in one index)
      bool full = false;
      if(input1[0]=='y' || input1[0]=='Y'){
	full = computerGenerate(numFirst,first,numLast,last,currentId, ht, size, keys);
      }
      else{
	full = addStudent(ht, size, keys);
      }

      //if the array is full, double the size and rehash the map
      if(full){
	cout << "more than 3 entris" << endl;
	size = size*2;
	rehash(ht,size,keys);
      }
      full = false;
    }
    else if(input[0]=='p' || input[0]=='P'){
      printStudent(ht, size);
    }
    else if(input[0]=='d' || input[0]=='D'){
      deleteStudent(ht, size, keys);
    }
    else if(input[0]=='q' || input[0]=='Q'){
      break;
    }
  }
  
  return 0;
  
}

bool computerGenerate(int numFirst,char* first, int numLast,char* last, int &currentId, Node** &hashtable, int size, vector<int> &keys){

  //ask the user for how many students he would like to generate
  cout << "how many students would you like to generate?" << endl;
  int num;
  cin >> num;
  cin.get();

  //set up the return bool and the seed for the random
  bool toReturn = false;
  srand(time(NULL));

  //cycle through the input from firstname file, and count how many spaces, once you reach the count that was randomly generated,
  //start adding the succeeding chars until the next space and add terminating null character
  for(int a = 0; a < num; a++){
    int firstindex = (int)((((double)rand())/RAND_MAX)*(numFirst-1));
    int count = 0;
    int strIndex = 0;
    char firstName[20];
    for(int b = 0; b < strlen(first); b++){
      if(first[b]==' '){
	count++;
      }
      if(count==(firstindex+1)){
	break;
      }
      if(count==firstindex && first[b]!=' '){
	firstName[strIndex] = first[b];
	strIndex++;
      }
    }
    firstName[strIndex] = '\0';

    //repeast above steps for the last name file
    int lastindex = (int)((((double)rand())/RAND_MAX)*(numLast-1));
    count = 0;
    strIndex = 0;
    char lastName[20];
    for(int b = 0; b < strlen(last); b++){
      if(last[b]==' '){
	count++;
      }
      if(count==(lastindex+1)){
	break;
      }
      if(count==lastindex && last[b]!=' '){
	lastName[strIndex] = last[b];
	strIndex++;
      }
    }
    lastName[strIndex] = '\0';

    //increment the id after setting it
    int id = currentId;
    currentId++;

    //generate gpa to be from 0--400 so when you divide by hundred, giving you a number between 0 and 4, rounded to the nearest hundredth
    int random = (int)((((double)rand())/RAND_MAX)*400);
    float GPA = ((double)(random))/100;

    //create a new student and set up the values
    Student* addS = new Student;
    strcpy(addS->firstName,firstName);
    strcpy(addS->lastName,lastName);
    addS->studentId = id;
    addS->GPA = GPA;

    //create a new node, and figure out the index. also add the id to the vector of keys
    int index = hashIt(addS->studentId, size);
    Node* addN = new Node;
    (addN->s) = addS;
    keys.push_back(id);

    //if hashtable at index is null, then that means this node is the first at that index
    if(hashtable[index]==nullptr){
      hashtable[index] = addN;
    }

    //if it's not null, that means you need to push the new node from the front and link the nodes
    else{
      Node* temp = hashtable[index];
      hashtable[index] = addN;
      addN->next = temp;
      Node* current = hashtable[index];
      int nodecount = 1;
      while(current->next!=NULL){
	current = current->next;
	nodecount++;
      }
      if(count>3){
	toReturn = true;
      }
    }
  }
  return toReturn;
}


void rehash(Node** &hashtable, int size, vector<int> keys){

  //create the new array of Nodes with the new size, then cycle through the keys and add them to the new hash table
  Node** temp = new Node*[size];
  for(vector<int>::iterator it = keys.begin(); it!=keys.end(); it++){
    temp[hashIt(*it,size)] = hashtable[hashIt(*it,size/2)]; 
  }

  //delete memory for old hashtable, and set up the new one
  delete[] hashtable;
  hashtable = temp;
}

int hashIt(int key, int size){

  //THIS WAS PROGRAMMED FOR 6 DIGIT IDS FROM 1-999999, AND WORKS THE BEST RANDOMIZED
  //calculate the digit of each of the number places, this makes it easier to alter the hash function
  int hundredthousands = (key/100000);
  int tenthousands = (key%100000-key%10000)/10000;
  int thousands = (key%10000-key%1000)/1000;
  int hundreds = (key%1000-key%100)/100;
  int tens = (key%100-key%10)/10;
  int ones = key %10;

  //equation for index
  int index = ((tens*10)+ones);
  index = index*(thousands+1);
  index += hundreds;
  index += tenthousands;
  index += hundredthousands;
  index = index%size;
  return index;
}

bool addStudent(Node** &hashtable, int size, vector<int> &keys){

        //get all the student values from user input
  	cout << "Please enter the student's first name" << endl;
	char firstNamee[20];
	cin >> firstNamee;
	cin.get();
	cout << "Please enter the student's last name" << endl;
	char lastNamee[20];
	cin >> lastNamee;
	cin.get();
	cout << "Please enter the 6 digit studen't id number" << endl;
	int studentIdd;
	cin >> studentIdd;
	cin.get();
	cout << "Please enter the student's GPA" << endl;
	float GPAA;
	cin >>  GPAA;
	cin.get();
	
	//set up values of student
        Student* addS = new Student;
	strcpy(addS->firstName,firstNamee);
	strcpy(addS->lastName,lastNamee);
	addS->studentId = studentIdd;
	addS->GPA = GPAA;

	//create new node and add the id to the vector of keys
	int index = hashIt(addS->studentId, size);
	Node* addN = new Node;
	(addN->s) = addS;
	keys.push_back(studentIdd);

	//if it's the first node at the index
	if(hashtable[index]==nullptr){
	  hashtable[index] = addN;
	  return false;
	}

	//if there are other nodes, link the list accordingly
	else{
	  Node* temp = hashtable[index];
	  hashtable[index] = addN;
	  addN->next = temp;
	  Node* current = hashtable[index];
	  int count = 1;
	  while(current->next!=NULL){
	    current = current->next;
	    count++;
	  }

	  //if there are more than 3 nodes at that index, the hashmap is "full" and needs to be expanded
	  if(count>3){
	    return true;
	  }
	  return false;
	}
}

void printStudent(Node** &hashtable, int size){

  //get id from the user, convert it to an index
  cout << "Please print out the student id of the student you wish to print" <<  endl;
  int id;
  cin >> id;
  cin.get();
  int index = hashIt(id, size);

  //if there's nothing at the index, there is no student with that id
  if(hashtable[index]==nullptr){
    errorMessage();
    return;
  }

  //if there is one node
  else if(hashtable[index]->next==NULL){

    //check to make sure the id matches up
    if(hashtable[index]->s->studentId!=id){
      errorMessage();
      return;
    }

    //print out the info of the node at the index
    char firstName[20];
    strcpy(firstName,hashtable[index]->s->firstName);
    cout << firstName;
    char lastName[20];
    strcpy(lastName,hashtable[index]->s->lastName);
    cout <<" " << lastName << ", ";
    cout << "student ID: " << id << ", ";
    float gpa1 = floor(hashtable[index]->s->GPA*100+0.5)/100;
    cout << "and GPA of " << gpa1 << endl;
  }

  //if there's more than one node at index
  else{
    Node* current;
    current = hashtable[index];

    //cycle through the nodes at that index until the id matchs up
    while(current->s->studentId!=id){
      if(current->next==NULL){
	errorMessage();
	return;
      }
      current = current->next;
    }

    //print out the info for that node
    char firstName[20];
    strcpy(firstName,current->s->firstName);
    cout << firstName;
    char lastName[20];
    strcpy(lastName,current->s->lastName);
    cout <<" " << lastName << ", ";
    cout << "student ID: " << id << ", ";
    float gpa1 = floor(current->s->GPA*100+0.5)/100;
    cout << "and GPA of " << gpa1 << endl;
  }
}

void deleteStudent(Node** &hashtable, int size, vector<int> &keys){

  //get the id from the user
  cout << "enter the ID number of the student you would like to delete" << endl;
  int id;
  cin >> id;
  cin.get();
  int index = hashIt(id, size);

  //same search method as print
  if(hashtable[index]==nullptr){
    errorMessage();
    return;
  }
  else if(hashtable[index]->next==NULL){
    if(hashtable[index]->s->studentId!=id){
      errorMessage();
      return;
    }
    //unlike print, you need to remove the key from the vector of keys before you erase the node
    for(vector<int>::iterator it = keys.begin(); it!=keys.end(); it++){
      if(*it==id){
	keys.erase(it);
	break;
      } 
    }

    //erase node
    Node* temp = hashtable[index];
    hashtable[index] = nullptr;
    delete (temp->s);
    delete temp;
  }
  else{
    Node* current;
    Node* previous;
    previous=NULL;
    current = hashtable[index];

    //cycle through the linked list to get the node that matches, and keep track of the previous node for linking purposes
    while(current->s->studentId!=id){
      if(current->next==NULL){
	errorMessage();
        return;
      }
      previous = current;
      current = current->next;
    }

    //erase from keys first
    for(vector<int>::iterator it = keys.begin(); it!=keys.end(); it++){
      if(*it==id){
	keys.erase(it);
	break;
      } 
    }

    //if node is at the end of the linked list, erase node, and then relink accordingly
    if(current->next==NULL){
      previous->next = NULL;
      delete (current->s);
      delete current;
    }

    //if node is the head node, erase node, and then relink accordingly
    else if(previous==NULL){
      hashtable[index] = current->next;
      delete (current->s);
      delete current;
    }

    //if node has a previous and a next, erase node then relink accordingly
    else{
      previous->next = current->next;
      delete (current->s);
      delete current;
    }
  }
}

//prints out an error message
void errorMessage(){
  cout << "this entry does not exist" << endl;
}
