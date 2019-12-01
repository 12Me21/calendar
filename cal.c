#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

double moon_phase(int year,int month,int day, double hour);

void print_center(char *str, int width){
	int len = strlen(str);
	int before = (width-len)/2;
	printf("%*s\n", before+len, str);
}

// re-calculate date to fill in day of week etc.
void fixtime(struct tm **ts){
	time_t t = mktime(*ts);
	*ts = localtime(&t);
}

int main(int argc, char **argv){
	// get first day of current month
	time_t current_time;
	time(&current_time);
	struct tm *date = localtime(&current_time);
	date->tm_mday = 1;
	// adjust month/year if needed
	if (argc >= 2)
		date->tm_mon = atoi(argv[1])-1;
	if (argc >= 3)
		date->tm_year = atoi(argv[2])-1900;

	fixtime(&date);
	
	int month = date->tm_mon;
	int year = date->tm_year+1900;
	int firstday = date->tm_wday;

	// print header
	char title[80];
	strftime(title, sizeof(title), "%B %Y", date);
	const char row[] = "+-----+-----+-----+-----+-----+-----+-----+";
	print_center(title, sizeof(row));
	puts("  Sun   Mon   Tue   Wed   Thu   Fri   Sat");
	puts(row);

	// get next month
	if (++(date->tm_mon) == 12) {
		date->tm_mon = 0;
		date->tm_year++;
	}
	fixtime(&date);
	// Calculate number days in current month
	int days_in_month = (date->tm_wday-firstday+7)%7 +28;

	// Moon phases
	char moon[32] = {0};
	int day;
	double ph, oph;
	for (day=0; day<=days_in_month+1; day++) {
		oph = ph;
		ph = moon_phase(year, month+1, day, 12.00);
		//time depends on location + moon location. 12:00 average is ok I guess
		if (day) {
			// put icons on days closest to each phase
			if (ph < oph)
				moon[day - (8-oph < ph)] = '.'; //new
			else if (oph < 4 && ph >= 4)
				moon[day - (4-oph < ph-4)] = 'O'; //full
			else if (oph < 2 && ph >= 2)
				moon[day - (2-oph < ph-2)] = ')'; //first quarter
			else if (oph < 6 && ph >= 6)
				moon[day - (6-oph < ph-6)] = '('; //last quarter
		}
	}

	// print main calendar
	int day_of_week = 0;
	int half = 0;
	day = -firstday+1;
	while (1) {
		if (day<=0 || day>days_in_month) { //empty cell
			printf("!     ");
		} else {
			if (!half) { // top half
				printf("!%-5d", day);
			} else { // bottom half
				if (moon[day])
					printf("!    %c", moon[day]);
				else
					printf("!     ");
			}
		}
		day++;
		// end of row
		if (++day_of_week == 7) {
			day_of_week = 0;
			puts("!");
			if (!half) { // top half
				half = 1;
				day -= 7;
			} else { // bottom half
				puts(row);
				if (day>days_in_month)
					break;
				half = 0;
			}
		}
	}
	return 0;
}
