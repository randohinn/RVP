#ifndef EINK_H
#define EINK_H

#include <stdint.h>

/* Riistvaraprojekt UT 2020/21 - Rando Hinn, vastuvõtjamooduli eink libra */


#define WIDTH 40
#define HEIGHT 40

#define IF_INVERT_COLOR 0

#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define VCOM_LUT                                    0x20
#define W2W_LUT                                     0x21
#define B2W_LUT                                     0x22
#define W2B_LUT                                     0x23
#define B2B_LUT                                     0x24
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_CALIBRATION              0x40
#define TEMPERATURE_SENSOR_SELECTION                0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define RESOLUTION_SETTING                          0x61
#define GET_STATUS                                  0x71
#define AUTO_MEASURE_VCOM                           0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82
#define PARTIAL_WINDOW                              0x90
#define PARTIAL_IN                                  0x91
#define PARTIAL_OUT                                 0x92
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAM                              0xA1
#define READ_OTP_DATA                               0xA2
#define POWER_SAVING                                0xE3

uint8_t epdwidth;

void EINK_init();
void EINK_toggle_reset();
void EINK_set_data();
void EINK_set_command();
void EINK_send_command(unsigned char command);
void EINK_send_data(unsigned char data);
void EINK_clear_frame();
void EINK_display_frame(unsigned char* frame_buffer_black, unsigned char* frame_buffer_red);
void EINK_draw_absolute_pixel(unsigned char* image, int x, int y, int colored);
void EINK_set_partial_black(unsigned char* buffer, int x, int y, int w, int l);
void EINK_set_partial_red(unsigned char* buffer, int x, int y, int w, int l);
void EINK_wait();

#endif