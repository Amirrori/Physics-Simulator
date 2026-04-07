import subprocess

subprocess.run(
    "clang main.c -o main $(pkg-config --cflags --libs raylib) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo && ./main",
    check=True,
    shell=True
)