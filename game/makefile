
.PHONY: all

all: game_exec.gb

game_exec.gb: game.o main.o
	rgblink --dmg --tiny --map game.map --sym game.sym -o game_exec.gb main.o game.o
	rgbfix -v -p 0xFF game_exec.gb

game.o: game.asm hardware.inc chef.inc customers.inc utils.inc *.tlm *.chr
	rgbasm -o game.o game.asm

main.o: main.asm hardware.inc *.tlm *.chr
	rgbasm -o main.o main.asm