#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include "aeroport.h"

unsigned liste_vide_comp(ListeCompagnie *liste)
{
    return !(liste->tete || liste->queue);
}

unsigned liste_vide(Liste *liste)
{
    return !(liste->tete || liste->queue);
}

ListeCompagnie *creer_liste_comp()
{
    ListeCompagnie *liste = (ListeCompagnie *) malloc(sizeof(ListeCompagnie));
    liste->tete = NULL;
    liste->queue = NULL;
    return liste;
}

Liste *creer_liste()
{
    Liste *liste = (Liste *) malloc(sizeof(Liste));
    liste->tete = NULL;
    liste->queue = NULL;
    return liste;
}

unsigned taille_liste_comp(ListeCompagnie *liste)
{
    CelluleCompagnie *c;
    unsigned length;
    for(c = liste->tete, length = 0; c; c = c->suivant, length++);
    return length;
}

unsigned taille_liste_av(Liste *liste)
{
    Cellule *c;
    unsigned length;
    for(c = liste->tete, length = 0; c; c = c->suivant, length++);
    return length;
}

Cellule *recherche_cellule(Liste *liste, bool (*oper)(Cellule *, unsigned))
{
    if(!liste_vide(liste))
    {
        Cellule *c;
        unsigned i;
        for(c = liste->tete, i=0; c && (*oper)(c,i); c = c->suivant, i++);
        return c;
    }
    return NULL;
}

Cellule *recherche_id(Liste *liste, char *id)
{
    if(!liste_vide(liste))
    {
        Cellule *c;
        for(c = liste->tete; c && strcmp(c->cle->identifiant, id); c = c->suivant);
        return c;
    }
    return NULL;
}

Cellule *recherche_pos(Liste *liste, unsigned pos)
{
    if(!liste_vide(liste))
    {
        Cellule *c;
        unsigned i;
        for(c = liste->tete, i=0; c && i<pos; c = c->suivant, i++);
        return c;
    }
    return NULL;
}

unsigned recherche_pos_av(Liste *liste, Cellule *c)
{
    if(!liste_vide(liste))
    {
        Cellule *c1;
        unsigned i;
        for(c1 = liste->tete, i = 0; c1 && c1 != c; c1 = c1->suivant, i++);
        return i;
    }
}

CelluleCompagnie *recherche_acr_comp(ListeCompagnie *liste, char *acr)
{
    if(!liste_vide_comp(liste))
    {
        CelluleCompagnie *c;
        for(c = liste->tete; c && strncmp(c->cle->acronyme,acr,3); c = c->suivant);
        return c;
    }
    return NULL;
}

CelluleCompagnie *recherche_pos_comp(ListeCompagnie *liste, unsigned pos)
{
    if(!liste_vide_comp(liste))
    {
        CelluleCompagnie *c;
        unsigned i;
        for(c = liste->tete, i=0; c && i<pos; c = c->suivant);
        return c;
    }
    return NULL;
}

unsigned recherche_cell_pos_comp(ListeCompagnie *liste, CelluleCompagnie *c)
{
    if(!liste_vide_comp(liste))
    {
        CelluleCompagnie *c1;
        unsigned i;
        for(c1 = liste->tete, i = 0; c1 && c1 != c; c1 = c1->suivant, i++);
        return i;
    }
}

CelluleCompagnie *creer_cellule_comp(Compagnie *comp)
{
    CelluleCompagnie *c = (CelluleCompagnie *) malloc(sizeof(CelluleCompagnie));
    c->cle = comp;
    c->suivant = NULL;
    return c;
}

void insere_comp(ListeCompagnie *liste, Compagnie *comp)
{
    CelluleCompagnie *c = creer_cellule_comp(comp);
    if(c)
    {
        if(!liste)
            liste = creer_liste_comp();
        if(liste_vide(liste))
        {
            liste->tete = c;
            liste->queue = c;
        }else
        {
            liste->queue->suivant = c;
            liste->queue = c;
        }
    }
}

