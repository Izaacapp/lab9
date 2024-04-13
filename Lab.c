#include <stdio.h>
#include <stdlib.h>
//By Izaac Plambeck
//COP3502C
//Lab 9


#define HASH_SIZE 10

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType* next;
};

// Hash table element
struct HashType
{
    struct RecordType* head;
};

// Compute the hash function
int hash(int x)
{
    const double A = 0.6180339887; 
    double val = x * A;
    val -= (int)val;
    return (int)(HASH_SIZE * val);
}

// Parses input file to an array of records
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }
        fclose(inFile);
    }
    return dataSz;
}

// Inserts a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id);
    struct RecordType* newRecord = (struct RecordType*)malloc(sizeof(struct RecordType));
    if (newRecord == NULL)
    {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    *newRecord = record;
    newRecord->next = NULL;

    // If bucket is empty, add new record as the head
    if (hashTable[index].head == NULL)
    {
        hashTable[index].head = newRecord;
    }
    else
    {
        // Traverse to the end of the list and append new record
        struct RecordType* current = hashTable[index].head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newRecord;
    }
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* hashTable, int hashSz)
{
    int i;

    printf("\nRecords in Hash Structure:\n");
    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct RecordType* current = hashTable[i].head;
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->id, current->name, current->order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    struct HashType hashTable[HASH_SIZE] = {0};

    recordSz = parseData("input_lab_9.txt", &pRecords);

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i]);
    }

    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free allocated memory
    for (int i = 0; i < recordSz; ++i)
    {
        struct RecordType* current = pRecords[i].next;
        while (current != NULL)
        {
            struct RecordType* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pRecords);

    return 0;
}
