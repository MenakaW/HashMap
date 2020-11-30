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
struct Node {

  Node* next = NULL;
  Student* s = NULL;
  
};

bool addStudent(Node** &hashtable, int size, vector<int> &keys);
void printStudent(Node** &hashtable, int size);
void deleteStudent(Node** &hashtable, int size, vector<int> &keys);
int hashIt(int key, int size);
void rehash(Node** &hashtable, int size, vector<int> keys);
void errorMessage();
bool computerGenerate(int numFirst,char* first, int numLast,char* last, int &currentId, Node** &hashtable, int size, vector<int> &keys);

int main() {
  int currentId = 1;
  int size = 101;
  Node** ht = new Node*[size];
  vector<int> keys;
  for(int a = 0; a < 101; a++){
    ht[a]=nullptr;
  }

  
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

  int numFirst = 0;
  for(int a = 0; a < strlen(first); a++){
    if(first[a]==' '){
      numFirst++;
    }
  }
  numFirst++;
  
  int numLast = 0;
  for(int a = 0; a < strlen(last); a++){
    if(last[a]==' '){
      numLast++;
    }
  }
  numLast++;
  
  while(true){
    cout << "Type add,print,delete, or quit" <<  endl;
    char input[20];
    cin >> input;
    cin.get();
    if(input[0]=='a' || input[0]=='A'){
      cout << "do you want to computer generate list of students? (yes/no)" << endl;
      char input1[20];
      cin >> input1;
      cin.get();
      bool full = false;
      if(input1[0]=='y' || input1[0]=='Y'){
	computerGenerate(numFirst,first,numLast,last,currentId, ht, size, keys);
      }
      else{
	full = addStudent(ht, size, keys);
      }
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
  cout << "how many students would you like to generate?" << endl;
  bool toReturn = false;
  int num;
  cin >> num;
  cin.get();
  srand(time(NULL));
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
    int id = currentId;
    currentId++;
    
    int random = (int)((((double)rand())/RAND_MAX)*400);
    float GPA = ((double)(random))/100;
    
    Student* addS = new Student;
    strcpy(addS->firstName,firstName);
    strcpy(addS->lastName,lastName);
    addS->studentId = id;
    addS->GPA = GPA;
    //sz=ize, keys, hashtable
    int index = hashIt(addS->studentId, size);
    Node* addN = new Node;
    (addN->s) = addS;
    keys.push_back(id);
    if(hashtable[index]==nullptr){
      hashtable[index] = addN;
    }
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
  Node** temp = new Node*[size];
  for(vector<int>::iterator it = keys.begin(); it!=keys.end(); it++){
    temp[hashIt(*it,size)] = hashtable[hashIt(*it,size/2)]; 
  }
  delete[] hashtable;
  hashtable = temp;
}

int hashIt(int key, int size){
  //123456
  int hundredthousands = (key/100000);
  int tenthousands = (key%100000-key%10000)/10000;
  int thousands = (key%10000-key%1000)/1000;
  int hundreds = (key%1000-key%100)/100;
  int tens = (key%100-key%10)/10;
  int ones = key %10;
  int index = ((tens*10)+ones);
  index = index*(thousands+1);
  index += hundreds;
  index += tenthousands;
  index += hundredthousands;
  index = index%size;
  /*
  int shortened = key%100000;
  shortened = (shortened-key%10)/10;
  int index = ((shortened%1000)/10)+(shortened%10)+(shortened/1000);
  index = index % 101;
  return index;*/
  return index;
}

bool addStudent(Node** &hashtable, int size, vector<int> &keys){
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
        Student* addS = new Student;
	strcpy(addS->firstName,firstNamee);
	strcpy(addS->lastName,lastNamee);
	addS->studentId = studentIdd;
	addS->GPA = GPAA;
	int index = hashIt(addS->studentId, size);
	Node* addN = new Node;
	(addN->s) = addS;
	keys.push_back(studentIdd);
	if(hashtable[index]==nullptr){
	  hashtable[index] = addN;
	  return false;
	}
	else{
	  Node* temp = hashtable[index];
	  hashtable[index] = addN;
	  addN->next = temp;
	  Node* current = hashtable[index];
	  int count = 1;
	  while(current->next!=NULL){
	    current = current->next;
	    count++;
	    cout << current << " nodes" << endl;
	  }
	  if(count>3){
	    return true;
	  }
	  return false;
	}
}

void printStudent(Node** &hashtable, int size){
  cout << "Please print out the student id of the student you wish to print" <<  endl;
  int id;
  cin >> id;
  cin.get();
  int index = hashIt(id, size);
  if(hashtable[index]==nullptr){
    errorMessage();
    return;
  }
  else if(hashtable[index]->next==NULL){
    if(hashtable[index]->s->studentId!=id){
      errorMessage();
      return;
    }
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
  else{
    Node* current;
    current = hashtable[index];
    while(current->s->studentId!=id){
      if(current->next==NULL){
	errorMessage();
	return;
      }
      current = current->next;
    }
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
  cout << "enter the ID number of the student you would like to delete" << endl;
  int id;
  cin >> id;
  cin.get();
  int index = hashIt(id, size);
  if(hashtable[index]==nullptr){
    errorMessage();
    return;
  }
  else if(hashtable[index]->next==NULL){
    if(hashtable[index]->s->studentId!=id){
      errorMessage();
      return;
    }
    for(vector<int>::iterator it = keys.begin(); it!=keys.end(); it++){
      if(*it==id){
	keys.erase(it);
	break;
      } 
    }
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
    while(current->s->studentId!=id){
      if(current->next==NULL){
	errorMessage();
        return;
      }
      previous = current;
      current = current->next;
    }
    for(vector<int>::iterator it = keys.begin(); it!=keys.end(); it++){
      if(*it==id){
	keys.erase(it);
	break;
      } 
    }
    if(current->next==NULL){
      previous->next = NULL;
      delete (current->s);
      delete current;
    }
    else if(previous==NULL){
      hashtable[index] = current->next;
      delete (current->s);
      delete current;
    }
    else{
      previous->next = current->next;
      delete (current->s);
      delete current;
    }
  }
}

void errorMessage(){
  cout << "this entry does not exist" << endl;
}
