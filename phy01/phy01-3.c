#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



const int gk_digits_count = 10;
const int gk_letters_count = 26;

int get_index_by_symbol(char symbol) {
	int index = 0;
	if (symbol <= '9')
		index = symbol - '0';
	else if (symbol <= 'Z')
		index = symbol - 'A' + gk_digits_count;
	else
		index = symbol - 'a' + gk_digits_count + gk_letters_count;
	
	return index;
}

char get_symbol_by_index(int index) {
	char symbol = 0;
	if (index < gk_digits_count)
		symbol = index + '0';
	else if (index < gk_digits_count + gk_letters_count)
		symbol = index + 'A' - gk_digits_count;
	else
		symbol = index + 'a' - gk_digits_count - gk_letters_count;

	return symbol;
}

void print_set(uint64_t set) {
	for (int i = 0; i < gk_digits_count + 2 * gk_letters_count; ++i)
		if ((set >> i) & 1)
			printf("%c", get_symbol_by_index(i));
}

int main(int argc, char* argv[]) {
	uint64_t current_set = 0;
	uint64_t next_set = 0;
	char input = 0;
	while((input = getchar()) != -1) {
		if (input == '~') {
			current_set = ~current_set;
			next_set = 0;
		}
		else if (input == '&') {
			current_set &= next_set;
			next_set = 0;
		}
		else if (input == '^') {
			current_set ^= next_set;
			next_set = 0;
		}
		else if (input == '|') {
			current_set |= next_set;
			next_set = 0;
		}
		else
			next_set |= (uint64_t)1 << get_index_by_symbol(input);
	}
	print_set(current_set);	
	return 0;
}

