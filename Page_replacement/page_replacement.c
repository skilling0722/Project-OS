

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

void menu();						//print menu
void menu_select();					//select menu
void gotoxy();						//menu position
int getch();						//for input
void input_exit();					//input to exit
void Insert();						//1. Insert
void Show();						//2. Show
char** create_page();				//create page

int size=3;							//memory size
int add=0;							//memory position check

typedef struct memory {
	char *Contain;
	int Pageno;
	int Pagecount;
} MEMORY;

MEMORY* Set_memory_size();			//Setting memory size
MEMORY *memory;

int getch(){
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

void input_exit(MEMORY *memory, char **page){
	int input=0;
	printf("Please input 'space' to exit\n");
	while(1){
		input = getch();
		if(input == 32){
			system("clear");
			menu_select(memory, page);
		}
	}
}

void gotoxy(int x, int y){
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}

void menu(){
	gotoxy(0, 3);
	puts("1. Insert");
	gotoxy(0, 4);
	puts("2. Show");
	gotoxy(0, 5);
	puts("3. Setting of memory size");
	gotoxy(0, 6);
	puts("4. Exit");
}

void menu_select(MEMORY *memory,char **page) {
	int key=0;
	system("clear");
	menu();
	while(1) {
		gotoxy(0,7);
		printf("Enter Your Choice: ");
		scanf("%d", &key);
		switch(key){
			case 1: 
				Insert(memory, page);
				input_exit(memory, page);
				break;
			case 2:
				Show(memory);
				input_exit(memory, page);
				break;
			case 3:
				memory = Set_memory_size(memory);
				input_exit(memory, page);
				break;
			case 4:
				free(memory);
				free(page);
				exit(1);
		}
	}
}

void Insert(MEMORY *memory, char** page){
	int input;
	int i, j;
	char *Contain_temp;
	int Pageno_temp;
	int Pagecount_temp;
	while(1){
		printf("\nEnter page no between 1 to 10: ");
		scanf("%d", &input);
		if(input <= 10 && input > 0) break;
		while(getchar() != '\n');
	}
	printf("\n%s\n", page[input-1]);
	for(i=0; i<size; i++){
		if(page[input-1] == memory[i].Contain){
			Contain_temp = memory[i].Contain;
			Pageno_temp = memory[i].Pageno;
			Pagecount_temp = memory[i].Pagecount+1;
			for(j=i;j<size-1;j++ ){
				memory[j].Contain = memory[j+1].Contain;
				memory[j].Pageno = memory[j+1].Pageno;
				memory[j].Pagecount = memory[j+1].Pagecount;
			}
			memory[add-1].Contain = Contain_temp;
			memory[add-1].Pageno = Pageno_temp;
			memory[add-1].Pagecount = Pagecount_temp;
			break;
		} else if(i == size - 1) {
			if(add < size) {
				memory[add].Contain = page[input-1];
				memory[add].Pageno = input;
				memory[add].Pagecount = 1;
				add++;
			} else if(add == size){
				for(j=0; j<size-1; j++){
					memory[j].Contain = memory[j+1].Contain;
					memory[j].Pageno = memory[j+1].Pageno;
					memory[j].Pagecount = memory[j+1].Pagecount;
				}
				memory[size-1].Contain = page[input-1];
				memory[size-1].Pageno = input;
				memory[size-1].Pagecount=1;
			}
		}
	}
}

void Show(MEMORY *memory) {
	int i=0;
	printf("\nContain\tPageno\tPagecount\n");
	for(i=0; i<size;i++){
		printf("%s \t %d \t %d\n", memory[i].Contain, memory[i].Pageno, memory[i].Pagecount);
	}
}

MEMORY* Set_memory_size(MEMORY *memory){
	int check;
	size = 0;
	while(1){
		printf("Enter size to change: ");
		scanf("%d", &size);
		if(size > 0) {
			printf("size: %d\n", size);
			break;
		}
		while(getchar() != '\n');
	}
	free(memory);
	memory = (MEMORY *)calloc(size, sizeof(MEMORY));
	add = 0;
	return memory;
}

char**  create_page(){
	int i=0;
	char **page = malloc(sizeof(char *)*10);
	for(i=0;i<10;i++){
		page[i] = malloc(sizeof(char)*size);
	}
	page[0] = "aaa";
	page[1] = "bbb";
	page[2] = "ccc";
	page[3] = "ddd";
	page[4] = "eee";
	page[5] = "fff";
	page[6] = "ggg";
	page[7] = "hhh";
	page[8] = "iii";
	page[9] = "jjj";
	return page;
}
int main(){
	char **page;
	page = create_page();
	memory = (MEMORY *)malloc(sizeof(MEMORY)*3);
	menu_select(memory, page);
	return 0;
}
