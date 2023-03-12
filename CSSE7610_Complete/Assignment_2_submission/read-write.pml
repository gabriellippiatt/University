/* Assignment 2 - Question 2*/
/* Gabriel Lippiatt - 45296710*/
/*
	Comment:
	To avoid multiple critical references, temporary variables were used
	I ran the Automata and varified the states were correct.
*/

byte monitorCount = 0;
byte c = 0;
byte x1 = 0;
byte x2 = 0;
bool lock = false;

inline enterMon() {
    atomic {
        !lock;
        lock = true;
        monitorCount++;
    }
}
inline leaveMon() {
    atomic { 
        lock = false;
        monitorCount--;
    }
}
typedef Condition {
    bool gate;
    byte waiting;
}
Condition OktoWrite;
#define emptyC(OktoWrite) (OktoWrite.waiting == 0)
inline waitC(C) {
    atomic {
        C.waiting++;
        leaveMon();
        C.gate;
        C.gate = false;
        C.waiting--;
    }
}
inline signalC(C) {
    atomic {
        if 
        :: (C.waiting > 0) ->
            C.gate = true;
            enterMon();
        :: else
        fi;
    }
}
active proctype p() {	// reader
	do 
		:: 	true;
		do :: true ->
			do :: true ->
				byte c0 = c;
				if
					:: !(c0 % 2 == 0);
					:: else ->
						break;
				fi;
			od
		byte d1 = x1;
		byte d2 = x2;
		if
			:: !(c0 == c);
			:: else ->
				break;
		fi;
		od
	od
}
active proctype q() {	// writer
	do 
		:: 	true;
		byte d1 = 1;
		byte d2 = 2;
		byte temp = c;
		c = temp + 1;
		waitC(OktoWrite);
		x1 = d1;
		x2 = d2;
		temp = c;
		c = temp -1;
		signalC(OktoWrite);
	od
}
active proctype k() {	// incrementer
	do 
		:: 	true;
		do :: true ->
			do :: true ->
				byte c0 = c;
				if
					:: !(c0 % 2 == 0);
					:: else ->
						break;
				fi;
			od
		byte d1 = x1;
		byte d2 = x2;
		if
			:: !(c0 == c);
			:: else ->
				break;
		fi;
		od
	od
	byte temp = c;
	waitC(OktoWrite);
	c = temp + 1;
	x1 = d1+1;
	x2 = d2+1;
	temp = c;
	c = temp -1;
	signalC(OktoWrite);
}