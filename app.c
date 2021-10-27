#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<time.h>

#define or ||
#define and &&
#define MAX 500

int totalQuestions = 0, cnt = 0;

int min(int a, int b){
	if(a < b)
		return a;
	return b;
}

struct User{
	int ID;
	char password[MAX], name[MAX];
	char type;
} curUser;

struct Option{
	char data[MAX];
	char correct;
};

struct BinaryTreeNode{

	int ID;
	char question[MAX];
	struct Option options[4];

	struct BinaryTreeNode *left, *right;
} *root = NULL;

struct QuizNode{
	float time_taken;
	int score, front, back, sz;
	struct BinaryTreeNode data[MAX];
} curQuiz;

void logout(){

	/*

	Sets current user to NULL.

	*/

	curUser.ID = 0;
	strcpy(curUser.password, "\0");
	curUser.type = '\0';

	printf("Logged Out!\n");
}

void putRequest(){

	/*

	Appends the Username, ID, Password and type to "Requests.txt".

	*/

	FILE *fptr = fopen("Requests.txt", "a");
	fprintf(fptr, "%s %d %s %c\n", curUser.name, curUser.ID, curUser.password, curUser.type);
	fclose(fptr);
}

void checkInfo(){

	/*
	Checks if the User ID and password entered are valid or not.
	Validation is given if an identical entry exists in the 
	"TeachersIDPW.txt" or "StudentsIDPW.txt". If the ID does not 
	exist in the entry then a request is put in the "request.txt" 
	that can be approved when Admin login is done.
	*/

	if(curUser.ID == 0){
		if(strcmp(curUser.password, "admin") == 0){
			printf("Welcome Admin!\n");
			curUser.type = 'A';
		}
		else
			printf("Nice Try!\n");
	}

	else{

		FILE *fptr;

		if((fptr = fopen("TeachersIDPW.txt", "r")) == NULL){
			fptr = fopen("TeachersIDPW.txt", "w");
			fclose(fptr);
		}

		fptr = fopen("TeachersIDPW.txt", "r");

		int tmpID;
		bool flag = 0;
		char tmpPass[MAX], tmpName[MAX];

		while(!feof(fptr)){
			fscanf(fptr, "%d %s %s", &tmpID, tmpPass, tmpName);
			if(tmpID == curUser.ID){
				flag = 1;
				if(strcmp(tmpPass, curUser.password) == 0){
					printf("Welcome %s!\n", tmpName);
					curUser.type = 'T';
					strcpy(curUser.name, tmpName);
					break;
				}
				else{
					curUser.type = '\0';
					printf("Wrong Password!\n");
					break;
				}
			}	
		}

		fclose(fptr);

		if(flag)
			return;

		if((fptr = fopen("StudentsIDPW.txt", "r")) == NULL){
			fptr = fopen("StudentsIDPW.txt", "w");
			fclose(fptr);
		}

		fptr = fopen("StudentsIDPW.txt", "r");

		while(!feof(fptr)){
			fscanf(fptr, "%d %s %s", &tmpID, tmpPass, tmpName);
			if(tmpID == curUser.ID){
				flag = 1;
				if(strcmp(tmpPass, curUser.password) == 0){
					printf("Welcome %s!\n", tmpName);
					curUser.type = 'S';
					strcpy(curUser.name, tmpName);
					break;
				}
				else{
					curUser.type = '\0';
					printf("Wrong Password!\n");
					break;
				}
			}	
		}

		fclose(fptr);

		if(flag)
			return;

		printf("Please enter your name:\t");
		scanf("%s", curUser.name);
		getc(stdin);
		do{
			printf("Please enter User type:\t(T)eacher\t(S)tudent\t");
			curUser.type = toupper(getc(stdin));
			getc(stdin);
		} while(curUser.type != 'S' and curUser.type != 'T');

		putRequest();
		logout();

		printf("A request has been put. Please wait for the Admin to approve!\n");
	}
}

void getInfo(){

	/*
	Gets information during the start of the program from the user.
	*/

	printf("Enter your Unique ID:\t");
	scanf("%d", &curUser.ID);
	getc(stdin);

	printf("Enter your password:\t");
	scanf("%s", curUser.password);
}

void addInfo(){

	/*
	Adds User the respective database.
	*/

	FILE *fptr;

	if(curUser.type == 'T')
		fptr = fopen("TeachersIDPW.txt", "a");

	if(curUser.type == 'S')
		fptr = fopen("StudentsIDPW.txt", "a");

	fprintf(fptr, "%d %s %s\n", curUser.ID, curUser.password, curUser.name);

	fclose(fptr);
}

