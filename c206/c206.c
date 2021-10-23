
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->firstElement = NULL; //nastaveni prvniho prvku na NULL
    list->lastElement = NULL; //nastaveni posledniho prvku na NULL
    list->activeElement = NULL; //nastaveni aktivniho prvku na NULL
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    //vytvoreni pomocneho prku na pomoc s vymazavanim
    struct DLLElement *current = NULL;
    current = list->lastElement; //nastaveni prvku na prvni prvek ze seznamu
    //kontrola prazdnosti seznamu
    if(list->firstElement == NULL){
        return;
    }
    //prochazeni seznamu dokud nebude prazdny
    while(current->previousElement != NULL){
        current = current->previousElement; //nastaveni aktualniho prvku na predchozi prvek
        free(current->nextElement); //vymazani prvku ktery je nejvice v pravo
    }
    free(current); //vymazani posledniho prvku
    list->lastElement = NULL; //nastaveni posledniho prvku na NULL
    list->firstElement = NULL; //nastaveni prvniho prvku na NULL
    list->activeElement = NULL; //nastaveni aktivniho prvku na NULL
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    struct DLLElement *newElement = (struct DLLElement*) malloc(sizeof(struct DLLElement)); //alokace pameti pro novy prvek
    //konrola alokace
    if(newElement == NULL){
        DLL_Error();
        return;
    }
    newElement->data = data; //vlozeni dat do noveho prvku
    newElement->nextElement = NULL; //nastaveni nasledujiciho prvku po novem prvku na NULL
    newElement->previousElement = NULL; //nastaveni predchoziho prvku noveho prvku na NULL
    //kdyz jeste v seznamu neni zadny prvek
    if(list->firstElement == NULL){
        list->firstElement = newElement; //nastavime novy prvek jako prvni prvek seznamu
        list->lastElement = newElement; //nastavime novy prvek jako posledni prvek seznamu
    } else {
        list->firstElement->previousElement = newElement; //nastavime novy prvek jako predchozi prvek prvniho prvku
        newElement->nextElement = list->firstElement; //nastavime nasledujici prvek noveho prvku
        list->firstElement = newElement; //nastavime novy prvek jako prvni v seznamu
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    struct DLLElement *newElement = (struct DLLElement*) malloc(sizeof(struct DLLElement)); //alokace pameti pro nevy prvek
    //kontrola alokace
    if(newElement == NULL){
        DLL_Error();
        return;
    }
    newElement->data = data; //vlozeni dat do noveho prvku
    newElement->nextElement = NULL; //nastaveni nasledujiciho prvku na NULL
    newElement->previousElement = NULL; //nastaveni predchoziho prvku na null
    //je li seznam prazdny
    if(list->firstElement == NULL){
        list->firstElement = newElement; //nastavime novy prvek jako prvni prvek
        list->lastElement = newElement; //nastavime novy prvek jako posledni prvek
    } else {
        newElement->previousElement = list->lastElement; //nastaveime prvek po novem prvku posledni prvek
        list->lastElement->nextElement = newElement; //nastavime predchozi prvek posledniho prvku novy prvek
        list->lastElement = newElement; //nastavime novy prvek jako posleni
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list->activeElement = list->firstElement; //nastavime prvni prvek jako aktivn9
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    list->activeElement = list->lastElement; //natavime posledni prvek jako aktini
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    //z kontrolujeme jestli seznam neni prazdny
    if(list->firstElement == NULL){
        DLL_Error();
        return;
    }
    *dataPtr = list->firstElement->data; //vlozime do ukazatele data prvniho prvku
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    //zkontorlujeme jestli seznam neni prazdny
    if(list->lastElement == NULL){
        DLL_Error();
        return;
    }
    *dataPtr = list->lastElement->data; //vlozime do ukazatele data posledniho prvku
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    struct DLLElement *delElement = NULL; //vytvorime si prvek na vymazavani
    if(list->firstElement == NULL){
        return;
    }
    //pokud je prvni prvek aktivni
    if(list->firstElement == list->activeElement){
        list->activeElement = NULL; //nastavime aktivni prvek na NULL
    }
    delElement = list->firstElement; //nastavime prvni prvek na prvek na vymazani
    //pokud ma seznam pouze jeden prvek
    if(list->firstElement->nextElement == NULL){
        free(delElement); //vymazeme prvek
        list->firstElement = NULL; //nastavime prvni prvek na NULL
        list->lastElement = NULL; //nastavime posledni prvek na NULL
    }else{
        list->firstElement = list->firstElement->nextElement; //nastavime druhy prvek jako prvni
        free(delElement);//vymazeme prvni prvek
        list->firstElement->previousElement = NULL; //nastavime predchozi prvek prvniho prvku na NULL
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    struct DLLElement *delElement = NULL; //vytvorime si prvek na vymazavani
    //zkontorlujeme jestli seznam neni prazdny
    if(list->lastElement == NULL){
        return;
    }
    //pokud je posledni prvek aktivni
    if(list->lastElement == list->activeElement){
        list->activeElement = NULL; //nastavime aktivni na NULL
    }
    delElement = list->lastElement; //vlozime posledni prvek do prvku na vymazani
    //pokud je v sezmanu pouze jeden prvek
    if(list->lastElement->previousElement == NULL){
        free(delElement); //vymazeme posledni prvek
        list->firstElement = NULL; //nastavime prvni prvek na NULL
        list->lastElement = NULL; //nastavime posledni prvek na NULL
    }else{
        list->lastElement = list->lastElement->previousElement; //nastavime predposledni prvek jako posledni prvek
        free(delElement); //vymazeme posledni prvek
        list->lastElement->nextElement = NULL; //nastavime nasledujici prvek po poslednim jako NULL
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    struct DLLElement *delElement = NULL; //nastavime prvek na vymazavani
    //pokud nemame aktivni prvek nebo nasledujici prvek po aktivnim
    if(list->activeElement == NULL || list->activeElement->nextElement == NULL){
        return;
    }
    delElement = list->activeElement->nextElement; //nastavime prvek na vymazani jako nasledujici po aktivnim
    //kdyz je prvek na vymazani poslednim prvkem
    if(delElement == list->lastElement){
        list->lastElement = list->activeElement; //nastavime aktivni prvek jako posledni
        list->activeElement->nextElement = NULL; //nastavime nasledujici prvek po poslednim na NULL
    }else{
        list->activeElement->nextElement = list->activeElement->nextElement->nextElement; //nastavime nasledujici prvek aktivvniho prvku prvek za vymazanym prvkem
        list->activeElement->nextElement->nextElement->previousElement = list->activeElement; //nastavime predchoziho prvku za vymazanym prvkem aktivni prvek
    }
    free(delElement); //vymazeme prvek
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    struct DLLElement *delElement = NULL; //vytvorime prvek pro vymazani
    //zjistujeme jestli mame aktivni prvek a jestli aktivni prvek neni prvnim prvkem
    if(list->activeElement == NULL || list->activeElement->previousElement == NULL){
        return;
    }
    delElement = list->activeElement->previousElement; //nastavime predchozi prvek po aktivnim jako prvek pro vymazani
    //pokud je prvek ktery se ma vymazat prvni prvek
    if(delElement == list->firstElement){
        list->firstElement = list->activeElement;//nastavime aktivni prvek jako prvni prvek
        list->activeElement->previousElement = NULL;//nastavime predchozi prvek aktivniho prvku na NULL
    }else{
        list->activeElement->previousElement = list->activeElement->previousElement->previousElement; //nastavime predchozi prvek aktivniho prvku prvek pred prvkem ktery se ma vymazat
        list->activeElement->previousElement->previousElement->nextElement = list->activeElement; //nastavime nasledujici prvek prvku pred prvkem tkety jsme vymazali aktivni prvek
    }
    free(delElement); //vymazeme prvek
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    //zkontrolujeme jestli mame aktivni prvek
    if(list->activeElement == NULL){
        return;
    }
    struct DLLElement *newElement = (struct DLLElement*) malloc(sizeof(struct DLLElement)); //alokujeme novy prvek
    //zkontrolujeme alokaci prvku
    if(newElement == NULL){
        DLL_Error();
        return;
    }
    newElement->data = data;//vlozime data do noveho prvku
    //pokud je aktivni prvek posledni
    if(list->activeElement == list->lastElement){
        list->activeElement->nextElement = newElement; //nastavime nasledujici prvek po aktivnim prvku novy prvek
        newElement->previousElement = list->activeElement; //nastavime nasledujici prvek po novem prvku aktivni prvek
        list->lastElement = newElement; //nastavime novy prvek jako posledni prvek
        newElement->nextElement = NULL; //nasledujici prvek po novem prvku NULL
    }else{
        newElement->nextElement = list->activeElement->nextElement; //nastavime nasledujici prvek po novem prvku nasledujici po aktivnim prvkem
        list->activeElement->nextElement = newElement; //nastavime nasledujici prvek po aktivnim novy prvek
        newElement->previousElement = list->activeElement; //nastavime predchozi prvek noveho prvku aktivni prvek
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    //pokud nemame aktivni prvek
    if(list->activeElement == NULL){
        return;
    }
    struct DLLElement *newElement = (struct DLLElement*) malloc(sizeof(struct DLLElement)); //alokujeme novy prvek
    //zkontrolujeme alokaci noveho prvku
    if(newElement == NULL){
        DLL_Error();
        return;
    }
    newElement->data = data; //vlozime data do noveho prvku
    //pokud je aktivni prvek prvni prvek v seznamu
    if(list->activeElement == list->firstElement){
        list->activeElement->previousElement = newElement; //nastavime predchozi prvek aktivniho prvku novy prvek
        newElement->nextElement = list->activeElement; //nastavime nasledujici prvek noveho prvku aktivni prvek
        newElement->previousElement = NULL; //nastavime predchozi prvek noveho prvku NULL
        list->firstElement = newElement; //nastavime novy prvek jako prvni prvek
    }else{
        newElement->previousElement = list->activeElement->previousElement; //nastavime prvek pred novim prvkem
        list->activeElement->previousElement = newElement; //nastavime prvek pred aktivnim prvkem
        newElement->nextElement = list->activeElement; //nastavime prvek po novem prvku
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    //pokud mame aktivni prvek
    if(list->activeElement == NULL){
        DLL_Error();
        return;
    }
    *dataPtr = list->activeElement->data; //navteme data z aktivniho prvku
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    //pokud nemame aktivni prvek
    if(list->activeElement == NULL){
        return;
    }
    list->activeElement->data = data; //prepiseme obsah aktivniho prvku
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    //pokud mame aktivni prvek
    if(list->activeElement == NULL){
        return;
    }
    list->activeElement = list->activeElement->nextElement; //nastavime aktivni prvek na prvek nasledujici
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    //kdyz je prvek aktivni
    if(list->activeElement == NULL){
        return;
    }
    list->activeElement = list->activeElement->previousElement; //nastavime aktivitu na predchozi prvek
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return list->activeElement != NULL; //zjistime jestli mame aktivni prvek
}

/* Konec c206.c */
