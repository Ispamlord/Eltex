#ifndef CONTACT_LIST_H
#define CONTACT_LIST_H

#include <stddef.h>

#define MAX_LENGHT 50

typedef struct Contact {
    char firstname[MAX_LENGHT];
    char secondname[MAX_LENGHT];
    char middlename[MAX_LENGHT];
    char place_work[MAX_LENGHT];
    char name_work[MAX_LENGHT];
    char email[MAX_LENGHT];
    char phone[MAX_LENGHT];
    char link_mess[MAX_LENGHT];
    char profile[MAX_LENGHT];
} Contact;

typedef struct Item {
    Contact con;
    struct Item* next;
    struct Item* prev;
} Item;

// Функции управления списком
Item* find_contact(Item* head, char* firstname, char* secondname);
Item* Edit_contact(Item* head, char* firstname, char* secondname, char* phone, char* link, char* place_work, char* name_work, char* email, char* profile, char* link_mess);
Contact create_contact(char* firstname, char* secondname, char* middlename, char* place_work, char* name_work, char* email, char* phone, char* link_mess, char* profile);
Item* Add_contacts(Item* head, Contact con);
void printList(Item* head);
void deleteList(Item* head);
Item* Delete_contact(Item* head, char* firstname, char* secondname);
void PrintContact(Contact con);

#endif // CONTACT_LIST_H
