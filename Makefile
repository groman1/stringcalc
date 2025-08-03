calc:
	gcc src/main.c -lm -o stringcalc

debug:
	gcc src/main.c -lm -g -o stringcalc-g