void fixation_acronyme(char *acronyme)
{
    for(unsigned i = 0; acronyme[i]!='\0' && i<3; i++)
        {
            if(islower(acronyme[i]))
                acronyme[i] = toupper(acronyme[i]);
        }
}

Compagnie *creer_compagnie(char *nom, char *acr)
{
    Compagnie *c = (Compagnie *) malloc(sizeof(Compagnie));
    if(c)
    {
        fixation_acronyme(acr);
        (c->nom) = (char *) malloc(strlen(nom)*sizeof(char));
        strcpy(c->nom,nom);
        strncpy(c->acronyme,acr,3);
        return c;
    }
}

void gerer_id(Avion *av, unsigned numero)
{
    unsigned l = floor(log10(abs(numero)))+1;
    if(l == 3)
    {
        char *numero_str = (char *) malloc(sizeof(char));
        itoa(numero,numero_str,10);
        strncpy((av->identifiant)+3,numero_str,4);
        strncpy((av->identifiant)+6,"\0",1);
        //free(numero_str);
    }
}

void gerer_compagnie(Avion *av, ListeCompagnie *liste, char *acronyme)
{
    fixation_acronyme(acronyme);
    strncpy(av->identifiant,acronyme,3);
    CelluleCompagnie *c = recherche_acr_comp(liste,acronyme);
    Compagnie *compagnie;
    if(c)
    {
        compagnie = c->cle;
        av->compagnie = compagnie;
    }
}

Avion *creer_avion(ListeCompagnie *liste_comp, unsigned numero, int carburant, int consommation, char *heure, char *acronyme)
{
    Avion *av = (Avion *) malloc(sizeof(Avion));
    gerer_compagnie(av,liste_comp,acronyme);
    gerer_id(av,numero);
    av->carburant = carburant;
    av->consommation = consommation;
    strncpy(av->heure_decollage,heure,4);
    return av;
}

Cellule *creer_cellule(Avion *av)
{
    Cellule *c = (Cellule *) malloc(sizeof(Cellule));
    c->cle = av;
    c->suivant = NULL;
    return c;
}

void insere_tete_av(Liste *liste, Avion *av)
{
    if(liste)
    {
        Cellule *c = creer_cellule(av);
        if(c)
        {
            c->suivant = liste->tete;
            if(liste_vide(liste))
            {
                liste->tete = c;
                liste->queue = c;
            }else
            {
                liste->tete = c;
            }
        }
    }
}

void insere_queue_av(Liste *liste, Avion *av)
{
    if(liste)
    {
        Cellule *c = creer_cellule(av);
        if(c)
        {
            if(liste_vide(liste))
                insere_tete_av(liste,av);
            else
            {
                liste->queue->suivant = c;
                liste->queue = c;
            }
        }
    }
}

void insere_pos_av(Liste *liste, Avion *av, unsigned pos)
{
    if(liste)
    {
        Cellule *c = creer_cellule(av);
        if(c)
        {
            if(liste_vide(liste) || pos == 0)
                insere_tete_av(liste,av);
            else if(pos >= taille_liste_av(liste))
                insere_queue_av(liste,av);
            else
            {
                Cellule *avant_c = recherche_pos(liste,pos-1);
                c->suivant = avant_c->suivant;
                avant_c->suivant = c;
            }
        }
    }
}

void supprimer_compagnie(ListeCompagnie *liste, CelluleCompagnie *c)
{
    if(liste && !liste_vide(liste))
    {
        unsigned pos = recherche_cell_pos_comp(liste,c);
        if(liste->tete == liste->queue)
            supprimer_tete_av(liste);
        else if(pos == 0)
        {
            if(liste->tete == liste->queue)
            {
                free(liste->tete);
                liste->tete = NULL;
                liste->queue = NULL;
            }else
            {
                liste->tete = c->suivant;
                free(c);
            }
        }
        else if(pos >= taille_liste_comp(liste)-1)
        {
            if(liste->tete == liste->queue)
                supprimer_tete_av(liste);
            else
            {
                Cellule *c = recherche_cell_pos_comp(liste,taille_liste_av(liste)-2);
                c->suivant = NULL;
                free(liste->queue);
                liste->queue = c;
            }
        }
        else
        {
            CelluleCompagnie *c1 = recherche_pos_comp(liste,pos-1);
            c1->suivant = c->suivant;
            free(c1);
        }
    }
}

