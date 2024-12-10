#include "contact_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Item* find_contact(Item* head, char* firstname, char* secondname) {
    Item* item = head;
    while (item != NULL) {
        if (strncmp(item->con.firstname, firstname, MAX_LENGHT) == 0 && strncmp(item->con.secondname, secondname, MAX_LENGHT) == 0) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

Item* Edit_contact(Item* head, char* firstname, char* secondname, char* phone, char* link, char* place_work, char* name_work, char* email, char* profile, char* link_mess) {
    Item* item = find_contact(head, firstname, secondname);
    if (item == NULL) {
        printf("Такого контакта не существует\n");
        return head;
    }
    strncpy(item->con.place_work, place_work, MAX_LENGHT);
    strncpy(item->con.name_work, name_work, MAX_LENGHT);
    strncpy(item->con.email, email, MAX_LENGHT);
    strncpy(item->con.phone, phone, MAX_LENGHT);
    strncpy(item->con.link_mess, link_mess, MAX_LENGHT);
    strncpy(item->con.profile, profile, MAX_LENGHT);
    return head;
}

Contact create_contact(char* firstname, char* secondname, char* middlename, char* place_work, char* name_work, char* email, char* phone, char* link_mess, char* profile) {
    Contact newcontact;
    strncpy(newcontact.firstname, firstname, MAX_LENGHT);
    strncpy(newcontact.secondname, secondname, MAX_LENGHT);
    strncpy(newcontact.middlename, middlename, MAX_LENGHT);
    strncpy(newcontact.place_work, place_work, MAX_LENGHT);
    strncpy(newcontact.name_work, name_work, MAX_LENGHT);
    strncpy(newcontact.email, email, MAX_LENGHT);
    strncpy(newcontact.phone, phone, MAX_LENGHT);
    strncpy(newcontact.link_mess, link_mess, MAX_LENGHT);
    strncpy(newcontact.profile, profile, MAX_LENGHT);
    return newcontact;
}

Item* Add_contacts(Item* head, Contact con) {
    Item* newcontact = malloc(sizeof(Item));
    if (!newcontact) {
        printf("Ошибка выделения памяти\n");
        return head;
    }
    newcontact->con = con;
    newcontact->next = newcontact->prev = NULL;
    if (head == NULL) {
        return newcontact;
    }
    Item* current = head;
    while (current != NULL) {
        if (strncmp(con.secondname, current->con.secondname, MAX_LENGHT) < 0 || (strncmp(con.secondname, current->con.secondname, MAX_LENGHT) == 0 && strncmp(con.firstname, current->con.firstname, MAX_LENGHT) < 0)) {
            newcontact->next = current;
            newcontact->prev = current->prev;
            if (current->prev) {
                current->prev->next = newcontact;
            }
            else {
                head = newcontact;
            }
            current->prev = newcontact;
            return head;
        }
        if (current->next == NULL) {
            current->next = newcontact;
            newcontact->prev = current;
            return head;
        }
        current = current->next;
    }
    printf("Такой контакт уже существует\n");
    free(newcontact);
    return head;
}

void printList(Item* head) {
    for (Item* current = head; current != NULL; current = current->next) {
        PrintContact(current->con);
    }
}

void deleteList(Item* head) {
    for (Item* item = head; item != NULL; item = head) {
        head = head->next;
        free(item);
    }
}

Item* Delete_contact(Item* head, char* firstname, char* secondname) {
    if (head == NULL) {
        return head;
    }
    Item* current = find_contact(head, firstname, secondname);
    if (current == head) {
        head = current->next;
        if (head != NULL) {
            head->prev = NULL;
        }
    }
    else {
        current->prev->next = current->next;
        if (current->next) {
            current->next->prev = current->prev;
        }
    }
    free(current);
    return head;
}

void PrintContact(Contact con) {
    printf("Фамилия: %s\nИмя: %s\nОтчество: %s\nКомпания: %s\nПрофессия: %s\nЕмаил: %s\nНомер телефона: %s\nСоцсеть: %s\nПрофиль: %s\n",
        con.secondname, con.firstname, con.middlename, con.place_work, con.name_work, con.email, con.phone, con.link_mess, con.profile);
}
