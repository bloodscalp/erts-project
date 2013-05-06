/***********************************************************************
 *  Program principal de la demo uCosII
 *
 *
 *	Filename      : app.c
 *  Version       : V1.1
 *  Programmer(s) : Xavier Meyer
 *
 ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <includes.h>
#include "stm32f10x_includes.h"
#include "model.c"
#include "thread_car_model.c"

/************************************************************************
 *
 * Definition du STACK et globale de l'USART1
 *
 ************************************************************************/

// Descripteur de fichier pour le USART1
FILE *fp_usart1;

// Stack de la tache
//static  OS_STK          AppTaskStk[APP_TASK_STK_SIZE];
// Stack de la tache

/* ----------------------------------------------- */

#if OS_STK_GROWTH == 0
#define init_pile(stack,size) (&stack[0])
#else
#define init_pile(stack,size) (&stack[size-1])
#endif

#define TAILLE_PILE_TACHE 	512
#define MAX_TASKS 			16

#define false 0
#define true 1

int PRIORITE_TACHE=4;
OS_STK pileTache[MAX_TASKS][TAILLE_PILE_TACHE];

void init_cible_ntrt(void);
void initialise_taches (void);


float speed;
/************************************************************************
 *
 * Definition de la tache
 *
 ************************************************************************/
static void thread_ihm (void *p_arg)
{

	(void)p_arg;


	char cmd = '0';
	char param[] = {'0','0', '0'};
	uint8_t flag = 0;
	uint8_t v = 0;
	uint8_t v0 = 0;
	uint8_t t = 0;
	int8_t a = 0;
	uint8_t throttle = 0;
	uint8_t breaks = 0;


	// attente de la commande de start pour demarrer
	while (cmd != '1') {
		// scrute les entrees
		if(USART1_CNT_IN > 0) {
			cmd = USART1_BUFFER_IN[0];
			USART1_CNT_IN = 0;
		}

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
	flag = 1;

	//envois de la reponse de debut
	fprintf(fp_usart1, "S1\r\n");

	// Attente d'une seconde
	OSTimeDly(OS_TICKS_PER_SEC / 1);

	while (flag) {

		// scrute les entrees
		if(USART1_CNT_IN > 0){
			cmd = USART1_BUFFER_IN[0];

			if(cmd == 'A' || cmd == 'B'){
				param[0] = USART1_BUFFER_IN[1];
				param[1] = USART1_BUFFER_IN[2];
				param[2] = USART1_BUFFER_IN[3];
			}

			USART1_CNT_IN = 0;
		}

		switch(cmd)
		{
			case '0' :
				fprintf(fp_usart1, "S0\r\n");
				flag = 1;
				break;
			case 'R' :
				fprintf(fp_usart1, "Reset ! \r\n");
				break;
			case 'S':
				fprintf(fp_usart1, "Set ! \r\n");
				break;
			case 'A' :
				throttle = (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0');
				break;
			case 'B' :
				breaks = (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0');
				break;
			default :
				break;
		}

		// si on a accelere ou freine, on modifie l'acceleration
	//	if (cmd == 'A' || cmd == 'B') {
			if (1){
				//a = throttle - breaks;
				//v0 = v;
				//t = 0;
				//fprintf(fp_usart1, "accelleratrion T%d\r\n",a);
				simulation(&speed, breaks, throttle);
				fprintf(fp_usart1, "sortie de %d\r\n ",a);
				fprintf(fp_usart1,"breaks are: %d, throttle is: %d, speed is: %f\r\n", breaks,throttle,speed);

		}

		// si l'acceleration n'est pas nulle on modifie la vitesse
	/*	if (a != 0){
			if ((a*t +v0 >= 0) && (a*t +v0 <= 255))
				v = a*t +v0;
			else if (a*t +v0 > 255)
				v = 255;
			else
				v = 0;

			// Attente d'une seconde (pour ne pas envoyer 2 messages de suites sans 1 sec. de delais)
			OSTimeDly(OS_TICKS_PER_SEC / 1);

			fprintf(fp_usart1, "V%d\r\n",v);
		}*/

		// incrementation du temps
		t++;

		cmd = 1;

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
}

/************************************************************************
 *
 * Definition de la tache
 *
 ************************************************************************/
static void thread_regulation (void *p_arg)
{
	(void)p_arg;
	while (1) {
		OSTimeDly(OS_TICKS_PER_SEC);
	}
}


/************************************************************************
 *
 * Initialisation tick et des taches
 *
 ***********************************************************************/
void initialise_taches (void) {
	// Init Sys. Tick
	OS_CPU_SysTickInit();
	#if OS_TASK_STAT_EN > 0
		OSStatInit();
	#endif

	int i=0;
	OSTaskCreate(thread_ihm, (void *)i,
				init_pile(pileTache[i], TAILLE_PILE_TACHE),
				PRIORITE_TACHE+i);
	i=1;
	OSTaskCreate(thread_regulation, (void *)i,
				init_pile(pileTache[i], TAILLE_PILE_TACHE),
				PRIORITE_TACHE+i);

	i=2;
	OSTaskCreate(thread_car_model, (void *)i,
				init_pile(pileTache[i], TAILLE_PILE_TACHE),
				PRIORITE_TACHE+i);
	OSTaskDel(OS_PRIO_SELF);
}

void init_cible_ntrt(void) {
	BSP_IntDisAll(); 	// Desactivation des interruptions
	OSInit(); 			// Initialisation de uCosII
	BSP_IntEnAll(); 	// Activation des interruptions
	BSP_Init(); 		// Initialisation de la carte NTRT

	// Init. de l'USART1
	//char cmdMain = '\0';
	fp_usart1 = fopen ("USART1","w");
}

/************************************************************************
 *
 * Programme principal
 *
 ***********************************************************************/
int main (void)
{
	init_cible_ntrt();
	OSTaskCreate(initialise_taches, (void *)0,
					init_pile(pileTache[MAX_TASKS], TAILLE_PILE_TACHE),
					PRIORITE_TACHE+MAX_TASKS);

	// Lancement de l'OS
	OSStart();
	while(1){}
}


