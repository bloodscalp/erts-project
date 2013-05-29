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

#include "app.h"
#include "regulation.h"
#include "thread_car_model.h"
#include "thread_ihm.h"

/************************************************************************
 *
 * Definition du STACK et globale de l'USART1
 *
 ************************************************************************/



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
 * Initialisation tick et des taches
 *
 ***********************************************************************/
void initialise_taches (void)
{
	// Init Sys. Tick
	OS_CPU_SysTickInit();
	#if OS_TASK_STAT_EN > 0
		OSStatInit();
	#endif

	OSTaskCreate(thread_ihm, NULL,
				init_pile(pileTache[0], TAILLE_PILE_TACHE),
				PRIORITE_TACHE);

	OSTaskCreate(thread_regulation, NULL,
				init_pile(pileTache[1], TAILLE_PILE_TACHE),
				PRIORITE_TACHE+1);

	OSTaskCreate(thread_car_model, NULL,
				init_pile(pileTache[2], TAILLE_PILE_TACHE),
				PRIORITE_TACHE+2);

	OSTaskDel(OS_PRIO_SELF);
}

void init_cible_ntrt(void)
{
	BSP_IntDisAll(); 	// Desactivation des interruptions
	OSInit(); 			// Initialisation de uCosII
	BSP_IntEnAll(); 	// Activation des interruptions
	BSP_Init(); 		// Initialisation de la carte NTRT

}

/************************************************************************
 *
 * Programme principal
 *
 ***********************************************************************/
int main (void)
{

	init_cible_ntrt();


	fp_usart1 = fopen("USART1","w");
	fprintf(fp_usart1, "--- PROGRAM STARTED\r\n");

	OSTaskCreate(initialise_taches, (void *)0,
					init_pile(pileTache[MAX_TASKS], TAILLE_PILE_TACHE),
					PRIORITE_TACHE+MAX_TASKS);

	// Lancement de l'OS
	OSStart();
	while(1){}
}


