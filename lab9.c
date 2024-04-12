#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 13

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct HashNode{

	struct RecordType data;
	struct HashNode* next;

};

// Fill out this structure
struct HashType{

	struct HashNode* table;
	int size;

};

// Compute the hash function
int hash(int x){
	return x % MAX_LENGTH;
}

// parses input file to an integer array
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
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
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
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz){
	int i;

	for (i=0;i<hashSz;++i){
		// if index is occupied with any records, print all

		printf("Index %d: ", i);
		struct HashNode* curr = pHashArray -> table[i].next;
		while(curr){
			printf("%d, %c, %d\n", curr -> data.id, curr -> data.name, curr -> data.order);
			curr = curr -> next;
		}

		printf("NULL.\n");
	}

	printf("\n");
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation


	struct HashType hashTable;
	hashTable.size = recordSz;
	hashTable.table = (struct HashNode*)malloc(sizeof(struct HashNode) * hashTable.size);

	if(!hashTable.table){

		printf("hash table memory allocation failure\n");
		exit(EXIT_FAILURE);

	}

	for(int i = 0; i < hashTable.size; i++){
		hashTable.table[i].next = NULL;
	}


	for(int i = 0; i < hashTable.size; i++){
		int id = hash(pRecords[i].id);

		struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
		if(!newNode){
			printf("new Node memory allocation failure at i = %d\n", i);
			exit(EXIT_FAILURE);
		}

		newNode -> data = pRecords[i];
		newNode -> next = hashTable.table[id].next;
		hashTable.table[id].next = newNode;
	}

	displayRecordsInHash(&hashTable, hashTable.size);

	for(int i = 0; i < hashTable.size; i++){

		struct HashNode* curr = hashTable.table[i].next;

		while(curr){

			struct HashNode* tmp = curr;
			curr = curr -> next;
			free(tmp);

		}
	}

	free(hashTable.table);
	free(pRecords);

}