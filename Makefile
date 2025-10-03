calc:
	gcc src/main.c -lm -o stringcalc
jit:
	gcc src/main.c src/rawtui.c -lm -D JITCALC -o stringcalc
debug:
	gcc src/main.c -lm -g -o stringcalc-g
