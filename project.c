#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 256

typedef struct line{
    char *string;            //Inizializzo uno lista concatenata che rappresenta il file intermedio di output
    struct line *next;
}line_I;

typedef struct function{
    char type_of_command;
    int n1, n2, last_line;
    line_I *lines, *last, *last_cur ,*last_file;
    struct function *next;
    struct function *prev;
}edit_history; // Lista edit_history = cronologia delle modifiche apportate

line_I *change(int n1, int n2, line_I *head);
line_I *positioning(int n, line_I *list);
line_I *new_line(char *line);
void print(int n1, int n2, line_I *list);
line_I *delete(int n1, int n2, line_I *list);
void add_edit(int n1, int n2, char type_of_command, line_I *lines, line_I *last_m, line_I *last_f);
line_I *move_edit(int dir, line_I *list);
edit_history *clear_history(edit_history *point);

int last_line_number=0;
edit_history *current = NULL; //inizializzazione del puntatore alla struttura dello storico delle modifiche
edit_history *last=NULL, *first=NULL;
int empty=0;
int margin_right=0, margin_left=0;
line_I *last_node=NULL;

int main() {
    char funct;
    char *command;
    int n1, n2;
    line_I *list = NULL; //inizializzazione della lista di output intermedio
    int dir=0;

    command = malloc(256  * sizeof(char));
    while(1){
        fgets(command, 256 * sizeof(char), stdin);
        if(*command=='q'){
            return 0;
        }
        else{
            if(sscanf(command, "%d,%d%c", &n1, &n2, &funct)==3){
                if(funct=='c'){
                    if(dir!=0){
                        list=move_edit(dir, list); //muovo il puntatore current ed eseguo l'annullamento o il ripristino delle modifiche
                        dir=0;
                    }
                    if(current!=last){
                        current=clear_history(current);
                        last=current;
                        margin_right=0;
                    }
                    list=change(n1, n2, list);
                }
                if(funct=='d'){
                    if(n1==0) n1=1; // Gestisco il caso 0,xd
                    if(dir!=0){
                        list=move_edit(dir, list); //muovo il puntatore current ed eseguo l'annullamento o il ripristino delle modifiche
                        dir=0;
                    }
                    if(current!=last){
                        current=clear_history(current);
                        last=current;
                        margin_right=0;
                    }
                    list=delete(n1, n2, list);
                }
                if(funct=='p'){
                    if(dir!=0){
                        list=move_edit(dir, list); //muovo il puntatore current ed eseguo l'annullamento o il ripristino delle modifiche
                        dir=0;
                    }
                    print(n1, n2, list);
                }
            }
            if(sscanf(command, "%d%c", &n1, &funct)==2){
                if(funct=='r'){
                    if(dir<0){
                        if(dir+margin_left<0){
                            dir=-margin_left;
                        }
                    }
                    dir=dir+n1; //Mi devo muovere a destra di n1 posizioni
                }
                if(funct=='u'){
                    if(dir>0){
                        if(margin_right<dir){
                            dir=margin_right;
                        }
                    }
                    dir=dir-n1; //Mi devo muovere a sinistra di n1 posizioni
                }
            }
        }
    }
    return 0;
}

