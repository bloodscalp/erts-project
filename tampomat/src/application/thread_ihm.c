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

#include "app.h"
#include "thread_ihm.h"
#include "getset.h"

/************************************************************************
 *
 * Definition de la tache
 *
 ************************************************************************/
 
 
void thread_ihm (void *p_arg)
{

	char cmd = '0';
	char param[] = {'0','0', '0'};// param�tre pour le decomposer la commande de gaz ou break

	//statusReg Status_on= on;

	uint8_t flag = 0;
//	uint8_t v = 0;
//	uint8_t v0 = 0;
	uint8_t t = 0;
//	int8_t a = 0;

	//---Global Variable------------------------------
	//change plac
	uint8_t throttle = 0;
	uint8_t breaks = 0;
//	uint8_t ACC =0;
//	uint8_t DEC = 0;
	//bool set=FALSE;
	//bool reset=FALSE;
	// attente de la commande de start pour d�marrer
	fprintf(fp_usart1,"S0\r\n");
	while (cmd != '1') {
		set_statusRegOn(off);

		// scrute les entrees
		if(USART1_CNT_IN > 0) {
			cmd = USART1_BUFFER_IN[0];
			USART1_CNT_IN = 0;
		}

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
	set_statusReg(on);

	flag = 1;//on va triater l'entr�e
	//envois de la reponse de debut
	//fprintf(fp_usart1," systeme enclenche\r\n");

	// Attente d'une seconde
	fprintf(fp_usart1,"S1\r\n");
	OSTimeDly(OS_TICKS_PER_SEC / 10);

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
		//fprintf(fp_usart1, "T%03u\r\n",(unsigned int)(get_throttle()));
		//fprintf(fp_usart1, "V%03u\r\n",(unsigned int)(get_speed_sensor()));
		switch(cmd)
		{
			case '0' :
				fprintf(fp_usart1, "S0\r\n");
				OSTimeDly(OS_TICKS_PER_SEC / 10);

				flag = 0;
				break;
			case 'R' :
				fprintf(fp_usart1, "Reset ! \r\n");
				set_cmd_res(TRUE);
				OSTimeDly(OS_TICKS_PER_SEC / 10);


				break;
			case 'S':
				fprintf(fp_usart1, "Set ! \r\n");
				set_cmd_set(TRUE);
				OSTimeDly(OS_TICKS_PER_SEC / 10);
				break;
			case 'D' :
				fprintf(fp_usart1, "DEC ! \r\n");
				set_cmd_dec(TRUE);
				OSTimeDly(OS_TICKS_PER_SEC / 10);
				break;
			case 'U' :
				fprintf(fp_usart1, "ACC ! \r\n");
				set_cmd_dec(TRUE);
				OSTimeDly(OS_TICKS_PER_SEC / 10);
				break;
			case 'A' :
				throttle = (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0');

				//fprintf(fp_usart1, "V%03u\r\n",(unsigned int)(throttle));
				set_throttle(throttle);
				break;
			case 'B' :
				breaks = (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0');
				set_dec_sensor(breaks);
				break;
			default :
				break;
		}

		// si on a accelere ou freine, on modifie l'acceleration
		if (cmd == 'A' || cmd == 'B') {
			//for debug
			//fprintf(fp_usart1," THREAD IHM DETECTED THROTTLE\r\n");
			fprintf(fp_usart1, "T%03u\r\n",(unsigned int)(throttle));
		}
		else
		{
			fprintf(fp_usart1, "V%03u\r\n",(unsigned int)(get_speed_sensor()));
			//printf("%f",get_speed_sensor());
		}
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		// FIXME
		switch (get_statusReg()) {

		case on:
			fprintf(fp_usart1,"S1\r\n");
			break;

		case off:
			fprintf(fp_usart1,"S0\r\n");
			break;

		case standby:
			fprintf(fp_usart1,"SS\r\n");

			break;

		case interrupted:
			fprintf(fp_usart1,"SI\r\n");

			break;
		default :
			break;
		}

		t++;
		cmd = 1;

		// Attente d'une seconde
		OSTimeDly(OS_TICKS_PER_SEC / 1);
	}
}


