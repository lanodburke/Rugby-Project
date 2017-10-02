#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
// Donal Burke - G00337729

// player struct
typedef struct player_s {
	int irfu;
	char name[20];
	char surname[20];
	int age;
	float height;
	char club[20];
	char email[20];
	int position;
	int tackles;
	int metres;
	float weight;

}player_t;

// list struct
struct node {
	player_t player;
	struct node* NEXT;
};

// user / login struct
typedef struct {
	char username[6];
	char password[6];
}user;

// login methods
int validate_user(user* login);
void init_user(user* login);
void display_user(user* login);

// list methods
void addnode(struct node* headptr, int irfu);
void addStart(struct node** headptr, int irfu);
void deleteStart(struct node** headptr);
void delete(struct node* headptr, int irfunumber);
void displayDetails(struct node* headptr);
void displayAll(struct node* headptr);
int search(struct node* headptr, int irfu);
void sort(struct node* headptr);
void updateStatistics(struct node* headptr);
void generateStatistics(struct node* headptr, int option);
void printToReportFile(struct node* headptr);
void inputFile(struct node* head, FILE* infile);
int initialInputFile(struct node** head, FILE * infile);
void outputFile(struct node* headptr);
void sortHeightWeight(struct node* head);

void main() {
	user* login;
	login = (user*)malloc(sizeof(user) * 3);

	struct node* head_ptr;
	head_ptr = NULL;

	int option, irfu, players, i;
	FILE* rugbyFile;

	init_user(login);
	
	// read in previous data from file
	if ((rugbyFile = fopen("Rugby.txt", "r")) == NULL) {
		printf("Starting new File\n");
	}
	else {
		players = initialInputFile(&head_ptr, rugbyFile);
		for (i = 1; i < players; i++) {
			inputFile(head_ptr, rugbyFile);
		}
		fclose(rugbyFile);
	}
	// validate the login, then start the menu
	if (validate_user(login) == 1) {
		do {
			printf("\n------- Menu -------------------------------------------------------------------\n\t1) Add player\n\t2) Display all players to screen\n\t3) Display player details\n\t4) Update a players statistics\n\t5) Delete player\n\t6) Generate statistics\n\t7) Print player details to report file\n\t8) List all players in the following categories in order of height\n");
			printf("\tEnter option: ");
			scanf("%d", &option);
			switch (option) {
			case -1:
				// outputs to file if they enter -1 and exits program
				outputFile(head_ptr);
				exit(0);
				break;
			case 1:
				printf("Please enter the IRFU Number: ");
				scanf("%d", &irfu);
				if (head_ptr == NULL) {
					addStart(&head_ptr, irfu);
					printf("\n");
				}
				else {
					if (!(search(head_ptr, irfu) == 1)) {
						addnode(head_ptr, irfu);
						printf("\n");
					}
					else {
						printf("This Player already exists.....\n");
					}
				}
				break;
			case 2:
				displayAll(head_ptr);
				printf("\n");
				break;
			case 3:
				displayDetails(head_ptr);
				printf("\n");
				break;
			case 4:
				updateStatistics(head_ptr);
				printf("\n");
				break;
			case 5:
				if (head_ptr == NULL) {
					printf("There are no players to delete!");
				}
				else if (length(head_ptr) == 1) {
					deleteStart(&head_ptr);
				}
				else {
					printf("Please enter the IRFU of the player you want to delete:\n");
					scanf("%d", &irfu);
					delete(head_ptr, irfu);
				}
				break;
			case 6:
				generateStatistics(head_ptr, 0);
				printf("\n");
				break;
			case 7:
				printToReportFile(head_ptr);
				printf("\n");
				break;
			case 8:
				sortHeightWeight(head_ptr);
				printf("\n");
				break;
			default:
				printf("That is not a valid option!\n");
				break;
			}
		} while (option != -1);
	}
}
// initialise user struct
void init_user(user* login) {
	FILE* file;
	int i = 0;
	file = fopen("login.txt", "r");
	if (file == NULL) {
		printf("File doesnt exist!");
	}
	else {
		do {
			fscanf(file, "%s %s", login[i].username, login[i].password);
			i++;
		} while (feof(file) == 0);
	}
}
// method to check the users in the file - not used
void display_user(user* login) {
	int i;
	for (i = 0; i < 3; i++) {
		printf("\nUsername: %s, Password: %s", (login + i)->username, (login + i)->password);
	}
}
// validate user - enter username and password
int validate_user(user* login) {
	char name[6], password[128], c;
	int index = 0;
	int i;

	printf("Please enter your username: ");
	scanf("%s", name);
	printf("Please enter your password (6 characters): ");
	while ((c = getch()) != 13) {
		if (index < 0)
			index = 0;
		if (c == 8) {
			putch('\b');
			putch(NULL);
			putch('\b');
			index--;
			continue;
		}
		password[index++] = c;
		putch('*');
	}
	password[index] = '\0';
	printf("\n");

	for (i = 0; i < 3; i++) {
		if (strcmp(name, (login + i)->username) == 0 && strcmp(password, (login + i)->password) == 0) {
			printf("You have successfully logged in!\n");
			return 1;
		}
	}
	printf("That is not a valid username or password!\n");
	return 0;
}
// add a node
void addnode(struct node* headptr, int irfunumber) {
	char tempemail[25];
	int found = 0;

	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));
	temp = headptr;

	while (temp->NEXT != NULL) {
		temp = temp->NEXT;
	}

	struct node *newNode;
	newNode = (struct node*)malloc(sizeof(struct node));

	newNode->player.irfu = irfunumber;
	printf("Please enter: First Name : Surname : Age : Height : Weight : Club : Email.\n");
	scanf("%s %s %d %f %f %s %s", newNode->player.name, newNode->player.surname, &newNode->player.age, &newNode->player.height, &newNode->player.weight, newNode->player.club, tempemail);
	// email validation
	while (found != -1) {
		if (strchr(tempemail, '@') && strchr(tempemail, 'com') && strchr(tempemail, '.')) {
			strcpy(newNode->player.email, tempemail);
			found = -1;
		}
		else {
			printf("Email is not valid please include @ and .com!\n");
			printf("Please Enter the Email of the Player: ");
			scanf("%s", tempemail);
			if (strchr(tempemail, '@') && strchr(tempemail, 'com') && strchr(tempemail, '.')) {
				strcpy(newNode->player.email, tempemail);
				found = -1;
			}
		}
	}

	printf("Please Enter the new Position of the Player:\n");
	printf("1) Prop\n2)Hooker\n3) Second Row\n4) Back Row\n5) Half Back\n6) Centres\n7) Wingers/Full Back\n");
	scanf("%d", &newNode->player.position);

	printf("How many tackles does the player miss per match?:\n");
	printf("1) Never\n2) Less than three times per match\n3) Less than five times per match\n4) More than five times per match\n");
	scanf("%d", &newNode->player.tackles);

	printf("How many metres does the player make in a game:\n");
	printf("1) None\n2) Less than 10 metres\n3) Less than 20 metres\n4) More than 20 metres\n");
	scanf("%d", &newNode->player.metres);

	newNode->NEXT = NULL;

	temp->NEXT = newNode;
	// sort the players 
	sort(headptr);
}

