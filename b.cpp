#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INIT_POPULATION 100000
#define SIM_YEARS 200
#define AI_COMING_YEAR 30
#define AI_LOSEJOBRATE 0.5

typedef struct {
	int age;
	int is_working;
	int unemployed_year;
	bool havebaby;
} Person;

int losejobperYear() {
	return INIT_POPULATION *(1-AI_LOSEJOBRATE)/ AI_COMING_YEAR;
}

int main() {
	Person* population = (Person*)malloc(INIT_POPULATION * sizeof(Person));
	int pop_count = INIT_POPULATION;
	static int new_babylastyear = 0;
	// Initialize population
	srand(time(NULL));
	for (int i = 0; i < INIT_POPULATION; i++) {
		population[i].age = 20 + rand() % 41;
		population[i].is_working = 1;
		population[i].unemployed_year = -1;
		population[i].havebaby = false;
	}

	for (int year = 0; year < SIM_YEARS; year++) {
		int new_babies = 0;
		int dead_num = 0;
		int dead_num_forborn = 0;
		int losejob_num = 0;

		// Process each person
		for (int i = 0; i < pop_count; i++) {
			population[i].age++;

			// Update employment status
			if (population[i].is_working) {
				if (population[i].age >= 70) {
					population[i].is_working = 0;
					population[i].unemployed_year = year;
				}
				if (year <= AI_COMING_YEAR && losejob_num <losejobperYear()){
					population[i].is_working = 0;
					population[i].unemployed_year = year;
					losejob_num++;
				}
			}
			else {
				if ((year - population[i].unemployed_year >= 30 && population[i].unemployed_year != -1 && population[i].age< 70)
					||(population[i].age >= 40) && (rand() / (double)RAND_MAX < 0.5) ? 1 : 0 && population[i].unemployed_year == -1
					|| population[i].age >= 95){
					// Remove deceased
					if (population[i].unemployed_year == -1)
						dead_num_forborn++;
					population[i] = population[pop_count - 1];
					pop_count--;
					i--;
					dead_num++;
					continue;
				}
			}

			// Reproduction
			if (population[i].is_working && (population[i].age >30 && population[i].age <45) && population[i].havebaby == false ) {
				new_babies++;
				population[i].havebaby = true;
			}
		}

		// Add new babies
		new_babies = (float)new_babies/1.8181;
		population = (Person*)realloc(population, (pop_count + new_babies) * sizeof(Person));
		for (int i = 0; i < new_babies; i++) {
			population[pop_count + i].age = 0;
			population[pop_count + i].is_working = (rand() /(double)RAND_MAX < 0.9) ? 1 : 0;
			//printf(" %d is_working\n", population[pop_count + i].is_working);
			population[pop_count + i].unemployed_year = -1;
			population[pop_count + i].havebaby = false;
		}
		pop_count += new_babies;
		new_babylastyear = new_babies;
		//printf("After %d years: %d people %d-%d dead - borndead %d new  %d losejob_num\n", year + 1, pop_count, dead_num, dead_num_forborn, new_babies, losejob_num);

		printf(" %d \n", pop_count);


		// Print results
		if (year == 49 || year == 99 || year == 199) {
			//printf("After %d years: %d people\n", year + 1, pop_count);
		}
	}

	free(population);
	return 0;
}