void supprimer_tete_av(Liste *liste)
{
    if(liste && !liste_vide(liste))
    {
        if(liste->tete == liste->queue)
        {
            free(liste->tete);
            liste->tete = NULL;
            liste->queue = NULL;
        }else
        {
            Cellule *c = liste->tete;
            liste->tete = c->suivant;
            free(c);
        }
    }
}

void supprimer_queue_av(Liste *liste)
{
    if(liste && !liste_vide(liste))
    {
        if(liste->tete == liste->queue)
            supprimer_tete_av(liste);
        else
        {
            Cellule *c = recherche_pos(liste,taille_liste_av(liste)-2);
            c->suivant = NULL;
            free(liste->queue);
            liste->queue = c;
        }
    }
}

void suppression_pos_av(Liste *liste, unsigned pos)
{
    if(liste && !liste_vide(liste))
    {
        if(liste->tete == liste->queue)
            supprimer_tete_av(liste);
        else if(pos == 0)
            supprimer_tete_av(liste);
        else if(pos == taille_liste_av(liste)-1)
            supprimer_queue_av(liste);
        else if(pos>0 && pos<taille_liste_av(liste)-1)
        {
            Cellule *c = recherche_pos(liste,pos-1);
            Cellule *c1 = c->suivant;
            c->suivant = c1->suivant;
            free(c1);
        }
    }
}

void suppression_av_donne(Liste *liste, Cellule *c)
{
    if(liste && !liste_vide(liste))
    {
        unsigned pos = recherche_pos_av(liste,c);
        if(pos<taille_liste_av(liste))
            suppression_pos_av(liste,pos);
    }
}

void detruire(Liste *liste)
{
    while(!liste_vide(liste))
        supprimer_tete_av(liste);
}

void detruire_comp(ListeCompagnie *liste)
{
    while(!liste_vide_comp(liste))
        supprimer_compagnie(liste,liste->tete);
}

void parcourir(HWND hWnd, Liste *liste, void (*oper)(HWND, Liste *, Cellule *, unsigned))
{
    Cellule *c;
    unsigned i;
    if(!liste_vide(liste))
        for(c = liste->tete, i = 0; c; c = c->suivant, i++)
            (*oper)(hWnd,liste, c, i);
}

void parcourir_comp(HWND hWnd, ListeCompagnie *liste, void (*oper)(HWND hWnd, ListeCompagnie *, CelluleCompagnie *, unsigned))
{
    CelluleCompagnie *c;
    unsigned i;
    if(!liste_vide_comp(liste))
        for(c = liste->tete, i = 0; c; c = c->suivant, i++)
            (*oper)(hWnd,liste, c, i);
}

void supprimer_compagnie_av(Liste *liste, ListeCompagnie *liste_comp, CelluleCompagnie *c)
{
    Cellule *c1;
    int x;
    do
    {
        for(x = 0,c1 = liste->tete; c1; c1 = c1->suivant)
    {
        if(c1->suivant->cle->compagnie == c->cle)
        {
            suppression_av_donne(liste,c1->suivant);
            x++;
        }
    }
    }while(x);
    supprimer_compagnie(liste_comp,c);
}

int listHeight(Liste *liste)
{
    int i;
    i = (30+((taille_liste_av(liste)+2)*25));
    return i;
}

FILE *fp;
void enregistrer_av(HWND hWnd, Liste *liste, Cellule *c, unsigned i)
{
    char *nom, heure[5], acronyme[4];
    Avion *av = c->cle;
    strncpy(heure, av->heure_decollage, 4);
    heure[4] = '\0';
    strncpy(acronyme,av->compagnie->acronyme,3);
    acronyme[3] = '\0';
    fprintf(fp,"%s;%d;%d;%s;%s\n",av->identifiant,av->carburant,av->consommation,heure,acronyme);
}

