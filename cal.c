#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

double moon_phase(int year,int month,int day, double hour);

//char phases[] = ".)bDOCd(";

void pcenter(char *str, int width){
	int len = strlen(str);
	int before = (width-len)/2;
	printf("%*s\n", before+len, str);
}

void fixtime(struct tm **ts){
	time_t t = mktime(*ts);
	*ts = localtime(&t);
}

int main(int argc, char **argv){
	int month, year;
	time_t t;
	time(&t);
	struct tm *date = localtime(&t);
	date->tm_mday = 1;
	if (argc >= 2)
		date->tm_mon = atoi(argv[1])-1;
	if (argc == 3)
		date->tm_year = atoi(argv[2])-1900;
	fixtime(&date);
	month = date->tm_mon;
	year = date->tm_year+1900;
	int firstday = date->tm_wday;
	
	char monthname[80];
	strftime(monthname, 80, "%B", date);

	if (++(date->tm_mon) == 12) {
		date->tm_mon = 0;
		date->tm_year++;
	}
	fixtime(&date);
	int dim = (date->tm_wday-firstday+7)%7 +28;
	pcenter(monthname, 6*7+1);
	printf("  Sun   Mon   Tue   Wed   Thu   Fri   Sat\n");
	char *row = "+-----+-----+-----+-----+-----+-----+-----+\n";
	char *empty = "!     !     !     !     !     !     !     !\n";
	printf(row);
	int w = 0;
	int half=0;
	char moon[31] = {0};
	double dif_new, dif_first, dif_full, dif_last;
	double odif_new, odif_first, odif_full, odif_last;
	int i;
	double ph;
	for (i=0;i<=dim+1;i++) {
		double oph = ph;
		ph = moon_phase(year, month+1, i, 16);
		//printf("%f\n", ph);
		if (ph < oph && i) {
			if (8-oph < ph) //was prev
				moon[i-1] = '.';
			else
				moon[i] = '.';
		}
		if (oph < 4 && ph >= 4 && i) {
			if (4-oph < ph-4) //was prev
				moon[i-1] = 'O';
			else
				moon[i] = 'O';
		}
		if (oph < 2 && ph >= 2 && i) {
			if (2-oph < ph-2) //was prev
				moon[i-1] = ')';
			else
				moon[i] = ')';
		}
		if (oph < 6 && ph >= 6 && i) {
			if (6-oph < ph-6) //was prev
				moon[i-1] = '(';
			else
				moon[i] = '(';
		}
	}
	
	i = -firstday+1;
	while(1){
		if (i<=0 || i>dim)
			printf("!     ");
		else{
			if (!half){
				printf("!%-5d", i);
			}else{
				if (moon[i])
					printf("!    %c", moon[i]);
				else
					printf("!     ");
			}
		}
		i++;
		w++;
		if(w==7){
			w=0;
			printf("!\n");
			if (!half){
				half=1;
				i-=7;
			}else{
				//printf(empty);
				printf(row);
				if(i>dim)
					break;
				half=0;
			}
		}
	}
}
