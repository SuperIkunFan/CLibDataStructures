Target := Lqr
CC := gcc

path := $(shell pwd)


source := ./Rb_Tree.o




${Target}: ${source}
	${CC} ${source} $(ccflags) -g -o ${Target} ${ldFlags}

%.o:%.c
	${CC} ${ldFlags} ${ccflags} $< -c -g -o $@

.PHONY: clean Test
clean:
	rm ./*.o
	rm ./${Target}
Test:
	valgrind --track-origins=yes --tool=memcheck --leak-check=full --show-leak-kinds=all ./${Target}