void enregistrer_comp(HWND hWnd, ListeCompagnie *liste, CelluleCompagnie *c, unsigned i)
{
    Compagnie *comp = c->cle;
    char acr[4];
    strncpy(acr,comp->acronyme,3);
    acr[3] = '\0';
    fprintf(fp,"%s;%s\n",comp->nom,acr);
}

void enregistrer(const char *fichier, const Liste *liste, const ListeCompagnie *liste_comp)
{
    fp = fopen(fichier,"w");
    if(fp)
    {
        parcourir_comp(NULL,liste_comp,enregistrer_comp);
        fprintf(fp,"liste\n");
        parcourir(NULL,liste,enregistrer_av);
        fclose(fp);
    }
}

Avion *charger_av(ListeCompagnie *liste_comp, char *ligne)
{
    char heure[5], acronyme[4], id[7], stock[10], *temp, *temp2;
    int carburant, consommation, i;
    unsigned numero;
    strncpy(id,ligne,6);
    id[6] = '\0';
    temp = ligne+7;
    temp2 = strchr(temp,';');
    i = strlen(temp)-strlen(temp2);
    strncpy(stock,temp,i);
    stock[i] = '\0';
    carburant = atoi(stock);
    temp = strchr(temp2+1,';');
    i = strlen(temp2+1)-strlen(temp);
    strncpy(stock,temp2+1,i);
    stock[i] = '\0';
    consommation = atoi(stock);
    strncpy(heure,temp+1,4);
    heure[4] = '\0';
    strncpy(acronyme,temp+6,3);
    acronyme[3] = '\0';
    numero = atoi(id+3);
    Avion *av = creer_avion(liste_comp,numero,carburant,consommation,heure,acronyme);
    return(av);
}

Compagnie *charger_comp(const char *ligne)
{
    char *nom, acronyme[4], *temp;
    int i;
    Compagnie *comp;
    temp = strchr(ligne,';');
    i =strlen(ligne)-strlen(temp);
    nom = malloc((i+1)*sizeof(char));
    strncpy(nom,ligne,i);
    nom[i]='\0';
    strncpy(acronyme,temp+1,3);
    acronyme[3]='\0';
    comp = creer_compagnie(nom,acronyme);
    return comp;
}

void charger(const char *fichier, Liste *liste, ListeCompagnie *liste_comp)
{
    fp = fopen(fichier,"r");
    if(fp)
    {
        char ligne[150];
        fgets(ligne,150,fp);
        while(strcmp(ligne,"liste\n")&&!feof(fp))
        {
            Compagnie *comp;
            comp = charger_comp(ligne);
            CelluleCompagnie *c = creer_cellule_comp(comp);
            insere_comp(liste_comp,comp);
            fgets(ligne,150,fp);
        }
        fgets(ligne,150,fp);
        while(!feof(fp))
        {
            Avion *av = charger_av(liste_comp,ligne);
            insere_queue_av(liste,av);
            fgets(ligne,150,fp);
        }
    }
}

Cellule *selectionMin(Liste *liste, unsigned de, unsigned jusq)
{
    Cellule *min;
    unsigned i;
    Cellule *c;
    for(c = liste->tete,i = 0; c && i<taille_liste_av(liste) && i<jusq;c = c->suivant,i++)
    {
        if(i<de)
            min = c;
        else
        {
            if(min->cle->consommation>c->cle->consommation)
                min = c;
        }
    }
    return min;
}

void trierListe(Liste *liste)
{
    unsigned i;
    Avion *av;
    Cellule *c;
    for(i = taille_liste_av(liste);i>0;i--)
    {
        c = selectionMin(liste,1,i);
        av = c->cle;
        insere_pos_av(liste,av,i);
        suppression_av_donne(liste,c);
    }
}
