
#include "./ggg_open_button.h"

/***************************static pfunc************************/

static button_time_t button_get_systime(void);
static void button_key_start_scheduler(void);
static button_pressed_t button1_is_pressed(void);
static button_pressed_t button2_is_pressed(void);

/***************************argument***************************/

volatile static button_time_t g_button_systime;

button_info_t button_group[] =
    {
        {.button_time_prv = 0, .button_status = BUTTON_STATUS_RELEASE,
         .button_time_interval = {.confirm_time_interval = 20, .short_time_interval = 50, .long_time_interval = 100, .attack_time_interval = 30},
         .button_hitcnt_temp = BUTTON_HITCNT_NONE, .button_hitcnt_final = BUTTON_HITCNT_NONE, .button_is_pressed = button1_is_pressed},

        {.button_time_prv = 0, .button_status = BUTTON_STATUS_RELEASE,
         .button_time_interval = {.confirm_time_interval = 20, .short_time_interval = 60, .long_time_interval = 120, .attack_time_interval = 30},
         .button_hitcnt_temp = BUTTON_HITCNT_NONE, .button_hitcnt_final = BUTTON_HITCNT_NONE, .button_is_pressed = button2_is_pressed},
};

/***************************define***************************/

#define BUTTON_GROUP_SIZE (sizeof(button_group) / sizeof(button_group[0]))

/***************************func***************************/

int main(void)
{
    /*TEST */
}
button_hitcnt_et button_status_get(uint32_t button_group_index)
{
    button_hitcnt_et res = button_group[button_group_index].button_hitcnt_final;
    button_group[button_group_index].button_hitcnt_final = BUTTON_HITCNT_NONE;
    return res;
}
static void button_key_start_scheduler(void)
{
    for (size_t i = 0; i < BUTTON_GROUP_SIZE; ++i)
    {
        switch (button_group[i].button_status)
        {
        case BUTTON_STATUS_RELEASE:
        {
            button_group[i].button_hitcnt_temp = BUTTON_HITCNT_NONE;
            if (BUTTON_IS_PRESSED == button_group[i].button_is_pressed())
            {
                if (button_get_systime() - button_group[i].button_time_prv >
                    button_group[i].button_time_interval.confirm_time_interval)
                {
                    button_group[i].button_time_prv = button_get_systime();
                    button_group[i].button_status = BUTTON_STATUS_PRESSED_CONFIRM;
                }              
            }
        }
            break;

        case BUTTON_STATUS_PRESSED_CONFIRM:
        {
            if (BUTTON_IS_PRESSED == button_group[i].button_is_pressed())
            {
                if (button_get_systime() - button_group[i].button_time_prv >
                    button_group[i].button_time_interval.short_time_interval)
                {
                    button_group[i].button_time_prv = button_get_systime();
                    button_group[i].button_status = BUTTON_STATUS_PRESSED_SHORT;
                }
            }
            else
            {
                button_group[i].button_time_prv = button_get_systime();
                button_group[i].button_status = BUTTON_STATUS_RELEASE;
            }                 
        }
            break;

        case BUTTON_STATUS_PRESSED_SHORT:
        {
            if (BUTTON_IS_PRESSED == button_group[i].button_is_pressed())
            {
                if (button_get_systime() - button_group[i].button_time_prv >
                    button_group[i].button_time_interval.long_time_interval)
                {
                    button_group[i].button_hitcnt_temp = BUTTON_HITCNT_LONG;  /* press long*/
                    button_group[i].button_time_prv = button_get_systime();
                    button_group[i].button_status = BUTTON_STATUS_PRESSED_LONG;
                }               
            }
            else
            {
                button_group[i].button_hitcnt_temp++;
                button_group[i].button_time_prv = button_get_systime();
                button_group[i].button_status = BUTTON_STATUS_WAIT_HIT;
            }
        }
            break;

        case BUTTON_STATUS_PRESSED_LONG:
        {
            if (BUTTON_IS_PRESSED == button_group[i].button_is_pressed())
            {
                {
                    button_group[i].button_time_prv = button_get_systime();
                    button_group[i].button_status = BUTTON_STATUS_PRESSED_LONG;
                }               
            }
            else
            {
                button_group[i].button_time_prv = button_get_systime();
                button_group[i].button_status = BUTTON_STATUS_RELEASE;
                button_group[i].button_hitcnt_final =  button_group[i].button_hitcnt_temp;

            }
        }
            break;

        case BUTTON_STATUS_WAIT_HIT:
        {
            if (BUTTON_ISNOT_PRESSED == button_group[i].button_is_pressed())
            {
                if (button_get_systime() - button_group[i].button_time_prv >
                    button_group[i].button_time_interval.attack_time_interval)
                {
                    button_group[i].button_time_prv = button_get_systime();
                    button_group[i].button_status = BUTTON_STATUS_RELEASE;
                    button_group[i].button_hitcnt_final =  button_group[i].button_hitcnt_temp;
                }               
            }
            else
            {
                if (button_get_systime() - button_group[i].button_time_prv >
                    button_group[i].button_time_interval.confirm_time_interval)
                {
                    button_group[i].button_time_prv = button_get_systime();
                    button_group[i].button_status = BUTTON_STATUS_PRESSED_CONFIRM;
                } 
            }
        }
            break;

        default:
            break;
        }
    }
}



void button_time_handler(void)
{
    ++g_button_systime;
    button_key_start_scheduler();
}
static button_time_t button_get_systime(void)
{
    return g_button_systime;
}

/*自己根据芯片库函数处理下按键检测到按下的状态*/
static button_pressed_t button1_is_pressed(void)
{
    /*
     *按键被按下 return BUTTON_IS_PRESSED       (true)
     *按键未按下 return BUTTON_ISNOT_PRESSED    (false)
     */
    return BUTTON_ISNOT_PRESSED;
}

static button_pressed_t button2_is_pressed(void)
{
    /*
     *按键被按下 return BUTTON_IS_PRESSED       (true)
     *按键未按下 return BUTTON_ISNOT_PRESSED    (false)
     */

    return BUTTON_ISNOT_PRESSED;
}