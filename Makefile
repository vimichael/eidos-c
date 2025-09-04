default:
	clang main.c -o build/main -lX11 -lXi -lXcursor -lGL -ldl -lm

run:
	cd build && prime-run ./main
