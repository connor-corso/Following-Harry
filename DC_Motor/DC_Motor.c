#include "pico/stdlib.h";
#include "hardware/pwm.h";

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
    uint chan;
    uint speed;
    uint freq;
    uint resolution;
    bool on;
} Motor;

void motorInit(Motor *m, uint gpio, uint freq)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    m->gpio = gpio;
    m->slice = pwm_gpio_to_slice_num(gpio);
    m->chan = pwm_gpio_to_channel(gpio);
    m->speed = 0;
    m->freq = freq;
    m->resolution = pwm_set_freq_duty(m->slice, m->chan, m->freq, m->speed);

    m->on = false;
}

void motorspeed(Motor *m, int s)
{
    pwm_set_duty
}

int main()
{

    return 0;
}
