#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/*=======================================================*/

/*				CONFIGURANDO BOTOES				*/
//	Botao 1 - OLED
#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			16
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX)

//	Botao 2 - OLED
#define BUT2_PIO			PIOC
#define BUT2_PIO_ID			12
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK	(1u << BUT2_PIO_IDX)

//	Botao 3 - OLED
#define BUT3_PIO			PIOA
#define BUT3_PIO_ID			10
#define BUT3_PIO_IDX		19
#define BUT3_PIO_IDX_MASK	(1u << BUT3_PIO_IDX)

/*				CONFIGURANDO LEDs				*/

// LED 1 - OLED
#define LED1_PIO			PIOA
#define LED1_PIO_ID			ID_PIOA
#define LED1_PIO_IDX		0
#define LED1_PIO_IDX_MASK	(1 << LED1_PIO_IDX)

// LED 2 - OLED
#define LED2_PIO			PIOC
#define LED2_PIO_ID			ID_PIOC
#define LED2_PIO_IDX		30
#define LED2_PIO_IDX_MASK	(1 << LED2_PIO_IDX)

// LED 3 - OLED
#define LED3_PIO			PIOB
#define LED3_PIO_ID			ID_PIOB
#define LED3_PIO_IDX		2
#define LED3_PIO_IDX_MASK	(1 << LED3_PIO_IDX)

/*=======================================================*/

/*				DEFININDO FUNCOES INIT				*/
void but_init(void);
void led_init(void);

/*				DFININDO FUNCOES					*/
void pisca_led(int n, int t, int l);

/*			Definindo as flags de botoes			*/
volatile char but_flag1;
volatile char but_flag2;
volatile char but_flag3;

/*			Funcoes de Callback				*/
// Inicando as funcoes
void but1_callback(void);
void but2_callback(void);
void but3_callback(void);

// Funcoes

void but1_callback(void) {
	but_flag1 = 1;
}

void but2_callback(void) {
	but_flag2 = 1;
}

void but3_callback(void) {
	but_flag3 = 1;
}

/*			Funcao para inicializar botoes			*/
void but_init(void) {
	// Enable Peripheral Clock
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP); // SUBIDA
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP); // DESCIDA
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP); // SUBIDA
	
	pio_handler_set(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but1_callback);
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but2_callback);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but3_callback);
	
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, 3); // BOTAO 1 - Prioridade 4
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 3); // BOTAO 2 - Prioridade 4
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 3); // BOTAO 3 -Prioridade 4
}

void led_init(void) {
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, PIO_DEFAULT);
	
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, PIO_DEFAULT);
	
	pmc_enable_periph_clk(LED3_PIO_ID);
	pio_configure(LED3_PIO, PIO_OUTPUT_0, LED3_PIO_IDX_MASK, PIO_DEFAULT);
}

void pisca_led(int n, int t, int l){
	if (l == 1) {
		for (int i=0;i<n;i++){
			pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_ms(t);
			pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_ms(t);
		}
	} else if (l == 2){
		for (int i=0;i<n;i++){
			pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
			delay_ms(t);
			pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			delay_ms(t);
		}
	} else if (l == 3) {
		for (int i=0;i<n;i++){
			pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);
			delay_ms(t);
			pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
			delay_ms(t);
		}
	}
}

int main (void)
{
	board_init();
	sysclk_init();
	but_init();
	led_init();
	delay_init();
	

  // Init OLED
	gfx_mono_ssd1306_init();
  
  // Escreve na tela um circulo e um texto
	gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
	gfx_mono_draw_string("mundo", 50,16, &sysfont);

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
		if (but_flag1) {
			gfx_mono_draw_string("				", 10,10, &sysfont);
			gfx_mono_draw_string("but_flag1", 10,10, &sysfont);
			pisca_led(5, 200, 1);
			but_flag1 = 0;
		}
		if (but_flag2) {
			gfx_mono_draw_string("				", 10,10, &sysfont);
			gfx_mono_draw_string("but_flag2", 10,10, &sysfont);
			pisca_led(5, 200, 2);
			but_flag2 = 0;
		}
		if (but_flag3) {
			gfx_mono_draw_string("				", 10,10, &sysfont);
			gfx_mono_draw_string("but_flag3", 10,10, &sysfont);
			pisca_led(5, 200, 3);
			but_flag3 = 0;
		}
		
	}
}
