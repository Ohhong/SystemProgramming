#define _CRT_SECURE_NO_WARNINGS
#define MAX_INPUT 100 //Max input size

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct node {
	char str[MAX_INPUT];  //This for saving instruction
	struct node *next;	//Pointer for next node
}Node;	//For history node

typedef struct Llist {
	Node *head;	//Pointer for top node of history instruction
	Node *tail;	//Pointer for bottom node of history instruction
}Llist;	//For history structure

typedef struct Opcode {
	int opcode;	//This for saving opcode
	char mnemonic[10];	//This for saving mnemonic
	char format[10];	//This for saving format
	struct Opcode *next;	//Pointer for next node
}Opcode;	//For hash table structure

typedef struct Mnemonic_list {
	Opcode *next;	//Pointer for top node of hash table
}Mnemonic_list;	//This for making hash table

Llist *hi;	//Linked list of instruction
Mnemonic_list hash[20];	//Array of hash table
unsigned char memory[65536][16];	// 1Mbyte memory

void start_system();	//Allocating memory of history list and hash table //Initialising memory

void end_system();	//Free the memory of history list and hash table

int strbool(char *str);	//Determine if it is correct instruction

int strbool2(char *str, int *ins);	//Determine if it is correct instruction

void instruction(char *str, int *ins);	//Decide which function to execute

void help();	//Print all of instruction

void reset();	//Set the value to zero

void Push_hi(char* str);	//Push the instructions at list of hi

void history();	//Print the history of instructions that was typed by user

int dir();	//Print the current directory's directorys and files

int change_dec(char *str, int end);	//Change hexdecimal to decimal

int dump(int start, int end, int *start_p);	//Print the memory from inputed start index to end index

int edit(int *ins);	//Set the memory from inputed value

int fill(int start, int end, int value);	//Fill the memory to inputed value from inputed start index to end index

int hash_funtion(char *mnemonic);	//Return index of hash table

void make_hash();	//Making hash table

void opcodelist();	//Print opcodelist

int opcode_mnemonic(char* str);	//Print opcode of mnemonic