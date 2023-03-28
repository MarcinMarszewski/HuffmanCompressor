compile:
	cc src/main.c src/key.c src/node.c src/fileWriter.c src/fileReader.c src/treeWriter.c src/compress.c src/decompress.c

compress:
	./a.out data/simple.txt data/squashed_simple.txt 0

decompress:
	./a.out data/squashed_simple.txt data/decompressed_simple.txt 1

all:
	make compile
	make compress
	make decompress
