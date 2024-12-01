#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_CONTACTS 20
#define MAX_LENGHT 512

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
}Contact;
typedef struct Item Item;

typedef struct Item {
    Contact con;
    Item* next;
    Item* prev;
}Item;
void writen_to_file(const char* filename, Contact* contact);

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
        printf("takogo ne suchestuet");
        return head;
    }
    strncpy(item->con.email, email, MAX_LENGHT);
    strncpy(item->con.phone, phone, MAX_LENGHT);
    strncpy(item->con.link_mess, link_mess, MAX_LENGHT);
    strncpy(item->con.profile, profile, MAX_LENGHT);
    return head;

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
    printf("такой контакт уже существует");
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
    printf("Фамилия: %s\nИмя:%s \nОтчество: %s\nКомпания: %s\nПрофессия: %s\nЕмаил: %s\nНомер телефона: %s\nСоцсеть: %s\nПрофиль: %s\n", con.secondname, con.firstname, con.middlename, con.place_work, con.name_work, con.email, con.phone, con.link_mess, con.profile);
}
void Write_All_to_file(const char* filename,Item* head){
    Item* current = head;
    while(current){
        writen_to_file(filename, &current->con);
        current= current->next;
    }
}
void writen_to_file(const char* filename, Contact* contact) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error! Wrong file");
        exit(EXIT_FAILURE);
    }
    ssize_t writen = write(fd,contact,sizeof(Contact));
    if(writen!=sizeof(Contact)){
        perror("Error! Structure is wrong");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);


}

void read_contact_from_file(const char *filename, Contact *contact) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    ssize_t read_bytes = read(fd, contact, sizeof(Contact));
    PrintContact(*contact);
    read(fd, contact, sizeof(Contact));
    PrintContact(*contact);
    read(fd, contact, sizeof(Contact));
    PrintContact(*contact);
    if (read_bytes != sizeof(Contact)) {
        perror("Ошибка чтения структуры");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}
Item* read_all_contacts_from_file(const char* filename, Item* head) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    Contact temp_contact;
    ssize_t read_bytes;

    while ((read_bytes = read(fd, &temp_contact, sizeof(Contact))) == sizeof(Contact)) {
        head = Add_contacts(head, temp_contact); // Используем ранее написанную функцию для добавления в список
    }

    if (read_bytes != 0) {
        perror("Ошибка чтения структуры или файл поврежден");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return head;
}
int main() {
    const char* filename = "contacts.txt";

    // Создаем тестовые данные
    Contact test_contacts[] = {
        create_contact("John", "Doe", "Michael", "TechCorp", "Engineer", "john.doe@example.com", "+123456789", "Messenger1", "Profile1"),
        create_contact("Jane", "Smith", "Anne", "HealthCorp", "Doctor", "jane.smith@example.com", "+987654321", "Messenger2", "Profile2"),
        create_contact("Alice", "Johnson", "Marie", "EduCorp", "Teacher", "alice.johnson@example.com", "+192837465", "Messenger3", "Profile3"),
        create_contact("Bob", "Brown", "Edward", "FinCorp", "Analyst", "bob.brown@example.com", "+564738291", "Messenger4", "Profile4"),
        create_contact("Charlie", "White", "Henry", "MediaCorp", "Journalist", "charlie.white@example.com", "+837261945", "Messenger5", "Profile5")
    };

    // Инициализация списка
    Item* contact_list = NULL;

    // Добавляем тестовые данные в список
    for (int i = 0; i < 5; i++) {
        contact_list = Add_contacts(contact_list, test_contacts[i]);
    }
    Contact *contact;
    printf("Список контактов после добавления:\n");
    //printList(contact_list);

    // Запись в файл
    Write_All_to_file(filename, contact_list);
    printf("Контакты записаны в файл '%s'.\n", filename);

    // Удаляем список из памяти
    deleteList(contact_list);

    // Чтение из файла
    contact_list = read_all_contacts_from_file(filename, NULL);
    //read_contact_from_file(filename, contact);
    printf("Список контактов после чтения из файла:\n");
    printList(contact_list);
    
    deleteList(contact_list);
    
    return 0;
}