void addStart(struct node** headptr, int irfunumber) {
	char tempemail[25];
	int found = 0;

	struct node* newNode;
	newNode = (struct node*)malloc(sizeof(struct node));
	newNode->player.irfu = irfunumber;

	printf("Please enter: First Name : Surname : Age : Height : Weight : Club : Email.\n");
	scanf("%s %s %d %f %f %s %s", newNode->player.name, newNode->player.surname, &newNode->player.age, &newNode->player.height, &newNode->player.weight, newNode->player.club, tempemail);

	while (found != -1) {
		if (strchr(tempemail, '@') && strchr(tempemail, 'com') && strchr(tempemail, '.')) {
			strcpy(newNode->player.email, tempemail);
			found = -1;
		}
		else {
			printf("Email is not valid please include @ and .com!\n");
			printf("Please Enter the Email of the Player: ");
			scanf("%s", tempemail);
			if (strchr(tempemail, '@') && strchr(tempemail, 'com') && strchr(tempemail, '.')) {
				strcpy(newNode->player.email, tempemail);
				found = -1;
			}
		}
	}

	printf("Please Enter the new Position of the Player:\n");
	printf("1) Prop\n2)Hooker\n3) Second Row\n4) Back Row\n5) Half Back\n6) Centres\n7) Wingers/Full Back\n");
	scanf("%d", &newNode->player.position);

	printf("How many tackles does the player miss per match?:\n");
	printf("1) Never\n2) Less than three times per match\n3) Less than five times per match\n4) More than five times per match\n");
	scanf("%d", &newNode->player.tackles);

	printf("How many metres does the player make in a game:\n");
	printf("1) None\n2) Less than 10 metres\n3) Less than 20 metres\n4) More than 20 metres\n");
	scanf("%d", &newNode->player.metres);

	newNode->NEXT = *headptr;

	*headptr = newNode;
}

