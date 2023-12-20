#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define Node struct node
#define Tree struct tree

Tree{
    int lett,frq;
    Tree*left;
    Tree*right;
};

Node{
    Tree*root;
    Node*next;
    Node*prev;
};

void hoffAlgorithm();
void insertOrder(Tree*data);
Tree*dequeue();
void queue(int data);
int searchASCII(int data);
Tree*newTree(int data, int n);
Node*newNode(Tree*data);
void order();
void inorder(FILE*codify, Tree*r, int n);
int emptyQueue();
void lines(FILE*Read, int num);
void codex(char*binary);
void decodeBinary(Tree *root, char *binary);

Node*begin=NULL;
Node*end=NULL;

int main(){
    int len,i,j,N=0;
    char code[100],*binary=NULL,temp[100],arch[100];
    printf("Introduce el nombre del archivo a codificar: ");
    scanf("%s", arch);
    FILE*frase = fopen(arch,"rb");
    if(frase==NULL){
        printf("Error en el archivo\n");
        return 0;
    }
    while(!feof(frase)){
        char chrt=fgetc(frase);
        if(chrt == -1)
            break;
        N++;
        queue(chrt);
    }
    fclose(frase);
    order();
    hoffAlgorithm();
    FILE*codify=fopen("Decode.txt", "wb");
    inorder(codify, begin->root, 7);
    fclose(codify);
    /////////////////////////////////////////////////
    FILE*Read = fopen("Decode.txt", "rb");
    FILE*Palabra = fopen("Frase.txt","rb");
    if(Read == NULL || Palabra == NULL){
        printf("Error en el archivo\n");
        return 0;
    }
    while(!feof(Palabra)){
        char chrt=fgetc(Palabra);
        if(chrt == -1)
            break;
        for(i=0;i<N;i++){
            lines(Read, i + 1);
            char c=fgetc(Read);
            if(chrt == c){
                fseek(Read, 2, SEEK_CUR);
                fscanf(Read, " %s", &code);
                len=strlen(code);
                if(binary == NULL){
                    binary=malloc((len + 1) * sizeof(char));
                    for(j=0;j<(len+1);j++){
                        binary[j]=0;
                    }
                }else{
                    binary=realloc(binary, (strlen(binary) + len + 1) * sizeof(char));
                }
                strcat(binary, code);
                goto cont;
            }
        }
        cont:
        rewind(Read);
    }
    fclose(Palabra);
    fclose(Read);
    codex(binary);
    decodeBinary(begin->root, binary);
}

Tree*newTree(int data, int n){
    Tree*new=(Tree*)malloc(sizeof(Tree));
    new->lett=data;
    new->frq=n;
    new->left=NULL;
    new->right=NULL;
    return new;
}

Node*newNode(Tree*data){
    Node*new=(Node*)malloc(sizeof(Node));
    new->root=data;
    new->next=NULL;
    new->prev=NULL;
    return new;
}

void queue(int data){
    if(searchASCII(data) == 1)
        return;
    Tree*pTree= newTree(data, 1);
    Node*new= newNode(pTree);
    if(begin == NULL && end == NULL){
        begin=new;
        end=new;
    }else{
        end->next=new;
        new->prev=end;
        end=new;
    }
}

int searchASCII(int dato){
    Node*temp=begin;
    while(temp != NULL){
        if(temp->root->lett == dato){
            temp->root->frq++;
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

void order(){
    if(begin != NULL){
        Node*current=begin;
        Node*temp=begin->next;
        while(current != NULL){
            while(temp != NULL){
                if(temp->root->frq < current->root->frq){
                    //temp->prev->next=temp->next;
                    if(temp->next == NULL){
                        end=temp->prev;
                        end->next=NULL;
                    }else{
                        temp->prev->next=temp->next;
                        temp->next->prev=temp->prev;
                    }
                    temp->prev= temp->next=NULL;
                    temp->next=current;
                    if(current == begin){
                        begin=temp;
                        current->prev=temp;
                    }else{
                        temp->prev=current->prev;
                        current->prev->next=temp;
                        current->prev=temp;
                    }
                    current=temp;
                    temp=current->next->next;
                }else{
                    temp=temp->next;
                }
            }
            current=current->next;
            if(current == NULL){
                temp=NULL;
            }else{
                temp=current->next;
            }
        }
    }
}

void insertOrder(Tree*dato){
    Node*new= newNode(dato);
    Node*temp=begin,*prev=NULL;
    while(temp != NULL){
        if(temp->root->frq >= new->root->frq){
            if(temp == begin){
                new->next=begin;
                begin->prev=new;
                begin=new;
            }else{
                prev->next=new;
                new->prev=prev;
                new->next=temp;
                temp->prev=new;
            }
            return;
        }
        prev=temp;
        temp=temp->next;
    }
    if(begin == NULL && end == NULL){
        begin=new;
        end=new;
    }else{
        end->next=new;
        new->prev=end;
        end=new;
    }
}

Tree*dequeue(){
    Node*temp;
    if(emptyQueue()){
        return NULL;
    }else{
        temp=begin;
        begin=begin->next;
        temp->next=NULL;
        if(begin != NULL){
            begin->prev=NULL;
        }else{
            end=NULL;
        }
    }
    Tree*result=temp->root;
    free(temp);
    return result;
}

int emptyQueue(){
    if(begin == NULL && end == NULL){
        return 1;
    }else{
        return 0;
    }
}

void lines(FILE*Read, int num){
    int cont= num - 1;
    fseek(Read, 0, SEEK_SET);
    prueba:
    if (cont > 0)
    {
        while (fgetc(Read) != '\n');
        cont--;
        goto prueba;
    }
}

void hoffAlgorithm(){
    int n;
    if(begin == end)
        return;
    Tree*new1= dequeue();
    Tree*new2= dequeue();
    n= new1->frq + new2->frq;
    Tree*pTree= newTree(46, n);
    pTree->left=new1;
    pTree->right=new2;
    insertOrder(pTree);
    hoffAlgorithm();

}

void inorder(FILE*codify, Tree*r, int n){
    if(r!=NULL){
        inorder(codify, r->left, n * 10);
        if(r->lett != 46){
            if(r == NULL){
                perror("Error en el cargado del archivo");
                return;
            }
            fprintf(codify, "%c %i\n", r->lett, n);
        }
        inorder(codify, r->right, n * 10 + 1);
    }
}

void codex(char*binary){
    FILE*ASCII=fopen("Decode.txt","ab");
    int len=strlen(binary);
    printf("\n%i Bits\n",len);
    printf("Codigo Binario: ");
    puts(binary);
    printf("\n");
    fclose(ASCII);
}

void decodeBinary(Tree *root, char *binary) {
    Tree *current = root;
    int len = strlen(binary);
    printf("Frase original: \n");
    for (int i = 0; i < len; i++) {
        if (binary[i] == '0') {
            current = current->left;
        } else if (binary[i] == '1') {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
            // Llegamos a una hoja, imprime el carácter correspondiente
            printf("%c", current->lett);
            current = root; // Reinicia desde la raíz para decodificar el siguiente carácter
        }
    }
}
