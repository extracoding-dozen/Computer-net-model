build_asan:
	cc -c lib/pairs/pairs.c -Wall -Wextra -fsanitize=address -o pairs.o -g3 
	cc -c lib/algorythm.c -Wall -Wextra -fsanitize=address -o algorythm.o -lreadline -g3
	cc -c lib/queue/queue.c -Wall -Wextra -fsanitize=address -o queue.o -g3
	cc -c main.c -Wall -Wextra -fsanitize=address -o main.o -g3
	cc *.o -fsanitize=address -lreadline -o bin_asan -g3
	rm *.o

build_asan_addit:
	cc -c lib/pairs/pairs.c -Wall -Wextra -fsanitize=address -o pairs.o -g3 
	cc -c lib/algorythm.c -Wall -Wextra -fsanitize=address -o algorythm.o -lreadline -g3
	cc -c lib/queue/queue.c -Wall -Wextra -fsanitize=address -o queue.o -g3
	cc -c main.c -Wall -Wextra -fsanitize=address -o main.o -g3 -D ADDITIONAL
	cc *.o -fsanitize=address -lreadline -o bin_asan -g3
	rm *.o