

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<time.h>

void menu_select();			// menu select
void menu();				// print menu
void gotoxy();				// menu position
void menu_star();			// print *
void read_file();			// read proc.txt
void number_read_process();		// proc.txt process count
void print_result();			// print result
void FCFS();				// FCFS
void SJF();				// SJF
void SRTF();				// SRTF
void Priority();			// Priority
void RR();				// RR
int* cpu_sort();			// SJF Burst time sort
int* priority_sort();			// Priority sort
int* srt_cpu_sort();			// SRTF burst time sort
void reset_process();			// reset process value
void input_exit();			// please input 'esc' to exit
void InitQueue();			// reset queue
int IsEmpty();				// queue empty?
void Enqueue(); 			// enqueue
int Dequeue(); 				// dequeue
int howqueue();				// how many?

int process_count = 0;
int process_number = 0;
int process_num = 0;
int menu_select_position =3;
int i, j, temp, number;

typedef struct inputval {
	int proc_num;
	int cpu_time;
	int arrival_time;
	int priority;
	int cal_cpu_time;
	int complete;
	int complete_time;
} INPUTVALUE;

INPUTVALUE *valuearr;
INPUTVALUE* create_random_process();	//generate random process
INPUTVALUE* random_process();		// generate random process
	
typedef struct Node {
	int data;
	struct Node *next;
} Node;

typedef struct Queue {
	Node *front;
	Node *rear;
	int count;
} Queue;

void InitQueue(Queue *queue) {
	queue->front = queue->rear = NULL;
	queue->count = 0;
}

int IsEmpty(Queue *queue) {
	return queue->count == 0;  
}

int howqueue(Queue *queue) {
	return queue->count;
}

void Enqueue(Queue *queue, int data) {
	Node *now = (Node *)malloc(sizeof(Node));
	now->data = data;
	now->next = NULL;
	if (IsEmpty(queue)) {		
		queue->front = now;  
	}
	else {				
		queue->rear->next = now;
	}
	queue->rear = now; 
	queue->count++;
}

int Dequeue(Queue *queue) {
	int re = 0;
	Node *now;
	if (IsEmpty(queue))
	{
		return re;
	}
	now = queue->front;
	re = now->data;
	queue->front = now->next;
	free(now);
	queue->count--;
	return re;
}

int  getch(){
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] =1;
	buf.c_cc[VTIME]=0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

void input_exit(INPUTVALUE *valuearr){
	int key=0;
	printf("Please input 'space' to exit\n");
	while(1){
		key = getch();
		if(key == 32){
			system("clear");
			menu_star(menu_select_position);
			menu();
			break;
		}
	}
	reset_process(valuearr);
}

void reset_process(INPUTVALUE *valuearr){
	for(i=0;i<process_num;i++){
		valuearr[i].cal_cpu_time =0;
		valuearr[i].complete =0;
		valuearr[i].complete_time=0;
	}
}

void menu_select(INPUTVALUE *valuearr) {
	int key=0;
	int read_done = 0;
	int random_done = 0;
	system("clear");
	menu();
	menu_star(menu_select_position);
	while (1) {
		key = getch();
		if (key == 27) {
			key = getch();
			if( key == 91){
				key = getch();
				switch (key){
				case 65:
					if(menu_select_position > 3)
						menu_select_position--;
					system("clear");
					menu_star(menu_select_position);
					menu();
					break;
				case 66:
					if(menu_select_position < 10)
						menu_select_position++;
					system("clear");
					menu_star(menu_select_position);
					menu();
					break;
				}
			}
		}
		else if (key == 10) {
			switch (menu_select_position) {
			case 3:
				if (read_done==0) {
					system("clear");
					number_read_process();
					process_num = process_number / 4;
					valuearr = (INPUTVALUE *)malloc(sizeof(INPUTVALUE)*process_num);
					read_file(valuearr);
					read_done = 1;
					input_exit(valuearr);
				}
				break;
			case 4:
				if(random_done == 0){
					if(read_done == 1) free(valuearr);
					system("clear");
					valuearr = random_process(valuearr);
					process_num = number;
					random_done=1; 
					input_exit(valuearr);
				}
				break;
			case 5:
				system("clear");
				FCFS(valuearr);
				input_exit(valuearr);
				break;
			case 6:
				system("clear");
				SJF(valuearr);
				input_exit(valuearr);
				break;
			case 7:
				system("clear");
				SRTF(valuearr);
				input_exit(valuearr);
				break;
			case 8:
				system("clear");
				Priority(valuearr);
				input_exit(valuearr);
				break;
			case 9:
				system("clear");
				RR(valuearr);
				input_exit(valuearr);
				break;
			case 10:
				free(valuearr);
				exit(1);
			}
		}
	}
}

