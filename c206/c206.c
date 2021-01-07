
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

//      implementace: Ladislav Dokoupil <xdokou14@stud.fit.vutbr.cz>, říjen 2020

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    //ukazatele nastavim na NULL
    L->Act = NULL;
    L->Last = NULL;
    L->First = NULL;

}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    //pruchod seznamem s postupnym mazanim prvku
    while (L->First != NULL){
        tDLElemPtr temp = L->First;
        L->First = L->First->rptr;
        free(temp);
    }

    //ukazatele nastavim na NULL
    L->Act = NULL;
    L->Last = NULL;
    L->First = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokace a kontrola
    tDLElemPtr elem = malloc(sizeof(struct tDLElem));
    if(!elem){
        DLError();
        return;
    }
    //nastaveni dat noveho prvku
    elem->data = val;
    elem->lptr = NULL;
    elem->rptr = L->First;

    //pokud neni prvek jediny nastavi levy ukazatel byvaleho prvniho prvku na novy
    if(L->First)
        L->First->lptr = elem;

    //pokud se jedna o jediny prvek je i posledni
    if(!L->Last)
        L->Last = elem;

    //uprava ukazatele na novy prvni prvek
    L->First = elem;

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokace a kontrola
    tDLElemPtr elem = malloc(sizeof(struct tDLElem));
    if(!elem){
        DLError();
        return;
    }
    //nastaveni dat noveho prvku
    elem->data = val;
    elem->lptr = L->Last;
    elem->rptr = NULL;

    //pokud neni prvek jediny nastavi pravy ukazatel byvaleho posledniho prvku na novy
    if(L->Last)
        L->Last->rptr = elem;
    //pokud se jedna o jediny prvek je i prvni
    if(!L->First)
        L->First = elem;
    //uprava ukazatele na novy posledni prvek
    L->Last = elem;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
 L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
 L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

//seznam je prazdny -> chyba
if(!L->First){
    DLError();
    return;
}

 *val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

//seznam je prazdny -> chyba
if(!L->Last){
    DLError();
    return;
}

 *val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

tDLElemPtr first = L->First;

//seznam je prazdny
if(!first)
    return;

// pokud je prvni prvek aktualni/posledni nastavim na NULL
if(first == L->Act)
    L->Act = NULL;
if(first == L->Last)
    L->Last = NULL;
else{
    //seznam obsahuje vice prvku
    //levy ukazatel 2. prvku nastavim na NULL
     first->rptr->lptr = NULL;
}
//uprava ukazatelu a uvolneni pameti
L->First = L->First->rptr;
free(first);

}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/

    tDLElemPtr last = L->Last;

    //seznam je prazdny
    if(!last)
        return;

    // pokud je posledni prvek aktualni/prvni nastavim na NULL
    if(last == L->Act)
        L->Act = NULL;
    if(L->First == L->Last)
        L->First = NULL;
    else{
        //seznam obsahuje vice prvku
        //pravy ukazatel predposledniho prvku nastavim na NULL
        last->lptr->rptr = NULL;
    }

    //uprava ukazatelu a uvolneni pameti
    L->Last = L->Last->lptr;
    free(last);
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    //prvek neni aktivni, nebo je posledni
	if(!L->Act ||  L->Act == L->Last)
        return;

    tDLElemPtr delElem = L->Act->rptr;
    tDLElemPtr rElem = delElem->rptr;

	//odstranovy prvek je posledni
	if(delElem == L->Last)
	    L->Last = L->Act;
	//prvek za odstranovanym existuje
	if(rElem)
        rElem->lptr = L->Act;

    //uprava ukazatelu a uvolneni pameti
    L->Act->rptr = rElem;
    free(delElem);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/


    //prvek neni aktivni, nebo je prvni
    if(!L->Act ||  L->Act == L->First)
        return;

    tDLElemPtr delElem = L->Act->lptr;
    tDLElemPtr lElem = delElem->lptr;
    //odstranovy prvek je prvni
    if(delElem == L->First)
        L->First = L->Act;
    //prvek pred odstranovanym existuje
    if(lElem)
        lElem->rptr = L->Act;
    //uprava ukazatelu a uvolneni pameti
    L->Act->lptr = lElem;
    free(delElem);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

//seznam neni aktivni
if(!L->Act)
    return;

//alokace a kontrola
tDLElemPtr elem = malloc(sizeof(struct tDLElem));
if(!elem){
    DLError();
    return;
}

//nastaveni dat noveho prvku
elem->data = val;
elem->lptr = L->Act;
elem->rptr = L->Act->rptr;
//uprava ukazatele praveho prvku
if(elem->rptr)
    elem->rptr->lptr = elem;
//uprava ukazatele leveho prvku
L->Act->rptr = elem;

//prvek byl jediny
if(L->Act == L->Last)
    L->Last = elem;

}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //seznam neni aktivni
    if(L->Act)
        return;

    //alokace a kontrola
    tDLElemPtr elem = malloc(sizeof(struct tDLElem));
    if(!elem){
        DLError();
        return;
    }
    //nastaveni dat noveho prvku
    elem->data = val;
    elem->rptr = L->Act;
    elem->lptr = L->Act->lptr;
    //uprava ukazatele leveho prvku
    if(elem->lptr)
        elem->lptr->rptr = elem;
    //uprava ukazatele praveho prvku
    L->Act->lptr = elem;

    //prvek byl jediny
    if(L->Act == L->First)
        L->First = elem;

}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

//seznam neni aktivni
if(!L->Act){
    DLError();
    return;
}
*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if(L->Act)
        L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
 if(L->Act)
     L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    if(L->Act)
        L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return L->Act != NULL;
}

/* Konec c206.c*/
