#include "Project3.h"

void start_system() {	//Allocate memory for starting
	int i, j;

	progaddr = 0;

	for (i = 0; i < 20; i++) bp[i] = -1;	//Initialising break points to -1

	hi = (Llist*)malloc(sizeof(Llist));

	hi->head = NULL;
	hi->tail = NULL;

	for (i = 0; i < 20; i++) estab[i].next = NULL;

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
	Symboltable *free_temp3, *temp3;
	int i;

	temp = hi->head;
	
	Free_symboltable();		//Free to symboltable
	Free_estable();			//Free to estable

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

	for (i = 0; i < 26; i++) {
		temp3 = symbol[i].next;
		if (temp3 != NULL) {
			while (temp3->next != NULL) {
				free_temp3 = temp3;
				temp3 = temp3->next;
				free(free_temp3);
			}
			free(temp3);
		}
	}
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
	else if (!strncmp(str, "opcode", 6)) {	//If it is correct instruction then ins[0] = 12
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
	else if (!strncmp(str, "assemble ", 9)) {	//If it is correct instruction then ins[0] = 13

		ins[1] = strspn(&str[9], " \t") + 9;
		ins[2] = strcspn(&str[ins[1]], " \t\0") + ins[1];

		i = strspn(&str[ins[2]], " \t") + ins[2];

		if (str[i] == '\0') {
			if (!strncmp(&str[ins[2] - 4], ".asm", 4)) ins[0] = 13;
			else ins[0] = 0;
		}
		else ins[0] = 0;
	}
	else if (!strncmp(str, "type ", 5)) {	//If it is correct instruction then ins[0] = 15

		ins[1] = strspn(&str[5], " \t") + 5;
		ins[2] = strcspn(&str[ins[1]], " \t\0") + ins[1];

		i = strspn(&str[ins[2]], " \t") + ins[2];

		if (str[i] == '\0') ins[0] = 15;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "symbol", 6)) {	//If it is correct instruction then ins[0] = 14
		if (strbool(&str[6])) ins[0] = 14;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "progaddr", 8)) {	//If it is correct instruction then ins[0] = 16
		i = strspn(&str[8], " \t");

		if (str[i + 8] == '\0') ins[0] = 0;
		else {
			ins[1] = strspn(&str[i + 8], "1234567890ABCDEF");
			if (strbool(&str[i + 8 + ins[1]])) {
				ins[1] = change_dec(&str[i + 8], ins[1] - 1);
				ins[0] = 16;
			}
			else ins[0] = 0;
		}		
	}
	else if (!strncmp(str, "loader", 6)) {	//If it is correct instruction then ins[0] = 17
		i = strspn(&str[6], " \t");

		if (str[i + 6] == '\0') ins[0] = 0;
		else {
			ins[0] = 17;
			ins[1] = i + 6;
		}
	}
	else if (!strncmp(str, "run", 3)) {	//If it is correct instruction then ins[0] = 18
		if (strbool(&str[3])) ins[0] = 18;
		else ins[0] = 0;
	}
	else if (!strncmp(str, "bp", 2)) {
		if (strbool(&str[2])) ins[0] = 19;	//If it is correct instruction then ins[0] = 19
		else {
			i = strspn(&str[2], " \t");

			if (i == 0) ins[0] = 0;
			else if (!strncmp(&str[i + 2], "clear", 5)) {
				ins[1] = strspn(&str[i + 7], " \t");
				if (str[i + ins[1] + 7] == '\0') ins[0] = 20;	//If it is correct instruction then ins[0] = 20
				else ins[0] = 0;
			}
			else {
				ins[1] = 0;
				while (1) {
					if (str[i + 2 + ins[1]] == '\0' || str[i + 2 + ins[1]] == '\t' || str[i + 2 + ins[1]] == ' ') {
						ins[2] = strspn(&str[i + 2 + ins[1]], " \t");
						if (str[i + 2 + ins[1] + ins[2]] == '\0') {
							ins[2] = change_dec(&str[i + 2], ins[1] - 1);
							ins[0] = 21;	//If it is correct instruction then ins[0] = 21
						}
						else ins[0] = 0;
						break;
					}
					else if ((48 <= str[i + 2 + ins[1]] && str[i + 2 + ins[1]] <= 57) || (65 <= str[i + 2 + ins[1]] && str[i + 2 + ins[1]] <= 70) || (97 <= str[i + 2 + ins[1]] && str[i + 2 + ins[1]] <= 102)) ins[1]++;
					else {
						ins[0] = 0;
						break;
					}
				}
			}
		}
	}
	else ins[0] = 0;	//If it isn't correct instruction then ins[0] = 0
}

void help() {	//Print mnemonic
	printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp][start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\nassemble filename\ntype filename\nsymbol\n");
}

