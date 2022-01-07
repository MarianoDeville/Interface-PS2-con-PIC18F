/********************************************************************************/
/*				TERMINAL CON TECLADO PS2 GENERICO Y DISPLAY LCD.				*/
/*				Revisión:				1.02									*/
/*				PIC:					PIC16F877A								*/
/*				Compilador:				MPLAB IDE 8.53 - HI-TECH 9.60			*/
/*				Checksum:				0xc4e8									*/
/*				Fecha de creación:		27/12/2007								*/
/*				Fecha de modificación:	11/03/2011								*/
/*				Autor:					Mariano Ariel Deville					*/
/********************************************************************************/
/*									MACROS										*/
/*..............................................................................*/
#define	PIC_CLK			20000000	// Defino la frecuencia del crystal en Hz.	*/
#define	ENTRADA			1			//											*/
#define	SALIDA			0			//											*/
/*------------------------------------------------------------------------------*/
/*				Defino los nombres de los pines de E/S							*/
/*..............................................................................*/
#define	DATO_PS2		RD1			//											*/
#define	MAN_AUTO		RC1			//											*/
#define	LED_VERDE		RC2			//											*/
#define	LED_ROJO		RC3			//											*/
#define BCKLGH			RD5			//											*/
#define BUZZER			RC0			//											*/
/********************************************************************************/
/*						VARIABLES GLOBALES										*/
/*..............................................................................*/
bank1 unsigned char rs_str[20],teclado_str[20],pos_str_rs;
/*------------------------------------------------------------------------------*/
/*								Archivos cabecera.								*/
/*..............................................................................*/
#include "htc.h"			// Cabecera para el compilador.						*/
#include "string.h"			// Para poder usar el strcpy().						*/
#include "delay.c"			// Rutinas de demora.								*/
#include "TecladoPS2.c"		// Driver para teclado PS2.							*/
#include "lcd.c"			// Driver display LCD 2x20.							*/
#include "RS232.c"			// Driver comunicación RS232.						*/
__CONFIG(DEBUGDIS & PROTECT & LVPDIS & BOREN & PWRTEN & WDTEN & HS);
/********************************************************************************/
/*						PROTOTIPO DE FUNCIONES									*/
/*..............................................................................*/
void interrupt isr(void);

