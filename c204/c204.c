
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char elNow; //deklarace promenne pro element z vrcholu zasobniku
    Stack_Top(stack, &elNow); //vlozeni prvku z vrcholu do promenne elNow
    //prochazi zasobnik dokud nenarazi na oteviraci zavorku
    while(elNow != '('){
        postfixExpression[*postfixExpressionLength] = elNow; //vlozeni prvku z vrcholu zasobniku do vysledneho postfixoveho vyrazu s pouzitim delky postfixoveho vyrazu
        (*postfixExpressionLength)++;
        Stack_Pop(stack); //odstraneni prvku z vrcholu zasobniku
        Stack_Top(stack, &elNow); //nacteni prvku ze zasobniku
    }
    Stack_Pop(stack); //vymazani oteviraci zavoky
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    //kontrola prazdnosti zasobniku
    if(Stack_IsEmpty(stack)){
        Stack_Push(stack, c);
        return;
    }
    char last; //deklarace veliciny pro vrchni prvek ze zasoprniku
    Stack_Top(stack, &last); // nacteni vrchniho prvku
    //vyrazeni moznosti oteviraci zavorky
    if(last == '('){
        Stack_Push(stack, c); //vlozeni oteviraci zavotky do zasobniku
        return;
    }
    //podminka pro prvky s vetsi prioritou
    if ((last == '+' || last == '-') && (c == '*' || c == '/')){
        Stack_Push(stack, c); //vlozeni prvku s vetsi prioritou do zasobniku
        return;
    }
    postfixExpression[*postfixExpressionLength] = last; //vlozeni prvku s mensi prioritou z vrcholu zasobniku do retezce postfixoveho vyrazu
    (*postfixExpressionLength)++;
    Stack_Pop(stack); //vuymazani prvku

    doOperation(stack, c, postfixExpression, postfixExpressionLength); //rekurzivni volani dokud prvek nebude mit vetsi prioritu nebo nebude prvek na vrcholu zavorka
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    Stack *stack = (Stack *)malloc(sizeof(Stack)); //alokace zasobniku na odkarani operatoru
    //kontrola alokace
    if(stack == NULL) {
        return NULL;
    }
    char *postfixExpression = (char *)malloc(sizeof(char)*MAX_LEN);//alokace znakoveho retezce pro postfixovy vyrazu
    //kontrola alokace
    if(postfixExpression == NULL){
        free(stack); //pri zpatne alokaci odstaneni zasobniku
        return NULL;
    }
    //inicializace zasobniku
    Stack_Init(stack);
    int i = 0; //promenna pro postupne vybirani znaku z pole
    unsigned int j = 0; //promenna pro vkladani do pole po znaku
    char item = infixExpression[i];//vlozeni prvniho znaku z pole
    //prochazeni celeho pole dokud v nem bude znak
    while(item != '\0') {
        //pokud je prvek z infixoveho vyrazu otevviraci zavorka
        if (item == '(') {
            Stack_Push(stack, item); //vlozime znak do zasobniku
        } else if ((item >= '0' && item <= '9') || (item >= 'a' && item <= 'z') || (item >= 'A' && item <= 'Z')) { //kdyz bude znak cislo male nebo velke pismeno
            postfixExpression[j] = item;//vlozime znak do postfixove vyrazu
            j++;
        } else if (item == '+' || item == '-' || item == '*' || item == '/') { //pokud bude znak operator
            doOperation(stack, item, postfixExpression, &j); //zavolame funkci pro vyhodnocovani operatoru a predame ji aktualni operator
        } else if (item == ')') { //kdyz bude znak uzaviraci zavoka
            untilLeftPar(stack, postfixExpression, &j); //zavolame fukci pro vyprazneni zasobniku pro oteviraci zavorku
        }
        i++;
        item = infixExpression[i]; //vlozime si dalsi znak v poradi do veliciny item
    }
    //vypradneni zasobniku dokud nebude prazny
    while(!Stack_IsEmpty(stack)){
        Stack_Top(stack, &postfixExpression[j]); //vlozeni prvku z vrcholu zasoniku do postfixoveho vyrazu
        j++;
        Stack_Pop(stack); //vymazani prvku z vrcholu zasobniku
    }
    postfixExpression[j] = '='; //vlozeni rovna se do postfixoveho vyrazu
    j++;
    postfixExpression[j] = '\0'; //zakonceni postccixoveho vyrazu znakem \0
    free(stack); //odstaneni zasobniku
    return postfixExpression; //vraceni postfixoveho vyrazu
}

/* Konec c204.c */