void reset() {	//Reset memory to zero
	int i, j;

	for (i = 0; i < 256; i++) {
		for (j = 0; j < 16; j++) {
			memory[i][j] = 0;
		}
	}
	total_len = 0;
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

int Search_opcode(char* str, int* opcode, int* format) {	//Checks whether the opcode is in the table and return 0 or 1
	int index = 0;
	Opcode *temp;
	
	index = hash_funtion(str);

	temp = hash[index].next;
	
	while (temp != NULL) {
		if (!strcmp(temp->mnemonic, str)) {
			*opcode = temp->opcode;
			*format = temp->format[0] - 48;
			return 1;		//The opcode is in the table
		}
		temp = temp->next;
	}
	return 0;		//The opcode is not in the table
}

int Read_file(char* line, char* out1, char* out2, char* out3) {		//Read the line in the file and put symbol, mnemonic and values into each of the strings out1, out2 and out3.
	int i, j;

	if (line[0] == '.' || line[0] == '\n') {		//If there is '.' or '\n' in line[0] then return 0
		return 0;
	}
	else if (line[0] == ' ' || line[0] == '\t') {
		out1[0] = '\0';		//If there is space in line[0] then put the 'EOF' in out1
		i = strspn(line, " \t");
		j = strcspn(&line[i], " \t\n");
		strncpy(out2, &line[i], j);		//Put the mnemonic in out2
		out2[j] = '\0';

		if (line[i + j - 1] != '\n') {
			i = strspn(&line[i + j], " \t") + i + j;
			j = strcspn(&line[i], "\n");
			strncpy(out3, &line[i], j);		//Put the values in out3
			out3[j] = '\0';
		}
		else out3[0] = '\0';		//If there is any value then put the 'EOF' in out3
	}
	else {		//In this case, line[0] does not have space
		j = strcspn(line, " \t");
		strncpy(out1, line, j);
		out1[j] = '\0';		//Put the symbol in out1

		i = strspn(&line[j], " \t") + j;
		j = strcspn(&line[i], " \t\n");
		strncpy(out2, &line[i], j);		//Put the mnemonic in out2
		out2[j] = '\0';

		if (line[i + j - 1] != '\n') {
			i = strspn(&line[i + j], " \t") + i + j;
			j = strcspn(&line[i], "\n");
			strncpy(out3, &line[i], j);
			out3[j] = '\0';		//Put the values in out3
		}
		else out3[0] = '\0';		//If there is any value then put the 'EOF' in out3
	}

	return 1;
}

void Push_symbol(char* str, int locctr) {		//Push the symbol in symboltable for make symboltable
	Symboltable *new, *temp;
	int index;

	new = (Symboltable*)malloc(sizeof(Symboltable));

	strcpy(new->symbol, str);
	new->locctr = locctr;
	new->next = NULL;

	index = str[0] - 65;		//Index is in alphabetical order

	if (symbol[index].next == NULL) {		//If there is nothing in the corresponding index then insert new symbol
		symbol[index].next = new;
	}
	else {
		temp = symbol[index].next;
		if (strcmp(str, temp->symbol) == 1) {		//The order in which they are put in the symbol table is in reverse alphabetical order
			new->next = temp;
			symbol[index].next = new;
		}
		else {
			while (temp->next != NULL) {
				if (strcmp(str, temp->next->symbol) == 1) {
					new->next = temp->next;
					temp->next = new;
					return;
				}
				temp = temp->next;
			}
			temp->next = new;
		}
	}
}

int Search_symbol(char* str) {		//Search symbol and return location counter of symbol
	Symboltable *temp;
	int index;

	index = str[0] - 65;		//Index is in alphabetical order

	temp = symbol[index].next;

	while (temp != NULL) {
		if (!strcmp(temp->symbol, str)) return temp->locctr;
		temp = temp->next;
	}

	return -1;
}

void Print_symbol() {		//Print symbol table in reverse alphabetical order.
	Symboltable *temp;
	int i, num = 0;

	for (i = 25; i >= 0; i--) {		//index 25 contains a symbol starting with z, and index 0 contains a symbol starting with a
		temp = symbol[i].next;
		while (temp != NULL) {
			printf("\t%s\t%04X\n", temp->symbol, temp->locctr);
			temp = temp->next;
			num = 1;
		}
	}
	if (num == 0) printf("Empty Symboltable\n");	//If there is nothing symbol then print error message
}

void Free_symboltable() {		//Free to symbol table
	Symboltable *temp, *free_temp;
	int i;

	for (i = 0; i < 26; i++) {
		temp = symbol[i].next;
		if (temp != NULL) {
			while (temp->next != NULL) {
				free_temp = temp;
				temp = temp->next;
				free(free_temp);
			}
			free(temp);
		}
		symbol[i].next = NULL;
	}
}

int Search_resister(char *str) {		//Search resister's number and return that

	if (strcmp("A", str) == 0)
		return 0;	//A register
	else if (strcmp("X", str) == 0)
		return 1;	//X register
	else if (strcmp("L", str) == 0)
		return 2;	//L register
	else if (strcmp("B", str) == 0)
		return 3;	//B register
	else if (strcmp("S", str) == 0)
		return 4;	//S register
	else if (strcmp("T", str) == 0)
		return 5;	//T register
	else if (strcmp("F", str) == 0)
		return 6;	//F register
	else if (strcmp("PC", str) == 0)
		return 8;	//PC register
	else if (strcmp("SW", str) == 0)
		return 9;	//SW register

	return -1;
}

int pass1(char *str) {		//Algorithm of pass 1 // Make symbol table
	FILE* fp = fopen(str, "r");
	int locctr, i, j, line_count = 5, num, mod_loc = 0;
	char line[50], out1[10], out2[10], out3[30];

	if (fp == NULL) {		//If there isn't file then print error message
		printf("There isn't %s\n", str);
		return -1;
	}
	Free_symboltable();

	fgets(line, sizeof(line), fp);
	Read_file(line, out1, out2, out3);		//Read file and put it in out1, out2 and out3

	if (!strcmp(out2, "START")) {		//If out2 contain 'START' then set location counter to input number
		locctr = change_dec(out3, strlen(out3) - 1);
		line_count += 5;
	}
	else {		//If it is not, set the location counter to zero.
		fseek(fp, 0L, SEEK_SET);		//Return the file pointer to the beginning
		locctr = 0;
	}

	while (!feof(fp)) {

		fgets(line, sizeof(line), fp);		//Read a line

		if (Read_file(line, out1, out2, out3) == 0) {		//If this line contains a '.' Or '\n' then read next line
			line_count += 5;
			continue;
		}

		if (out1[0] != '\0' && Search_symbol(out1) != -1) {		//If symbols duplicate then print error message
			printf("Error duplication symbol at line %d\n", line_count);
			return -1;
		}

		if (out2[0] == '+') {		//Format 4 //location counter +4
			if (Search_opcode(&out2[1], &i, &j) == 1) {
				if (out1[0] != '\0') Push_symbol(out1, locctr);		//If line contains symbol then put it in symbol table
				locctr += 4;
			}
			mod_loc++;
			line_count += 5;
		}
		else if (Search_opcode(out2, &i, &j) == 1) {
			if (out1[0] != '\0') Push_symbol(out1, locctr);		//If line contains symbol then put it in symbol table

			if (j == 3) locctr += 3;		//Format 3 //location counter +3
			else if (j == 2)locctr += 2;	//Format 2 //location counter +2
			else if (j == 1)locctr += 1;	//Format 1 //location counter +1
			line_count += 5;
		}
		else if (!strcmp(out2, "WORD")) {		//WORD //location counter +3
			if (out1[0] != '\0') Push_symbol(out1, locctr);		//If line contains symbol then put it in symbol table
			locctr += 3;
			line_count += 5;
		}
		else if (!strcmp(out2, "BYTE")) {		//BYTE
			if (out1[0] != '\0') Push_symbol(out1, locctr);		//If line contains symbol then put it in symbol table

			if (out3[0] == 'C') locctr += strlen(out3) - 3;		//This is string or character //location counter +the number of character
			else if (out3[0] == 'X') locctr += 1;		//This is hexadecimal number //loaction counter +1
			line_count += 5;
		}
		else if (!strcmp(out2, "RESW")) {		//RESW //location counter +(decimal number*3)
			if (out1[0] != '\0') Push_symbol(out1, locctr);		//If line contains symbol then put it in symbol table
			num = 0;
			j = 1;
			for (i = strlen(out3) - 1; i >= 0; i--) {
				num += j * (out3[i] - 48);
				j *= 10;
			}
			locctr += 3 * num;
			line_count += 5;
		}
		else if (!strcmp(out2, "RESB")) {		//RESB //location counter +decimal number
			if (out1[0] != '\0') Push_symbol(out1, locctr);		//If line contains symbol then put it in symbol table
			num = 0;
			j = 1;
			for (i = strlen(out3) - 1; i >= 0; i--) {
				num += j * (out3[i] - 48);
				j *= 10;
			}
			locctr += num;
			line_count += 5;
		}
		else if (!strcmp(out2, "BASE")) {		//If line contains 'BASE' then read next line
			line_count += 5;
			continue;
		}
		else if (!strcmp(out2, "END")) break;		//If line contains 'END' then break loop
		else {
			printf("Invalid operator at line %d\n", line_count);		//If line contains invalid operator then print error message
			return -1;
		}
	}
	fclose(fp);

	pass2(str, locctr, mod_loc);

	return 1;
}

void pass2(char *str, int end_locctr, int mod_loc) {		//Algorithm of pass2 //Make object file and listing file
	FILE* fp = fopen(str, "r");
	FILE* fp2;
	FILE* fp3;
	int locctr, num, opcode, i, j, format, base, symbol_loc, start_loc, *modify_loc, T_size = 0, line_count = 5;
	char line[50], out1[10], out2[10], out3[30], *str2, *str3;
	unsigned char objcode[4], objfile[30];

	modify_loc = (int*)malloc(sizeof(int)*mod_loc);		//For modification record
	mod_loc = 0;

	i = strlen(str);
	str2 = (char*)malloc(sizeof(char)*i);		//For put listing filename in str2
	str3 = (char*)malloc(sizeof(char)*i);		//For put object filename in str3

	strcpy(str2, str);
	strcpy(str3, str);

	strncpy(&str2[i - 3], "lst", 3);
	strncpy(&str3[i - 3], "obj", 3);	

	fp2 = fopen(str2, "w");		//file open for listing file
	fp3 = fopen(str3, "w");		//file open for object file

	fgets(line, sizeof(line), fp);
	Read_file(line, out1, out2, out3);

	if (!strcmp(out2, "START")) {		//If out2 contain 'START' then set location counter to input number
		locctr = change_dec(out3, strlen(out3) - 1);
		fprintf(fp2, "%-5d%04d\t%s", line_count, locctr, line);		//Print listing file
		line_count += 5;
		if (out1[0] == '\0') fprintf(fp3, "H      %06X%06X\n", locctr, end_locctr - locctr);		//Print head record when there isn't function name
		else fprintf(fp3, "H%-6s%06X%06X\n", out1, locctr, end_locctr - locctr);		//Print head record when there is function name
	}
	else {
		fseek(fp, 0L, SEEK_SET);
		locctr = 0;
		fprintf(fp3, "H      %06X%06X\n", locctr, end_locctr - locctr);		//Print head record when there isn't 'START'
	}

	start_loc = locctr;

	while (!feof(fp)) {

		fgets(line, sizeof(line), fp);
		
		if (Read_file(line, out1, out2, out3) == 0) {		//If this line contains a '.' Or '\n' then read next line
			fprintf(fp2, "%-5d     \t%s", line_count, line);
			line_count += 5;
			continue;
		}
		else if (!strcmp(out2, "BASE")) {		//BASE
			base = Search_symbol(out3);		//Base location counter set symbol's location counter
			fprintf(fp2, "%-5d     \t%s", line_count, line);
			line_count += 5;
			continue;
		}
		line[strlen(line) - 1] = '\0';

		if (locctr != end_locctr) {			//Print until location counter becomes end location counter
			fprintf(fp2, "%-5d%04X\t", line_count, locctr);
			line_count += 5;
		}

		if (!strcmp(out2, "RESW")) {	//RESW
			num = 0;
			j = 1;
			for (i = strlen(out3) - 1; i >= 0; i--) {
				num += j * (out3[i] - 48);
				j *= 10;
			}
			locctr += 3 * num;
			fprintf(fp2, "%-25s\n", line);

			if (T_size != 0) {		//If T_size isn't 0 then print a line in objfile
				fprintf(fp3, "%02X", T_size);
				for (i = 0; i < T_size; i++) {
					fprintf(fp3, "%02X", objfile[i]);
				}
				fprintf(fp3, "\n");
				T_size = 0;
			}
			continue;
		}
		else if (!strcmp(out2, "RESB")) {		//RESB
			num = 0;
			j = 1;
			for (i = strlen(out3) - 1; i >= 0; i--) {
				num += j * (out3[i] - 48);
				j *= 10;
			}
			locctr += num;
			fprintf(fp2, "%-25s\n", line);
			if (T_size != 0) {		//If T_size isn't 0 then print a line in objfile
				fprintf(fp3, "%02X", T_size);
				for (i = 0; i < T_size; i++) {
					fprintf(fp3, "%02X", objfile[i]);
				}
				fprintf(fp3, "\n");
				T_size = 0;
			}
			continue;
		}		
		else if (!strcmp(out2, "END")) {
			if (T_size != 0) {		//If T_size isn't 0 then print a line in objfile
				fprintf(fp3, "%02X", T_size);
				for (i = 0; i < T_size; i++) {
					fprintf(fp3, "%02X", objfile[i]);
				}
				fprintf(fp3, "\n");
			}

			for (i = 0; i < mod_loc; i++) fprintf(fp3, "M%06X05\n", modify_loc[i]);		//Print modification record
			fprintf(fp2, "%-5d     \t%s\n", line_count, line);			
			fprintf(fp3, "E%06X\n", start_loc);		//Print end record
			free(modify_loc);
			break;
		}

		if (T_size <= 0) {		//If T_size <= 0 then print head of text record and address
			fprintf(fp3, "T%06X", locctr + T_size);
			T_size *= -1;
		}

		if (out2[0] == '+') {		//Format 4
			Search_opcode(&out2[1], &opcode, &format);
			locctr += 4;
			
			if (out3[0] == '#') {		//Immediate addressing
				objcode[0] = opcode + 1;
				if (out3[1] < 48 || out3[1]>57) {		//If out3 is string
					symbol_loc = Search_symbol(&out3[1]);
					modify_loc[mod_loc] = locctr - 3;
					mod_loc++;
				}
				else {		//If out3 is number
					j = 1;
					symbol_loc = 0;
					for (i = strlen(out3) - 1; i > 0; i--) {
						symbol_loc += (out3[i] - 48)*j;
						j *= 10;
					}
				}
			}
			else if (out3[0] == '@') {		//Indirect addressing
				symbol_loc = Search_symbol(&out3[1]);
				objcode[0] = opcode + 2;
				modify_loc[mod_loc] = locctr - 3;
				mod_loc++;
			}
			else {		//Simple addressing
				symbol_loc = Search_symbol(out3);
				objcode[0] = opcode + 3;
				modify_loc[mod_loc] = locctr - 3;
				mod_loc++;
			}
			objcode[3] = symbol_loc % 256;		//Generate objcode and insert it
			objcode[1] = 16 + (symbol_loc / 65536);
			objcode[2] = (symbol_loc / 256) - (objcode[1] % 16) * 256;

			fprintf(fp2, "%-25s%02X%02X%02X%02X\n", line, objcode[0], objcode[1], objcode[2], objcode[3]);		//Print listing file

			if (T_size > 25) {		//If T_size greater than 25 then print a line in objfile
				fprintf(fp3, "%06X", T_size);
				for (i = 0; i < T_size; i++) {
					fprintf(fp3, "%02X", objfile[i]);
				}
				fprintf(fp3, "\n");
				T_size = -4;
				objfile[0] = objcode[0];		//Insert objcode in objfile
				objfile[1] = objcode[1];
				objfile[2] = objcode[2];
				objfile[3] = objcode[3];
			}
			else {		
				objfile[T_size] = objcode[0];		//Insert objcode in objfile
				objfile[T_size + 1] = objcode[1];
				objfile[T_size + 2] = objcode[2];
				objfile[T_size + 3] = objcode[3];
				T_size += 4;
			}
		}
		else if (Search_opcode(out2, &opcode, &format) == 1) {

			if (format == 3) {		//Format 3
				locctr += 3;
				if (out3[0] == '#') {		//Immediate addressing
					objcode[0] = opcode + 1;
					if (out3[1] < 48 || out3[1] > 57) {		//If out3 is string
						symbol_loc = Search_symbol(&out3[1]);
						if ((symbol_loc - locctr) < -2048 || (symbol_loc - locctr) > 2047) {		//Base relative
							symbol_loc = symbol_loc - base;
							objcode[1] = 4 * 16;
						}
						else if (symbol_loc < locctr) {		//PC relative and transform displacement to 2's complement number
							num = locctr - symbol_loc;
							symbol_loc = 4096 - num;
							objcode[1] = 2 * 16;
						}
						else {		//PC relative
							symbol_loc = symbol_loc - locctr;
							objcode[1] = 2 * 16;
						}						
					}
					else {		//If out3 is decimal number
						j = 1;
						symbol_loc = 0;
						for (i = strlen(out3) - 1; i > 0; i--) {
							symbol_loc += (out3[i] - 48)*j;
							j *= 10;
						}
						objcode[1] = 0;
					}
					objcode[1] += symbol_loc / 256;		//Generate objcode and insert it
					objcode[2] = symbol_loc % 256;
				}
				else if (out3[0] == '@') {		//Indirect addressing
					objcode[0] = opcode + 2;
					symbol_loc = Search_symbol(&out3[1]);
					if ((symbol_loc - locctr) < -2048 || (symbol_loc - locctr) > 2047) {		//Base relative
						symbol_loc = symbol_loc - base;
						objcode[1] = 4 * 16;
					}
					else if (symbol_loc < locctr) {		//PC relative and transform displacement to 2's complement number
						num = locctr - symbol_loc;
						symbol_loc = 4096 - num;
						objcode[1] = 2 * 16;
					}
					else {		//PC relative
						symbol_loc = symbol_loc - locctr;
						objcode[1] = 2 * 16;
					}					
					objcode[1] += symbol_loc / 256;		//Generate objcode and insert it
					objcode[2] = symbol_loc % 256;
				}
				else {		//Simple addresing
					objcode[0] = opcode + 3;
					if (!strcmp(out2, "RSUB")) {		//If out2 is 'RSUB' then insert 4F000000 in objcode
						objcode[1] = 0;
						objcode[2] = 0;
					}
					else {
						if (out3[strlen(out3) - 1] == 'X') {		//If out3 contains x then flag bit of x set 1
							out3[strcspn(out3, ",")] = '\0';
							symbol_loc = Search_symbol(out3);
							objcode[1] = 8 * 16;
						}
						else {
							symbol_loc = Search_symbol(out3);
							objcode[1] = 0;
						}

						if ((symbol_loc - locctr) < -2048 || (symbol_loc - locctr) > 2047) {		//Base relative
							symbol_loc = symbol_loc - base;
							objcode[1] += 4 * 16;
						}
						else if (symbol_loc < locctr) {		//PC relative and transform displacement to 2's complement number
							num = locctr - symbol_loc;
							symbol_loc = 4096 - num;
							objcode[1] += 2 * 16;
						}
						else {
							symbol_loc = symbol_loc - locctr;		//PC relative
							objcode[1] += 2 * 16;
						}

						objcode[1] += symbol_loc / 256;		//Generate objcode and insert it
						objcode[2] = symbol_loc % 256;
					}
					
				}
				fprintf(fp2, "%-25s%02X%02X%02X\n", line, objcode[0], objcode[1], objcode[2]);		//Print listing file

				if (T_size > 26) {				//If T_size greater than 26 then print a line in objfile
					fprintf(fp3, "%02X", T_size);
					for (i = 0; i < T_size; i++) {
						fprintf(fp3, "%02X", objfile[i]);
					}
					fprintf(fp3, "\n");
					T_size = -3;

					objfile[0] = objcode[0];		//Insert objcode in objfile
					objfile[1] = objcode[1];
					objfile[2] = objcode[2];
				}
				else {
					objfile[T_size] = objcode[0];		//Insert objcode in objfile
					objfile[T_size + 1] = objcode[1];
					objfile[T_size + 2] = objcode[2];
					T_size += 3;
				}
			}
			else if (format == 2) {		//Format 2
				locctr += 2;
				objcode[0] = opcode;
				
				i = strcspn(out3, ",\0");
				if (out3[0] == '#') {		//If parameter is decimal
					if (out3[2] == '\0') objcode[1] = (out3[1] - 48);	//Just one parameter
					else objcode[1] = ((out3[1] - 48) * 10) + (out3[2] - 48);//Two parameter that is two decimal number
				}
				else if (out3[i] == '\0') objcode[1] = 16 * Search_resister(out3);		//Just one resister
				else if (out3[i + 2] == '#') {		//one resister one decimal number
					if ((strlen(out3) - (i + 2)) == 2) objcode[1] = (out3[i + 3] - 48);
					else objcode[1] = ((out3[i + 3] - 48) * 10) + (out3[i + 4] - 48);
					out3[i] = '\0';
					objcode[1] += 16 * Search_resister(out3);
				}
				else {		//two resister
					objcode[1] = Search_resister(&out3[i + 2]);
					out3[i] = '\0';
					objcode[1] += 16 * Search_resister(out3);
				}				
				fprintf(fp2, "%-25s%02X%02X\n", line, objcode[0], objcode[1]);		//Print listing file

				if (T_size > 27) {				//If T_size greater than 27 then print a line in objfile
					fprintf(fp3, "%06X", T_size);
					for (i = 0; i < T_size; i++) {
						fprintf(fp3, "%02X", objfile[i]);
					}
					fprintf(fp3, "\n");
					T_size = -2;
					objfile[0] = objcode[0];		//Insert objcode in objfile
					objfile[1] = objcode[1];
				}
				else {
					objfile[T_size] = objcode[0];		//Insert objcode in objfile
					objfile[T_size + 1] = objcode[1];
					T_size += 2;
				}
			}
			else if (format == 1) {		//Format 1
				locctr += 1;
				objcode[0] = opcode;

				fprintf(fp2, "%-25s%02X\n", line, objcode[0]);		//Print listing file

				if (T_size > 27) {		//If T_size greater than 27 then print a line in objfile
					fprintf(fp3, "%06X", T_size);
					for (i = 0; i < T_size; i++) {
						fprintf(fp3, "%02X", objfile[i]);
					}
					fprintf(fp3, "\n");
					T_size = -1;
					objfile[0] = objcode[0];		//Insert objcode in objfile
				}
				else {
					objfile[T_size] = objcode[0];		//Insert objcode in objfile
					T_size += 1;
				}
			}
		}
		else if (!strcmp(out2, "WORD")) {		//WORD
			locctr += 3;
			num = 0;
			j = 1;
			for (i = strlen(out3) - 1; i >= 0; i--) {
				num += (out3[i] - 48)*j;
				j *= 10;
			}
			fprintf(fp2, "%-25s%06X\n", line, num);		//Print listing file

			objcode[3] = symbol_loc % 256;		//Generate objcode and insert it
			objcode[1] = 16 + (symbol_loc / 65536);
			objcode[2] = (symbol_loc / 256) - (objcode[1] % 16) * 256;

			if (T_size > 26) {		//If T_size greater than 26 then print a line in objfile
				fprintf(fp3, "%06X", T_size);
				for (i = 0; i < T_size; i++) {
					fprintf(fp3, "%02X", objfile[i]);
				}
				fprintf(fp3, "\n");
				T_size = -1;
				objfile[0] = num / 65536;		//Insert objcode in objfile
				objfile[1] = (num / 256) - (objfile[0] * 256);
				objfile[2] = num % 256;
			}
			else {
				objfile[T_size] = num / 65536;		//Insert objcode in objfile
				objfile[T_size + 1] = (num / 256) - (objfile[0] * 256);
				objfile[T_size + 2] = num % 256;
				T_size += 3;
			}
		}
		else if (!strcmp(out2, "BYTE")) {		//BYTE
			if (out3[0] == 'C') {		//If out3 contains characters
				num = strlen(out3) - 3;
				
				locctr += num;
				fprintf(fp2, "%-25s", line);

				for (i = 0; i < num; i++) fprintf(fp2, "%02X", out3[i + 2]);
				fprintf(fp2, "\n");

				if (T_size + num > 29) {		//If T_size greater than 29 then print a line in objfile
					fprintf(fp3, "%06X", T_size);
					for (i = 0; i < T_size; i++) {
						fprintf(fp3, "%02X", objfile[i]);
					}
					fprintf(fp3, "\n");
					T_size = -1 * num;
					for (i = 0; i < num; i++) objfile[i] = out3[i + 2];		//Insert objcode in objfile
				}
				else {
					for (i = 0; i < num; i++) objfile[T_size + i] = out3[i + 2];		//Insert objcode in objfile
					T_size += num;
				}
			}
			else if (out3[0] == 'X') {		//If out3 contains hexadecimal number
				locctr += 1;
				objcode[0] = change_dec(&out3[2], 1);
				fprintf(fp2, "%-25s%02X\n", line, objcode[0]);		//Print listing file

				if (T_size > 28) {		//If T_size greater than 28 then print a line in objfile
					fprintf(fp3, "%06X", T_size);
					for (i = 0; i < T_size; i++) {
						fprintf(fp3, "%02X", objfile[i]);
					}
					fprintf(fp3, "\n");
					T_size = -1;
					objfile[0] = objcode[0];		//Insert objcode in objfile
				}
				else {
					objfile[T_size] = objcode[0];		//Insert objcode in objfile
					T_size += 1;
				}
			}
		}
	}

	printf("\toutput file : [%s], [%s]\n", str2, str3);		//If complete pass2 then print message

	fclose(fp);
	fclose(fp2);
	fclose(fp3);
}

void type(char *str) {		//Print the contents of a file
	FILE* fp = fopen(str, "r");
	char line[50];


	if (fp == NULL) {		//If there isn't file then print error message
		printf("There isn't %s\n", str);
	}
	else {
		fgets(line, sizeof(line), fp);
		while (!feof(fp)) {			//If file pointer point end of file then escape loop
			printf("%s", line);
			fgets(line, sizeof(line), fp);
		}
	}
}

void Push_symbol2(char *str, int loc, int len, int num) {		//Push the symbol for ESTAB

	ESTAB *temp, *new;
	
	new = (ESTAB*)malloc(sizeof(ESTAB));
	strcpy(new->symbol, str);
	new->loc = loc;
	new->len = len;
	new->next = NULL;
	
	if (estab[num].next == NULL) estab[num].next = new;
	else {
		temp = estab[num].next;
		while (temp->next != NULL) temp = temp->next;
		temp->next = new;
	}
}

int Search_symbol2(char *str) {		//Search the symbol at ESTAB
	ESTAB *temp;
	int i;

	for (i = 0; i < 3; i++) {
		temp = estab[i].next;

		while (temp != NULL) {
			if (strcmp(str, temp->symbol) == 0) return temp->loc;		//Return loaction counter
			else temp = temp->next;
		}
	}
	return -1;
}

void Free_estable() {		//Free to ESTAB
	ESTAB *temp, *free_temp;
	int i;

	for (i = 0; i < 3; i++) {
		temp = estab[i].next;
		if (temp != NULL) {
			while (temp->next != NULL) {
				free_temp = temp;
				temp = temp->next;
				free(free_temp);
			}
			free(temp);
		}
		estab[i].next = NULL;
	}
}

void Print_estable(int num) {		//Print ESTAB
	ESTAB *temp;
	int i, total = 0;
	
	printf("control section\t\tsymbol name\t\tadrress\t\tlength\n");
	printf("--------------------------------------------------------------------------------------\n");

	for (i = 0; i < num; i++) {
		temp = estab[i].next;
		printf("%-15s\t\t           \t\t%04X   \t\t%04X  \n", temp->symbol, temp->loc, temp->len);
		total += temp->len;
		temp = temp->next;
		while (temp != NULL) {
			printf("               \t\t%-11s\t\t%04X\n", temp->symbol, temp->loc);
			temp = temp->next;
		}
	}
	printf("--------------------------------------------------------------------------------------\n");
	printf("               \t\t           \t\ttotal length\t%04X\n", total);

	total_len = total;
}

int pass3(char *str, int csaddr, int num) {		//pass1 by linking and loader
	FILE* fp = fopen(str, "r");
	char symbol[7], line[100], value[7];
	int len, i, v, j;

	if (fp == NULL) {		//If there isn't file then print error message
		printf("There isn't %s\n", str);
		Free_estable();
		return -1;
	}

	fgets(line, sizeof(line), fp);

	if (line[0] == 'H') {		//Read to head record

		strncpy(symbol, &line[1], 6);
		symbol[6] = '\0';
		i = strcspn(symbol, " \0");
		symbol[i] = '\0';
		
		if (Search_symbol2(symbol) != -1) {		//If symbol is duplicated then print error message
			printf("Duplication symbol error\n");
			fclose(fp);
			Free_estable();
			return -1;
		}

		strncpy(value, &line[13], 6);
		value[6] = '\0';
		len = change_dec(value, 5);

		Push_symbol2(symbol, csaddr, len, num);		//Push symbol in ESTAB

		fgets(line, sizeof(line), fp);

		if (line[0] == 'D') {		//Read to definition record
			i = strlen(line) / 12;
			for (j = 0; j < i; j++) {
				strncpy(symbol, &line[12*j + 1], 6);
				v = strcspn(symbol, " \0");
				symbol[v] = '\0';
				if (Search_symbol2(symbol) != -1) {		//If symbol is duplicated then print error message
					printf("Duplication symbol error\n");
					Free_estable();
					fclose(fp);
					return -1;
				}
				strncpy(value, &line[12*j + 7], 6);
				v = change_dec(value, 5) + csaddr;
				Push_symbol2(symbol, v, -1, num);
			}
		}
		fclose(fp);
		return csaddr + len;
	}
	else {
		printf("No head record\n");
		Free_estable();
		fclose(fp);
		return -1;
	}
}

int strbool3(char *str, char *str1, char *str2, char *str3) {		//Return linking file's name
	int i, j;

	i = strcspn(str, " \t");
	strncpy(str1, str, i);
	str1[i] = '\0';
	j = strspn(&str[i], " \t");

	if (str[i + j] == '\0') return 1;		//Just one program

	j = i + j;
	i = strcspn(&str[j], " \t");
	strncpy(str2, &str[j], i);
	str2[i] = '\0';
	i = i + j;
	j = strspn(&str[i], " \t");
	
	if (str[i + j] == '\0') return 2;		//Need to link two program

	j = i + j;
	i = strcspn(&str[j], " \t");
	strncpy(str3, &str[j], i);
	str3[i] = '\0';
	i = i + j;
	j = strspn(&str[i], " \t");

	if (str[i + j] == '\0') return 3;		//Need to link three program
	else return -1;
}

int pass4(char *str, int csaddr, int num) {		//pass2 by linking and loader
	FILE* fp = fopen(str, "r");
	char line[100], value[7];
	int len, tlen, tstart, i, j, k, *ref = (int*)malloc(sizeof(int) * 2);
	unsigned char v;

	fgets(line, sizeof(line), fp);

	strncpy(value, &line[13], 6);
	value[6] = '\0';
	len = change_dec(value, 5);		//Program length
	
	fgets(line, sizeof(line), fp);	
	
	while (1) {
		if (line[0] == 'E') {		//Read to end record
			if (line[1] != '\0' && line[1] != '\n') {
				strncpy(value, &line[1], 6);
				execaddr = change_dec(value, 5);		//Set excution address
			}
			fclose(fp);
			return csaddr + len;
		}
		else if (line[0] == 'M') {		//Read to modification record
			ref[0] = csaddr;
			strncpy(value, &line[1], 6);
			k = change_dec(value, 5) + csaddr;

			j = memory[k / 16][k % 16] * 65536;
			j += memory[(k + 1) / 16][(k + 1) % 16] * 256;
			j += memory[(k + 2) / 16][(k + 2) % 16];
			i = change_dec(&line[10], 1);

			if (memory[k / 16][k % 16] >= 128) j = (16777216 - j)*(-1);

			if (i - 1 > sizeof(ref)) {		//If undefined external symbol
				printf("Undefined external symbol\n");
				fclose(fp);
				return -1;
			}
			else if (line[9] == '+') j += ref[i - 1];
			else j -= ref[i - 1];

			if (j < 0) j += 16777216;

			memory[k / 16][k % 16] = j / 65536;
			memory[(k + 1) / 16][(k + 1) % 16] = (j % 65536) / 256;
			memory[(k + 2) / 16][(k + 2) % 16] = j % 256;

			fgets(line, sizeof(line), fp);
		}
		else if (line[0] == 'T') {		//Read to text record
			strncpy(value, &line[1], 6);
			tstart = change_dec(value, 5) + csaddr;
			strncpy(value, &line[7], 2);
			tlen = change_dec(value, 1);
			j = 9;

			for (i = 0; i < tlen; i++) {
				strncpy(value, &line[j], 2);
				v = change_dec(value, 1);
				memory[(tstart + i) / 16][(tstart + i) % 16] = v;
				j += 2;
			}
			fgets(line, sizeof(line), fp);			
		}
		else if (line[0] == 'D') fgets(line, sizeof(line), fp);		//Read to definition record
		else if (line[0] == 'R') {		//Read to reference record
			free(ref);
			line[strlen(line) - 1] = '\0';
			j = ((strlen(line) - 2) / 8) + 2;
			ref = (int*)malloc(sizeof(int)*j);
			ref[0] = csaddr;

			for (i = 1; i < j; i++) {
				strncpy(value, &line[8 * i - 5], 6);
				value[6] = '\0';
				k = strcspn(value, " \0");
				value[k] = '\0';
				ref[i] = Search_symbol2(value);

				if (ref[i] == -1) {
					printf("Undefined external symbol\n");
					fclose(fp);
					return -1;
				}
			}
			fgets(line, sizeof(line), fp);
		}
		else fgets(line, sizeof(line), fp);
	}

	return -1;
}

int format3(unsigned char *opcode) {		//Return value to format3
	int value = 0;

	if (opcode[0] % 4 == 0) {		//SIC
		value = ((opcode[1] % 128) * 256) + opcode[2];
		if (opcode[1] / 16 >= 8) value += reg[1];
	}
	else if (opcode[0] % 4 == 1) {		//Immediate addressing
		value = ((opcode[1] % 16) * 256) + opcode[2];
		if (opcode[1] % 16 >= 8) {		//If value is negative number
			value = 4096 - value;
			value = value * (-1);
		}
		if (opcode[1] / 16 == 2) value += reg[8];		//PC relative
		else if (opcode[1] / 16 == 4) value += reg[3];	//BASE relative
		else if (opcode[1] / 16 == 10) value = value + reg[8] + reg[1];		//PC relative + Index addresing
		else if (opcode[1] / 16 == 12) value = value + reg[3] + reg[1];		//BASE relative + Index addresing
	}
	else if (opcode[0] % 4 == 2) {		//Indirect addressing
		value = ((opcode[1] % 16) * 256) + opcode[2];
		if (opcode[1] % 16 >= 8) {
			value = 4096 - value;
			value = value * (-1);
		}
		if (opcode[1] / 16 == 2) value += reg[8];		//PC relative
		else if (opcode[1] / 16 == 4) value += reg[3];	//BASE relative
		else if (opcode[1] / 16 == 10) value = value + reg[8] + reg[1];		//PC relative + Index addresing
		else value = value + reg[3] + reg[1];		//BASE relative + Index addresing

		opcode[0] = memory[value / 16][value % 16];
		opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
		opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

		value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
	}
	else {		//Simple addressing
		value = ((opcode[1] % 16) * 256) + opcode[2];
		if (opcode[1] % 16 >= 8) {		//If value is negative number
			value = 4096 - value;
			value = value * (-1);
		}
		if (opcode[1] / 16 == 2) value += reg[8];		//PC relative
		else if (opcode[1] / 16 == 4) value += reg[3];	//BASE relative
		else if (opcode[1] / 16 == 10) value = value + reg[8] + reg[1];		//PC relative + Index addresing
		else value = value + reg[3] + reg[1];		//BASE relative + Index addresing
	}

	return value;
}

int format4(unsigned char *opcode) {		//Return value to format4
	int value = 0;

	if (opcode[0] % 4 == 1) {		//Immediate addressing
		value = ((opcode[1] % 16) * 65536) + (opcode[2] * 256) + opcode[3];

		if (opcode[1] % 16 >= 8) {		//If value is negative number
			value = 1048576 - value;
			value = value * (-1);
		}
	}
	else {		//Simple addressing
		value = ((opcode[1] % 16) * 65536) + (opcode[2] * 256) + opcode[3];
		if (opcode[1] / 16 >= 8) value += reg[1];
	}

	return value;
}

int run(int start) {		//Excute program by program address
	unsigned char opcode[4];
	int format, value, i;

	reg[8] = start;

	while (1) {
		opcode[0] = memory[reg[8] / 16][reg[8] % 16];
		reg[8]++;
		format = opcode[0] / 16;

		if (9 <= format && format <= 11) {
			opcode[1] = memory[reg[8] / 16][reg[8] % 16];
			reg[8]++;

			if (format == 10) {		//COMPR				
				if (reg[opcode[1] / 16] > reg[opcode[1] % 16]) reg[9] = -1;
				else if (reg[opcode[1] / 16] == reg[opcode[1] % 16]) reg[9] = 0;
				else reg[9] = 1;
			}
			else if (opcode[0] % 16 == 4) reg[opcode[1] / 16] = 0;		//CLEAR
			else if (opcode[0] % 16 == 8) {		//TIXR
				reg[1]++;
				if (reg[opcode[1] / 16] > reg[1]) reg[9] = -1;
				else if (reg[opcode[1] / 16] == reg[1]) reg[9] = 0;
				else reg[9] = 1;
			}
			else reg[8]--;
		}
		else {
			opcode[1] = memory[reg[8] / 16][reg[8] % 16];
			reg[8]++;
			opcode[2] = memory[reg[8] / 16][reg[8] % 16];
			reg[8]++;
			
			if (format == 0) {
				if ((opcode[1] / 16) % 2 == 0) {		//format 3
					value = format3(opcode);

					if (opcode[0] % 16 < 4) {		//LDA
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[0] = value;
					}
					else if (opcode[0] % 16 < 8) {		//LDX
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[1] = value;
					}
					else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {			//STA						
						memory[value / 16][value % 16] = reg[0] / 65536;
						memory[(value + 1) / 16][(value + 1) % 16] = (reg[0] % 65536) / 256;
						memory[(value + 2) / 16][(value + 2) % 16] = reg[0] % 256;
					}
					else reg[8] -= 2;
				}
				else {		//format 4
					opcode[3] = memory[reg[8] / 16][reg[8] % 16];
					reg[8]++;

					value = format4(opcode);

					if (opcode[0] % 16 < 4) {		//LDA
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[0] = value;
					}
					else if (opcode[0] % 16 < 8) {		//LDX
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[1] = value;
					}
					else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {		//STA
						memory[value / 16][value % 16] = reg[0] / 65536;
						memory[(value + 1) / 16][(value + 1) % 16] = (reg[0] % 65536) / 256;
						memory[(value + 2) / 16][(value + 2) % 16] = reg[0] % 256;
					}
					else reg[8] -= 3;
				}
			}
			else if (format == 1) {		//STX, STL
				if ((opcode[1] / 16) % 2 == 0) {		//format 3
					value = format3(opcode);

					if (opcode[0] % 16 < 4) {		//STX
						memory[value / 16][value % 16] = reg[1] / 65536;
						memory[(value + 1) / 16][(value + 1) % 16] = (reg[1] % 65536) / 256;
						memory[(value + 2) / 16][(value + 2) % 16] = reg[1] % 256;
					}
					else if (opcode[0] % 16 < 8) {			//STL
						memory[value / 16][value % 16] = reg[2] / 65536;
						memory[(value + 1) / 16][(value + 1) % 16] = (reg[2] % 65536) / 256;
						memory[(value + 2) / 16][(value + 2) % 16] = reg[2] % 256;
					}
					else reg[8] -= 3;
				}
				else {			//format 4
					opcode[3] = memory[reg[8] / 16][reg[8] % 16];
					reg[8]++;
					value = format4(opcode);

					if (opcode[0] % 16 < 4) {		//STX
						memory[value / 16][value % 16] = reg[1] / 65536;
						memory[(value + 1) / 16][(value + 1) % 16] = (reg[1] % 65536) / 256;
						memory[(value + 2) / 16][(value + 2) % 16] = reg[1] % 256;
					}
					else if (opcode[0] % 16 < 8) {			//STL
						memory[value / 16][value % 16] = reg[2] / 65536;
						memory[(value + 1) / 16][(value + 1) % 16] = (reg[2] % 65536) / 256;
						memory[(value + 2) / 16][(value + 2) % 16] = reg[2] % 256;
					}
					else reg[8] -= 3;
				}
			}
			else if (format == 2) {		//COMP
				if (8 <= opcode[0] % 16 && opcode[0] % 16 <= 11) {
					if ((opcode[1] / 16) % 2 == 0) {		//format 3
						value = format3(opcode);

						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}

						if (reg[0] > value) reg[9] = -1;
						else if (reg[0] == value) reg[9] = 0;
						else reg[9] = 1;
					}
					else {			//format 4
						opcode[3] = memory[reg[8] / 16][reg[8] % 16];
						reg[8]++;
						value = format4(opcode);

						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}

						if (reg[0] > value) reg[9] = -1;
						else if (reg[0] == value) reg[9] = 0;
						else reg[9] = 1;
					}
				}
				else reg[8] -= 2;
			}
			else if (format == 3) {		//JEQ, JLT, J
				if ((opcode[1] / 16) % 2 == 0) {		//format 3
					value = format3(opcode);
					
					if (opcode[0] % 16 < 4) {		//JEQ
						if (reg[9] == 0) reg[8] = value;
					}
					else if (8 <= opcode[0] % 16 && opcode[0] % 16 < 12) {		//JLT
						if (reg[9] == -1) reg[8] = value;
					}
					else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {		//J
						reg[8] = value;
					}
					else reg[8] -= 2;
				}
				else {		//format 4
					opcode[3] = memory[reg[8] / 16][reg[8] % 16];
					reg[8]++;
					value = format4(opcode);

					if (opcode[0] % 16 < 4) {		//JEQ
						if (reg[9] == 0) reg[8] = value;
					}
					else if (8 <= opcode[0] % 16 && opcode[0] % 16 < 12) {		//JLT
						if (reg[9] == -1) reg[8] = value;
					}
					else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {		//J
						reg[8] = value;
					}
					else reg[8] -= 3;
				}
			}
			else if (format == 4) {		//JSUB, RSUB
				if ((opcode[1] / 16) % 2 == 0) {		//format 3
					value = format3(opcode);
					
					if (8 <= opcode[0] % 16 && opcode[0] % 16 < 12) {		//JSUB
						reg[2] = reg[8];
						reg[8] = value;
					}
					else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {		//RSUB
						reg[8] = reg[2];
					}
					else reg[8] -= 2;
				}
				else {		//format 4
					opcode[3] = memory[reg[8] / 16][reg[8] % 16];
					reg[8]++;
					value = format4(opcode);

					if (8 <= opcode[0] % 16 && opcode[0] % 16 < 12) {		//JSUB
						reg[2] = reg[8];
						reg[8] = value;
					}
					else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {		//RSUB
						reg[8] = reg[2];
					}
					else reg[8] -= 3;
				}
			}
			else if (format == 5) {		//LDCH, STCH
				if ((opcode[1] / 16) % 2 == 0) {		//format 3
					value = format3(opcode);

					if (opcode[0] % 16 < 4) {		//LDCH
						if (opcode[0] % 4 != 1) {
							value = memory[value / 16][value % 16];;
							if (opcode[0] % 16 >= 8) {
								value = 256 - value;
								value = value * (-1);
							}
						}
						else {
							if (value < 0) value = ((value*(-1)) % 256)*-1;
							else value = value % 256;
						}

						reg[0] = value;
					}
					else if (opcode[0] % 16 < 8) {			//STCH
						memory[value / 16][value % 16] = reg[0] % 256;
					}
					else reg[8] -= 2;
				}
				else {			//format 4
					opcode[3] = memory[reg[8] / 16][reg[8] % 16];
					reg[8]++;
					value = format4(opcode);

					if (opcode[0] % 16 < 4) {		//LDCH
						if (opcode[0] % 4 != 1) {
							value = memory[value / 16][value % 16];;
							if (opcode[0] % 16 >= 8) {
								value = 256 - value;
								value = value * (-1);
							}
						}
						else {
							if (value < 0) value = ((value*(-1)) % 256)*-1;
							else value = value % 256;
						}

						reg[0] = value;
					}
					else if (opcode[0] % 16 < 8) {			//STCH
						memory[value / 16][value % 16] = reg[0] % 256;
					}
					else reg[8] -= 3;
				}
			}
			else if (format == 6) {		//LDB
				if (8 <= opcode[0] % 16 && opcode[0] % 16 < 12) {
					if ((opcode[1] / 16) % 2 == 0) {		//format 3
						value = format3(opcode);
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[3] = value;
					}
					else {			//format 4
						opcode[3] = memory[reg[8] / 16][reg[8] % 16];
						reg[8]++;
						value = format4(opcode);
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[3] = value;
					}
				}
				else reg[8] -= 2;
			}
			else if (format == 7) {		//LDT
				if (4 <= opcode[0] % 16 && opcode[0] % 16 < 8) {
					if ((opcode[1] / 16) % 2 == 0) {		//format 3
						value = format3(opcode);
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[5] = value;
					}
					else {			//format 4
						opcode[3] = memory[reg[8] / 16][reg[8] % 16];
						reg[8]++;
						value = format4(opcode);
						if (opcode[0] % 4 != 1) {
							opcode[0] = memory[value / 16][value % 16];
							opcode[1] = memory[(value + 1) / 16][(value + 1) % 16];
							opcode[2] = memory[(value + 2) / 16][(value + 2) % 16];

							value = (opcode[0] * 65536) + (opcode[1] * 256) + opcode[2];
							if (opcode[0] % 16 >= 8) {
								value = 16777216 - value;
								value = value * (-1);
							}
						}
						reg[5] = value;
					}
				}
				else reg[8] -= 2;
			}
			else if (format == 13) {	//RD, WD
				if (8 <= opcode[0] % 16 && opcode[0] % 16 < 12) {
					reg[0] = 0;
				}
				else if (12 <= opcode[0] % 16 && opcode[0] % 16 <= 15) {

				}
				else reg[8] -= 2;
			}
			else if (format == 14) {	//TD
				if (opcode[0] % 16 < 4) reg[9] = -1;
				else reg[8] -= 2;
			}
			else reg[8] -= 2;
		}

		if (reg[8] == progaddr + total_len) {		//PC register == Program address + Program total length
			flag = 0;
			return 0;
		}
		else {
			i = 0;
			while (bp[i] != -1) {
				if (reg[8] == bp[i]) {		//PC register == Break point
					bpi = i;
					flag = 1;
					return 1;
				}
				i++;
			}
		}
	}
	
	return 0;
}

