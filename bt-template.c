#include <libbip.h>
#include <alipay-bt.h>
#include "bt-template.h"

//	screen menu structure - different for each screen
struct regmenu_ screen_data = {
	55,				  //	main screen number, value 0-255, for custom windows it is better to take from 50 and above
	1,				  //	auxiliary screen number (usually 1)
	0,				  //	0
	0,				  //	pointer to the function of processing a wheelbarrow, swipes, long button press
	key_press_screen, //	a pointer to the button click handling function
	0,				  //	a pointer to the timer callback function
	0,				  //	0
	show_screen,	  //	pointer to the screen display function
	0,				  //
	draw_screen		  //	long button press
};

int main(int param0, char **argv)
{
	show_screen((void *)param0);
}

void custom_msg_handler(byte *data, int len)
{
	struct app_data_ **app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *app_data = *app_data_p;

	// alipay_send_host_data(data, len); // send same data back to host

	_memcpy(app_data->recv_data, data, len);
	app_data->recv_data[len] = 0;
	app_data->recv_len = len;
	draw_screen();
}

void show_screen(void *param0)
{
	struct app_data_ **app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *app_data;

	Elf_proc_ *proc;

	if ((param0 == *app_data_p) && get_var_menu_overlay())
	{

		app_data = *app_data_p;
		*app_data_p = NULL;

		reg_menu(&screen_data, 0);

		*app_data_p = app_data;
	}
	else
	{
		reg_menu(&screen_data, 0);
		*app_data_p = (struct app_data_ *)pvPortMalloc(sizeof(struct app_data_));
		app_data = *app_data_p;

		_memclr(app_data, sizeof(struct app_data_));

		proc = param0;

		if (param0 && proc->ret_f)
			app_data->ret_f = proc->elf_finish;
		else
			app_data->ret_f = show_watchface;

		app_data->recv_len = -1;
		app_data->recv_data = pvPortMalloc(21);
		_memclr(app_data->recv_data, 21);
		alipay_task_create();
		alipay_set_msg_handler(custom_msg_handler);

		// make sure the app doesn't go to sleep
		set_display_state_value(8, 1);
		// set_display_state_value(4, 1); // backlight
		set_display_state_value(2, 1);
	}

	draw_screen();
}

void key_press_screen()
{
	struct app_data_ **app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *app_data = *app_data_p;

	alipay_task_delete();
	alipay_set_msg_handler(NULL);

	vPortFree(app_data->recv_data);

	show_menu(app_data->ret_f, (unsigned int)show_screen);
};

void draw_screen()
{
	struct app_data_ **app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *app_data = *app_data_p;

	vibrate(1, 70, 0);

	set_bg_color(COLOR_BLACK);
	fill_screen_bg();
	set_graph_callback_to_ram_1();
	load_font();
	set_fg_color(COLOR_WHITE);

	if (app_data->recv_len == -1)
	{
		text_out_center("no data", 88, 60);
	}
	else
	{
		text_out_center((char *)app_data->recv_data, 88, 60);
	};

	repaint_screen_lines(0, 176);
};