line_I *change(int n1, int n2, line_I *head){
    char *fp_inter;
    line_I *node;
    int count=n2-n1+1; //conta quante linee devo cambiare (cioè quante volte devo eseguire il ciclo)
    line_I *backup=NULL, *temp=NULL, *head_line_edit=NULL, *last_mod=NULL, *last_punt_file=NULL;

    if(n1==1) { // La change avviene in testa alla lista
        if (head == NULL) { // La lista è ancora vuota
            node = NULL;
            while ((count > 0)) {
                scanf("%m[^\n]", &fp_inter);
                scanf("\n");
                //fp_inter=malloc(MAX_CHAR*sizeof(char));
                //fgets(fp_inter, 1024, stdin);
                head_line_edit = NULL;
                last_mod=NULL;
                node = new_line(fp_inter);
                if (head == NULL) {
                    head = node;
                    last_node = node;
                } else {
                    last_node->next = node;
                    last_node = node;
                }
                node = node->next;
                if (count == 1) {
                    last_punt_file=node;
                    last_line_number = n2;
                }
                count--;
            }
        } else {
            head_line_edit = head;
            backup = head;
            while (count > 0) {
                scanf("%m[^\n]", &fp_inter);
                scanf("\n");
                //fp_inter=malloc(MAX_CHAR*sizeof(char));
                //fgets(fp_inter, 1024, stdin);
                node = new_line(fp_inter);
                if (backup == head) { // Siamo alla prima iterazione
                    head = node;
                }
                if (temp != NULL) temp->next = node;
                temp = node;
                if(backup->next==NULL){
                    last_node=node;
                }
                if (count == 1) { // Siamo all'ultima iterazione
                    node->next = backup->next;
                    last_punt_file=node;
                    last_mod=backup;
                    backup->next = NULL;
                }
                if (backup->next != NULL) backup = backup->next;
                count--;
            }
            if (n2 > last_line_number) last_line_number = n2;
        }
    }
    else{
            if(n1>last_line_number) { //Devo fare un inserimento in fondo, dunque non sto a chiamare positioning
                node = NULL;
                head_line_edit = NULL;
                while ((count > 0)) {
                    scanf("%m[^\n]", &fp_inter);
                    scanf("\n");
                    //fp_inter=malloc(MAX_CHAR*sizeof(char));
                    //fgets(fp_inter, 1024, stdin);
                    if (node == NULL) { //Inserimento in coda
                        temp = backup;
                        backup = NULL;
                        if (temp != NULL) {
                            temp->next = backup;
                        }
                        node = new_line(fp_inter);
                        last_punt_file=last_node;
                        last_node->next = node;
                        last_node = node;
                        node = node->next;
                        if (count == 1) {
                            last_line_number = n2;
                        }
                    }
                    count--;
                }
            }
            else {
                if(n2>last_line_number){ // Change a cavallo tra la lista attuale e la coda
                    node=positioning((n1-1), head);
                    while ((count > 0)) {
                        scanf("%m[^\n]", &fp_inter);
                        scanf("\n");
                        //fp_inter=malloc(MAX_CHAR*sizeof(char));
                        //fgets(fp_inter, 1024, stdin);
                        if(count==(n2-n1+1)){ //Siamo alla prima iterazione
                            head_line_edit=node->next;
                        }
                        node->next=new_line(fp_inter);
                        if(count==1){ //Siamo all'ultima iterazione
                            last_node=node->next;
                        }
                        node=node->next;
                        count--;
                    }
                    last_line_number=n2;

                }
                else{ // Change in mezzo alla lista
                    node = positioning((n1 - 1),head); //Vado a posizionarmi al posto giusto nella lista dell'output intermedio
                    while ((count > 0)) {
                        scanf("%m[^\n]", &fp_inter);
                        scanf("\n");
                        //fp_inter=malloc(MAX_CHAR*sizeof(char));
                        //fgets(fp_inter, 1024, stdin);
                        if(count==(n2-n1+1)){ // Siamo alla prima iterazione
                            backup = node->next;
                            head_line_edit=backup;
                        }
                        node->next = new_line(fp_inter); // Creo un nodo della lista dove salvo la riga che vado a modificare
                        if(last_node==backup) last_node=node->next; // Se modifico last_node, devo aggiornarlo
                        if(count==1){ // Siamo all'ultima iterazione
                            node=node->next;
                            last_mod=backup;
                            last_punt_file=node;
                            node->next=backup->next;
                            backup->next=NULL;
                        }
                        else{
                            backup=backup->next;
                            node = node->next;
                        }
                        count--;
                    }
                }
            }
            if(n2>last_line_number) last_line_number=n2;
    }
    add_edit(n1, n2, 'c', head_line_edit, last_mod, last_punt_file);
    fscanf(stdin, ".\n");
    margin_left++;
    return head;
}

line_I *positioning(int n, line_I *head){
    line_I *node_to_begin;

    if(n==0){
        return NULL;
    }
    if(n==1){
        return head;
    }
    node_to_begin=head; //Mi pongo all'inizio della lista
    for(int i=1; i<n; i++){
        if(n>last_line_number){
            return NULL;
        }
        if(node_to_begin==NULL){
            return NULL;
        }
       node_to_begin=node_to_begin->next;
    }
    return node_to_begin; //Tale funzione serve per portarmi alla riga dell'output intermedio dalla quale devo iniziare a fare operazioni
}

