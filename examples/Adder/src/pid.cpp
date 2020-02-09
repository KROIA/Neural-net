/*
	Version 00.00.00
	Datum	12.07.2019
	Autor 	Alex Krieg
*/

#include "pid.h"

PID::PID()
{
	this->kp(0.1);
	this->ki(0.01);
	this->kd(0.1);
	this->setup();
}
PID::PID(double kp,double ki,double kd)
{
	this->kp(kp);
	this->ki(ki);
	this->kd(kd);
	this->setup();
}
PID::~PID()
{
	
}
void PID::setup()
{
	this->setSpeed(__SPEED_NORMAL);
    _enable_P_I_D = 0x7;
	this->input(0);
	this->expected(0);
    this->maximum(10);
    this->minimum(-10);
	this->reset();
}

void PID::update()
{
	// reading the elapsed time for the "d" -value
	//unsigned long _elapsedTime;
    __debug_timer2 = __debug_timer1;
    __debug_timer1 = std::chrono::high_resolution_clock::now();
    __debug_time_span = std::chrono::duration_cast<std::chrono::microseconds>(__debug_timer2 - __debug_timer1);
    __debug_run_time = 1;//__debug_time_span.count()*1000;
	_output = 0;
    if(__debug_run_time == 0){_elapsedTime = 1;}
    _elapsedTime = __debug_run_time;

	

	
	// calculating the error of the system
    _error =  _expected - _input;
	_pid_P = 0;
	_pid_D = 0;
	
    if((_enable_P_I_D & 0x1) != 0)
	{
		_pid_P = _kp * _error;
		_output += _pid_P;
	}
	
	// incrementing the "i" proportion
    if((_enable_P_I_D & 0x2) != 0)
	{
		_pid_I = _pid_I + _ki * _error;
		_output += _pid_I;
	}
	
	// calculate the "p" and "d" proportion and adding them all up including the "i"
    if((_enable_P_I_D & 0x4) != 0)
	{
        //double time = (double)_elapsedTime;
		if(_speedModus == __SPEED_FAST)
		{
            __debug_run_time /=1000;
		}
        _pid_D += _kd*((_error - _lastError)/__debug_run_time);
		_output += _pid_D;
	}
	
	// check for min/max boundry. If there is any, the increment value will be decresed 
	// to prevent an overflow. Output will by set to the boundry value.
	if(_min != 0 && _output < _min)
	{
		_pid_I -= _output - _min;
		_output = _min;
	}else if(_max != 0 && _output > _max)
	{
		_pid_I -= _output - _max;
		_output = _max;
	}
	_lastError = _error;
}
void PID::input(double input)
{
	_input = input;
}
void PID::expected(double expected)
{
	_expected = expected;
}
double PID::output()
{
	return _output;
}
double PID::error()
{
	return _error;
}

void PID::kp(double kp)
{
	_kp = kp;
}
void PID::ki(double ki)
{
	_ki = ki;
}
void PID::kd(double kd)
{
	_kd = kd;
}
void PID::I(double i)
{
	_pid_I = i;
}
double PID::P()
{
	return _pid_P;
}
double PID::I()
{
	return _pid_I;
}
double PID::D()
{
	return _pid_D;
}
void PID::maximum(double max)
{
	// max value can't be smaller than the min value
	if(max < _min)
		max = _min;
	_max = max;
}
void PID::minimum(double min)
{
	// min value can't be larger than the max value
	if(min > _max)
		min = _max;
	_min = min;
}

double PID::kp()
{
	return _kp;
}
double PID::ki()
{
	return _ki;
}
double PID::kd()
{
	return _kd;
}
double PID::maximum()
{
	return _max;
}
double PID::minimum()
{
	return _min;
}
void PID::reset()
{
	_pid_I = 0;
	_lastTime = 0;
	_lastError = 0;
	_output = 0;
}
void PID::setSpeed(bool speed)
{
	_speedModus = speed;
}
void PID::enable_P()
{
    _enable_P_I_D |= 0x1;
}
void PID::enable_I()
{
    _enable_P_I_D |= 0x2;
}
void PID::enable_D()
{
    _enable_P_I_D |= 0x4;
}
void PID::disable_P()
{
    _enable_P_I_D &= 0x6;
}
void PID::disable_I()
{
    _enable_P_I_D &= 0x5;
}
void PID::disable_D()
{
    _enable_P_I_D &= 0x3;
}
