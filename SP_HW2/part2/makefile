test: shell.c builtin.o is_background.o parse.o run_command.o redirect_out.o redirect_in.o pipe_present.o pipe_command.o shell.h
	gcc -o shell builtin.o is_background.o parse.o run_command.o redirect_out.o redirect_in.o pipe_present.o pipe_command.o shell.c

pipePos: pipe_present.o
	gcc -o pipe pipe_present.c

play: play.o
	gcc -o play play.c
