#include "Project1.h"

void start_system() {	//Allocate memory for starting
	int i, j;

	hi = (Llist*)malloc(sizeof(Llist));

	hi->head = NULL;
	hi->tail = NULL;

	for (i = 0; i < 65536; i++) {
		for (j = 0; j < 16; j++) {
			memory[i][j] = 0;	//Initialising memory to zero
		}
	}

	make_hash();	//Making hash table
}

void end_system() {	//Free to heap memory
	Node *free_temp, *temp;
	Opcode *free_temp2, *temp2;
	int i;

	temp = hi->head;
	
	if (temp != NULL) {
		while (temp->next != NULL) {
			free_temp = temp;
			temp = temp->next;
			free(free_temp);
		}
		free(temp);
		free(hi);
	}		//Free to history list
	
	for (i = 0; i < 20; i++) {
		temp2 = hash[i].next;
		if (temp2 != NULL) {
			while (temp2->next != NULL) {
				free_temp2 = temp2;
				temp2 = temp2->next;
				free(free_temp2);
			}
			free(temp2);
		}
	}		//Free to hash table list
}

int strbool(char *str) {	//Eliminate space and return if it is correct instruction
	int i = 0;

	while (str[i] != '\0') {
		if (str[i] == ' ' || str[i] == '\t') i++;
		else return 0;
	}
	return 1;
}

int strbool2(char *str, int *ins) {	//Eliminate space and return if it is correct instruction and save values in ins
	int i = 0, count = 1, j, num;

	if (str[i] == '\0') ins[0] = -1;
	else if (str[i] != ' ' && str[i] != '\t') ins[0] = 0;
	else {
		i++;
		while (str[i] != '\0') {
			if (str[i] == ' ' || str[i] == '\t') i++;
			else if ((48 <= str[i] && str[i] <= 57) || (65 <= str[i] && str[i] <= 70) || (97 <= str[i] && str[i] <= 102)) {
				if (count == 4) {
					ins[0] = 0;
					return 0;
				}
				j = i;
				while ((48 <= str[j] && str[j] <= 57) || (65 <= str[j] && str[j] <= 70) || (97 <= str[j] && str[j] <= 102)) j++;

				num = change_dec(&str[i], j - i - 1);		//Change hexadecimal to decimal

				for (i = j; str[i] == ' ' || str[i] == '\t'; i++) {}

				if (str[i] == '\0') {
					ins[count] = num;
					ins[0] = 1;
					return 0;
				}
				else if (str[i] == ',') {
					ins[count] = num;
					count++;
					ins[0] = 0;
					i++;
				}
				else {
					ins[0] = 0;
					return 0;
				}
			}
			else {
				ins[0] = 0;
				return 0;
			}
		}
		if (ins[0] == 0) ins[0] = 0;
		else ins[0] = 1;
	}

	return 0;
}

