
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <stdlib.h> 	// srand
#include<time.h>
using namespace std;

void *team1();
void *team2();
void attack(int ally_damage, int *enemy_health);
void heal(int *ally_health);
pthread_t canc_t1, canc_t2; // get to know thread id
pthread_mutex_t mutex_hp1, mutex_hp2;
int hp_1 = 100; //for global access in threads
int dmg_1 = 10;
int hp_2 = 100;
int dmg_2 = 10;


int main() {
	srand(time(0)); // initiate random seed
	pthread_t thread1, thread2; // thread names
	int  ithr1, ithr2;

	ithr1 = pthread_create( &thread1, NULL, (void* (*)(void *))team1, NULL); //initiate threads
	ithr2 = pthread_create( &thread2, NULL, (void* (*)(void *))team2, NULL);

	pthread_join( thread1, NULL); //wait for process to finish
	pthread_join( thread2, NULL);
	if ( hp_1 <= 0 && hp_2 <= 0)
	{
		cout << "It's a tie! \n";
	} else if (hp_1 <= hp_2)
	{
		cout << "Team 2 won! \n";
	}
	else if (hp_2 <= hp_1)
	{
		cout << "Team 1 won! \n";
	}
	return 0;
}

void *team1() {
	canc_t1 = pthread_self(); // is needed to cancel the thread
	do {
		if (hp_1 <= 0) {pthread_cancel(canc_t2); break;}
		pthread_mutex_lock(&mutex_hp2); // lock access to hp_2 during execution for other thread
		attack(dmg_1, &hp_2);
		cout << "Team 1 attacked, Team 2 hp is " << hp_2 << endl;
		pthread_mutex_unlock(&mutex_hp2); // unlock access
		sleep(1); //wait
		if (hp_2 <= 0) {pthread_cancel(canc_t2); break;}
		pthread_mutex_lock(&mutex_hp1); // lock access to hp_1 during execution for other thread
		heal(&hp_1);
		cout << "Team 1 healed, Team 1 hp is " << hp_1 << endl;
		pthread_mutex_unlock(&mutex_hp1); // unlock access
	} while (1);
	pthread_exit(0);
}

void *team2() {
	canc_t2 = pthread_self();
	do {
		if (hp_2 <= 0) {pthread_cancel(canc_t1); break;}
		pthread_mutex_lock(&mutex_hp1);
		attack(dmg_2, &hp_1);
		cout << "Team 2 attacked, Team 1 hp is " << hp_1 << endl;
		pthread_mutex_unlock(&mutex_hp1);
		sleep(1); //wait
		if (hp_1 <= 0) {pthread_cancel(canc_t1); break;}
		pthread_mutex_lock(&mutex_hp2);
		heal(&hp_2);
		cout << "Team 2 healed, Team 2 hp is " << hp_2 << endl;
		pthread_mutex_unlock(&mutex_hp2);
	} while (1);
	pthread_exit(0);
}

void attack(int ally_damage, int *enemy_health) { 	// attacks the enemy team for random amount of damage
	int cur_damage = ally_damage + rand() % 10;
	*enemy_health = *enemy_health - cur_damage;
};

void heal(int *ally_health) {						// heals team for random amount of health, less than damage
	*ally_health = *ally_health + rand() % 10;
};

