#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>

typedef struct Operatiune{
    char codBanca[6];
    char codClient[11];
    char IDOperatiune[14];
    struct Operatiune *next;
}Operatiune;
enum sortBy{
    codBanca,codClient
};
int isEmpty(Operatiune *head ){
    if(head==NULL) return 1;
    else return 0;
}
void copyUntilSeparator(const char *source, char *destination, int destinationSize, char separator){
    char *pozitieSeparator = strchr(source, separator);

    if (pozitieSeparator != NULL) {
        // Găsim separatorul în șir
        int indexSeparator = pozitieSeparator - source;
        if(indexSeparator<destinationSize){
            memmove(destination, source,  indexSeparator);
            destination[indexSeparator]='\0';
        }
        else{
            /*Dacă dimensiunea destinației e mai mica decât șirul până la separator
             * nu copiem nimic pentru a nu cauza deformarea în memorie a șirului inițial
             */
        }
    }
    else {
        // Dacă separatorul nu este găsit, șirul rămâne neschimbat
    }
}
void removeUntilSeparator(char *sir, char separator) {
    char *pozitieSeparator = strchr(sir, separator);
    if (pozitieSeparator != NULL) {
        // Găsim separatorul în șir
        int indexSeparator = pozitieSeparator - sir;
        memmove(sir, sir + indexSeparator + 1, strlen(sir) - indexSeparator);
    }
    else {
        // Dacă separatorul nu este găsit, șirul rămâne neschimbat
    }
}
void createOperation(const char codBanca[6], const char codClient[11], const  char IDOperatiune[11],  Operatiune **head) {
    Operatiune *node = malloc(sizeof(Operatiune));

    if (node == NULL) {
        fputs("Error: Out of memory\n", stderr);
        exit(1);
    } else {

        strcpy(node->codBanca, codBanca);
        strcpy(node->codClient, codClient);
        strcpy(node->IDOperatiune, IDOperatiune);
        if (*head == NULL) {
            node->next = *head;
            *head = node;
        } else {
            Operatiune *iterr = *(head);
            while (iterr->next != NULL)
                iterr = iterr->next;
            iterr->next = node;
            node->next = NULL;


        }
    }
}
void Swap(Operatiune * op1, Operatiune *op2)
{
    Operatiune t;
    Operatiune *ne1, *ne2;
    ne1=op1->next;
    ne2=op2->next;
    t=*op1;
    *op1=*op2;
    *op2=t;
    op1->next=ne1;
    op2->next=ne2;
    return;
}
void sort(Operatiune * head,enum sortBy field){
    Operatiune *current = head, *index = NULL;
    if(head == NULL) {
        return ;
    }
    else {
        while(current != NULL) {
            index = current->next;
            while(index != NULL) {
                switch (field) {
                    case codClient:
                        if(strcmp(current->codClient,index->codClient)>0   ) {
                            Swap(current,index);
                        };break;
                    case codBanca:
                        if(strcmp(current->codBanca,index->codBanca)>0   ) {
                            Swap(current,index);
                        };break;
                    default:
                        continue;

                }
                index = index->next;
            }
            current = current->next;
        }
    }
}
int parseFile(Operatiune **head,char *fileName){
    FILE *fptr;
    fptr = fopen(fileName,"r");
    char operation[36];
    if(fptr == NULL)
    {
        printf("Eroare la deschiderea fisierului '%s' !",fileName);
        getch();
        exit(1);
    }
    char codBanca[6];
    char codClient[11];
    char IDOperatiune[14];
    while (fgets(operation, sizeof(operation), fptr)) {
        if (operation[0] == '\r' || operation[0] == '\n') {
            continue; // Ignoră caracterul '\r' si '\n'
        }
        //Citim fiecare operațiune rând cu rând  în variabila operation apoi extragem din ea datele
        copyUntilSeparator(operation, codBanca, 6, ';');
        removeUntilSeparator(operation, ';');
        copyUntilSeparator(operation, codClient, 11, ';');
        removeUntilSeparator(operation, ';');
        copyUntilSeparator(operation, IDOperatiune, 14, ';');
        createOperation(codBanca,codClient,IDOperatiune,head);
    }
    fclose(fptr);
    return 0;

}
int processingData(Operatiune *head,char *fileName){
    if(isEmpty(head)){
        printf("Nu exista nici o operatiune in lista!");
        getch();
        exit(1);
    }
    sort(head,codClient);
    sort(head,codClient);
    FILE *fptr,*fptr1;
    fptr = fopen("output.txt","w");
    char operation[37];
    char codBanca[6];
    char IDOperatiune[14];
    double sumaOperatiune;
    double totalSum=0;

    if(fptr == NULL)
    {
        printf("Eroare la crearea fisierului 'output.txt' !");
        getch();
        exit(1);
    }
    while(head!=NULL){
        fptr1 = fopen(fileName,"r");
        if(fptr1 == NULL)
        {
            printf("Eroare la deschiderea fisierului '%s' !",fileName);
            getch();
            exit(1);
        }
        while (fgets(operation, sizeof(operation), fptr1)) {
            if (operation[0] == '\r' || operation[0] == '\n') {
                continue; // Ignoră caracterul '\r' si '\n'
            }
            //Citim fiecare operațiune rând cu rând  în variabila operation apoi extragem din ea datele
            copyUntilSeparator(operation, codBanca, 6, ';');
            removeUntilSeparator(operation, ';');
            copyUntilSeparator(operation, IDOperatiune, 14, ';');
            removeUntilSeparator(operation, ';');
            char *pozitieSeparator = strchr(operation, ',');
            if (pozitieSeparator != NULL) {
                *pozitieSeparator='.';
            }
            sumaOperatiune = strtod(operation, '\0');
            if(strcmp(head->codBanca,codBanca)==0 && strcmp(head->IDOperatiune,IDOperatiune)==0){
                totalSum+=sumaOperatiune;
                break;
            }


        }
        if(head->next!=NULL ){
            if(strcmp(head->codBanca,head->next->codBanca)!=0 || strcmp(head->codClient,head->next->codClient)!=0) {
                fprintf(fptr, "%s;%s;%012.3f\n", head->codBanca, head->codClient, totalSum);
                totalSum = 0;
            }
        }
        else if(head->next==NULL){
            fprintf(fptr,"%s;%s;%012.3f\n",head->codBanca,head->codClient,totalSum);
        }
        fclose(fptr1);
        head=head->next;
    }
    fclose(fptr);
    return 0;
}
void print(Operatiune * head){
    int i=0;
    while(head!=NULL){
        printf("\n Opratiune %i ",++i);
        printf("\n Cod banca %s ",head->codBanca);
        printf("\n Cod client %s ",head->codClient);
        printf("\n ID operatiune %s ",head->IDOperatiune);
        printf("\n-----------------------------------------------------");
        head=head->next;
    }
}
int main() {
    Operatiune *head=NULL;
    parseFile(&head,"operations.txt");
    print(head);
    processingData(head,"operations1.txt");
    print(head);
    getch();
    return 0;
}