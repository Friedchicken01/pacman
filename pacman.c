#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define H 31
#define W 28

int kbhit();
void delay(int milliseconds);
void Render(int m[H][W], char c[5], int pacman[2], int ghost[4][4], int point);
void Move(int m[H][W], int ghost[4][4], int x);
void Render2(int m[H][W], char c[5], int x);

int main(){
	FILE *fr;
	int i, j, point = 0, z = 0, p = 0, f = 0;
	char c[] = {' ', '#', '.', 'o', '-'};
	int m[H][W], pacman[] = {23, 14}, ghost[4][4] = {{11, 13, 0, 0}, {13, 16, 0, 0}, {13, 14, 0, 0}, {13, 12, 0, 0}};
	
	fr = fopen("pac.txt", "r");
	
	for(i = 0; i<H; i++){
		for(j = 0; j<W; j++){
			fscanf(fr, "%d", &m[i][j]);
		}
	}
	
	Render2(m, c, 0);
	getchar();
	
	z = 0;
	Render(m, c, pacman, ghost, point);
	delay(100);
	while("The Earth is flat"){
		if(kbhit()){
			switch(getchar()){
				case 'w':
				if(m[pacman[0] - 1][pacman[1]] != 1 && m[pacman[0] - 1][pacman[1]] != 4){
					pacman[0]--;
				}
				break;
				case 's':
				if(m[pacman[0] + 1][pacman[1]] != 1 && m[pacman[0] + 1][pacman[1]] != 4){
					pacman[0]++;
				}
				break;
				case 'a':
				if(m[pacman[0]][pacman[1] - 1] != 1 && m[pacman[0]][pacman[1] - 1] != 4){
					if(pacman[1] == 1){
						pacman[1] = 26;
					}else{
						pacman[1]--;
					}
				}
				break;
				case 'd':
				if(m[pacman[0]][pacman[1] + 1] != 1 && m[pacman[0]][pacman[1] + 1] != 4){
					if(pacman[1] == 26){
						pacman[1] = 1;
					}else{
						pacman[1]++;
					}
				}
				break;
			}
		}
		if(m[pacman[0]][pacman[1]] == 2){
			m[pacman[0]][pacman[1]] = 0;
			p += 1;
			point += 10;
		}
		if(m[pacman[0]][pacman[1]] == 3){
			m[pacman[0]][pacman[1]] = 0;
			f = 100;
		}
		if(z % 3 == 0){
			Move(m, ghost, 0);
			for(i = 1; i<4; i++){
				if(z > 60 * i){
					Move(m, ghost, i);
				}
				if(z == 60 * i){
					ghost[i][0] = 11;
				}
			}
		}
		Render(m, c, pacman, ghost, point);
		for(i = 0; i<4; i++){
			if(f == 0){
				if(ghost[i][0] == pacman[0] && ghost[i][1] == pacman[1]){
					Render2(m, c, 1);
					return 1;
				}
			}else{
				if(ghost[i][0] == pacman[0] && ghost[i][1] == pacman[1]){
					point += 100;
					ghost[i][0] = 11;
					ghost[i][1] = 14;
				}
			}
		}
		if(p >= 240){
			Render2(m, c, 2);
			return 1;
		}
		z++;
		if(f > 0){
			f--;
			for(i = 0; i<4; i++){
				if(ghost[i][3] == 0){
					ghost[i][3] = 1;
				}else{
					ghost[i][3] = 0;
				}
			}			
		}
		printf("%d", f);
		delay(50);
	}
	
	return 0;
}

void Render(int m[H][W], char c[5], int pacman[2], int ghost[4][4], int point){
	int i, j, k;
	system("clear");
	printf("POINTS: %.4d\n", point);
	for(i = 0; i<H; i++){
		for(j = 0; j<W; j++){
			if(m[i][j] == 1 || m[i][j] == 4){
				printf("%c%c", c[m[i][j]], c[m[i][j]]);
			}else if(i == ghost[0][0] && j == ghost[0][1]){
				if(ghost[0][3] == 0){
					printf("{}");
				}else{
					printf("}{");
				}
			}else if(i == ghost[1][0] && j == ghost[1][1]){
				if(ghost[1][3] == 0){
					printf("{}");
				}else{
					printf("}{");
				}
			}else if(i == ghost[2][0] && j == ghost[2][1]){
				if(ghost[2][3] == 0){
					printf("{}");
				}else{
					printf("}{");
				}
			}else if(i == ghost[3][0] && j == ghost[3][1]){
				if(ghost[3][3] == 0){
					printf("{}");
				}else{
					printf("}{");
				}
			}else if(i == pacman[0] && j == pacman[1]){
				printf("()");
			}else{
				printf("%c ", c[m[i][j]]);
			}
		}
		printf("\n");
	}
}

void Move(int m[H][W], int ghost[4][4], int x){
	srand(clock()+rand()+time(NULL));
	int i = 1, c;
	while(i){
		c = 1 + rand()%4;
		if(c == 1 && m[ghost[x][0] - 1][ghost[x][1]] != 1 && m[ghost[x][0] - 1][ghost[x][1]] != 4 && ghost[x][2] != 2){
			ghost[x][0]--;
			ghost[x][2] = 1;
			i = 0;
		}
		if(c == 2 && m[ghost[x][0] + 1][ghost[x][1]] != 1 && m[ghost[x][0] + 1][ghost[x][1]] != 4 && ghost[x][2] != 1){
			ghost[x][0]++;
			ghost[x][2] = 2;
			i = 0;
		}
		if(c == 3 && m[ghost[x][0]][ghost[x][1] - 1] != 1 && m[ghost[x][0]][ghost[x][1] - 1] != 4 && ghost[x][2] != 4){
			if(ghost[x][1] == 1){
				ghost[x][1] = 26;
			}else{
				ghost[x][1]--;
			}
			ghost[x][2] = 3;
			i = 0;
		}
		if(c == 4 && m[ghost[x][0]][ghost[x][1] + 1] != 1 && m[ghost[x][0]][ghost[x][1] + 1] != 4 && ghost[x][2] != 3){
			if(ghost[x][1] == 26){
				ghost[x][1] = 0;
			}else{
				ghost[x][1]++;
			}	
			ghost[x][2] = 4;
			i = 0;
		}
	}
}

int kbhit(){
	struct termios oldt, newt;
  	int ch;
  	int oldf;
 
  	tcgetattr(STDIN_FILENO, &oldt);
  	newt = oldt;
  	newt.c_lflag &= ~(ICANON | ECHO);
  	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  	ch = getchar();
 
  	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  	if(ch != EOF){
		ungetc(ch, stdin);
    	return 1;
	}
	return 0;
}

void delay(int milliseconds){
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

void Render2(int m[H][W], char c[5], int x){
	int i, j, z;
	system("clear");
	printf("\n");
	for(i = 0; i<H; i++){
		z = 0;
		for(j = 0; j<W; j++){
			if(j == 17){
				z = 0;
			}
			if(i == 14 && j == 11 && z == 0){
				if(x == 0){
					printf(" START GAME ");
				}else if(x == 2){
					printf("  VICTORY   ");
				}else{
					printf(" GAME OVER  ");
				}
				z = 1;
			}else if((m[i][j] == 1 || m[i][j] == 4) && z == 0){
				printf("%c%c", c[m[i][j]], c[m[i][j]]);
			}else if(z == 0){
				printf("%c ", c[m[i][j]]);
			}
		}
		printf("\n");
	}
}