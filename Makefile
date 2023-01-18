all:
	make exec
	make ide
	make install
	make run
	
exec:
	gcc -g -Wall src/exec.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o build/exec
	
ide:
	gcc -g -Wall src/ide.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o build/ide

install:
	cp build/* ~/.pictrel/code/

run:
	./build/ide
