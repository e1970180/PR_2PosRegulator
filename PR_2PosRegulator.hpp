#pragma once

#include <arduino.h>

class PR_2PosRegulator {
	public:
		
		PR_2PosRegulator(const float hysteresysL, const float hysteresysH, const bool isInverse, const uint32_t minPeriodSwitch = 0);
		
		void		setup(const float hysteresysL, const float hysteresysH, const bool isInverse, const uint32_t minPeriodSwitch = 0);
		bool		loop(const float targetVal, const float currValue);
		bool		loop(const float currValue);
		
		bool		get();
		void		setTarget(const float targetVal);
		float		getTarget();
		
		//void		setMode(); toDO 
		//bool		isOn(); return _mode;
	
	protected:
		float		_hysteresysH;
		float		_hysteresysL;
		uint32_t	_minPeriodSwitch;				//[ms]
		uint32_t	_lastSwitch 		= 0;
		bool		_isInverseLogic;
		float		_targetVal			= 0;
		bool		_out 				= false;
		
		bool		_mode;
};


PR_2PosRegulator::PR_2PosRegulator(const float hysteresysL, const float hysteresysH, const bool isInverse, const uint32_t minPeriodSwitch) {
	
	setup(hysteresysL, hysteresysH, isInverse, minPeriodSwitch); 
}	

	
void	PR_2PosRegulator::setup(const float hysteresysL, const float hysteresysH, const bool isInverse, const uint32_t minPeriodSwitch) {
	
	_hysteresysL = -hysteresysL;
	_hysteresysH = hysteresysH;
	_isInverseLogic	 = isInverse;
	_minPeriodSwitch = minPeriodSwitch;
}

bool	PR_2PosRegulator::loop(const float targetVal, const float currValue) {
	
	setTarget(targetVal);
	return loop( currValue );
}

bool	PR_2PosRegulator::loop(const float currValue) {
	
	if ( PR_getTauMS(_lastSwitch) < _minPeriodSwitch ) return _out;
	
	float err = _targetVal - currValue;
	
	if (err < _hysteresysL) {
		_out = _isInverseLogic;			// = (false ^ _isInverseLogic)
		_lastSwitch = millis();
	}
	else if (err > _hysteresysH) {
		_out = !_isInverseLogic; 		// = (true  ^ _isInverseLogic)
		_lastSwitch = millis();
	}
	
	return _out;
}

void	PR_2PosRegulator::setTarget(const float targetVal) {
	_targetVal = targetVal;
}

bool	PR_2PosRegulator::get() 		{	return _out;		}
float	PR_2PosRegulator::getTarget()	{	return _targetVal;	}