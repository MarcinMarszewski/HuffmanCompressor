compile:
	cc src/main.c src/key.c src/node.c src/fileWriter.c src/fileReader.c src/treeWriter.c src/compress.c src/decompress.c -o compilex

compress16:
	./compilex -f data/simple.txt -s data/simple.zip -o 16

compress12:
	./compilex -f data/simple.txt -s data/simple.zip -o 12

compress8:
	./compilex -f data/simple.txt -s data/simple.zip

decompress:
	./compilex -f data/simple.zip -s data/simple.dec

compare:
	diff data/simple.txt data/simple.dec

test16:
	make compile
	make compress16
	make decompress
	make compare

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
	./compilex -f data/infile.txt -s data/infile.zip
	./compilex -f data/infile.zip -s data/infile.dec
	diff data/infile.txt data/infile.dec
