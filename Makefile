run:
	g++ main.cpp -Wall -Wextra -pedantic -lGL -lGLEW -lsfml-graphics -lsfml-system -lsfml-window -o stasio
	./stasio

windows:
	x86_64-w64-mingw32-g++-posix main.cpp -I glew-2.1.0/include -I glm/ -I sfml/include -L glew-2.1.0/lib/Release/x64/ -lglew32 -L sfml/lib -l sfml-system -l sfml-graphics -l sfml-window -l opengl32