#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <stdio.h>
#include <stdbool.h>

struct Address {
	char name[32];
	char surname[32];
	char email[64];
	char phone[15];
};

struct ListElement {
	struct Address data;
	struct ListElement *next;
};

// When empty, size = 0 and head = NULL.
struct AddressBook {
	struct ListElement *head;
	size_t size;
};

void safe_strncpy(char *dest, const char *src, size_t dest_size);
struct Address make_address(char *name, char *surname, char *email,
			    char *phone);
struct AddressBook create_address_book();
struct AddressBook read_addresses(FILE *file);
struct Address parse_line(char *line);
void display_addresses(const struct AddressBook *addresses);
void add_address_to_end(struct AddressBook *addresses,
			const struct Address addr);
bool add_address(struct AddressBook *addresses, const size_t pos,
		 const struct Address addr);
bool delete_address(struct AddressBook *addresses, const size_t pos);
struct Address *get_address(struct AddressBook *addresses, const size_t pos);
void delete_all_addresses(struct AddressBook *addresses);
void delete_address_book(struct AddressBook *addresses);
struct Address *find_address_by_name(struct AddressBook *addresses,
				     const char *name);
struct Address *find_address_by_surname(struct AddressBook *addresses,
					const char *surname);
struct Address *find_address_by_email(struct AddressBook *addresses,
				      const char *email);
struct Address *find_address_by_phone(struct AddressBook *addresses,
				      const char *phone);
void save_addresses_to_file(const struct AddressBook *addresses, FILE *file);

#endif
