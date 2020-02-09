/*
	Version 00.00.00
	Datum	12.07.2019
	Autor 	Alex Krieg
*/

#ifndef PID_H
#define PID_H


#define __SPEED_NORMAL 0
#define __SPEED_FAST 1

#include <chrono>
using namespace std::chrono;



class PID
{
	public:
		PID(); // Presets the pid values
        PID(double kp,double ki,double kd);
		~PID();
		
		void update();
        void input(double input);
        void expected(double expected);
        double output();
        double error();
		
        void kp(double kp);					// cosntant for the proportional factor
        void ki(double ki);					// constant for the increment factor
        void kd(double kd);					// constant for the diferential factor
		
        void maximum(double max);
        void minimum(double min);
		
        double kp();
        double ki();
        double kd();
		
        void I(double _ki);
		
        double P();
        double I();
        double D();
		
		
        double maximum();
        double minimum();
		
		void reset();
		void setSpeed(bool speed);	
		
		void enable_P();
		void enable_I();
		void enable_D();
		
		void disable_P();
		void disable_I();
		void disable_D();
		
		
		unsigned int elapsedTime(){
			return _elapsedTime;
		}
	private:
		void setup();
	
        double _kp;
        double _ki;
        double _kd;
		
        double _pid_P;
        double _pid_I;
        double _pid_D;
		
		
		
        double _input;
        double _expected;
        double _output;
        double _error;
        double _lastError;
        double _max;				// If value is 0 -> no maximum
        double _min;				// If value is 0 -> no minimum
		
		
		
		bool _speedModus;		// Use millis() or micros()
        int _enable_P_I_D;		// Bit 0 -> P Bit 1 -> I Bit 2 -> D
		
		unsigned long _lastTime;
		unsigned long _elapsedTime;

        // Use auto keyword to avoid typing long
        // type definitions to get the timepoint
        // at this instant use function now()
        std::chrono::high_resolution_clock::time_point __debug_timer1;
        std::chrono::high_resolution_clock::time_point __debug_timer2;
        std::chrono::duration<double> __debug_time_span;
        double __debug_run_time;

};
#endif