void instruction(char *str, int *ins) {	//Decide which function to execute
	int i;
	ins[0] = -1;
	ins[1] = -1;
	ins[2] = -1;
	ins[3] = -1;

	if (!strncmp(str, "quit", 4)) {
		if (strbool(&str[4])) ins[0] = 1;	//If it is correct instruction then ins[0] = 1
		else ins[0] = 0;
	}
	else if (!strncmp(str, "q", 1)) {
		if (strbool(&str[1])) ins[0] = 1;	//If it is correct instruction then ins[0] = 1
		else ins[0] = 0;
	}
	else if (!strncmp(str, "history", 7)) {
		if (strbool(&str[7])) ins[0] = 2;	//If it is correct instruction then ins[0] = 2
		else ins[0] = 0;
	}
	else if (!strncmp(str, "hi", 2)) {
		if (strbool(&str[2])) ins[0] = 2;	//If it is correct instruction then ins[0] = 2
		else ins[0] = 0;
	}
	else if (!strncmp(str, "help", 4)) {
		if (strbool(&str[4])) ins[0] = 3;	//If it is correct instruction then ins[0] = 3
		else ins[0] = 0;
	}
	else if (!strncmp(str, "h", 1)) {
		if (strbool(&str[1])) ins[0] = 3;	//If it is correct instruction then ins[0] = 3
		else ins[0] = 0;
	}
	else if (!strncmp(str, "reset", 5)) {
		if (strbool(&str[5])) ins[0] = 4;	//If it is correct instruction then ins[0] = 4
		else ins[0] = 0;
	}
	else if (!strncmp(str, "dump", 4)) {	//If it is correct instruction then ins[0] = 5
		strbool2(&str[4], ins);
		if (ins[3] != -1) ins[0] = 0;
		else if (ins[2] != -1 && ins[3] == -1 && ins[1] != -1 && ins[0] == 1) ins[0] = 7;
		else if (ins[1] != -1 && ins[2] == -1 && ins[3] == -1 && ins[0] == 1) ins[0] = 6;
		else if (ins[0] == -1) ins[0] = 5;
	}
	else if (!strncmp(str, "du", 2)) {	//If it is correct instruction then ins[0] = 5
		strbool2(&str[2], ins);
		if (ins[3] != -1) ins[0] = 0;
		else if (ins[2] != -1 && ins[3] == -1 && ins[1] != -1 && ins[0] == 1) ins[0] = 7;	//When two values come in, put 7 in ins[0]
		else if (ins[1] != -1 && ins[2] == -1 && ins[3] == -1 && ins[0] == 1) ins[0] = 6;	//When one value comes in, put 6 in ins[0]
		else if (ins[0] == -1) ins[0] = 5;	//When anyone value comes in, put 5 in ins[0]
	}
	else if (!strncmp(str, "dir", 3)) {	//If it is correct instruction then ins[0] = 8
		if (strbool(&str[3])) ins[0] = 8;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "d", 1)) {	//If it is correct instruction then ins[0] = 8
		if (strbool(&str[1])) ins[0] = 8;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "edit", 4)) {	//If it is correct instruction then ins[0] = 9
		strbool2(&str[4], ins);
		if (ins[3] != -1 || ins[2] == -1 || ins[1] == -1 || ins[0] == -1) ins[0] = 0;
		else if (ins[0] == 1) ins[0] = 9;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "e", 1)) {	//If it is correct instruction then ins[0] = 9
		strbool2(&str[1], ins);
		if (ins[3] != -1 || ins[2] == -1 || ins[1] == -1 || ins[0] == -1) ins[0] = 0;
		else if (ins[0] == 1) ins[0] = 9;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "fill", 4)) {	//If it is correct instruction then ins[0] = 10
		strbool2(&str[4], ins);
		if (ins[3] == -1 || ins[2] == -1 || ins[1] == -1 || ins[0] == -1) ins[0] = 0;
		else if (ins[0] == 1) ins[0] = 10;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "f", 1)) {	//If it is correct instruction then ins[0] = 10
		strbool2(&str[1], ins);
		if (ins[3] == -1 || ins[2] == -1 || ins[1] == -1 || ins[0] == -1) ins[0] = 0;
		else if (ins[0] == 1) ins[0] = 10;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "opcodelist", 10)) {	//If it is correct instruction then ins[0] = 11
		if (strbool(&str[10])) ins[0] = 11;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "opcode", 6)) {	//If it is correct instruction then ins[0] = 11
		i = 6;
		while (str[i] == ' ' || str[i] == '\t') {	//Elminate space and put the index of first character after space in ins[1]
			i++;
		}
		if (str[i] == '\0' || 65 > str[i] || str[i] > 90 || i == 6) ins[0] = 0;
		else {
			ins[0] = 12;
			ins[1] = i;
		}
	}
	else ins[0] = 0;	//If it isn't correct instruction then ins[0] = 0
}

void help() {	//Print mnemonic
	printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp][start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\n\n");
}

void reset() {	//Reset memory to zero
	int i, j;

	for (i = 0; i < 256; i++) {
		for (j = 0; j < 16; j++) {
			memory[i][j] = 0;
		}
	}
}

void Push_hi(char* str) {	//Push the new instruction to history list
	Node *newtemp = (Node*)malloc(sizeof(Node));

	strcpy(newtemp->str, str);

	if (hi->head == NULL) {
		hi->head = newtemp;
		hi->tail = newtemp;
	}
	else {
		hi->tail->next = newtemp;
		hi->tail = newtemp;
	}
	newtemp->next = NULL;
}

void history() {	//Print instruction of history list
	Node *temp;
	int count = 1;

	temp = hi->head;

	while (temp != NULL) {
		printf("%d %s\n", count, temp->str);
		temp = temp->next;
		count++;
	}
}

