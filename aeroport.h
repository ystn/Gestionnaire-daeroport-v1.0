#ifndef AEROPORT_H_INCLUDED
#define AEROPORT_H_INCLUDED

typedef struct compagnie
{
    char *nom;
    char acronyme[3];
}Compagnie;

typedef struct avion
{
    char identifiant[7];
    int carburant;
    int consommation;
    char heure_decollage[5];
    Compagnie *compagnie;
}Avion;

typedef struct cellule
{
    Avion *cle;
    struct cellule *suivant;
}Cellule;

typedef struct
{
    Cellule *tete;
    Cellule *queue;
}Liste;

typedef struct cellule_Compagnie
{
    Compagnie *cle;
    struct cellule_Compagnie *suivant;
}CelluleCompagnie;

typedef struct
{
    CelluleCompagnie *tete;
    CelluleCompagnie *queue;
}ListeCompagnie;

/*      liste des avions et avions      */
unsigned liste_vide_comp(ListeCompagnie *);
unsigned liste_vide(Liste *);
ListeCompagnie *creer_liste_comp();
Liste *creer_liste();
unsigned taille_liste_comp(ListeCompagnie *);
unsigned taille_liste_av(Liste *);
Cellule *recherche_id(Liste *, char *);
Cellule *recherche_pos(Liste *, unsigned);
unsigned recherche_pos_av(Liste *, Cellule *);
CelluleCompagnie *recherche_acr_comp(ListeCompagnie *, char *);
CelluleCompagnie *recherche_pos_comp(ListeCompagnie *, unsigned);
unsigned recherche_cell_pos_comp(ListeCompagnie *, CelluleCompagnie *);
void insere_comp(ListeCompagnie *, Compagnie *);
Compagnie *creer_compagnie(char *, char *);
void gerer_id(Avion *, unsigned);
void gerer_compagnie(Avion *, ListeCompagnie *, char *);
Avion *creer_avion(ListeCompagnie *, unsigned, int, int, char *, char *);
void insere_tete_av(Liste *, Avion *);
void insere_queue_av(Liste *, Avion *);
void insere_pos_av(Liste *, Avion *, unsigned);
void supprimer_compagnie(ListeCompagnie *, CelluleCompagnie *);
void supprimer_tete_av(Liste *);
void supprimer_queue_av(Liste *);
void suppression_pos_av(Liste *, unsigned);
void suppression_av_donne(Liste *, Cellule *);
void detruire(Liste *);
void detruire_comp(ListeCompagnie *);
void parcourir(HWND, Liste *, void (*)(HWND, Liste *, Cellule *, unsigned));
void parcourir_comp(HWND, ListeCompagnie *, void (*)(HWND, ListeCompagnie*, CelluleCompagnie *, unsigned));
void affiche_av(Liste *, Cellule *, unsigned);
void affiche_comp(ListeCompagnie *, CelluleCompagnie *, unsigned);
void affiche_liste_av(Liste *);
void affiche_liste_comp(ListeCompagnie *);
void supprimer_compagnie_av(Liste *, ListeCompagnie *, CelluleCompagnie *);
int listHeight(Liste *);
void enregistrer(const char *, const Liste *, const ListeCompagnie *);
void charger(const char *, Liste *, ListeCompagnie *);
void trierListe(Liste *);

#endif // AEROPORT_H_INCLUDED
