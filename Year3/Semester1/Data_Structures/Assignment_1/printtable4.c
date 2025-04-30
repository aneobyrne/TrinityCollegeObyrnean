#include<stdio.h>
#include<stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum
#include <stdbool.h> 

#define MAX_STRING_SIZE 20 //max length of a string
#define ARRAY_SIZE 30  //has table size
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n>>> " //invitation to the user
#define DATA_PROMPT ">>> Person ID    Deposition ID    Surname    Forename    Age    Person Type    Gender    Nationality    Religion    Occupation\n"
#define NEW_LINE_PROMPT ">>> " //where the user should input the name

typedef struct Element Element;
typedef struct Person Person;

struct Element{
    char name[MAX_STRING_SIZE];
    Person* personList; //linked list of people with the same surname
};

struct Person {
    char personID[MAX_STRING_SIZE];
    char depositionID[MAX_STRING_SIZE];
    char forename[MAX_STRING_SIZE];
    char surname[MAX_STRING_SIZE];
    char age[MAX_STRING_SIZE];
    char personType[MAX_STRING_SIZE];
    char gender[MAX_STRING_SIZE];
    char nationality[MAX_STRING_SIZE];
    char religion[MAX_STRING_SIZE];
    char occupation[MAX_STRING_SIZE];
    struct Person* nextPerson; //pointer to the next person with the same surname
} ;

Element* hashTable[ARRAY_SIZE]; //array representing the hash table using element pointers
int collisions = 0;
int num_terms = 0;

int hash_function1(char* s){ //generates a slot number for each name (s) wanted to insert into the hash table
    int hash = 0;
    while(*s){ //goes through each character of the name string if not NULL
       hash = (hash + *s - 'A') % ARRAY_SIZE; //this is based on ASCII (where A = 65),converting them into numbers and fitting hash between the array size range
       s++; //moves to next character of string
    }
    return hash; //number where the name will be stored in the hash table
}

Element* createNewElement(char* name, Person newPerson){ //creates a new element in which you store name and its count (frequency)
    Element *newElement = (Element*)malloc(sizeof(Element)); //allocates memory to a new element
    strcpy(newElement->name, name); //pass the input name as the new elements name
    Person *person = (Person*)malloc(sizeof(Person)); //allocates memory to a new person
    *person = newPerson; 
    person->nextPerson = NULL;
    newElement->personList = person; // initialize personList with new person
    //strcpy(newPerson->surname, name); //pass the input name as the new persons surname
	return newElement; //returns pointer to the new element created
}


Element* search (char* name){ //searches and returns the element with name name or NULL if the element is not present
    int slot = hash_function1(name); //get the slot number of the input name
    int startingSlot = slot; //retain the slot we started on

    while (hashTable[slot] != NULL) { //while not empty
     if (strcmp(hashTable[slot]->name, name) == 0) //if slot is occupied with the name we're searching for (function compares strings and if equal returns 0)
        return hashTable[slot]; //return found name
     slot = (slot + 1) % ARRAY_SIZE; //move to the next slot if name not found
     if (slot == startingSlot) //if we loop back to the original index
        break; //name not found
    }
    return NULL; //null slot hit
}

void insert(char* name, Person newPerson){ //add element at the correct place in the list
    int slot = hash_function1(name); //get the slot number where we're trying to fit the input name
    int startingSlot = slot; //retain the slot we started on

    while (hashTable[slot] != NULL){
        if (strcmp(hashTable[slot]->name, name) == 0) { //surname already exists
            return;
        }
        collisions++; //there's collision of hash slots
        slot = (slot +1) % ARRAY_SIZE; //move to next slot if there's collision
        if (slot == startingSlot) //if we loop back to the original index
          printf("Hash Table full");
    }
    //if slot was found
    hashTable[slot] = createNewElement(name, newPerson); //inserts new element to available slot
    num_terms++; //unique name was added
}