int dir() {		//Print directorys or files in current directory
	DIR *dp;
	struct dirent *dir;
	struct stat sta;
	int count = 0;

	dp = opendir(".");
	if (dp == NULL) {		//If there isn't directory then print error message
		printf("Directory open error\n");
		return 0;
	}

	while (1) {
		dir = readdir(dp);		//Read current directory's address
		if (dir != NULL) {
			stat(dir->d_name, &sta);	//Put the state of directory or file into sta
			printf("%s", dir->d_name);
			if (S_ISDIR(sta.st_mode)) {		//If it is directory then print * behind file name
				printf("/");
				if (count == 4) {
					printf("\n");
					count = 0;
				}
				else {
					printf("\t");
					count++;
				}
			}
			else if (sta.st_mode & S_IXUSR) {	//If it is excutable file then print * behind file name
				printf("*");
				if (count == 4) {
					printf("\n");
					count = 0;
				}
				else {
					printf("\t");
					count++;
				}
			}
			else {
				if (count == 4) {
					printf("\n");
					count = 0;
				}
				else {
					printf("\t");
					count++;
				}
			}
		}
		else {
			printf("\n");
			closedir(dp);
			break;
		}
	}
	return 0;
}

int change_dec(char *str, int end) {	//Change hexadecimal to decimal
	int dec = 0, num, h = 1;

	while (end >= 0) {

		if (48 <= str[end] && str[end] <= 57) num = str[end] - 48;
		else if (65 <= str[end] && str[end] <= 70) num = str[end] - 55;
		else num = str[end] - 87;

		dec += num * h;
		h *= 16;

		end--;
	}

	return dec;
}

int dump(int start, int end, int *start_p) {	//Print the memory from inputed start index to end index
	int starti = start / 16, startj = start % 16, len = end - start + 1, adress = start - startj;
	int i = 0, j = 0;

	if (end > 1048575 || end < 0) {		//If it is range over then print error message
		printf("Range error write again!\n");
		return 0;
	}
	else if (start > 1048575 || start < 0) {
		printf("Range error write again!\n");
		return 0;
	}
	else if (start > end) {
		printf("Range error write again!\n");
		return 0;
	}

	if (startj != 0) {
		printf("%05X ", adress);
	}

	while (j < startj) {
		printf("   ");
		j++;
	}

	while (i < len) {
		if (startj == 16) {
			printf("; ");
			for (j = 0; j < 16; j++) {		//Print character of ascii code value to memory
				if (memory[starti][j] >= 32 && memory[starti][j] <= 126) {
					printf("%c", memory[starti][j]);
				}
				else printf(".");
			}
			printf("\n");
			startj = 0;
			starti++;
			adress += 16;
		}
		else if (startj == 0) {		//Print starting address
			printf("%05X %02X ", adress, memory[starti][startj]);
			startj++;
			i++;
		}
		else {		//Print value to memory
			printf("%02X ", memory[starti][startj]);
			startj++;
			i++;
		}
	}

	for (j = startj; j < 16; j++) {		//Print space
		printf("   ");
	}

	printf("; ");
	for (j = 0; j < 16; j++) {		//Print character of ascii code value to memory
		if (memory[starti][j] >= 32 && memory[starti][j] <= 126) {
			printf("%c", memory[starti][j]);
		}
		else printf(".");
	}
	printf("\n");

	*start_p = end + 1;
	return 1;
}

int edit(int *ins) {	//Set the memory from inputed value

	if (ins[2] > 255 || ins[2] < 0) {	//If it is range over then print error message
		printf("Range error write again!\n");
		return 0;
	}
	else if (ins[1] < 0 || ins[1]>1048575) {
		printf("Range error write again!\n");
		return 0;
	}
	else memory[ins[1] / 16][ins[1] % 16] = ins[2];

	return 1;
}

int fill(int start, int end, int value) {	//Fill the memory to inputed value from inputed start index to end index
	int starti = start / 16, startj = start % 16, len = end - start + 1;

	if (end > 1048575 || end < 0) {	//If it is range over then print error message
		printf("Range error write again!\n");
		return 0;
	}
	else if (start > 1048575 || start < 0) {
		printf("Range error write again!\n");
		return 0;
	}
	else if (start > end) {
		printf("Range error write again!\n");
		return 0;
	}
	else if (value > 255 || value < 0) {
		printf("Range error write again!\n");
		return 0;
	}
	else {	//Fill the value to memory
		while (len > 0) {
			if (startj == 16) {
				startj = 0;
				starti++;
			}
			else {
				memory[starti][startj] = value;
				len--;
				startj++;
			}
		}
	}
	return 1;
}

int hash_funtion(char *mnemonic) {	//Return index of hash table

	int i, len, index = 0;

	len = strlen(mnemonic);

	for (i = 0; i < len; i++) {
		index += mnemonic[i];
	}

	index %= 20;		//Hash table size is 20

	return index;
}

