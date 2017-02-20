all:
	gcc utils.c shell.c -o shell

clean:
	rm shell
