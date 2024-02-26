#include "pico/stdlib.h"
#include "hardware/pwm.h"

uint32_t pwm_get_wrap(uint slice_num)
{
    valid_params_if(PWM, slice_num >= 0 &&
                             slice_num < NUM_PWM_SLICES);
    return pwm_hw->slice[slice_num].top;
}

uint32_t pwm_set_freq_duty(uint slice_num, uint chan,
                           uint32_t f, int d)
{
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
        divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

void pwm_set_duty(uint slice_num, uint chan, int d)
{
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num) * d / 100);
}


typedef struct
{
    uint gpio;
    uint slice;
    uint channel;
    uint speed;
    uint resolution;
    bool on;
    bool invert;
} Servo;

void ServoInit(Servo *s, uint gpio, bool invert)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    s->gpio = gpio;
    s->slice = pwm_gpio_to_slice_num(gpio);
    s->channel = pwm_gpio_to_channel(gpio);

    pwm_set_enabled(s->slice, false);

    s->on = false;
    s->speed = 0;
    s->resolution = pwm_set_freq_duty(s->slice,s->channel, 50, 0);
    
    if (s->channel)
    {
        pwm_set_output_polarity(s->slice, false, invert);
    }
    else
    {
        pwm_set_output_polarity(s->slice, invert, false);
    }
    s->invert = invert;
}

void pwm_set_dutyH(uint slice_num, uint channel, int d)
{
    pwm_set_chan_level(slice_num, channel, pwm_get_wrap(slice_num) * d / 10000);
}

void ServoOn(Servo *s)
{
    pwm_set_enabled(s->slice, true);
    s->on = true;
}

void ServoOff(Servo *s)
{
    pwm_set_enabled(s->slice, false);
    s->on = false;
}

void ServoPosition(Servo *s, uint p)
{
    pwm_set_dutyH(s->slice, s->channel, p*10+250);
}

int main()
{
    Servo s1;
    ServoInit(&s1, 20, true);
    ServoOn(&s1);

    while (true)
    {
        ServoPosition(&s1, 0);
        sleep_ms(9000);
        for (int i = 0; i < 101; i += 10)
        {
            ServoPosition(&s1, i);
            sleep_ms(800);
        }
        
        for (int i = 0; i < 101; i += 5)
        {
            ServoPosition(&s1, i);
            sleep_ms(500);
        }
        
        for (int i = 0; i < 101; i += 3)
        {
            ServoPosition(&s1, i);
            sleep_ms(400);
        }


        for (int i = 0; i < 101; i += 1)
        {
            ServoPosition(&s1, i);
            sleep_ms(300);
        }

        
    }

    return 0;
}