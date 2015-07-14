
all: clean soup

clean:
	rm -rf soup

soup: soup.cpp
	$(CC) -lstdc++ -o soup soup.cpp
	
