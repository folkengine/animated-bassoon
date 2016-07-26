all: ebin/roman.c
	erl -make

ebin/roman.c: src/roman.c
	cp $< $@
