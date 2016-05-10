#include "Arduino.h"
#include "asyncDelay.h"

typedef void (*GeneralFunction) ();

asyncDelay::asyncDelay(int refresh){
	_old = millis();
	_refresh =  refresh;
}

void asyncDelay::runJob(GeneralFunction f){	
	if(millis()>_old){
		_old = millis()+_refresh;
		f();
	}
}