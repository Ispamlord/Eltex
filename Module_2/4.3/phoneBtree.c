#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_CONTACTS 20
#define MAX_LENGHT 25

typedef struct {
    const char firstname[MAX_LENGHT];
    const char secondname[MAX_LENGHT];
    char middlename[MAX_LENGHT];
    char place_work[MAX_LENGHT];
    char name_work[MAX_LENGHT];
    char email[MAX_LENGHT];
    char phone[MAX_LENGHT];
    char link_mess[MAX_LENGHT];
    char profile[MAX_LENGHT];
}Contact;


typedef struct Tree {
    Contact con;
    Tree* left;
    Tree* right;
}Tree;
void deleteTree(Tree* root) {
    if (root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

void Edit_contact(Tree** q, char* firstname, char* secondname, char* phone, char* link, char* place_work, char* name_work, char* email, char* profile, char* link_mess) {
    if (*q == NULL) {
        return;
    }

    if (strncmp((*q)->con.secondname, secondname, MAX_LENGHT) == 0 && strncmp((*q)->con.firstname, firstname, MAX_LENGHT) == 0) {
        strncpy((*q)->con.email, email, MAX_LENGHT);
        strncpy((*q)->con.phone, phone, MAX_LENGHT);
        strncpy((*q)->con.link_mess, link_mess, MAX_LENGHT);
        strncpy((*q)->con.profile, profile, MAX_LENGHT);
        return;
    }
    if (strncmp((*q)->con.secondname, secondname, MAX_LENGHT) < 0 || (strncmp((*q)->con.secondname, secondname, MAX_LENGHT) == 0 && strncmp((*q)->con.firstname, firstname, MAX_LENGHT) < 0)) {
        Edit_contact(&(*q)->left, firstname, secondname, phone, link, place_work, name_work, email, profile, link_mess);
    }
    else {
        Edit_contact(&(*q)->right, firstname, secondname, phone, link, place_work, name_work, email, profile, link_mess);
    }

    printf("net takogo");
    return;
}
Contact create_contact(char* firstname, char* secondname, char* middlename, char* place_work,
    char* name_work,
    char* email,
    char* phone,
    char* link_mess, char* profile) {
    Contact newcontact;
    strncpy(newcontact.firstname, firstname, MAX_LENGHT);
    strncpy(newcontact.secondname, secondname, MAX_LENGHT);
    strncpy(newcontact.middlename, middlename, MAX_LENGHT);
    strncpy(newcontact.email, email, MAX_LENGHT);
    strncpy(newcontact.phone, phone, MAX_LENGHT);
    strncpy(newcontact.link_mess, link_mess, MAX_LENGHT);
    strncpy(newcontact.profile, profile, MAX_LENGHT);
    return newcontact;
}

void Add_contacts(Tree** q, Contact con) {
    if (*q == NULL) {
        *q = malloc(sizeof(Tree));
        (*q)->left = (*q)->right = NULL;
        (*q)->con = con;
        return;
    }
    if (strncmp((*q)->con.secondname, con.secondname, MAX_LENGHT) == 0 && strncmp((*q)->con.firstname, con.firstname, MAX_LENGHT) == 0) {
        printf("Такой контакт уже существует");
        return;
    }
    if (strncmp((*q)->con.secondname, con.secondname, MAX_LENGHT) < 0 || (strncmp((*q)->con.secondname, con.secondname, MAX_LENGHT) == 0 && strncmp((*q)->con.firstname, con.firstname, MAX_LENGHT) < 0)) {
        Add_contacts(&(*q)->left, con);
    }
    else {
        Add_contacts(&(*q)->right, con);
    }

}
Tree* deleteContact(Tree* root, const char* secondname, const char* firstname) {
    if (root == NULL) return NULL;

    if (strncmp(root->con.secondname, secondname, MAX_LENGHT) < 0 ||
        (strncmp(root->con.secondname, secondname, MAX_LENGHT) == 0 &&
            strncmp(root->con.firstname, firstname, MAX_LENGHT) < 0)) {
        root->left = deleteContact(root->left, secondname, firstname);
    }
    else if (strncmp(root->con.secondname, secondname, MAX_LENGHT) > 0 ||
        (strncmp(root->con.secondname, secondname, MAX_LENGHT) == 0 &&
            strncmp(root->con.firstname, firstname, MAX_LENGHT) > 0)) {
        root->right = deleteContact(root->right, secondname, firstname);
    }
    else {
        // Найден узел для удаления
        if (root->left == NULL) {
            Tree* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Tree* temp = root->left;
            free(root);
            return temp;
        }

        // Узел имеет двух потомков
        Tree* minNode = root->right;
        while (minNode->left != NULL) {
            minNode = minNode->left;
        }

        root->con = minNode->con;  // Замена на минимальный элемент
        root->right = deleteContact(root->right, minNode->con.secondname, minNode->con.firstname);
    }

    return root;
}

void printcontact(Contact con) {
    printf("Фамилия: %s\nИмя %s\nОтчество: %s\nEmail: %s\nСсылка на мессенджер: %s\nПрофиль %s\nТелефон: %s\nМесто работы: %s\nДолжность: %s\n", con.secondname, con.firstname, con.middlename, con.email, con.link_mess, con.profile, con.phone, con.place_work, con.name_work);
}
void printList(Tree* q) {
    if (q == NULL) return;
    printList(q->left);
    printcontact(q->con);
    printList(q->right);
}


int main() {
    Tree* head = NULL;

    // Добавление контактов
    Contact contact1 = create_contact("Иван", "Иванов", "Иванович", "Компания А", "Инженер", "ivanov@mail.com", "123456789", "messenger.com/ivanov", "profile.com/ivanov");
    Contact contact2 = create_contact("Петр", "Петров", "Петрович", "Компания Б", "Менеджер", "petrov@mail.com", "987654321", "messenger.com/petrov", "profile.com/petrov");
    Contact contact3 = create_contact("Сергей", "Сергеев", "Сергеевич", "Компания В", "Разработчик", "sergeev@mail.com", "112233445", "messenger.com/sergeev", "profile.com/sergeev");
    Contact contact4 = create_contact("Анна", "Антонова", "Александровна", "Компания Г", "Аналитик", "antonova@mail.com", "998877665", "messenger.com/antonova", "profile.com/antonova");
    Contact duplicateContact = create_contact("Иван", "Иванов", "Иванович", "Компания А", "Инженер", "ivanov@mail.com", "123456789", "messenger.com/ivanov", "profile.com/ivanov");

    Add_contacts(&head, contact1);
    Add_contacts(&head, contact2);
    Add_contacts(&head, contact3);
    Add_contacts(&head, contact4);

    printf("Контакты после добавления:\n");
    printList(head);

    printf("\nПопытка добавления дубликата Ивана Иванова:\n");
    Add_contacts(&head, duplicateContact);
    printList(head);

    printf("\nРедактирование телефона и email Сергея Сергеева:\n");
    Edit_contact(&head, "Сергей", "Сергеев", "554433221", "messenger.com/sergeev_new", "Компания В", "Разработчик", "sergeev_new@mail.com", "profile.com/sergeev", "link.com/sergeev");
    printList(head);

    printf("\nПопытка редактирования несуществующего контакта (Мария Морозова):\n");
    Edit_contact(&head, "Мария", "Морозова", "667788990", "messenger.com/morozova", "Компания Д", "Дизайнер", "morozova@mail.com", "profile.com/morozova", "link.com/morozova");
    printList(head);

    printf("\nУдаление контакта Анны Антоновой (лист):\n");
    head = deleteContact(head, "Анна", "Антонова");
    printList(head);

    printf("\nУдаление контакта Сергея Сергеева (с одним потомком):\n");
    head = deleteContact(head, "Сергей", "Сергеев");
    printList(head);

    printf("\nУдаление контакта Ивана Иванова (корень):\n");
    head = deleteContact(head, "Иван", "Иванов");
    printList(head);

    printf("\nУдаление контакта Петра Петрова (последний контакт):\n");
    head = deleteContact(head, "Петр", "Петров");
    printList(head);

    printf("\nПопытка удаления из пустого дерева:\n");
    head = deleteContact(head, "Неизвестный", "Контакт");
    printList(head);

    printf("\nОсвобождение памяти дерева:\n");
    deleteTree(head);

    return 0;
}