line_I *new_line(char *line){
    line_I *node;

    node = malloc(sizeof(line_I));
    node->next = NULL;
    node->string = line;
    return node;
}

void print(int n1, int n2, line_I *list){
    line_I *node_to_print;
    int count=n2-n1+1;

    node_to_print=positioning(n1, list);
    while(count>0){
        if(node_to_print!=NULL){
            fputs(node_to_print->string, stdout);
            fprintf(stdout, "\n");
            node_to_print = node_to_print->next;
        }
        else{
           fprintf(stdout, ".\n");
        }
        count--;
    }
}

line_I *delete(int n1, int n2, line_I *head_list){
    line_I *point, *prev, *backup=NULL, *temp=NULL;
    line_I *head_line_edit=NULL, *last_mod=NULL, *last_punt_file=NULL;
    int count = n2-n1+1;

    if((head_list==NULL)&&(count==n2-n1+1)){ //La lista è vuota, dunque la cancellazione non ha effetto
        add_edit(n1, n2, 'd', NULL, NULL, NULL);
        margin_left++;
        return head_list;
    }

    point=positioning((n1), head_list);

    if((n1<=last_line_number)&&(n2>=last_line_number)){ // Se la delete va a cancellare l'ultimo nodo della lista, devo aggiornare last_node
        last_node=positioning((n1)-1, head_list);
    }

    while(count>0){
        if(point==NULL){
            if(backup!=NULL){
                backup->next=NULL;
            }
            add_edit(n1, n2, 'd', head_line_edit, last_mod, last_punt_file);
            margin_left++;
            return head_list;
        }
        if(point==head_list){ //La cancellazione avviene in testa alla lista
            head_list=point->next;
            prev=point;
            point=point->next;
            temp=backup;
            backup=prev;
            if(temp!=NULL){
                temp->next=backup;
            }
            if(head_line_edit==NULL){
                head_line_edit=backup;
            }
            if(count==1){ // Siamo all'ultima iterazione
                last_mod=backup;
            }
            if(prev->next==NULL){ // Siamo nel caso di 1,2d con lista occupata fino a 1
                last_mod=prev;
            }
            last_line_number--; // La cancellazione è andata a buon fine dunque devo diminuire last_line_number
        }
        else{ // La cancellazione avviene in un qualsiasi punto della lista tranne che in testa
            prev=positioning((n1-1), head_list);
            prev->next=point->next;
            temp=backup;
            backup=point;
            if(temp!=NULL){
                temp->next=backup;
            }
            if(head_line_edit==NULL){
                head_line_edit=backup;
            }
            if(count==1){ // Siamo all'ultima iterazione
               last_mod=backup;
            }
            prev=prev->next;
            point=point->next;
            last_line_number--; // La cancellazione è andata a buon fine dunque devo diminuire last_line_number
        }
        count--;
    }
    backup->next=NULL;
    add_edit(n1, n2, 'd', head_line_edit, last_mod, last_punt_file);
    margin_left++;
    return head_list;
}

void add_edit(int n1, int n2, char type_of_command, line_I *lines, line_I *last_m, line_I *last_f){
    edit_history *temp;

    //La aggiunta di modifiche avviene sempre in coda alla struttura

    temp=current;
    current=malloc(sizeof(edit_history));
    current->n1=n1;
    current->n2=n2;
    current->last_line=last_line_number;
    current->type_of_command=type_of_command;
    current->lines=lines;
    current->last=last_node;
    current->last_cur=last_m;
    current->last_file=last_f;
    current->prev=temp;
    current->next=NULL;
    if(temp!=NULL){
        temp->next=current;
    }
    if(temp==NULL){
        first=current;
    }
    last=current;
    empty=0;
}

