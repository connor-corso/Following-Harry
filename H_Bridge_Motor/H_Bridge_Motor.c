#include "pico/stdlib.h";
#include "hardware/pwm.h";

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
    uint gpioForward;
    uint gpioReverse;
    uint gpioEnable;
    uint slice;
    uint Fchan;
    uint Rchan;
    bool forward;
    uint speed;
    uint freq;
    uint resolution;
    bool on;
} BiMotor;


void BiMotorInit(BiMotor *m, uint gpioForward, uint gpioReverse, uint freq, uint gpioEnable)
{
    // setup the gpio pins to drive the L293D through a transistor
    gpio_set_function(gpioForward, GPIO_FUNC_PWM);
    gpio_set_function(gpioReverse, GPIO_FUNC_PWM);
    
    // setup the gpio pins for the enable toggle on the L293D
    gpio_init(gpioEnable);
    gpio_set_dir(gpioEnable, GPIO_OUT);
    
    // save the gpio pins
    m->gpioForward = gpioForward;
    m->gpioReverse = gpioReverse;
    m->gpioEnable = gpioEnable;
    
    // if both of the gpio lines arent on the same slice of a pwm driver throw an error
    if (pwm_gpio_to_slice_num(gpioForward) != pwm_gpio_to_slice_num(gpioReverse))
    {
        return 1;
    }
    
    // save all of the slice and channel stuff
    m->slice = pwm_gpio_to_slice_num(gpioForward);

    m->Fchan = pwm_gpio_to_channel(gpioForward);
    m->Rchan = pwm_gpio_to_channel(gpioReverse);

    m->speed = 0;
    m->freq = 0;
    m->resolution = pwm_set_freq_duty(m->slice, m->Fchan, m->freq, m->speed);

    m->forward = true;
    m->on = false;
}

// sets the motor speed and direction
void BiMotorspeed(BiMotor *m, int s, bool forward)
{
    if (forward)
    {
        pwm_set_duty(m->slice, m->Rchan, 0);
        pwm_set_duty(m->slice, m->Fchan, s);
        m->forward = true;
    }
    else
    {
        pwm_set_duty(m->slice, m->Fchan, 0);
        pwm_set_duty(m->slice, m->Rchan, s);
        m->forward = false;
    }
    m->speed = s;
}

// turns on the pwm and sets the enable line to high
void BiMotorOn(BiMotor *m)
{
    gpio_put(m->gpioEnable, 1);
    pwm_set_enabled(m->slice, true);
    m->on = true;
}

//turns off the pwm and sets the enable line to low
void BiMotorOff(BiMotor *m)
{
    gpio_put(m->gpioEnable, 0);
    pwm_set_enabled(m->slice, false);
    m->on = false;
}

int main()
{
    BiMotor mot1;
    BiMotorInit(&mot1, 20, 21, 2000, 15);

    BiMotorOn(&mot1);
    while (true)
    {
        BiMotorspeed(&mot1, 50, true);
        sleep_ms(2000);
        BiMotorspeed(&mot1, 75, false);
        sleep_ms(2000);
        
        BiMotorspeed(&mot1, 100, true);
        sleep_ms(1000);
        BiMotorspeed(&mot1, 75, false);
        sleep_ms(500);

        BiMotorOff(&mot1);
        sleep_ms(3000);
        BiMotorOn(&mot1);
    }

    return 0;
}