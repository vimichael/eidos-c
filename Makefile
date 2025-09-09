default:
	clang main.c -o build/main -lX11 -lXi -lXcursor -lGL -ldl -lm

run:
	cd build && prime-run ./main

shader:
	./sokol-shdc --input assets/shaders/triangle.glsl --output triangle_shader.h --slang glsl430:hlsl5:metal_macos