void main (void)
{ 
	unsigned char i;
	CLRWDT();					// Borra el WDT
	ADCON1=0b00000110;			// Confiura el PORTA como todo digital.
 
	TRISA0=ENTRADA;
	TRISA1=ENTRADA;
	TRISA2=ENTRADA;
	TRISA3=ENTRADA;
	TRISA4=ENTRADA;
	TRISA5=ENTRADA;


	TRISB0=ENTRADA;				// Entrada de interrupcion RB0.
	TRISB1=ENTRADA;
	TRISB2=SALIDA;	   			// Salida para el LCD RS.
	TRISB3=SALIDA;   			// Salida para el LCD E.
	TRISB4=SALIDA; 	  			// Salida para el LCD AD4.
	TRISB5=SALIDA;   			// Salida para el LCD AD5.
	TRISB6=SALIDA;   			// Salida para el LCD AD6.
	TRISB7=SALIDA;   			// Salida para el LCD AD7.
 
	TRISC0=SALIDA;				// Salida para un BUZZER.
	TRISC1=ENTRADA;				// Entrada MANUAL/AUTOMÁTICO.
	TRISC2=SALIDA;				// Led verde.
	TRISC3=SALIDA;				// Led rojo.
	TRISC4=ENTRADA;
	TRISC5=ENTRADA;
	TRISC6=SALIDA;				// Salida TX.
	TRISC7=ENTRADA;				// Entrada RX.
 
	TRISD0=ENTRADA;
	TRISD1=ENTRADA;
	TRISD2=ENTRADA;
	TRISD3=ENTRADA;
	TRISD4=ENTRADA;
	TRISD5=SALIDA;				// Salida que controla el backlight del LCD.
	TRISD6=ENTRADA;
	TRISD7=ENTRADA;

	TRISE0=ENTRADA;
	TRISE1=ENTRADA;
	TRISE2=ENTRADA;
/********************************************************************************/
/*					TIMER 0 - NO UTILIZADO										*/
/*..............................................................................*/
	T0CS=0;					// Oscilador interno.								*/
	T0SE=0;					// Flanco ascendente.								*/
	PSA=1;					// Asigno el preescaler a WDT.						*/
	PS0=1;					// Configuro el Preescaler.							*/
	PS1=1;					// 													*/
	PS2=1;					// 													*/
	TMR0IF=0;				// Bajo la bandera de la interrupción.				*/
/********************************************************************************/
/*					TIMER 1 - NO UTILIZADO										*/
/*..............................................................................*/
	T1CKPS0=1; 				// Preescaler TMR1 a 1:8.							*/
	T1CKPS1=1; 				//													*/
	T1SYNC=1;				// No sincronizo con clock externo.					*/
	T1OSCEN=0;				// Oscilador deshabilitado.							*/
	TMR1CS=0;  				// Reloj interno Fosc/4.							*/
	TMR1IF=0;				// Bajo la bandera de la interrupción.				*/
	TMR1ON=0;				// Apago el TMR1.									*/
	TMR1H=0b00000000;		// Configuro el tiempo que tarda en generar			*/
	TMR1L=0b00000000;		// la interrupcion.									*/
/********************************************************************************/
/*					TIMER 2 - NO UTILIZADO										*/
/*..............................................................................*/
	TMR2ON=0;				// Timer 2 apagado.									*/
	T2CKPS0=0;				// Configuro el Preescaler.							*/
	T2CKPS1=0;				// 													*/
	TMR2IF=0;				// Bajo la bandera de la interrupción.				*/
/********************************************************************************/
/*					Configuración de las interrupciones							*/
/*..............................................................................*/
	GIE=1;					// Utilizo interrupciones.							*/
	PEIE=1;					// Interrupcion externa habilitada.					*/
	INTE=0;					// Interrupcion RB0/INT habilitada.					*/
	TMR0IE=0;				// Interrupcion desborde TMR0 deshabilitada.		*/
	TMR1IE=0;				// Interrupcion desborde TMR1 habilitada.			*/
	TMR2IE=0;				// Interrupcion desborde TMR2 deshabilitada.		*/
	CCP1IE=0;				// CCP1 Interrupt disable.							*/
	CCP2IE=0;				// CCP2 Interrupt disable.							*/
	CMIE=0;					// Comparator Interrupt disable.					*/
	EEIE=0;					// EEPROM Write Operation Interrupt disable.		*/
	SSPIE=0;				// Interrupcion por comunicacion I2C.				*/
	PSPIE=0;				// Slave Port Read/Write Interrupt disable.			*/
	BCLIE=0;				// Bus Collision Interrupt disable.					*/
	ADIE=0;					// Interrupcion del conversor AD deshabilitada.		*/
	RBIE=0;					// Interrupcion por RB deshabilitada.				*/
	RCIE=0;					// Interrupcion recepcion USART habilitada.			*/
 	INTEDG=0;				// Interrupcion en el flanco descendente de RB0.	*/
	RBPU=1;					// RB pull-ups estan deshabilitadas.				*/
/********************************************************************************/
	Lcd_Setup();							// Inicializa LCD y borra la pantalla.
	Serial_Setup(9600);						// Setea el puerto serie
	DelayS(2);								// Tiempo para que arranque el teclado.
	Reseteo_Teclado();						// Reseteo las variables del teclado.
	BCKLGH=1;
	pos_str_rs=0;
	rs_str[0]=0;
	CREN=0;
	CREN=1;									// Enable continuous reception.
	RCIE=1;
	for(;;)
	{
		if(MAN_AUTO)
			Imprimir_Lcd("MODO","AUTOMATICO",1);	// Imprimo en pantalla lo que voy escribiendo y recibiendo por RS232.
		else
			Imprimir_Lcd(rs_str,teclado_str,1);	// Imprimo en pantalla lo que voy escribiendo y recibiendo por RS232.
		LED_VERDE=MAN_AUTO;
		if(LED_VERDE==LED_ROJO)				// Son iguales? hubo un cambio de estado
			Reseteo_Teclado();				// Reseteo las variables del teclado.
		LED_ROJO=!MAN_AUTO;
		if(!INTE)							// Mando el mensaje automático.
		{
			PutStr("PAT:");
			PutStr(teclado_str);			// Envio por RS232 lo capturado por teclado o el mensaje automatico.
			PutCh(13);
			PutCh(10);
			pos_str_rs=0;					// Reseteo la cadena.
			rs_str[0]=0;					// Reseteo la cadena.
			Reseteo_Teclado();				// Reseteo las variables del teclado.
		}
		DelayMs(50);						// Antialiasin.
	}
}	
/***************************************************************************************/
/*		     			Llamada a rutina de interrupcion							   */
/***************************************************************************************/
void interrupt isr(void)
{
	unsigned char buffer_rs;
	CLRWDT();
	if(INTF && INTE)					// La interrupcion se genero por un evento externo.
	{
		if(pos_ps2>=3&&pos_ps2<=10)		// Los bit 3 a 10 se considerran datos. Paridad, start y stop son ignorados
		{
			lect_ps2=(lect_ps2>>1);		// Desplazo los bits un lugar
			if(DATO_PS2)				// Dependiendo del dato que leo en el pin del pic
				lect_ps2=(lect_ps2|0x80);	// escribo un 1 en el bit mas significativo.
		}
		pos_ps2--;						// Voy al siguiente bit.
	   	if(!pos_ps2)					// Si llegue al final de la cadena de bits
		{
			Interpretar_Teclado();		// Llamo la funcion que busca el equivalente ASCII.
			pos_ps2=11;					// Vuelvo el bit de posición al lugar 11
			lect_ps2=0;					// y vacio la variable lectura.
		}
		INTF=0;							// Borro la bandera de la interrupcion externa.
		return;
	}
	if(RCIF && RCIE)					// Si la interrupción se produjo por una comunicacion serial.
	{
		buffer_rs=RCREG;				// Buffereo el valor leido.
		if(buffer_rs>=32)				// Filtro los caracteres no imprimibles.
		{
			rs_str[pos_str_rs++]=buffer_rs;	// Almaceno en la cadena el caracter enviado por RS232.
			rs_str[pos_str_rs]=0;
			if(pos_str_rs>19)
				pos_str_rs=19;			// En caso que la variable caiga fuera del rango la llevo a un valor dentro.
			return;
		}
		else
		{
			if((buffer_rs==10||buffer_rs==13))	// Si el caracter es un ENTER
			{
				CREN=0;					// Deshabilito la recepción continua.
				if(MAN_AUTO)			// Si esta en AUTO (1), envio un mensage automático.
				{
					INTE=0;
					strcpy(teclado_str,"XXX999");
				}
				CREN=1;					// Habilito la recepción continua.
				pos_str_rs=0;			// y reseteo la cadena.
			}
			return;
		}
	}
	return;
}
