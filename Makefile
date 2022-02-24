CXX = g++-5 
CXXFLAGS = -std=c++14 -Wall -Werror=vla -g
LDFLAGS = -lX11
EXEC = chess
OBJECTS = main.o chess.o chess_piece_creator.o human_player.o  chess_AI.o chess_text_monitor.o chess_piece.o chess_instruction.o window.o chess_graphics_monitor.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${LDFLAGS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC}