void gotoxy(int x, int y) {
	printf("\033[%d;%df", y, x);
	fflush(stdout);
	}

void number_read_process() {
	FILE *fp=0;	
	int num=0;
	fp = fopen("proc.txt", "r");
	if (fp == NULL) printf("fail");
	else {
		while (feof(fp) == 0) {
			fscanf(fp,"%d", &num);
			process_number++;
		}
		fclose(fp);
	}
}
void read_file(INPUTVALUE *valuearr) {
	printf("-----------------------------FILE READ-----------------------------\n");
	FILE *fp=0;
	fp = fopen("proc.txt", "r");
	if (fp == NULL) printf("fail");
	else {
		while (feof(fp) == 0) {
			fscanf(fp,"%d %d %d %d", &valuearr[process_count].proc_num, &valuearr[process_count].cpu_time, &valuearr[process_count].arrival_time, &valuearr[process_count].priority);
			printf("Process[%d]:\tCpu_time: %d    \tArrival_time: %d   \tPriority: %d\n", valuearr[process_count].proc_num, valuearr[process_count].cpu_time, valuearr[process_count].arrival_time, valuearr[process_count].priority);
			process_count++;
			if(process_count == process_number/4) break;
		}
		fclose(fp);
		printf("-----------------------------SUCCESS---------------------------------\n");
	}
}
INPUTVALUE* create_random_process(int number){
	valuearr = (INPUTVALUE *)malloc(sizeof(INPUTVALUE)*number);
	return valuearr;
}
INPUTVALUE* random_process(INPUTVALUE *valuearr) {
	printf("---------------------------GENERATE RANDOM PROCESS---------------------------------\n");
	srand(time(NULL));
	while (1) {
		printf("Enter number of processes to generate(Please enter no more than 10000):  ");
		scanf("%d", &number);
		getch();
		if (number > 0 && number <= 10000) break;
	}
	valuearr = create_random_process(number);
	int dup_check;
	for (i = 0; i < number; i++) {
		while (1) {
			valuearr[i].arrival_time = rand() % 10000 + 1;
			dup_check = 0;
			for (j = 0; j < i; ++j) {
				if (valuearr[j].arrival_time == valuearr[i].arrival_time) {
					dup_check = 1;
					break;
				}
			}
			if (!dup_check) break;
		}
	}
	for (i = 0; i < number; i++) {
		valuearr[i].proc_num = i+1;
		valuearr[i].cpu_time = rand() % 100 + 1;
		valuearr[i].priority = rand() % 100 + 1;
	}
	for (i = 0; i < number; i++) {
		printf("Process[%d]:\tCpu_time=%d   \tArrival_time=%d   \tPriority=%d\n", valuearr[i].proc_num, valuearr[i].cpu_time, valuearr[i].arrival_time, valuearr[i].priority);
	}
	printf("--------------------------------------------------------------------------------------\n");
	return valuearr;
}

void menu() {
	gotoxy(20, 1);
	puts("Main Menu");
	gotoxy(18, 2);
	puts("-------------");
	gotoxy(3, 3);
	puts("Read processes from proc.txt");
	gotoxy(3, 4);
	puts("Generate random processes");
	gotoxy(3, 5);
	puts("First Come First Serve (FCFS)");
	gotoxy(3, 6);
	puts("Shortest Job First (SJF)");
	gotoxy(3, 7);
	puts("Shortest Remaing Time First (SRTF)");
	gotoxy(3, 8);
	puts("Priority");
	gotoxy(3, 9);
	puts("Round Robin (RR)");
	gotoxy(3, 10);
	puts("Exit");
	gotoxy(3, 12);
	puts("*** Note: User the arrow keys to choos the menu");
}

void menu_star(int y) {
    gotoxy(0, y);
	puts("*");
}

void print_result(INPUTVALUE *valuearr){
	float avg_waiting_time=0;
	float avg_turnarround_time=0;
	for (i = 0; i < process_num; i++) {
		printf("Process[%d]:\tWaiting time: %d   \tTurnarround time: %d     \tComplete time: %d\n",
			valuearr[i].proc_num,
			valuearr[i].complete_time - valuearr[i].arrival_time - valuearr[i].cpu_time,
			valuearr[i].complete_time - valuearr[i].arrival_time,
			valuearr[i].complete_time
		);
		avg_turnarround_time += valuearr[i].complete_time - valuearr[i].arrival_time;
		avg_waiting_time += valuearr[i].complete_time - valuearr[i].arrival_time - valuearr[i].cpu_time;

	}
	printf("AVG Waiting time: %5.3f\n", avg_waiting_time/process_num);
	printf("AVG turnarround time: %5.3f\n", avg_turnarround_time/process_num);
	printf("--------------------------------------------------------------------------\n");
}