void approveRequests(){

	/*

	Displays user info from "Requests.txt". Depending on Admin's response
	the User info is added to the respective file.

	*/

	FILE *fptr = fopen("Requests.txt", "r");

	if(fptr == NULL){
		printf("No Requests!\n");
		return;
	}

	char flag;

	while(!feof(fptr)){
		fscanf(fptr, "%s %d %s %c\n", curUser.name, &curUser.ID, curUser.password, &curUser.type);
		printf("Username:\t%s\n", curUser.name);
		printf("User ID:\t%d\n", curUser.ID);
		printf("Password:\t%s\n", curUser.password);
		if(curUser.type == 'T')
			printf("Type:\tTeacher\n");
		else
			printf("Type:\tStudent\n");

		printf("Approve?\t(Y)es\t(N)o\n");
		
		scanf(" %c", &flag);
		flag = toupper(flag);

		if(flag == 'Y'){
			addInfo();
			printf("Approved!\n");
		}
		else{
			printf("Not Approved!\n");
		}
	}

	fclose(fptr);
	remove("Requests.txt");
}

struct BinaryTreeNode *insert(struct BinaryTreeNode *node, struct BinaryTreeNode key){

	/*

	Copies data from key to the actual data in the binary tree at
	the appropriate node.

	*/

	if(node == NULL){
		node = (struct BinaryTreeNode *)malloc(sizeof(struct BinaryTreeNode));
		node->ID = key.ID;
		strcpy(node->question, key.question);
		int i;
		for(i = 0; i < 4; i++){
			strcpy(node->options[i].data, key.options[i].data);
			node->options[i].correct = key.options[i].correct;
		}
		node->right = node->left = NULL;
	}

	else if(key.ID < node->ID)
		node->left = insert(node->left, key);
	else
		node->right = insert(node->right, key);

	return node;
}

struct BinaryTreeNode *minValueNode(struct BinaryTreeNode *node){
	struct BinaryTreeNode *cur = node;

	while(cur->left != NULL)
		cur = cur->left;

	return cur;
}

struct BinaryTreeNode *delete(struct BinaryTreeNode *node, int key){

	if(node == NULL)
		return node;

	if(key < node->ID)
		node->left = delete(node->left, key);

	else if(key > node->ID)
		node->right = delete(node->right, key);

	else{
		if(node == root){
			free(root);
			return NULL;
		}
		else if(node->left == NULL){
			struct BinaryTreeNode *temp = node->right;
			free(node);
			return temp;
		}
		else if(node->right == NULL){
			struct BinaryTreeNode *temp = node->left;
			free(node);
			return temp;
		}
		else{
			struct BinaryTreeNode *temp = minValueNode(node->right);

			node->ID = temp->ID;
			strcpy(node->question, temp->question);
			int i;
			for(i = 0; i < 4; i++){
				strcpy(node->options[i].data, temp->options[i].data);
				node->options[i].correct = temp->options[i].correct;
			}
		}
	}
}

void deleteQuestionFromDB(int key){

	FILE *fptr = fopen("Questions.txt", "r"), *fptr2 = fopen("temp.txt", "w");

	struct BinaryTreeNode cur;

	int i;

	while(!feof(fptr)){

		fscanf(fptr, "%d", &cur.ID);

		getc(fptr);
		fgets(cur.question, MAX, fptr);

		for(i = 0; i < 4; i++){
			fgets(cur.options[i].data, MAX, fptr);
			cur.options[i].correct = getc(fptr);
			getc(fptr);
		}

		getc(fptr);

		if(cur.ID != key){
			fprintf(fptr2, "%d\n", cur.ID);
			fputs(cur.question, fptr2);
			for(i = 0; i < 4; i++){
				fprintf(fptr2, "%s%c\n", cur.options[i].data, cur.options[i].correct);
			}
		}
	}

	fclose(fptr);
	fclose(fptr2);

	remove("Questions.txt");
	rename("temp.txt", "Questions.txt");
}

bool search(struct BinaryTreeNode *node, int key){

	/*
	Searches for a particular ID in the binary tree.
	*/

	if(node == NULL)
		return false;
	if(node->ID == key)
		return true;
	if(node->ID > key)
		return search(node->left, key);
	return search(node->right, key);
}

int generateID(){

	/*
	Generates a random unique ID for the question between [1...1e4]
	for insertion in the binary tree.
	*/

	int tmp;
	do{
		tmp = rand()%10000;
		tmp++; 
	}while(search(root, tmp) == true);
	return tmp;
}

