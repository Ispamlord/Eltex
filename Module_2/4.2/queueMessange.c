#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node {
    int priority; 
    char data[256]; 
    struct Node* next; 
} Node;
typedef struct PriorityQueue {
    Node* head; 
} PriorityQueue;

void initQueue(PriorityQueue* queue) {
    queue->head = NULL;
}
Node* dequeue(PriorityQueue* queue) {
    if (queue->head == NULL) return NULL; 

    Node* temp = queue->head;
    queue->head = queue->head->next;
    return temp;
}
Node* dequeueByPriority(PriorityQueue* queue, int priority) {
    if (queue->head == NULL) return NULL;

    if (queue->head->priority == priority) {
        Node* temp = queue->head;
        queue->head = queue->head->next;
        return temp;
    }

    Node* current = queue->head;
    while (current->next != NULL && current->next->priority != priority) {
        current = current->next;
    }

    if (current->next == NULL) return NULL; 

    Node* temp = current->next;
    current->next = current->next->next;
    return temp;
}
void dequeueByMinPriority(PriorityQueue* queue, int minPriority) {
    while (queue->head != NULL && queue->head->priority >= minPriority) {
        Node* temp = dequeue(queue);
        printf("Extracted: [%d] %s\n", temp->priority, temp->data);
        free(temp);
    }
}
void enqueue(PriorityQueue* queue, const char* data, int priority) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data, data);
    newNode->priority = priority;
    newNode->next = NULL;

    if (queue->head == NULL || queue->head->priority > priority) {
        newNode->next = queue->head;
        queue->head = newNode;
    } else {
        Node* current = queue->head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

int main() {
    PriorityQueue queue;
    initQueue(&queue);

    enqueue(&queue, "Message 1", 10);
    enqueue(&queue, "Message 2", 20);
    enqueue(&queue, "Message 3", 5);
    enqueue(&queue, "Message 4", 15);

    Node* extracted = dequeue(&queue);
    if (extracted) {
        printf("Dequeued: [%d] %s\n", extracted->priority, extracted->data);
        free(extracted);
    }

    extracted = dequeueByPriority(&queue, 15);
    if (extracted) {
        printf("Dequeued by priority: [%d] %s\n", extracted->priority, extracted->data);
        free(extracted);
    }

    printf("Dequeue elements with priority >= 10:\n");
    dequeueByMinPriority(&queue, 10);

    return 0;
}