
all: clean soup

clean:
	rm -rf soup

soup: soup.cpp
	$(CC) -g -lstdc++ -o soup soup.cpp
	
