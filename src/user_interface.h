#ifndef UI_H
#define UI_H

#include <address_book.h>

int execute_user_command(struct AddressBook *addresses);
void add_addr_to_end(struct AddressBook *addresses);
void insert_addr(struct AddressBook *addresses);
void delete_addr(struct AddressBook *addresses);
void display_addr(const struct Address *addr);
void display_specific_addr(struct AddressBook *addresses);
void find_by_keyword(struct AddressBook *addresses);
struct Address create_address();

#endif
