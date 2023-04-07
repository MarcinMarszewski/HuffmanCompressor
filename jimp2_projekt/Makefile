compile:
	cc src/main.c src/key.c src/node.c src/fileWriter.c src/fileReader.c src/treeWriter.c src/compress.c src/decompress.c

compress12:
	./a.out -f data/simple.txt -s data/simple.zip -o 12

compress8:
	./a.out -f data/simple.txt -s data/simple.zip -p haslo

decompress:
	./a.out -f data/simple.zip -s data/simple.dec -p haslo

compare:
	diff data/simple.txt data/simple.dec

test12:
	make compile
	make compress12
	make decompress
	make compare

test8:
	make compile
	make compress8
	make decompress
	make compare

testBig8:
	make compile
	./a.out -f data/infile.txt -s data/infile.zip
	./a.out -f data/infile.zip -s data/infile.dec
	diff data/infile.txt data/infile.dec
