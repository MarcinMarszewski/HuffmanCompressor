compile:
	@cc src/main.c src/key.c src/node.c src/fileWriter.c src/fileReader.c src/treeWriter.c src/compress.c src/decompress.c -o compilex

testAll: data/big.txt data/plain.txt data/toSmall.txt data/painting.bmp data/duck.png data/Compilex.png verbose badPassword

data/big.txt data/plain.txt data/toSmall.txt data/painting.bmp data/duck.png data/Compilex.png: compile
	@./compilex -f $@ -s data/file.squish -o 8
	@./compilex -f data/file.squish -s data/file.decomp
	@diff $@ data/file.decomp
	@./compilex -f $@ -s data/file.squish -o 12
	@./compilex -f data/file.squish -s data/file.decomp
	@diff $@ data/file.decomp
	@./compilex -f $@ -s data/file.squish -o 16
	@./compilex -f data/file.squish -s data/file.decomp
	@diff $@ data/file.decomp
	@./compilex -f $@ -s data/file.squish -o 8 -p hasloabc
	@./compilex -f data/file.squish -s data/file.decomp -p hasloabc
	@diff $@ data/file.decomp
	@./compilex -f $@ -s data/file.squish -o 12 -p haslo123
	@./compilex -f data/file.squish -s data/file.decomp -p haslo123
	@diff $@ data/file.decomp
	@./compilex -f $@ -s data/file.squish -o 16 -p hasloqwe
	@./compilex -f data/file.squish -s data/file.decomp -p hasloqwe
	@diff $@ data/file.decomp

verbose: compile
	@./compilex -f data/plain.txt -s data/file.squish -o 8 -v
	@./compilex -f data/file.squish -s data/file.decomp -v
	@diff data/plain.txt data/file.decomp

badPassword: compile
	@./compilex -f data/plain.txt -s data/file.squish -o 8 -p haslo	
	@./compilex -f data/file.squish -s data/file.decomp -p zlehaslo
	@diff data/plain.txt data/file.decomp