void make_hash() {	//Making hash table

	FILE* fp = fopen("opcode.txt", "r");	//Read opcode.txt
	Opcode *newnode, *temp;
	int index, i;
	int opcode;
	char mnemonic[10], format[10];

	if (fp == NULL) {		//If there isn't opcode.txt then print error message
		printf("There isn't opcode.txt\n");
		exit(0);
	}
	else {
		for (i = 0; i < 20; i++) {
			hash[i].next = NULL;
		}
		temp = (Opcode*)malloc(sizeof(Opcode));
		while (fscanf(fp, "%x%s%s", &opcode, mnemonic, format) != EOF) {	//Read opcode, mnemonic, format and put it to newnode
			newnode = (Opcode*)malloc(sizeof(Opcode));
			newnode->opcode = opcode;
			strcpy(newnode->mnemonic, mnemonic);
			strcpy(newnode->format, format);
			index = hash_funtion(mnemonic);
			temp = hash[index].next;
			newnode->next = temp;
			hash[index].next = newnode;
		}
	}
	fclose(fp);
}

void opcodelist() {	//Print opcodelist
	int i;
	Opcode *temp;

	for (i = 0; i < 20; i++) {
		temp = hash[i].next;

		if (temp != NULL) {
			printf("%d : [%s,%02X] ", i, temp->mnemonic, temp->opcode);
			temp = temp->next;
			while (temp != NULL) {
				printf("-> [%s,%02X]", temp->mnemonic, temp->opcode);
				temp = temp->next;
			}
		}
		else {
			printf("%d : ", i);	//If there is any node then just print to only index
		}
		printf("\n");
	}
}

int opcode_mnemonic(char* str) {		//Print opcode of mnemonic
	int i = 0, index = 0;
	Opcode *temp;
	while (1) {
		if (65 <= str[i] && str[i] <= 90) {		//Determine index through sum value of such character in inputed mnemonic
			index += str[i];
			i++;
		}
		else if (str[i] == '\0') {
			temp = hash[index % 20].next;
			while (1) {
				if (temp == NULL) {		//If there isn't mnemonic then print error message
					printf("There isn't mnemonic\n");
					return 0;
				}
				else if (!(strncmp(temp->mnemonic, str, i))) {
					if (strlen(temp->mnemonic) == i) {
						printf("opcode is %02X\n", temp->opcode);
						return 0;
					}
					else {
						temp = temp->next;
					}
				}
				else {
					temp = temp->next;
				}
			}
		}
		else {
			printf("Please write again\n");
			return 0;
		}
	}
}

int main() {

	char *str;
	int i, start_p = 0, ins[4];
	
	start_system();

	while (1) {
		printf("sicsim> ");
		str = (char*)malloc(sizeof(char)*MAX_INPUT);
		fgets(str, MAX_INPUT, stdin);	//Saving instruction at str
		str[strlen(str) - 1] = '\0';

		i = 0;
		while (str[i] == '\t' || str[i] == ' ') i++;

		instruction(&str[i], ins);

		switch (ins[0]) {
		case 0: 
			printf("Please write again\n"); 
			break;
		case 1: //q or quit
			end_system();
			free(str);
			return 0; 
		case 2: //hi or history
			Push_hi(str);
			history();
			break;
		case 3: //h or help
			help();
			Push_hi(str);
			break;
		case 4: //reset
			reset();
			Push_hi(str);
			break;
		case 5: //du or dump
			if (dump(start_p, start_p + 159, &start_p))Push_hi(str);			
			break;
		case 6: //du or dump start
			if (ins[1] > 1048415) {
				if (dump(ins[1], 1048575, &start_p))Push_hi(str);
			}
			else if (dump(ins[1], ins[1] + 159, &start_p))Push_hi(str);
			break;
		case 7: //du or dump start, end
			if (dump(ins[1], ins[2], &start_p))Push_hi(str);
			break;
		case 8: //d or dir
			dir();
			Push_hi(str); 
			break;		
		case 9: //e or edit
			if (edit(ins)) Push_hi(str);
			break;
		case 10: //f or fill
			if (fill(ins[1], ins[2], ins[3])) Push_hi(str);			
			break;
		case 11: //opcodelist
			opcodelist();
			Push_hi(str);
			break;
		case 12: //opcode mnemonic
			opcode_mnemonic(&str[ins[1] + i]);
			Push_hi(str);
			break;		
		}
		free(str);
	}

	return 0;
}