void addQuestion(){

	/*
	Adds Question to the database and the Binary Tree:
	1. Asks for the question.
	2. Asks for the options and their correctness.
	3. Inserts them into Binary Tree and database.
	*/

	struct BinaryTreeNode newQuestion;

	newQuestion.ID = generateID();

	printf("Enter Question:\t");
	getc(stdin);
	gets(newQuestion.question);

	int i;

	for(i = 0; i < 4; i++){
		printf("Enter Option #%d:\t", i+1);
		scanf("%s", newQuestion.options[i].data);
		printf("Is this the correct option?\t(Y)es\t(N)o\n");
		getc(stdin);
		newQuestion.options[i].correct = toupper(getc(stdin));
	}

	FILE *fptr = fopen("Questions.txt", "a");
	
	fprintf(fptr, "%d\n", newQuestion.ID);
	fputs(newQuestion.question, fptr);
	fprintf(fptr, "\n");

	for(i = 0; i < 4; i++){
		fprintf(fptr, "%s\n%c\n", newQuestion.options[i].data, newQuestion.options[i].correct);
	}

	root = insert(root, newQuestion);
	totalQuestions++;

	printf("Added!\n");

	fclose(fptr);
}

void displayQuestions(struct BinaryTreeNode *node){

	/*
	Uses Inorder Traversal to display IDs, Questions and Answers
	of the nodes present in the Binary Tree.
	*/

	if(node == NULL)
		return;

	displayQuestions(node->left);

	if(node->question[strlen(node->question) - 1] != '\n')
		printf("ID:\t%d\nQ:\t%s\n", node->ID, node->question);
	else
		printf("ID:\t%d\nQ:\t%s", node->ID, node->question);

	int i;

	for(i = 0; i < 4; i++){
		if((node->options[i]).correct == 'Y'){
			if((node->options[i]).data[strlen((node->options[i]).data) - 1] != '\n')
				printf("A:\t%s\n", (node->options[i]).data);
			else
				printf("A:\t%s", (node->options[i]).data);
		}
	}

	displayQuestions(node->right);
}

void deleteQuestion(){

	/*
	Searches and Deletes Question by their respective IDs:

	1. Displays all the Questions and their IDs.
	2. Asks for confirming the deletion by asking the user for
	their password again.
	3. Deletes the question from the binary tree and "Questions.txt".

	*/

	FILE *fptr = fopen("Questions.txt", "r");

	if(fptr == NULL){
		printf("No Questions in the database!\n");
		return;
	}

	fclose(fptr);

	displayQuestions(root);

	printf("\n");

	int tmpID;
	char tmpPass[MAX];

	printf("Enter ID of the question to be deleted:\t");
	scanf("%d", &tmpID);

	if(!search(root, tmpID)){
		printf("Invalid ID!");
	}

	else{
		printf("Enter your password again to confirm deletion:\t");
		scanf("%s", tmpPass);
		if(strcmp(tmpPass, curUser.password) == 0){
			root = delete(root, tmpID);
			deleteQuestionFromDB(tmpID);
			printf("Deleted!\n");
			totalQuestions--;
		}
	}

	if(root == NULL)
		remove("Questions.txt");
}

void makeTree(){

	/*

	Makes the Binary Tree of the questions and answers when the
	program is started.

	1. Reads Questions and Answers from "Questions.txt".
	2. Inserts them into the Binary Tree on the basis of their IDs.

	*/

	FILE *fptr;

	if((fptr = fopen("Questions.txt", "r")) == NULL){
		return;
	}

	fclose(fptr);
 
	fptr = fopen("Questions.txt", "r");

	int i;
	struct BinaryTreeNode key;

	while(!feof(fptr)){

		fscanf(fptr, "%d", &key.ID);

		getc(fptr);
		fgets(key.question, MAX, fptr);

		for(i = 0; i < 4; i++){
			fgets(key.options[i].data, MAX, fptr);
			key.options[i].correct = getc(fptr);
			getc(fptr);
		}

		getc(fptr);

		root = insert(root, key);
		totalQuestions++;
	}

	fclose(fptr);
}

void TeacherMenu(char option){

	/*

	Displays the options available for the teacher mode to operate on.

	*/

	switch(option){
		case 'A':
			addQuestion();
			break;
		case 'D':
			deleteQuestion();
			break;
		default:
			printf("Invalid Selection!\n");
			break;
	}
}

void AdminMenu(){

	char type = 'N';

	do{
		printf("Logout?\t(Y)es\t(N)o\n");
		scanf(" %c", &type);
		type = toupper(type);
		approveRequests();
	}while(type != 'Y');

	logout();
}