line_I *move_edit(int dir, line_I *list){
    int count=dir;
    int count_int=0;
    int num=0;
    line_I *point=NULL, *temp=NULL, *line=NULL, *head=NULL, *prec=NULL, *cur=NULL;

    if((current==NULL)&&(first==NULL)){ // Abbiamo fatto una undo o una print o una redo prima di fare qualsiasi modifica, dunque move_edit non fa nulla
        return list;
    }

    if(dir>0){
        if((margin_right==0)&&(current->next==NULL)){ //Vuol dire che siamo in fondo alla struttura dei redo, dunque non ha senso chiamare redo
            return list;
        }
        if(current!=NULL){ //Se sono alla prima istruzione, non devo skipparla
            current=current->next;
        }
        if(current==NULL){ //avevo annullato tutti i comandi
            current=first;
            empty=0;
        }
        while(count>0){
            num=0;
            if(current->type_of_command=='c') { // Redo di change
                if (current->n1 == 1) { //La change avviene in testa alla lista
                    head = list;
                    point = list;
                    cur = current->lines;
                    list = current->lines;
                    if(point==NULL){ //La lista di output intermedio è vuota
                        while (cur->next != NULL) {
                            cur = cur->next;
                        }
                        cur->next=point;
                        current->lines=point; // Aggiorno puntatore current_lines
                        // last_line_number=current->n2;
                    }
                    else{
                        if((current->n2)>last_line_number){ // Caso 2 (1,2 c con una sola riga occupata)
                            /*count_int = (current->n2) - (current->n1);
                            while(count_int>0){
                                if(point->next==NULL){
                                    break;
                                }
                                point=point->next;
                                count_int--;
                            }
                            while (cur->next != NULL) {
                                cur = cur->next;
                            }*/
                            point=current->last_file;
                            cur=current->last_cur;
                            cur->next=point->next; // Attacca al punto giusto della lista (testa) la parte contenuta in current->lines
                            current->lines=head; // Aggiorno puntatore current_lines
                            current->last_cur=point;
                            current->last_file=cur;
                            // last_line_number=current->n2; // Aggiorno numero di righe occupate
                        }
                        else{ // Caso 3 (1,2 c con 3 righe occupate)
                            /*count_int = (current->n2) - (current->n1);
                            while(count_int>0){
                                if(point->next==NULL) break;
                                point=point->next;
                                count_int--;
                            }
                            while (cur->next != NULL) {
                                cur = cur->next;
                            }*/
                            point=current->last_file;
                            cur=current->last_cur;
                            cur->next=point->next; // Attacca al punto giusto della lista (testa) la parte contenuta in current->lines
                            temp=head;
                            current->lines=head;
                            /*count_int = (current->n2) - (current->n1);
                            while(count_int>0){
                                if(temp->next==NULL) break;
                                temp=temp->next;
                                count_int--;
                            }*/
                            //temp->next=NULL; // Concludo la lista in current->lines con un puntatore a NULL
                            point->next=NULL; // Concludo la lista in current->lines con un puntatore a NULL
                            current->last_file=cur;
                            current->last_cur=point;
                        }
                    }
                }
                else {
                    if(current->n1>last_line_number){ //La change avviene oltre la fine della lista
                        //point = positioning((current->n1)-1, list);
                        point=current->last_file;
                        point->next = current->lines;
                        current->lines = NULL;
                        // last_line_number = current->n2;
                    }
                    else if((current->n2)>(current->last_line)){ // Siamo nel caso di 2,8c con lista occupata fino a 6
                        point=positioning((current->n1)-1, list);
                        head=point->next;
                        point->next=current->lines;
                        current->lines=head;
                    }
                    else{ // La change avviene in mezzo alla lista (2,8c con lista occupata fino a 6)
                        point = positioning((current->n1) - 1, list);
                        head = point->next;
                        temp = point->next;
                        //cur = current->lines;
                        point->next = current->lines; // Attacco al posto giusto current->lines
                        /*while (cur->next != NULL) { //Mi porto alla fine di current_lines
                            cur = cur->next;
                        }
                        count_int = (current->n2) - (current->n1);
                        while (count_int>0) {
                            if(temp->next==NULL){ //Siamo nel caso 2,3 c con lista occupata fino a 2, dunque dobbiamo aggiornare last_line_number
                                // last_line_number=current->n2;
                                break;
                            }
                            temp = temp->next;
                            count_int--;
                        }*/
                        cur=current->last_cur;
                        temp=current->last_file;
                        cur->next = temp->next; // Chiudo la lista
                        current->lines = head;
                        /*count_int = (current->n2) - (current->n1);
                        temp=head;
                        while (count_int>0) {
                            if(temp->next==NULL) break;
                            temp = temp->next;
                            count_int--;
                        }*/
                        temp->next = NULL; // Chiudo la lista current_lines con un puntatore a NULL
                        current->last_file=cur;
                        current->last_cur=temp;
                    }

                }
            }
            if(current->type_of_command=='d'){ // Redo di delete
                if(list==NULL){ //La lista è vuota e la cancellazione non ha effetto
                    current->lines=NULL;
                }
                else if((current->n1==1)&&(list!=NULL)){ //La cancellazione deve avvenire in testa
                    /*point=list;
                    count_int = (current->n2) - (current->n1);
                    while(count_int>0){
                        if(point->next==NULL){ // Caso in cui delete non riesce a cancellare tutte le righe che dovrebbe perchè ce ne sono meno in list
                            break;
                        }
                        point=point->next;
                        count_int--;
                    }*/
                    point=current->last_cur;
                    temp=point->next;
                    point->next=NULL;
                    current->lines=list;
                    list=temp;
                    // last_line_number=last_line_number-((current->n2)-(current->n1)+1); // Aggiorno last_line_number
                    // if(last_line_number<0) last_line_number=0; // Il limite inferiore di last_line_number è 0
                }
                else{
                    if(current->n1>last_line_number){ //La cancellazione avviene oltre l'ultimo elemento della lista di output intermedio e dunque non ha effetto
                        current->lines=NULL;
                    }
                    else{ //La cancellazione avviene in un punto qualsiasi tranne che in testa
                        point=positioning(((current->n1)-1), list);
                        temp=point->next;
                        cur=point;
                        /*count_int = (current->n2) - (current->n1) + 1;
                        while(count_int>0){
                            if(cur->next==NULL){
                                // last_line_number=(current->n1)-1; //Siamo nel caso in cui delete non riesce a cancellare perchè ci sono meno elementi di n2 in list
                                break;
                            }
                            // else last_line_number = last_line_number - ((current->n2)-(current->n1)+1);
                            cur=cur->next;
                            count_int--;
                        }*/
                        cur=current->last_cur;
                        point->next=cur->next;
                        cur->next=NULL;
                        current->lines=temp; // Aggiorno puntatore current_lines

                    }
                }
            }
            margin_left++;
            if(margin_right>0){
                margin_right--;
            }
            if(current->next==NULL){ //Significa che ho fatto il numero massimo di redo possibili
                last_line_number=current->last_line;
                last_node=current->last;
                count=0;
            }
            else{
                if(count==1){ // Se siamo all'ultima iterazione non aumento current
                    last_line_number=current->last_line;
                    last_node=current->last;
                    count--;
                }
                else{
                    last_line_number=current->last_line;
                    //if(current->last!=NULL){
                    //    if(current->last->next!=NULL) current->last->next=NULL;
                    //}
                    last_node=current->last;
                    current=current->next; //Mi muovo verso destra nella struttura dati della redo
                    count--;
                }
            }
        }
    } //Mi devo muovere verso destra

    if(dir<0){
        if(margin_left==0){ // Siamo all'estremo sinistro della struttura degli undo
            empty=1;
            return NULL;
        }
        while(count!=0){
            num=0;
            if(current->type_of_command=='c'){ // Undo di change
                if(current->n1==1){ //La change è avvenuta in testa
                    point=list;
                    head=list;
                    temp=list;
                    cur=current->lines;
                    list=current->lines;
                    if(current->lines==NULL){ //La lista di output intermedio era vuota
                        list=NULL;
                        current->lines=point;
                        // last_line_number=0;
                    }
                    else{ // La change è avvenuta in testa ma la lista non era vuota
                        //while(cur->next!=NULL){
                        //    cur=cur->next;
                            //num++;
                        //}
                        cur=current->last_cur;
                        count_int=(current->n2)-(current->n1);
                        //while(count_int>0){
                        //    point=point->next;
                        //    count_int--;
                        //}
                        point=current->last_file;
                        cur->next=point->next;
                        current->lines=head;
                        //count_int=(current->n2)-(current->n1);
                        //while(count_int>0){
                        //    temp=temp->next;
                        //    count_int--;
                        //}
                        point->next=NULL;
                        current->last_file=cur;
                        current->last_cur=point;
                        // if(last_line_number>num+1) last_line_number=num+1; // Siamo nel caso in cui ho fatto una undo di 1,2c ma la lista era piena fino a 1, dunque devo aggiornare last_line_number
                    }
                }
                else{ //La change è avvenuta in un qualsiasi altro punto della lista
                    if(current->lines==NULL){ //La change era avvenuta in coda alla lista
                       point=current->last_file;
                       temp=point->next;
                       current->lines=temp;
                       point->next=NULL;
                       // last_line_number=(current->n1)-1;
                    }
                    else if((current->n2)>(current->last_line)){ // Siamo nel caso di 2,8c con lista occupata fino a 5
                        point=positioning((current->n1)-1, list);
                        temp=point->next;
                        point->next=current->lines;
                        current->lines=temp;
                    }
                    else{ // Siamo nel caso di 2,6c con lista occupata fino a 8
                        point=positioning((current->n1)-1, list);
                        temp=point->next;
                        //cur=current->lines;
                        head=temp;
                        point->next=current->lines;
                        /*count_int=(current->n2)-(current->n1);
                        while(count_int>0){
                            temp=temp->next;
                            count_int--;
                        }
                        while(cur->next!=NULL){
                            cur=cur->next;
                            //num++;
                        }*/
                        cur=current->last_cur;
                        temp=current->last_file;
                        cur->next=temp->next;
                        temp->next=NULL;
                        current->lines=head;
                        current->last_file=cur;
                        current->last_cur=temp;
                        // if((current->n2)-(current->n1)>num) last_line_number=(current->n1)+num;
                    }
                }
            }
            if(current->type_of_command=='d'){ // Undo di delete
                cur=current->lines;
                if((cur==NULL)&&(list==NULL)){
                    list=NULL; //la lista era vuota e la delete non ha avuto effetto, dunque lascio la lista vuota e non devo aggiungere nessuna stringa nella struttura della undo
                }
                else if(current->n1==1){ //La cancellazione era avvenuta in testa alla lista
                    point=list;
                    list=cur;
                    /*count_int=(current->n2)-(current->n1);
                    //if(count_int==0){
                        // last_line_number=current->n2; // Non ho capito a cosa corrisponde questo caso
                    //}
                    while(count_int>0){
                        if(cur->next==NULL){
                            break;
                        }
                        cur=cur->next;
                        count_int--;
                    }*/
                    // if(point==NULL) last_line_number=(current->n2)-count_int;
                    cur=current->last_cur;
                    cur->next=point;
                    current->lines=NULL;
                }
                else{
                    if(current->lines==NULL){ //La cancellazione è avvenuta oltre la fine della lista e dunque non ha avuto effetto
                        current->lines=NULL;
                    }
                    else{
                        point=positioning((current->n1)-1, list);
                        temp=point->next;
                        point->next=cur;
                        /*count_int=(current->n2)-(current->n1);
                        while(count_int>0){
                            if(cur->next==NULL){
                                // last_line_number=(current->n2)-count_int;
                                break;
                            }
                            cur=cur->next;
                            count_int--;
                        }*/
                        cur=current->last_cur;
                        cur->next=temp;
                        current->lines=NULL;
                    }
                }
            }
            margin_left--;
            margin_right++;
            if((current->prev==NULL)&&(count<0)){ // Sono arrivato all'estremo sinistro della struttura dati della undo
                current=NULL;
                last_line_number=0;
                last_node=NULL;
                empty=1;
                return NULL;
            }
            else{
                current=current->prev; //Mi muovo a sinistra nella struttura dati della undo
                last_line_number=current->last_line;
                last_node=current->last;
                count++;
            }
        }
    } //Mi devo muovere verso sinistra
    return list;
}

edit_history *clear_history(edit_history *point){
    edit_history *cur=NULL, *temp=NULL;

    if(point==NULL){ // Avevamo annullato anche il primo comando in history
        point=first;
    }
    cur=point->next;
    if((empty==1)&&(point->prev!=NULL)){
        empty=0;
    }
    while(cur!=NULL){ // Libera tutti i puntatori che puntano ai comandi fino ad ora eseguiti
        temp=cur;
        cur=cur->next;
        //free(temp);
    }
    if(empty==1){ //Significa che ho annullato tutti i comandi in history e dunque clear_history deve cancellare anche il primo
        temp=first;
        point=NULL;
        //free(temp);
    }
    else{
        point->next=NULL;
    }

    if(last_node!=NULL){ // Sistemo last_node
        if(last_node->next!=NULL) last_node->next=NULL;
    }

    return point;
}