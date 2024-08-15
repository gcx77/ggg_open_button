#ifndef __GGG_OPEN_BUTTON_H__
#define __GGG_OPEN_BUTTON_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif /*end cpp*/

typedef uint32_t button_time_t;
typedef uint32_t button_keyval_t;
typedef bool     button_pressed_t;


#define BUTTON_IS_PRESSED      (true)
#define BUTTON_ISNOT_PRESSED   (false)

#define BUTTON_INDEX1      (0)
#define BUTTON_INDEX2      (1)

typedef enum
{
    BUTTON_STATUS_RELEASE = 0,
    BUTTON_STATUS_PRESSED_CONFIRM,
    BUTTON_STATUS_PRESSED_SHORT,
    BUTTON_STATUS_PRESSED_LONG,
    BUTTON_STATUS_WAIT_HIT,
  
} button_status_et;

typedef  enum
{
    BUTTON_HITCNT_NONE = 0,
    BUTTON_HITCNT_SHORT1,
    BUTTON_HITCNT_SHORT2,   
    BUTTON_HITCNT_SHORT3,
    BUTTON_HITCNT_SHORT4,
    BUTTON_HITCNT_LONG = 99,
} button_hitcnt_et;

typedef struct button_interval
{
    button_time_t confirm_time_interval;
    button_time_t short_time_interval;
    button_time_t long_time_interval;
    button_time_t attack_time_interval;

} button_interval_t,*button_interval_pt;

typedef struct button_key
{
    button_keyval_t button_keyval_release;
    button_keyval_t button_keyval_pressed_short;
    button_keyval_t button_keyval_pressed_long;
    button_keyval_t button_keyval_attack2;
    button_keyval_t button_keyval_attack3;
    button_keyval_t button_keyval_now;
} button_key_t,*button_key_pt;



typedef struct button_info
{
    button_status_et    button_status;
    button_time_t       button_time_prv;
    button_interval_t   button_time_interval;
    button_hitcnt_et    button_hitcnt_temp;
    button_hitcnt_et    button_hitcnt_final;   
    button_pressed_t    (*button_is_pressed)(void); 

} button_info_t, *button_info_pt;

/***************************func***************************/
/*定时器或者systick中，定时调用该函数 */
void button_time_handler(void);
/*获取某个按键状态，获取按键状态后自己选择处理方式 */
button_hitcnt_et button_status_get(uint32_t button_group_index);
/*需要自己实现按键检测高低电平状态，将其填充到button_is_pressed函数中 */
/***************************end***************************/
#ifdef __cplusplus
}
#endif /*end cpp*/

#endif /*end ggg_open_button.h*/