int main() {
	menu_select(valuearr);		
	return 0;
}

int* cpu_sort(int* arr, int number, INPUTVALUE *valuearr) {
	for (i = 0; i < number - 1; i++) {
		for (j = 0; j < number - 1; j++) {
			if (valuearr[arr[j]].cpu_time > valuearr[arr[j + 1]].cpu_time) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
	return arr;
}

int* srt_cpu_sort(int* arr, int number, INPUTVALUE *valuearr) {
	for (i = 0; i < number - 1; i++) {
		for (j = 0; j < number - 1; j++) {
			if (valuearr[arr[j]].cpu_time - valuearr[arr[j]].cal_cpu_time > valuearr[arr[j + 1]].cpu_time - valuearr[arr[j + 1]].cal_cpu_time) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
	return arr;
}

int* priority_sort(int* arr, int number, INPUTVALUE *valuearr) {
	for (i = 0; i < number - 1; i++) {
		for (j = 0; j < number - 1; j++) {
			if (valuearr[arr[j]].priority > valuearr[arr[j + 1]].priority) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
	return arr;
}
void FCFS(INPUTVALUE *valuearr) {
	Queue queue;
	printf("------------------------------------FCFS----------------------------------\n");
	InitQueue(&queue);
	int num = 0;
	int time = 0;
	int burst = 0;
	int process=-1;
	int inprocess = 0;
	while (1) {
		if (num == process_num) break;
		for (; num < process_num; time++) {
			printf("Time: %d, running....\n", time);
			if (inprocess < process_num) {
				for (i = 0; i < process_num; i++) {
					if (time == valuearr[i].arrival_time && valuearr[i].complete == 0) {
						Enqueue(&queue, valuearr[i].proc_num);
						inprocess++;
						break;
					}
				}
			}
			if (!IsEmpty(&queue)) {
				if (burst == 0) {
					process = Dequeue(&queue) - 1;
				}
			}
			if (process != -1) {
				burst++;
				if (valuearr[process].cpu_time == burst) {
					valuearr[process].complete_time = time+1;
					valuearr[process].complete = 1;
					num++;
					burst = 0;
					process = -1;
				}
			}
			
		}
	}
	printf("Time: %d, complete\n", time);
	print_result(valuearr);
}
void SJF(INPUTVALUE *valuearr) {
	Queue queue;
	InitQueue(&queue);
	printf("------------------------------------SJF----------------------------------\n");
	int num = 0;
	int time = 0;
	int burst =0;
	int process = -1;
	int sort_process = 0;
	int n_process = 0;
	int inprocess = 0;
	while (1) {
		if (num == process_num) break;
		for (; num < process_num; time++) {
		printf("Time: %d, running....\n", time);
			if (inprocess < process_num) {
				for ( i = 0; i < process_num; i++) {
					if (time == valuearr[i].arrival_time && valuearr[i].complete == 0) {
						Enqueue(&queue, valuearr[i].proc_num);
						inprocess++;
						break;
					}
				}
			}
			if (howqueue(&queue) > 1) {
				int *sort_arr;
				n_process = howqueue(&queue);
				sort_arr = (int*)malloc(sizeof(int)*howqueue(&queue));
				for (i = 0; i < n_process; i++) {
					sort_process = Dequeue(&queue) - 1;
					sort_arr[i] = valuearr[sort_process].proc_num-1;
				}
				cpu_sort(sort_arr, n_process, valuearr);
				for (i = 0; i < n_process; i++) {
					Enqueue(&queue, valuearr[sort_arr[i]].proc_num);
				}
				free(sort_arr);
			}

			if (!IsEmpty(&queue)) {
				if (burst == 0) {
					process = Dequeue(&queue) - 1;
				}
			}
			if (process != -1) {
				burst++;
				if (valuearr[process].cpu_time == burst) {
					valuearr[process].complete_time = time + 1;
					valuearr[process].complete = 1;
					num++;
					burst = 0;
					process = -1;
				}
			}

		}
	}
	printf("Time: %d, complete\n", time);
	print_result(valuearr);
}
void SRTF(INPUTVALUE *valuearr) {
	Queue queue;
	InitQueue(&queue);
	printf("------------------------------------SRTF----------------------------------\n");
	int num = 0;
	int time = 0;
	int burst = 0;
	int process = -1;
	int inprocess = 0;
	int sort_process = 0;
	int n_process = 0;
	float avg_turnarround_time = 0;
	while (1) {
		if (num == process_num) break;
		for (; num < process_num; time++) {
		printf("Time: %d, running....\n", time);
			if (inprocess < process_num) {
				for (i = 0; i < process_num; i++) {
					if (time == valuearr[i].arrival_time && valuearr[i].complete == 0) {
						Enqueue(&queue, valuearr[i].proc_num);
						inprocess++;
						break;
					}
				}
			}
			if (howqueue(&queue) > 1) {
				int *sort_arr;
				n_process = howqueue(&queue);
				sort_arr = (int*)malloc(sizeof(int)*howqueue(&queue));
				for (i = 0; i < n_process; i++) {
					sort_process = Dequeue(&queue) - 1;
					sort_arr[i] = valuearr[sort_process].proc_num - 1;
				}
				srt_cpu_sort(sort_arr, n_process, valuearr);
				for (i = 0; i < n_process; i++) {
					Enqueue(&queue, valuearr[sort_arr[i]].proc_num);
				}
				free(sort_arr);
			}
			if (!IsEmpty(&queue)) {
					process = Dequeue(&queue) - 1;
			}
			if (process != (-1)) {
				valuearr[process].cal_cpu_time++;
				if (valuearr[process].cpu_time == valuearr[process].cal_cpu_time) {
					valuearr[process].complete_time = time + 1;
					valuearr[process].complete = 1;
					num++;
					process = (-1);
					
				}
				else {
					Enqueue(&queue, valuearr[process].proc_num);
				}
			}

		}
	}
	printf("Time: %d, complete\n", time);
	print_result(valuearr);
}
void Priority(INPUTVALUE *valuearr) {
	Queue queue;
	InitQueue(&queue);
	printf("----------------------------------PRIORITY--------------------------------\n");
	int num = 0;
	int time = 0;
	int burst = 0;
	int process = -1;
	int sort_process = 0;
	int n_process = 0;
	int inprocess = 0;
	float avg_turnarround_time = 0;
	while (1) {
		if (num == process_num) break;
		for (; num < process_num; time++) {
		printf("Time: %d, running....\n", time);
			if (inprocess < process_num) {
				for ( i = 0; i < process_num; i++) {
					if (time == valuearr[i].arrival_time && valuearr[i].complete == 0) {
						Enqueue(&queue, valuearr[i].proc_num);
						inprocess++;
						break;
					}
				}
			}
			if (howqueue(&queue) > 1) {
				int *sort_arr;
				n_process = howqueue(&queue);
				sort_arr = (int*)malloc(sizeof(int)*howqueue(&queue));
				for (i = 0; i < n_process; i++) {
					sort_process = Dequeue(&queue) - 1;
					sort_arr[i] = valuearr[sort_process].proc_num-1;
				}
				priority_sort(sort_arr, n_process, valuearr);
				for (i = 0; i < n_process; i++) {
					Enqueue(&queue, valuearr[sort_arr[i]].proc_num);
				}
				free(sort_arr);
			}
			if (!IsEmpty(&queue)) {
				if (burst == 0) {
					process = Dequeue(&queue) - 1;
				}
			}
			if (process != -1) {
				burst++;
				if (valuearr[process].cpu_time == burst) {
					valuearr[process].complete_time = time + 1;
					valuearr[process].complete = 1;
					num++;
					burst = 0;
					process = -1;
				}
			}

		}
	}
	printf("Time: %d, complete\n", time);
	print_result(valuearr);
}

void RR(INPUTVALUE *valuearr) {
	Queue queue;
	InitQueue(&queue);
	printf("------------------------------------RR----------------------------------\n");
	int Q=-1;
	int quan=0;
	while (1) {
		printf("please input Q: \n");
		scanf("%d", &Q);
		getch();
		if (Q > 0) break;
	}
	int num = 0;
	int time = 0;
	int burst = 0;
	int process = -1;
	int inprocess = 0;
	int dq = 0;
	float avg_turnarround_time = 0;
	while (1) {
		if (num == process_num) break;
		for (; num < process_num; time++) {
		printf("Time: %d, running....\n", time);
			if (inprocess < process_num) {
				for ( i = 0; i < process_num; i++) {
					if (time == valuearr[i].arrival_time && valuearr[i].complete == 0) {
						Enqueue(&queue, valuearr[i].proc_num);
						inprocess++;
						break;
					}
				}
			}
			if (dq == 1) {
				Enqueue(&queue, valuearr[process].proc_num);
				dq = 0;
			}
			if (!IsEmpty(&queue) && quan==0) {
				process = Dequeue(&queue) - 1;
			}
			if (process != (-1)) {
				quan++;
				valuearr[process].cal_cpu_time++;
				if (valuearr[process].cpu_time == valuearr[process].cal_cpu_time) {
					valuearr[process].complete_time = time + 1;
					valuearr[process].complete = 1;
					num++;
					process = (-1);
					quan = 0;
				}
				else if(quan==Q){
					dq = 1;
				}
			}
			if (quan == Q) quan = 0;
		}
	}
	printf("Time: %d, complete\n", time);
	print_result(valuearr);
}
