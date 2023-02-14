#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "address_book.c"

#define PATH_BUFFER_SIZE 512
#define FILE_NAME "addresses.csv"

struct AddressBook populate_from_file();
unsigned int get_selection_number(unsigned int min, unsigned int max);
void add_addr_to_end(struct AddressBook *addresses);
void insert_addr(struct AddressBook *addresses);
void delete_addr(struct AddressBook *addresses);
void display_addr(const struct AddressBook *addresses);
void find_by_name(const struct AddressBook *addresses);
void find_by_surname(const struct AddressBook *addresses);
void find_by_email(const struct AddressBook *addresses);
void find_by_phone(const struct AddressBook *addresses);
struct Address create_address();

int main(void)
{
	struct AddressBook addr_book = populate_from_file();
	int retval = 0;

	while (true) {
		puts("Select an action by number:\n"
		     "1) Display address book\n"
		     "2) Add new address to the end\n"
		     "3) Add new address to specific position\n"
		     "4) Delete address\n"
		     "5) Display address in specified position\n"
		     "6) Find address by name\n"
		     "7) Find address by surname\n"
		     "8) Find address by email\n"
		     "9) Find address by phone number\n"
		     "10) Quit");

		unsigned int selected = get_selection_number(1, 10);
		switch (selected) {
		case 1:
			display_addresses(&addr_book);
			break;
		case 2:
			break;
		case 10:
			goto end;
		default:
			puts("Unreachable code entered");
			retval = 1;
			goto end;
		}
	}

end:
	delete_address_book(&addr_book);

	return retval;
}

struct AddressBook populate_from_file()
{
	char *home_dir = getenv("HOME");
	char file_path[PATH_BUFFER_SIZE];
	snprintf(file_path, PATH_BUFFER_SIZE, "%s/%s", home_dir, FILE_NAME);

	struct AddressBook addr_book = create_address_book();

	FILE *file = fopen(file_path, "r");
	if (file == NULL) {
		// Allocate only in this special case to avoid polluting the stack.
		char *error = malloc((PATH_BUFFER_SIZE + 22) * sizeof(*error));
		snprintf(error, PATH_BUFFER_SIZE + 22,
			 "Failed to open file '%s'", file_path);
		perror(error);
		free(error);
	} else {
		addr_book = read_addresses(file);
		fclose(file);
	}

	return addr_book;
}

char *read_string()
{
	char buffer[128];
	fgets(buffer, 128, stdin);
	size_t len = strlen(buffer);
	if (buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	}
	return strdup(buffer);
}

struct Address create_address()
{
	puts("Creating an address.");
	puts("Enter name:");
	char *name = read_string();
	puts("Enter surname:");
	char *surname = read_string();
	puts("Enter email:");
	char *email = read_string();
	puts("Enter phone number:");
	char *phone = read_string();

	struct Address addr = {
		.name = name, .surname = surname, .email = email, .phone = phone
	};
	return addr;
}

unsigned int get_selection_number(unsigned int min, unsigned int max)
{
	unsigned int selected = 0;

	do {
		if (scanf("%u", &selected) != 1) {
			puts("Please enter a positive number");
		} else if (selected < min || selected > max) {
			printf("Invalid choice. Enter a number between %u and %u\n",
			       min, max);
		} else {
			printf("Selected: %u\n", selected);
			return selected;
		}
	} while (true);
}

size_t get_index(size_t min, size_t max)
{
	size_t index = 0;
	do {
		printf("Enter a number between %zu and %zu:\n", min, max);

		if (scanf("%zu\n", &index) != 1) {
			puts("Please enter a positive number");
		} else if (index < min || index > max) {
			puts("Invalid number");
		} else {
			return index;
		}
	} while (true);
}

void add_addr_to_end(struct AddressBook *addresses)
{
	struct Address addr = create_address();
	add_address_to_end(addresses, addr);
}

void insert_addr(struct AddressBook *addresses)
{
	struct Address addr = create_address();
	size_t index = get_index(0, addresses->size);
	add_address(addresses, index, addr);
}

void delete_addr(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	size_t index = get_index(0, addresses->size - 1);
	delete_address(addresses, index);
}

void display_addr(const struct AddressBook *addresses);
void find_by_name(const struct AddressBook *addresses);
void find_by_surname(const struct AddressBook *addresses);
void find_by_email(const struct AddressBook *addresses);
void find_by_phone(const struct AddressBook *addresses);
