#include "contact_list.h"
#include <stdio.h>

int main(void) {
    Item* head = NULL;

    Contact contact1 = create_contact("Иван", "Иванов", "Иванович", "Компания А", "Инженер", "ivanov@mail.com", "123456789", "messenger.com/ivanov", "profile.com/ivanov");
    Contact contact2 = create_contact("Петр", "Петров", "Петрович", "Компания Б", "Менеджер", "petrov@mail.com", "987654321", "messenger.com/petrov", "profile.com/petrov");

    head = Add_contacts(head, contact1);
    head = Add_contacts(head, contact2);

    printf("Контакты после добавления:\n");
    printList(head);

    head = Edit_contact(head, "Иван", "Иванов", "555555555", "messenger.com/ivanov", "Компания А", "Инженер", "ivanov@mail.com", "profile.com/ivanov", "link.com/ivan");
    printList(head);

    head = Delete_contact(head, "Петр", "Петров");
    printList(head);

    deleteList(head);
    return 0;
}
