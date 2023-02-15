#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "address_book.c"

#define PATH_BUFFER_SIZE 512
#define FILE_NAME "addresses.csv"

void get_file_path(char *buffer, size_t buf_size);
void save_to_file(const struct AddressBook *addresses);
struct AddressBook populate_from_file();
unsigned int get_selection_number(unsigned int min, unsigned int max);
void read_string(char *buffer, size_t buf_size);

void add_addr_to_end(struct AddressBook *addresses);
void insert_addr(struct AddressBook *addresses);
void delete_addr(struct AddressBook *addresses);
void display_addr(const struct Address *addr);
void display_specific_addr(struct AddressBook *addresses);
void find_by_name(struct AddressBook *addresses);
void find_by_surname(struct AddressBook *addresses);
void find_by_email(struct AddressBook *addresses);
void find_by_phone(struct AddressBook *addresses);
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
			add_addr_to_end(&addr_book);
			break;
		case 3:
			insert_addr(&addr_book);
			break;
		case 4:
			delete_addr(&addr_book);
			break;
		case 5:
			display_specific_addr(&addr_book);
			break;
		case 6:
			find_by_name(&addr_book);
			break;
		case 7:
			find_by_surname(&addr_book);
			break;
		case 8:
			find_by_email(&addr_book);
			break;
		case 9:
			find_by_phone(&addr_book);
			break;
		case 10:
			goto end;
		default:
			puts("Unreachable code entered");
			retval = 1;
			goto end;
		}

		// Leave an empty line, it looks better.
		puts("");
	}

end:
	save_to_file(&addr_book);
	delete_address_book(&addr_book);

	return retval;
}

void get_file_path(char *buffer, size_t buf_size)
{
	char *home_dir = getenv("HOME");
	snprintf(buffer, buf_size, "%s/%s", home_dir, FILE_NAME);
}

struct AddressBook populate_from_file()
{
	char file_path[PATH_BUFFER_SIZE];
	get_file_path(file_path, sizeof(file_path));
	struct AddressBook addr_book = create_address_book();

	FILE *file = fopen(file_path, "r");
	if (file == NULL) {
		// Allocate only in this special case to avoid polluting the stack.
		char *error = malloc((sizeof(file_path) + 50) * sizeof(*error));
		sprintf(error, "Failed to open file '%s' for reading",
			file_path);
		perror(error);
		free(error);
		return addr_book;
	}
	addr_book = read_addresses(file);
	fclose(file);

	return addr_book;
}

void save_to_file(const struct AddressBook *addresses)
{
	char file_path[PATH_BUFFER_SIZE];
	get_file_path(file_path, sizeof(file_path));
	FILE *file = fopen(file_path, "w");
	if (file == NULL) {
		// Allocate only in this special case to avoid polluting the stack.
		char *error = malloc((sizeof(file_path) + 50) * sizeof(*error));
		sprintf(error, "Failed to open file '%s' for writing",
			file_path);
		perror(error);
		free(error);
		return;
	}
	save_addresses_to_file(addresses, file);
	fclose(file);
}

void read_string(char *buffer, size_t buf_size)
{
	fgets(buffer, (int)buf_size, stdin);
	size_t len = strlen(buffer);
	if (buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	}
}

struct Address create_address()
{
	char buffer[128];
	puts("Creating an address.");
	puts("Enter name:");
	read_string(buffer, sizeof(buffer));
	char *name = strdup(buffer);
	puts("Enter surname:");
	read_string(buffer, sizeof(buffer));
	char *surname = strdup(buffer);
	puts("Enter email:");
	read_string(buffer, sizeof(buffer));
	char *email = strdup(buffer);
	puts("Enter phone number:");
	read_string(buffer, sizeof(buffer));
	char *phone = strdup(buffer);

	struct Address addr = {
		.name = name, .surname = surname, .email = email, .phone = phone
	};
	return addr;
}

void discard_stdin_until_newline()
{
	while (getchar() != '\n') {
	}
}

unsigned int get_selection_number(unsigned int min, unsigned int max)
{
	unsigned int selected = 0;

	do {
		int matched = scanf("%u", &selected);
		discard_stdin_until_newline();
		if (matched != 1) {
			puts("Please enter a positive number");
		} else if (selected < min || selected > max) {
			printf("Invalid choice. Enter a number between %u and %u\n",
			       min, max);
		} else {
			return selected;
		}
	} while (true);
}

size_t get_index(size_t min, size_t max)
{
	size_t index = 0;
	do {
		printf("Enter a number between %zu and %zu:\n", min, max);
		int matched = scanf("%zu", &index);
		discard_stdin_until_newline();
		if (matched != 1) {
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
	puts("Enter position where the address should be inserted.");
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

void display_addr(const struct Address *addr)
{
	printf("%-15s\t%-15s\t%-30s\t%-12s\n", "NAME", "SURNAME", "EMAIL",
	       "PHONE");
	printf("%-15s\t%-15s\t%-30s\t%-12s\n", addr->name, addr->surname,
	       addr->email, addr->phone);
}

void display_specific_addr(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	size_t index = get_index(0, addresses->size);
	const struct Address *addr = get_address(addresses, index);
	display_addr(addr);
}

void find_by_name(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	char buffer[128];
	puts("Search by name:");
	read_string(buffer, sizeof(buffer));
	const struct Address *addr = find_address_by_name(addresses, buffer);
	if (addr == NULL) {
		puts("Address with specified name not found.");
		return;
	}
	display_addr(addr);
}

void find_by_surname(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	char buffer[128];
	puts("Search by surname:");
	read_string(buffer, sizeof(buffer));
	const struct Address *addr = find_address_by_surname(addresses, buffer);
	if (addr == NULL) {
		puts("Address with specified surname not found.");
		return;
	}
	display_addr(addr);
}

void find_by_email(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	char buffer[128];
	puts("Search by email:");
	read_string(buffer, sizeof(buffer));
	const struct Address *addr = find_address_by_email(addresses, buffer);
	if (addr == NULL) {
		puts("Address with specified email not found.");
		return;
	}
	display_addr(addr);
}

void find_by_phone(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	char buffer[128];
	puts("Search by phone:");
	read_string(buffer, sizeof(buffer));
	const struct Address *addr = find_address_by_phone(addresses, buffer);
	if (addr == NULL) {
		puts("Address with specified phone not found.");
		return;
	}
	display_addr(addr);
}