void Print_register() {		//Print register's value
	printf("A : %06X\tX : %06X\n", reg[0], reg[1]);		//Print A, X register
	printf("L : %06X\tPC: %06X\n", reg[2], reg[8]);		//Print L, PC register
	printf("B : %06X\tS : %06X\n", reg[3], reg[4]);		//Print B, S register
	printf("T : %06X\n", reg[5]);		//Print T register
}

int main() {

	char *str, str1[20], str2[20], str3[20];
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
		case 13: //assemble filename
			str[ins[2] + i] = '\0';
			if (pass1(&str[ins[1] + i]) == -1) Free_symboltable();
			else Push_hi(str);
			break;
		case 14: //symbol
			Print_symbol();
			Push_hi(str);
			break;
		case 15: //type filename
			str[ins[2] + i] = '\0';
			type(&str[ins[1] + i]);
			Push_hi(str);
			break;
		case 16: //progaddr
			progaddr = ins[1];
			Push_hi(str);
			break;
		case 17: //loader
			ins[2] = strbool3(&str[ins[1] + i], str1, str2, str3);
		
			if (ins[2] == -1) {
				printf("Please write again\n");
			}
			else {
				Free_estable();
				ins[1] = pass3(str1, progaddr, 0);
				if (ins[1] == -1) break;
				else if (ins[2] == 2) ins[1] = pass3(str2, ins[1], 1);
				else if (ins[2] == 3) {
					ins[1] = pass3(str2, ins[1], 1);
					if (ins[1] == -1) break;
					ins[1] = pass3(str3, ins[1], 2);
				}
				if (ins[1] == -1) break;

				ins[1] = pass4(str1, progaddr, 0);
				if (ins[1] == -1) break;
				else if (ins[2] == 2) ins[1] = pass4(str2, ins[1], 1);
				else if (ins[2] == 3) {
					ins[1] = pass4(str2, ins[1], 1);
					if (ins[1] == -1) break;
					ins[1] = pass4(str3, ins[1], 2);
				}
				if (ins[1] == -1) break;

				Print_estable(ins[2]);
				Push_hi(str);
			}
			break;
		case 18: //run
			if (total_len != 0) {
				if (flag == 0) {
					reg[2] = progaddr + total_len;
					i = run(progaddr + execaddr);
				}
				else {
					i = run(bp[bpi]);
				}
				Print_register();
				if (i == 0) printf("End Program\n");
				else {
					printf("Stop at checkpoint[%X]\n", bp[bpi]);
				}
			}
			else printf("You have to load program!\n");
			break;
		case 19: //bp
			i = 0;
			printf("breakpoint\n");
			printf("-----------\n");

			while (bp[i] != -1) {
				printf("%X\n", bp[i]);
				i++;
			}
			Push_hi(str);
			break;
		case 20: //bp clear
			for (i = 0; i < 20; i++) bp[i] = -1;
			flag = 0;
			printf("\t[ok] clear all breakpoints\n");
			Push_hi(str);
			break;
		case 21: //bp num
			i = 0;
			while (bp[i] != -1) i++;
			bp[i] = ins[2];
			printf("\t[ok] create breakpoint %X\n", bp[i]);
			Push_hi(str);
			break;
		}
		
		free(str);
	}

	return 0;
}