void printNumberOfOccurences(char* name){ //prints the number of time a name has appeared, or 0 if not in the file
    Element*element = search(name); //search for the input name
    if (element){  //if element found
       Person* person = element->personList;
       while (person != NULL) {
        printf("      %s      %s      %s      %s      %s      %s      %s      %s      %s      %s\n", person->personID, person->depositionID, name, person->forename, person->age, person->personType, person->gender, person->nationality, person->religion, person->occupation); 
        person = person->nextPerson;
       }
    }
    else 
      printf("%s not in table\n", name);

}

void addOrIncrement(char* name, Person newPerson){ //searches the name in the array, if it is add to linked list, if not insert
    Element*element = search(name); //search for the input name
    if (element) {
         Person* current = element->personList; //want to add as another existent person to list with same surname
         Person *person = (Person*)malloc(sizeof(Person)); //allocates memory to a new person
         *person = newPerson; 
         person->nextPerson = NULL;

         if (current == NULL)  //if no person exists yet, add the new person as the first person
                element->personList = person;
         else {
             while (current->nextPerson != NULL) { //look thorugh list till next empty slot
                    current = current->nextPerson;
            }
                current->nextPerson = person; //add the new person to the end of the list
            }
    }
    else 
      insert(name, newPerson); //add name if not found
} 

int next_token (char *buf, FILE *f, int string_max) { //reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
  int i = 0;
  char c = fgetc(f);
  bool quotes = false;

  while (c != EOF) {
    if (c == '"')
     quotes = !quotes;
     else if (c == ',' && !quotes)
      break;  //stop on comma if not inside quotes
     else if (c == '\n' && !quotes)
      break;  //stop on newline if not inside quotes
     else if (i < string_max - 1) {
      buf[i] = c;  //store the character in the buffer
      i++;
    }
     c = fgetc(f); //read in next character
  }
  buf[i] = '\0'; //to create a valid string we need to null-terminate
  if (c == ',') return 0;
  if (c == EOF || c == '\n') return 1;
  return 1;

}

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file (char *fname) {
	FILE *f; 
	char buf[1000];
    int cbuf = 0;

	f = fopen(fname, "r"); //read specified file
	if (!f) {  //check if file exists, if not output message of error
		printf("Unable to open %s\n", fname);
		return 0; 
	}
	fgets(buf, sizeof(buf), f); //read first line (skip header)

	while ( !feof(f) ) { // read until the end of the file
        Person person;
        
		cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.personID, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.depositionID, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE); //surname
        strcpy(person.surname, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.forename, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.age, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.personType, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.gender, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.nationality, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.religion, buf);
        cbuf = next_token(buf, f, MAX_STRING_SIZE);
        strcpy(person.occupation, buf);

         printf("      %s      %s      %s      %s      %s      %s      %s      %s      %s      %s\n", person.personID, person.depositionID, person.surname, person.forename, person.age, person.personType, person.gender, person.nationality, person.religion, person.occupation); 

        addOrIncrement(person.surname, person); 

        if (cbuf == 1) continue;
	}
    
    
    printf("File %s loaded\n", fname);
    float load = (float)num_terms / ARRAY_SIZE; 
    printf("     Capacity   : %i\n     Num Terms  : %i\n     Collisions : %i\n     Load       : %.2f%%\n", ARRAY_SIZE, num_terms, collisions, load);

	fclose(f); //close file
	return 1;
}
void printHashTable() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (hashTable[i] != NULL) { // Only print if the slot is occupied
            printf("Slot %d: %s\n", i, hashTable[i]->name);
        } else {
            printf("Slot %d: EMPTY\n", i);
        }
    }
}

int main(int argc, char *argv[]){ 
    char surname[MAX_STRING_SIZE];

    if (load_file("truncated.csv") == 0)
        printf("Error with loading file");

    else {
      /*  printf("%s", NAME_PROMPT); //print user invitattionsA
        scanf(" %[^\n]", &surname); //read the name user is sarching for, read until enter has been pressed
        while (strcmp(surname, "quit") != 0) { //if user doesn't type quit, print if searched name is in table and its frequency
           printf("%s", DATA_PROMPT);
           printNumberOfOccurences(surname);
           printf("%s ", NEW_LINE_PROMPT);
           scanf(" %[^\n]", &surname); //read the next name user is sarching for
        } */
      // printHashTable();

    }
    return 0; 
}
