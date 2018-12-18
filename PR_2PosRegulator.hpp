#pragma once

#include <arduino.h>

class PR_2PosRegulator {
	public:
		
		PR_2PosRegulator(const float hysteresysL, const float hysteresysH, const bool isDirectLogic, const uint32_t minPeriodSwitch = 0);
		
		void		setup(const float hysteresysL, const float hysteresysH, const bool isDirectLogic, const uint32_t minPeriodSwitch = 0);
		bool		loop(const float targetVal, const float currValue);
		bool		loop(const float currValue);
		
		bool		get();
		void		setTarget(const float targetVal);
		float		getTarget();
		
		void		setMode(const bool mode);				// on/off the regulator
		bool		isOn();
		bool		getMode();
	
	protected:
		float		_hysteresysH;
		float		_hysteresysL;
		uint32_t	_minPeriodSwitch;				//[ms]
		uint32_t	_lastSwitch 		= 0;
		bool		_isDirectLogic;
		float		_targetValue			= 0;
		bool		_out 				= false;
		
		bool		_isOnMode			= false;
};


PR_2PosRegulator::PR_2PosRegulator(const float hysteresysL, const float hysteresysH, const bool isDirectLogic, const uint32_t minPeriodSwitch) {
	
	setup(hysteresysL, hysteresysH, isDirectLogic, minPeriodSwitch); 
}	

//@	output logic is direct
//@              <---------|---------
//@	             |         |
//@	             |         |
//@	-------------|----+--->^
//@           hystL   |	  hystH                
//@                 target
//@	DirectLogic means if target > current then output is high. Example is heater 
//@ DirectLogic means if target < current then output is high. Example is cooler 

void	PR_2PosRegulator::setup(const float hysteresysL, const float hysteresysH, const bool isDirectLogic, const uint32_t minPeriodSwitch) {
	
	_hysteresysL = -hysteresysL;
	_hysteresysH = hysteresysH;
	_isDirectLogic	 = isDirectLogic;
	_minPeriodSwitch = minPeriodSwitch;
}

bool	PR_2PosRegulator::loop(const float targetVal, const float currValue) {
	
	setTarget(targetVal);
	return loop( currValue );
}

bool	PR_2PosRegulator::loop(const float currValue) {
	
	if (!_isOnMode)	{
		_out = false;							// if mode is "off" set out as "low"
		return _out; 		
	}
	
	if ( PR_getTauMS(_lastSwitch) < _minPeriodSwitch ) return _out;
	
	float err = _targetValue - currValue;
	
	if (err < _hysteresysL) {
		_out = !_isDirectLogic;			// 
		_lastSwitch = millis();
	}
	else if (err > _hysteresysH) {
		_out = _isDirectLogic; 			// 
		_lastSwitch = millis();
	}
	
	return _out;
}

void	PR_2PosRegulator::setTarget(const float targetVal) {
	_targetValue = targetVal;
}

bool	PR_2PosRegulator::get() 		{	return _out;		}
float	PR_2PosRegulator::getTarget()	{	return _targetValue;	}

void	PR_2PosRegulator::setMode(const bool mode)	{
	_isOnMode = mode;
}				
bool	PR_2PosRegulator::isOn()		{	return _isOnMode;	}
bool	PR_2PosRegulator::getMode()		{	return _isOnMode;	}