void deleteStart(struct node** headptr) {
	struct node *temp;

	temp = *headptr;
	*headptr = temp->NEXT;

	free(temp);
}

void delete(struct node* headptr, int irfunumber) {
	struct node *temp;
	struct node *oldtemp;
	int i;
	temp = (struct node*)malloc(sizeof(struct node));
	oldtemp = (struct node*)malloc(sizeof(struct node));
	temp = headptr;

	// loop through list til you get the element at position
	for (i = 0; i < length(headptr); i++) {
		if (temp->player.irfu == irfunumber) {
			break;
		}
		oldtemp = temp;
		temp = temp->NEXT;
	}

	// bypass the deleted element
	oldtemp->NEXT = temp->NEXT;

	// free up deleted element memory
	free(temp);
	printf("Node has been deleted\n\n");
}
// display the details for one player
void displayDetails(struct node* headptr) {
	char pos[7][20] = { "Prop ", "Hooker ", "Second Row ", "Back Row ", "Half Back ", "Centre ", "Wingers\Full Back " };
	char mets[4][10] = { "None ",  "< 10M", "< 20M", "> 20M" };
	char tacks[4][10] = { "Never ", "< 3 ",  "< 5 ",  "> 5 " };
	int irfunumber, option;
	char name [25], surname[25];

	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));

	temp = headptr;

	printf("Enter\n1) To search for player by IRFU:\n2) To search for player by full name:\n");
	scanf("%d", &option);

	if (option == 1) {
		printf("Please enter the players IRFU: ");
		scanf("%d", &irfunumber);
		printf("\n");
	}
	else if (option == 2) {
		printf("Please enter the players full name: ");
		scanf("%s %s", name, surname);
		printf("\n");
	}
	else {
		printf("That is not a valid option");
	}
	if (headptr != NULL) {
		printf("All Player Details\n");
	}
	else {
		printf("No details Available");
	}
	while (temp != NULL) {
		if (temp->player.irfu == irfunumber || strcmp(temp->player.name, name) == 0 && strcmp(temp->player.surname, surname) == 0) {
			printf("IRFU: %d\n", temp->player.irfu);
			printf("Name: %s %s\n", temp->player.name, temp->player.surname);
			printf("Age: %d\n", temp->player.age);
			printf("Height: %.2f\n", temp->player.height);
			printf("Weight: %.2f\n", temp->player.weight);
			printf("Club: %s\n", temp->player.club);
			printf("Email: %s\n", temp->player.email);
			printf("Position: %s\n", pos[temp->player.position - 1]);
			printf("Tackles Missed: %s\n", tacks[temp->player.tackles - 1]);
			printf("Metres: %s\n", mets[temp->player.metres - 1]);
			printf("\n");
		}
		temp = temp->NEXT;
	}
}
// display the irfu and first name for all players
void displayAll(struct node* headptr) {
	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));

	temp = headptr;

	if (headptr != NULL) {
		printf("Player Details:\n");
		printf("------------------------------\n");
	}
	else {
		printf("No details Available");
	}
	while (temp != NULL) {
		printf("IRFU: %d\n", temp->player.irfu);
		printf("Name: %s %s\n", temp->player.name, temp->player.surname);
		printf("\n");
		temp = temp->NEXT;
	}
}

