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
#include "globals.h"

/************************************************************************
 *
 * Definition de la tache
 *
 ************************************************************************/
 
 
void thread_ihm (void *p_arg)
{
	(void *) p_arg;

	char cmd = '0';
	char param[] = {'0','0', '0'};// paramètre pour le decomposer la commande de gaz ou break
	uint8_t flag = 0;
	uint8_t flag2 = 0;

	fprintf(fp_usart1,"S0\r\n");
	set_statusReg(off);


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
	//		if (cmd == '1'){
	//			flag=1; //on est ON
	//			flag2=1;
	//		}

		}

		if (flag ==1){
			switch(cmd) {
				case '0' :
					fprintf(fp_usart1, "S0\r\n");
					set_statusReg(off);
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
					set_cmd_acc(TRUE);
					OSTimeDly(OS_TICKS_PER_SEC / 10);
					break;

				default :
					break;
			}
		}

		switch(cmd) {
			case '1' :
				flag=1;
				flag2=1;
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

		OSTimeDly(OS_TICKS_PER_SEC / 10);
		if (flag2==1){
			set_statusReg(on);
			flag2=0;
		}

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

		cmd = 3;
		OSTimeDly(OS_TICKS_PER_SEC / 5);

	}
}



