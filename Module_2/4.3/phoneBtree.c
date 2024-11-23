#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_CONTACTS 20
#define MAX_LENGHT 25

typedef struct Contact{
    char firstname[MAX_LENGHT];
    char secondname[MAX_LENGHT];
    char middlename[MAX_LENGHT];
    char place_work[MAX_LENGHT];
    char name_work[MAX_LENGHT];
    char email[MAX_LENGHT];
    char phone[MAX_LENGHT];
    char link_mess[MAX_LENGHT];
    char profile[MAX_LENGHT];
}Contact;
typedef struct Tree Tree;

typedef struct Tree{
    Contact con;
    Item* next;
    Item* prev;
}Tree;


Tree* Delete_Contact(Tree* head) {
    Tree* item = head;
    if(item!=NULL){
        return item;
    }
}
Tree* Edit_contact(Tree* head,char* firstname, char* secondname,char* phone, char* link, char* place_work, char* name_work, char* email, char* profile,char* link_mess) {
    Tree* Tree = head;
    while(item != NULL) {
        if (strncmp(item->con.firstname, firstname, MAX_LENGHT) == 0 && strncmp(item->con.secondname, secondname, MAX_LENGHT) == 0) {
            strncpy(item->con.email, email, MAX_LENGHT);
            strncpy(item->con.phone, phone, MAX_LENGHT);
            strncpy(item->con.link_mess, link_mess, MAX_LENGHT);
            strncpy(item->con.profile, profile, MAX_LENGHT);
            return head;
        }
        item = item->next;
    }
    printf("net takogo");
    return item;
}
Contact create_contact(char* firstname, char* secondname, char* middlename, char* place_work,
    char* name_work,
    char* email,
    char* phone,
    char* link_mess,char* profile) {
    Contact newcontact;
    strncpy(newcontact.firstname, firstname,MAX_LENGHT);
    strncpy(newcontact.secondname, secondname,MAX_LENGHT);
    strncpy(newcontact.middlename, middlename,MAX_LENGHT);
    strncpy(newcontact.email, email,MAX_LENGHT);
    strncpy(newcontact.phone,phone, MAX_LENGHT);
    strncpy(newcontact.link_mess, link_mess,MAX_LENGHT);
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
    newcontact->next=newcontact->prev=NULL;
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
    printf("такой контакт уже существует");
    free(newcontact);
    return head;
}
void printList(Item* head) {
    for (Item* current = head; current != NULL; current = current->next) {
        PrintContact(current->con);
    }
}
void deleteList( Item* head) {
    for (Item* item = head; item != NULL; item = head) {
        head = head->next;
        free(item);
    }
}
Item* Delete_contact(Item* head, char* firstname, char* secondname) {
    if (head == NULL){
        return head;
    }
    for (Item* current = head; current != NULL; current = current->next) {
        if (strncmp(current->con.firstname, firstname, MAX_LENGHT) == 0 && strncmp(current->con.secondname, secondname, MAX_LENGHT) == 0) {
            
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
    }
}
void PrintContact(Contact con){
    printf("Фамилия: %s\nИмя:%s \nОтчество: %s\nКомпания: %s\nПрофессия: %s\nЕмаил: %s\nНомер телефона: %s\nСоцсеть: %s\nПрофиль: %s\n",con.secondname,con.firstname,con.middlename,con.place_work,con.name_work,con.email,con.phone,con.link_mess,con.profile);
}
int main(void) {
    Item* head = NULL;

    Contact contact1 = create_contact("Иван", "Иванов", "Иванович", "Компания А", "Инженер", "ivanov@mail.com", "123456789", "messenger.com/ivanov", "profile.com/ivanov");
    Contact contact2 = create_contact("Петр", "Петров", "Петрович", "Компания Б", "Менеджер", "petrov@mail.com", "987654321", "messenger.com/petrov", "profile.com/petrov");
    
    head = Add_contacts(head, contact1);
    head = Add_contacts(head, contact2);

    printf("Контакты после добавления:\n");
    printList(head);
    printf("\nРедактирование телефона Ивана Иванова:\n");
    head = Edit_contact(head, "Иван", "Иванов", "555555555", "messenger.com/ivanov", "Компания А", "Инженер", "ivanov@mail.com", "profile.com/ivanov", "link.com/ivan");
    printList(head);

    printf("\nУдаление Петра Петрова:\n");
    head = Delete_contact(head, "Петр", "Петров");
    printList(head);
    deleteList(head);
    return 0;
}