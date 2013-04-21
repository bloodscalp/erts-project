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

/************************************************************************
 *
 * Definition du STACK et globale de l'USART1
 *
 ************************************************************************/

// Descripteur de fichier pour le USART1
FILE *fp_usart1;

// Stack de la tache
static  OS_STK          AppTaskStk[APP_TASK_STK_SIZE];


/************************************************************************
 *
 * Definition de la tache
 *
 ************************************************************************/
static void AppTask (void *p_arg)
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

	// Init Sys. Tick
	OS_CPU_SysTickInit();

	#if OS_TASK_STAT_EN > 0
		OSStatInit();
	#endif

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
		if (cmd == 'A' || cmd == 'B') {
			a = throttle - breaks;
			v0 = v;
			t = 0;
			fprintf(fp_usart1, "T%d\r\n",a);
		}

		// si l'acceleration n'est pas nulle on modifie la vitesse
		if (a != 0){
			if ((a*t +v0 >= 0) && (a*t +v0 <= 255))
				v = a*t +v0;
			else if (a*t +v0 > 255)
				v = 255;
			else
				v = 0;

			// Attente d'une seconde (pour ne pas envoyer 2 messages de suites sans 1 sec. de delais)
			OSTimeDly(OS_TICKS_PER_SEC / 1);

			fprintf(fp_usart1, "V%d\r\n",v);
		}

		// incrementation du temps
		t++;

		cmd = 1;

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
}

/************************************************************************
 *
 * Programme principal
 *
 ***********************************************************************/
int main (void)
{
	// Verification de la taille du nom
	#if OS_TASK_NAME_SIZE > 13
		INT8U err;
	#endif

	BSP_IntDisAll(); 	// Desactivation des interruptions
	OSInit(); 			// Initialisation de uCosII
	BSP_IntEnAll(); 	// Activation des interruptions

	BSP_Init(); 		// Initialisation de la carte NTRT

	// Init. de l'USART1
	fp_usart1 = fopen ("USART1","w");

	// Cree la tache
	OSTaskCreateExt(AppTask,
					(void *)0,
					(OS_STK *)&AppTaskStk[APP_TASK_STK_SIZE-1],
					APP_TASK_PRIO,
					APP_TASK_PRIO,
					(OS_STK *)&AppTaskStk[0],
					APP_TASK_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	// Verification de la taille du nom
	#if OS_TASK_NAME_SIZE > 4
		OSTaskNameSet(APP_TASK_PRIO, (INT8U*)"Task", &err);
	#endif

	// Lancement de l'OS
	OSStart();

	while(1){}
}


