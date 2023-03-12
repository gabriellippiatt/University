/* Assignment 1 - Question 2*/
/* Gabriel Lippiatt - 45296710*/

/*
	Comment:
	To avoid multiple critical references in p2, p3, p4, q2, and q3,
	I introduced local temporary variable (tempIn and tempOut).
	I ran the Automata and varified the states were correct.
	I also used assertions for criticalP and criticalQ to confirm mutual exclution.
	I finally used claim varification using nostarve and mutex to confirm freedom from starvation.
*/

byte  in = 0;
byte out = 0;
byte N = 20;
byte d = 0;
byte buffer[N];
byte criticalP = 0;
byte criticalQ = 0;
bool PinCS=false;

ltl nostarve {[]<> PinCS}
ltl mutex {[] (criticalP <=1)}

active proctype p() {
	do 
		:: 	true;
	d=1;
	byte tempIn = in
	out != (tempIn+1)%N;
	criticalP++;
	PinCS=true;
	buffer[tempIn] = d;
	/*assert(criticalP==1);*/
	PinCS=false;
	in = (tempIn+1)%N;
	criticalP--;
	od
}

active proctype q() {
	do 
		:: 	true;
		in!=out;
	criticalQ++;
	/* assert(criticalQ==1);*/
	byte tempOut = out;
	d = buffer[tempOut];
	out=(tempOut+1)%N;
	criticalQ--;
	d=0;
	od
}