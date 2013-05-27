/***********************************************************************
 *  Programme d'interfacage avec la partie Java
 *
 *
 *	Filename      : thread_ihm.c
 *  Version       : V1.1
 *  Programmer(s) : Mikael Trigo
 *
 ***********************************************************************/


//

// Stack de la tache
//

#include <stdio.h>

#include "thread_ihm.h"
#include "getset.h"

/************************************************************************
 *
 * Definition de la tache
 *
 ************************************************************************/
 
 
void thread_ihm (void *p_arg)
{
	FILE *fp_usart1;		// FIX ME ,GLOBALE OU PAS?
	static  OS_STK          AppTaskStk[APP_TASK_STK_SIZE];

	char cmd = '0';
	char param[] = {'0','0', '0'};// paramètre pour le decomposer la commande de gaz ou break
	uint8_t flag = 0;
	uint8_t v = 0;
	uint8_t v0 = 0;
	uint8_t t = 0;
	int8_t a = 0;

	//---Global Variable------------------------------
	//change plac
	uint8_t throttle = 0;
	uint8_t breaks = 0;
	uint8_t ACC =0;
	uint8_t DEC = 0;
	//bool set=FALSE;
	//bool reset=FALSE;
	// attente de la commande de start pour démarrer
	while (cmd != '1') {
		// scrute les entrees
		if(USART1_CNT_IN > 0) {
			cmd = USART1_BUFFER_IN[0];
			USART1_CNT_IN = 0;
		}

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
	flag = 1;//on va triater l'entrée
	//envois de la reponse de debut
	fprintf(fp_usart1," sytème enclenché\r\n");

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
				//reset =TRUE;
				set_cmd_res(TRUE);
				break;
			case 'S':
				fprintf(fp_usart1, "Set ! \r\n");
				set_cmd_set(TRUE);
				break;
			case 'DE' :// FIX ME --PEUT ETRE UTLISER QUE UNE LETTRE a teset
				fprintf(fp_usart1, "DEC ! \r\n");
				set_cmd_dec(TRUE);
				break;
			case 'AC' :
				fprintf(fp_usart1, "ACC ! \r\n");
				set_cmd_dec(TRUE);
				break;
			case 'A' :
				throttle = (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0');
				set_throttle(throttle);
				break;
			case 'B' :
				breaks = (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0');
				break;
			default :
				break;
		}

		// si on a accelere ou freine, on modifie l'acceleration
		if (cmd == 'A' || cmd == 'B') {

				//for debug
				fprintf(fp_usart1," THREAD IHM DETECTED THROTTLE¦¦ BREAKS breaks are: %d, throttle is: %d\r\n", breaks,throttle);

		}

		t++;
		cmd = 1;

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
}