int search(struct node* headptr, int irfu) {
	struct node *temp;
	int found = 0;
	temp = headptr;
	while (temp != NULL) {
		if (temp->player.irfu == irfu) {
			found = 1;
		}
		temp = temp->NEXT;
	}
	return found;
}

int length(struct node* headptr) {
	int len = 0;
	while (headptr != NULL) {
		len++;
		headptr = headptr->NEXT;
	}

	return len;
}
// sorting the player
void sort(struct node* headptr) {
	struct node* temp;
	struct node* count;

	int irfunumber, age, tackles, metres, position;
	float height, weight;
	char name[20], surname[20], club[30], email[30];

	temp = headptr;

	for (; temp->NEXT != NULL; temp = temp->NEXT) {
		for (count = temp->NEXT; count != NULL; count = count->NEXT) {
			if (temp->player.irfu > count->player.irfu) {
				// copy values to temp variables
				irfunumber = temp->player.irfu;
				age = temp->player.age;
				tackles = temp->player.tackles;
				metres = temp->player.metres;
				height = temp->player.height;
				weight = temp->player.weight;
				strcpy(name, temp->player.name);
				strcpy(surname, temp->player.surname);
				strcpy(club, temp->player.club);
				strcpy(email, temp->player.email);
				position = temp->player.position;

				// moving up values
				temp->player.irfu = count->player.irfu;
				temp->player.age = count->player.age;
				temp->player.tackles = count->player.tackles;
				temp->player.metres = count->player.metres;
				temp->player.height = count->player.height;
				temp->player.weight = count->player.weight;
				strcpy(temp->player.name, count->player.name);
				strcpy(temp->player.surname, count->player.surname);
				strcpy(temp->player.club, count->player.club);
				strcpy(temp->player.email, count->player.email);
				temp->player.position = count->player.position;

				// setting values back to current node
				count->player.irfu = irfunumber;
				count->player.age = age;
				count->player.tackles = tackles;
				count->player.metres = metres;
				count->player.height = height;
				count->player.weight = weight;
				strcpy(count->player.name, name);
				strcpy(count->player.surname, surname);
				strcpy(count->player.club, club);
				strcpy(count->player.email, email);
				count->player.position = position;
			}
		}
	}
}
// updating a players stats
void updateStatistics(struct node* headptr) {
	char tempemail[25];
	int found = 0, option = 0, irfunumber;
	char name[25], surname[25];

	printf("Enter\n1) To search for player by IRFU:\n2) To search for player by full name:\n");
	scanf("%d", &option);

	if (option == 1) {
		printf("Please enter the players IRFU: ");
		scanf("%d", &irfunumber);
		printf("\n");
	}
	else if (option == 2) {
		printf("Please enter the players full name: ");
		scanf("%s %s", name, surname);
		printf("\n");
	}
	else {
		printf("That is not a valid option");
	}

	struct node *newNode;
	newNode = (struct node*)malloc(sizeof(struct node));

	newNode = headptr;

	while (newNode != NULL) {
		if (newNode->player.irfu == irfunumber || strcmp(newNode->player.name, name) == 0 && strcmp(newNode->player.surname, surname) == 0) {
			printf("Please Enter the new Position of the Player:\n");
			printf("1) Prop\n2)Hooker\n3) Second Row\n4) Back Row\n5) Half Back\n6) Centres\n7) Wingers/Full Back\n");
			scanf("%d", &newNode->player.position);

			printf("How many tackles does the player miss per match?:\n");
			printf("1) Never\n2) Less than three times per match\n3) Less than five times per match\n4) More than five times per match\n");
			scanf("%d", &newNode->player.tackles);

			printf("How many metres does the player make in a game:\n");
			printf("1) None\n2) Less than 10 metres\n3) Less than 20 metres\n4) More than 20 metres\n");
			scanf("%d", &newNode->player.metres);
		}
		newNode = newNode->NEXT;
	}
}
// generating stats based on height or weight
void generateStatistics(struct node* headptr, int option) {
	int choice;
	int positionChoice;
	float bottomWeight;
	float topWeight;
	char position[20];
	int tackles[4] = { 0,0,0,0 };
	int metres[4] = { 0,0,0,0 };
	int count = 0;

	FILE* reportFile;
	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));
	temp = headptr;
	do {
		printf("Enter 1 to select Player position as user criteria\n");
		printf("Enter 2 to select Player Weight as user criteria\n");
		scanf("%d", &choice);
		if (choice != 1 && choice != 2)
			printf("Error - try again\n");
	} while (choice != 1 && choice != 2);

	switch (choice) {
		case 1: 
			do {
				printf("Enter 1 to select prop\n");
				printf("Enter 2 to select hooker\n");
				printf("Enter 3 to select second row\n");
				printf("Enter 4 to select back row\n");
				printf("Enter 5 to select half back\n");
				printf("Enter 6 to select centres\n");
				printf("Enter 7 to select wingers/full back\n");
				scanf("%d", &positionChoice);
				if (positionChoice < 1 | positionChoice> 7)
					printf("Error - try again\n");
			} while (positionChoice < 1 || positionChoice > 7);

			for (int i = 0; i < length(headptr); i++) {
				if (temp->player.position == positionChoice) {
					tackles[temp->player.tackles - 1]++;
					metres[temp->player.metres - 1]++;
					count++;
				}
				temp = temp->NEXT;
			}
			break;
		case 2:
			do {
				printf("Enter 1 to select players under 80kg\n");
				printf("Enter 2 to select players under 100kg\n");
				printf("Enter 3 to select players under 120kg\n");
				printf("Enter 4 to select players above 120kg\n");
				scanf("%d", &positionChoice);
				if (positionChoice < 1 | positionChoice> 4)
					printf("Error - try again\n");
			} while (positionChoice < 1 | positionChoice> 4);

			switch (positionChoice) {
				case 1:
					bottomWeight = 0;
					topWeight = 79.9999;
					break;
				case 2:
					bottomWeight = 80;
					topWeight = 99.9999;
					break;
				case 3:
					bottomWeight = 100;
					topWeight = 119.9999;
					break;
				case 4:
					bottomWeight = 120;
					topWeight = 99999;
					break;
			}
			for (int i = 0; i < length(headptr); i++) {
				if (temp->player.weight >= bottomWeight && temp->player.weight <= topWeight) {
					tackles[temp->player.tackles - 1]++;
					metres[temp->player.metres - 1]++;
					count++;
				}
				temp = temp->NEXT;
			}
			break;
	}
	if ((reportFile = fopen("ReportFile.txt", "a")) == NULL) {
		printf("\nFile not found\n");
	}
	else {
		if (count != 0) {
			if (option == 0) {
				printf("Percentage of players who miss no tackles per match is: %d%%\n", (tackles[0] * 100 / count));
				printf("Percentage of players who miss less than 3 tackles per match is: %d%%\n", (tackles[1] * 100 / count));
				printf("Percentage of players who miss less than 5 tackles per match is: %d%%%\n", (tackles[2] * 100 / count));
				printf("Percentage of players who miss more than 5 tackles per match is: %d%%\n", (tackles[3] * 100 / count));
				printf("Percentage of players who make no metres in a game is: %d%%\n", (metres[0] * 100 / count));
				printf("Percentage of players who make less than 10 metres in a game is: %d%%\n", (metres[1] * 100 / count));
				printf("Percentage of players who make less than 20 metres in a game is: %d%%\n", (metres[2] * 100 / count));
				printf("Percentage of players who make more than 20 metres in a game is: %d%%\n\n", (metres[3] * 100 / count));
			}
			else if (option == 1) {
				fprintf(reportFile, "Percentage of players who miss no tackles per match is: %d%%\n", (tackles[0] * 100 / count));
				fprintf(reportFile, "Percentage of players who miss less than 3 tackles per match is: %d%%\n", (tackles[1] * 100 / count));
				fprintf(reportFile, "Percentage of players who miss less than 5 tackles per match is: %d%%%\n", (tackles[2] * 100 / count));
				fprintf(reportFile, "Percentage of players who miss more than 5 tackles per match is: %d%%\n", (tackles[3] * 100 / count));
				fprintf(reportFile, "Percentage of players who make no metres in a game is: %d%%\n", (metres[0] * 100 / count));
				fprintf(reportFile, "Percentage of players who make less than 10 metres in a game is: %d%%\n", (metres[1] * 100 / count));
				fprintf(reportFile, "Percentage of players who make less than 20 metres in a game is: %d%%\n", (metres[2] * 100 / count));
				fprintf(reportFile, "Percentage of players who make more than 20 metres in a game is: %d%%\n\n", (metres[3] * 100 / count));
			}
		}
		else {
			if (option == 0)
				printf("No players matched the user criteria\n");
			else if (option == 1)
				fprintf(reportFile, "No players matched the user criteria\n");
		}
	}
	fclose(reportFile);
}
// outputing the players to a file to be loaded everytime program starts
void outputFile(struct node* headptr) {
	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));
	temp = headptr;
	FILE * outfile;

	outfile = fopen("Rugby.txt", "w+");
	fprintf(outfile, "%d\n", length(headptr));

	while (temp != NULL) {
		fprintf(outfile, "%d\n", temp->player.irfu);
		fprintf(outfile, "%s\n", temp->player.name);
		fprintf(outfile, "%s\n", temp->player.surname);
		fprintf(outfile, "%d\n", temp->player.age);
		fprintf(outfile, "%f\n", temp->player.height);
		fprintf(outfile, "%f\n", temp->player.weight);
		fprintf(outfile, "%s\n", temp->player.club);
		fprintf(outfile, "%s\n", temp->player.email);
		fprintf(outfile, "%d\n", temp->player.position);
		fprintf(outfile, "%d\n", temp->player.tackles);
		fprintf(outfile, "%d\n", temp->player.metres);

		temp = temp->NEXT;
	}
	fclose(outfile);
}
// checking the players in the file
int initialInputFile(struct node** head, FILE * infile) {
	struct node* temp;
	temp = (struct node*)malloc(sizeof(struct node));
	int players = 0;
	fscanf(infile, "%d\n", &players);
	if (players != 0) {
		fscanf(infile, "%d\n%s\n%s\n%d\n%f\n%f\n%s\n%s\n%d\n%d\n%d\n",
			&temp->player.irfu, temp->player.name, temp->player.surname,
			&temp->player.age, &temp->player.height, &temp->player.weight, temp->player.club,
			temp->player.email, &temp->player.position, &temp->player.tackles, &temp->player.metres);
		temp->NEXT = *head;
		*head = temp;
	}
	return players;
}
// writing players to the file
void inputFile(struct node* head, FILE* infile) {
	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));
	temp = head;
	struct node* newNode;
	newNode = (struct node*)malloc(sizeof(struct node));
	while (temp->NEXT != NULL) {
		temp = temp->NEXT;
	}
	fscanf(infile, "%d\n%s\n%s\n%d\n%f\n%f\n%s\n%s\n%d\n%d\n%d\n",
		&newNode->player.irfu, newNode->player.name, newNode->player.surname,
		&newNode->player.age, &newNode->player.height, &newNode->player.weight, newNode->player.club,
		newNode->player.email, &newNode->player.position, &newNode->player.tackles, &newNode->player.metres);
	newNode->NEXT = NULL;
	temp->NEXT = newNode;
}
// printing out stats / player values to file
void printToReportFile(struct node* headptr) {
	FILE* reportFile;
	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));

	char pos[7][20] = { "Prop ", "Hooker ", "Second Row ", "Back Row ", "Half Back ", "Centre ", "Wingers\Full Back " };
	char mets[4][10] = { "None ",  "< 10M", "< 20M", "> 20M" };
	char tacks[4][10] = { "Never ", "< 3 ",  "< 5 ",  "> 5 " };

	if ((reportFile = fopen("ReportFile.txt", "w+")) == NULL) {
		printf("\nFile not found\n");
	}
	else {
		temp = headptr;

		while (temp != NULL) {
			fprintf(reportFile, "IRFU: %d\n", temp->player.irfu);
			fprintf(reportFile, "Name: %s %s\n", temp->player.name, temp->player.surname);
			fprintf(reportFile, "Age: %d\n", temp->player.age);
			fprintf(reportFile, "Height: %f\n", temp->player.height);
			fprintf(reportFile, "Weight: %f\n", temp->player.weight);
			fprintf(reportFile, "Club: %s\n", temp->player.club);
			fprintf(reportFile, "Email: %s\n", temp->player.email);
			fprintf(reportFile, "Position: %s\n", pos[temp->player.position - 1]);
			fprintf(reportFile, "Tackles Missed: %s\n", tacks[temp->player.tackles -1]);
			fprintf(reportFile, "Metres: %s\n\n", mets[temp->player.metres -1]);

			temp = temp->NEXT;
		}
		fclose(reportFile);
		generateStatistics(headptr, 1);
	}
	printf("Report has been saved\n");
}
// sorting the second row and back row players by height and weight (Bubble sort)
void sortHeightWeight(struct node* head) {
	struct node *temp;
	temp = (struct node*)malloc(sizeof(struct node));
	temp = head;
	player_t secondRowPlayers[100];
	player_t backRowPlayers[100];
	int backrow = -1, secondrow = -1;

	while (temp != NULL)  {
		if (temp->player.position == 2) {
			secondrow++;
			secondRowPlayers[secondrow] = temp->player;
		}
		if (temp->player.position == 3) {
			backrow++;
			backRowPlayers[backrow] = temp->player;
		}
		temp = temp->NEXT; 
	}
	player_t swap;

	for (int i = 0; i < (secondrow); i++) {
		for (int j = 0; j < secondrow - i; j++) {
			if (secondRowPlayers[j].height > secondRowPlayers[j + 1].height) {
				swap = secondRowPlayers[j];
				secondRowPlayers[j] = secondRowPlayers[j + 1];
				secondRowPlayers[j + 1] = swap;
			}
		}
	}

	for (int i = 0; i < (backrow); i++) {
		for (int j = 0; j < backrow - i; j++) {
			if (backRowPlayers[j].height > backRowPlayers[j + 1].height) {
				swap = backRowPlayers[j];
				backRowPlayers[j] = backRowPlayers[j + 1];
				backRowPlayers[j + 1] = swap;
			}
		}
	}
	printf("Ascending Order By Height for Second Row:\n");
	if (secondrow == -1)
		printf("No Second Row players found in list\n");
	for (int i = 0; i <= secondrow; i++)
		printf("Player %s %s with height %f\n", secondRowPlayers[i].name, secondRowPlayers[i].surname, secondRowPlayers[i].height);
	printf("\nAscending Order By Height for Second Row:\n");
	if (backrow == -1)
		printf("No Back Row players found in list\n");
	for (int i = 0; i <= backrow; i++)
		printf("Player %s %s with height %f\n", backRowPlayers[i].name, backRowPlayers[i].surname, backRowPlayers[i].height);
}