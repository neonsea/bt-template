#ifndef __APP_BT_TEMPLATE_H__
#define __APP_BT_TEMPLATE_H__

typedef unsigned char byte;

struct app_data_
{
	void *ret_f;
	int recv_len;
	byte *recv_data;
};

// template.c
void show_screen(void *return_screen);
void key_press_screen();
void draw_screen();
void custom_msg_handler(byte *data, int len);
#endif