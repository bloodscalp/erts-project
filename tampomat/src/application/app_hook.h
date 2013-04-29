/***********************************************************************
 *  Definition of specific Task function
 *
 *  Three simple task printing a counter on the USART1 are created
 *
 *	Filename      : app_hook.h
 *  Version       : V1.0
 *  Programmer(s) : Xavier Meyer
 *  Misc		  : In order to enable these functions the constant
 *  				"OS_APP_HOOKS_EN" in os_cfg.h must be set to 1.
 *  				With the default value of 0, defaults functions
 *  				provided by the Micrium port are used.
 *
 ***********************************************************************/



void          App_TaskCreateHook      (OS_TCB          *ptcb);
void          App_TaskDelHook         (OS_TCB          *ptcb);
void          App_TaskIdleHook        (void);
void          App_TaskStatHook        (void);
void          App_TaskSwHook          (void);
void          App_TCBInitHook         (OS_TCB          *ptcb);
