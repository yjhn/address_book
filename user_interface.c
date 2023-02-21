#include <string.h>

#include "user_interface.h"

static unsigned int get_selection_number(unsigned int min, unsigned int max);
static void read_string(char *buffer, size_t buf_size);

int execute_user_command(struct AddressBook *addresses)
{
	puts("Select an action by number:\n"
	     "1) Display address book\n"
	     "2) Add new address to the end\n"
	     "3) Add new address to specific position\n"
	     "4) Delete address\n"
	     "5) Display address in specified position\n"
	     "6) Find address\n"
	     "7) Quit");

	unsigned int selected = get_selection_number(1, 7);
	switch (selected) {
	case 1:
		display_addresses(addresses);
		break;
	case 2:
		add_addr_to_end(addresses);
		break;
	case 3:
		insert_addr(addresses);
		break;
	case 4:
		delete_addr(addresses);
		break;
	case 5:
		display_specific_addr(addresses);
		break;
	case 6:
		find_by_keyword(addresses);
		break;
	case 7:
		return 1;
	default:
		puts("Unreachable code entered");
		return -1;
	}

	// Leave an empty line, it looks better.
	puts("");
	return 0;
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
	struct Address addr;
	char buffer[128];
	puts("Creating an address.");
	puts("Enter name:");
	read_string(buffer, sizeof(buffer));
	safe_strncpy(addr.name, buffer, sizeof(addr.name));
	puts("Enter surname:");
	read_string(buffer, sizeof(buffer));
	safe_strncpy(addr.surname, buffer, sizeof(addr.surname));
	puts("Enter email:");
	read_string(buffer, sizeof(buffer));
	safe_strncpy(addr.email, buffer, sizeof(addr.email));
	puts("Enter phone number:");
	read_string(buffer, sizeof(buffer));
	safe_strncpy(addr.phone, buffer, sizeof(addr.phone));

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

void display_address_table_header()
{
	printf("%-15s\t%-15s\t%-30s\t%-12s\n", "NAME", "SURNAME", "EMAIL",
	       "PHONE");
}

void display_addr(const struct Address *addr)
{
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
	display_address_table_header();
	display_addr(addr);
}

void find_by_keyword(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		puts("Address book is empty");
		return;
	}

	bool header_displayed = false;
	char buffer[128];
	puts("Search by keyword:");
	read_string(buffer, sizeof(buffer));

	size_t len = strlen(buffer);
	struct ListElement *elem = addresses->head;
	while (elem != NULL) {
		if (strncmp(elem->data.name, buffer, len) == 0 ||
		    strncmp(elem->data.surname, buffer, len) == 0 ||
		    strncmp(elem->data.email, buffer, len) == 0 ||
		    strncmp(elem->data.phone, buffer, len) == 0) {
			if (!header_displayed) {
				display_address_table_header();
				header_displayed = true;
			}
			display_addr(&elem->data);
		}
		elem = elem->next;
	}
	if (!header_displayed) {
		puts("Address with specified phone not found.");
		return;
	}
}