void swap(int arr[], int i, int j){
	int temp;
	temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void shuffle(int arr[], int n){
	int i, j;
	for(i = n-1; i > 0;i--){
		j = rand()%(i+1);
		swap(arr, i, j);
	}
}

struct BinaryTreeNode *find(struct BinaryTreeNode *node, int cntNeeded){

	if(node == NULL)
		return NULL;

	struct BinaryTreeNode *temp = NULL, *ret = NULL;

	temp = find(node->left, cntNeeded);
	if(temp != NULL) ret = temp;

	cnt++;
	if(cntNeeded == cnt){
		 temp = node;
	}

	if(temp != NULL) ret = temp;

	temp = find(node->right, cntNeeded);
	if(temp != NULL) ret = temp;

	return ret;
}

void push(struct BinaryTreeNode *node){
	curQuiz.sz++;
	if(curQuiz.front == -1){
		curQuiz.front = curQuiz.back = 0;
		curQuiz.data[curQuiz.back] = *node;
	}

	else{
		curQuiz.back = (curQuiz.back + 1)%MAX;
		curQuiz.data[curQuiz.back] = *node;
	}
}

struct BinaryTreeNode pop(){
	curQuiz.sz--;
	struct BinaryTreeNode temp = curQuiz.data[curQuiz.front];
	if(curQuiz.back == curQuiz.front){
		curQuiz.front = curQuiz.back = -1;
	}
	else{
		curQuiz.front = (curQuiz.front + 1)%MAX;
	}
	return temp;
}

void pickQuestions(int limit){

	int arr[totalQuestions];

	for(int i = 1; i <= totalQuestions; i++)
		arr[i-1] = i;

	shuffle(arr, totalQuestions);

	for(int i = 0; i < min(limit, totalQuestions); i++){
		cnt = 0;
		push(find(root, arr[i]));
	}

	return;
}

void makeQuiz(){

	curQuiz.score = curQuiz.sz = 0;
	curQuiz.front = curQuiz.back = -1;

	pickQuestions(3);

	printf("Quiz is Ready!\n");
}

void startQuiz(){

	struct BinaryTreeNode cur;
	int i, option;

	while(curQuiz.sz > 0){
		cur = pop();

		printf("Q:\t%s", cur.question);
		
		for(i = 0; i < 4; i++){
			if(cur.options[i].data[strlen(cur.options[i].data) - 1] == '\n')
				printf("Option #%d:\t%s", i+1, cur.options[i].data);
			else
				printf("Option #%d:\t%s\n", i+1, cur.options[i].data);
		}
		printf("Option #5:\tAttempt Later!\n");
		printf("Option #6:\tPass!\n");

		scanf("%d", &option);

		if(option == 6){
			continue;
		}
		else if(option == 1 or option == 2 or option == 3 or option == 4){
			if(cur.options[option-1].correct == 'Y'){
				curQuiz.score += 4;
			}
			else{
				curQuiz.score -= 1;
			}
		}
		else{
			push(&cur);
		}
	}
}

void displayResult(){

	printf("Score:\t%d\n", curQuiz.score);
	printf("Time Taken:\t%f seconds\n", curQuiz.time_taken);

}

void StudentMenu(){

	char type;
	struct timeval start, end;

	while(1){
		printf("Do you want to take a quiz?\t(Y)es\t(N)o\n");
		scanf(" %c", &type);
		type = toupper(type);

		if(type == 'Y'){
			makeQuiz();

			gettimeofday(&start, NULL);
			startQuiz();
			gettimeofday(&end, NULL);

			curQuiz.time_taken = (float) (end.tv_usec - start.tv_usec) / 1000000 +
	         (float) (end.tv_sec - start.tv_sec);

			displayResult();
		}

		else if(type == 'N'){
			printf("Good day %s!\n", curUser.name);
			logout();
			break;
		}

		else{
			printf("Invalid Input!\n");
		}

	}
	
	return;
}

void Initialize(){

	/*
	Initializes the components of the program necessary for running:
	1. Makes a Binary Tree of the questions and answers.
	2. Put current user as Null.
	*/

	makeTree();
	logout();
	srand(time(0));
}

int main(){

	/*
	Main function provides the user with basic functions through a menu.
	1. Type of User
	2. User ID and Password
	3. Operation to be done
	*/

	bool flag = 0;
	char type = '\0';

	Initialize();

	printf("Welcome to the QuizMaster!\n");

	while(1){

		while(1){
			printf("(L)ogin\t(Q)uit\n");
			scanf(" %c", &type);
			type = toupper(type);
			if(type != 'L' and type != 'Q')
				printf("Invalid Input!\n");
			else
				break;
		}

		if(type == 'L'){
			do{
				getInfo();
				checkInfo();
			} while(curUser.type == '\0');
		}

		else{
			printf("Thank you for using QuizMaster!\nHave a good day!\n");
			break;
		}


		if(curUser.type == 'T'){ // Teacher
			while(1){

				printf("(A)dd a new question.\t(D)elete a question.\t(L)ogout.\n");
				
				scanf(" %c", &type);
				type = toupper(type);

				if(type == 'A' or type == 'D')
					TeacherMenu(type);
				else if(type == 'L'){
					logout();
					break;
				}
				else
					printf("Invalid Input!");
			}
		}

		else if(curUser.type == 'A'){
			AdminMenu();
		}

		else{
			StudentMenu();
		}
	}

	return 0;
}