/***********************************************************************
 *  Programme d'interfacage avec la partie Java
 *
 *	Filename    : 	thread_ihm.c
 *  Version     : 	V1.1
 *  Programmers : 	Mikael Trigo - Gregoire Hagmann
 *  				William Aebi - Christian Mueller
 *
 *  Email 		:	prenom.nom@master.hes-so.ch
 *
 ***********************************************************************/


#include <stdio.h>

#include "app.h"
#include "thread_ihm.h"
#include "globals.h"
#include "thread_car_model.h"


void thread_ihm (void *p_arg)
{
	(void *) p_arg;

	char cmd = '0';
	char param[] = {'0','0','0'};// paramètre pour le decomposer la commande de gaz ou break
	uint8_t flag = 0;

	fprintf(fp_usart1,"S0\r\n");

	while(1){
		// scrute les entrees
		// les butont set reset et consor n'on aucun effet si on est sur off
		if(USART1_CNT_IN > 0) {
			cmd = USART1_BUFFER_IN[0];

			if(cmd == 'A' || cmd == 'B'){
				param[0] = USART1_BUFFER_IN[1];
				param[1] = USART1_BUFFER_IN[2];
				param[2] = USART1_BUFFER_IN[3];
			}
			USART1_CNT_IN = 0;
		}

		switch(cmd) {
			case '0' :
				fprintf(fp_usart1, "S0\r\n");
				set_cmd_off(TRUE);
				flag = 0;
				break;

			case 'R' :
				fprintf(fp_usart1, "Reset ! \r\n");
				set_cmd_res(TRUE);
				break;

			case 'S':
				fprintf(fp_usart1, "Set ! \r\n");
				set_cmd_set(TRUE);
				break;

			case 'D' :
				fprintf(fp_usart1, "DEC ! \r\n");
				set_cmd_dec(TRUE);
				break;

			case 'U' :
				fprintf(fp_usart1, "ACC ! \r\n");
				set_cmd_acc(TRUE);
				break;

			default :
				break;
		}

		switch(cmd) {
			case '1' :
				set_cmd_on(TRUE);
				break;

			case 'A' :
				set_acc_sensor( (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0') );
				break;

			case 'B' :
				set_dec_sensor( (param[0]-'0')*100+(param[1]-'0')*10+(param[2]-'0') );
				break;

			default :
				break;
		}

		fprintf(fp_usart1, "T%03u\r\n",(unsigned int)(get_throttle()));
		fprintf(fp_usart1, "V%03u\r\n",(unsigned int)(get_speed_sensor()));

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

		thread_car_model(NULL);

		cmd = 3;
		OSTimeDly(CST_PERDIO_IHM);

	}
}



