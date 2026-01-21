/////////////////////////////
//////// Jeu 2048 ///////////
// Par : Viswadevi en 2024 //
/////////////////////////////

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#define BOARD_SIZE 4

// initialisation du terminal
void INIT_2048() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    srand(time(NULL));
}

// rétablir le terminal
void DONE_2048() {
    endwin();
    exit(0);
}

typedef struct {
    int value;
} Case;

Case board[BOARD_SIZE][BOARD_SIZE];
int score = 0; // initialisée à zéro pour stocker le score du joueur

// rempli le plateau de cases vides
void init_board(Case board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].value = 0;
        }
    }
}

// affichage du plateau de jeu, ainsi qu’une bannière avec le nom du jeu et le score
void display_board() {
    clear();
    move(0, 0); // déplace le curseur dans le coin supérieur gauche du terminal
    printw("== 2048 =============================\n");
    printw("========================== score: %d\n", score);
    for (int i = 0; i < BOARD_SIZE; i++) {
        printw("+--------+--------+--------+--------+ \n");
        printw("|        |        |        |        | \n"); // 1e | de la ligne
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].value == 0)
                printw("|        ");
            else if (board[i][j].value < 10)
                printw("|    %d   ", board[i][j].value);
            else if (board[i][j].value < 100)
                printw("|    %d  ", board[i][j].value);
            else
                printw("|   %d  ", board[i][j].value);
        }
        printw("|\n");
        printw("|        |        |        |        | \n"); // 3e | de la ligne
    }
    printw("+--------+--------+--------+--------+ \n\n");
    refresh(); // affiche sur le terminal les résultats des appels à printw
}

// On renvoie le nombre de cases libres sur le plateau.
int count_empty() {
    int libre = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].value == 0) {
                libre++;
            }
        }
    }
    return libre;
}

// On ajoute 2 si la case est vide
void add_two(int empty) {
    if (empty == 0) {
        return;
    } // Si vrai, fin de l'exécution.

    // On choisi un indice aléatoire parmi les cases vides
    int x = rand() % empty;

    // indice permet de parcourir le tableau à la recherche de x si la case est vide.
    int indice = 0;

    // On parcourt le tableau
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].value == 0) {
                if (indice == x) {
                    board[i][j].value = 2; // Insérer 2 sur la n-ième case vide
                    return;
                }
                indice++;
            }
        }
    }
}

// Fin du jeu ou continuation du jeu
int game_over(int add) {
    // On appelle la valeur du nombre de case vide.
    int empty = count_empty();

    if (empty == 0) {
        printw("============= GAME OVER =============\n");
        printw("=========== (press a key) ===========\n");
        getch();
        return 1; // Fin du jeu
    } else if (add) {
        add_two(empty); // Appelle fonction ajout 2
        display_board();
        return 0;
    } else {
        return 0;
    }
}

// Décale le plateau à gauche, sans faire les additions.
int shift_board() {
    int deplace = 0;
    // Création d'une copie temporaire
    Case copie[BOARD_SIZE][BOARD_SIZE];

    // Copie du case dans une variable temporaire
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            copie[i][j].value = board[i][j].value;
        }
    }

    // Décalage des valeurs vers la gauche
    for (int i = 0; i < BOARD_SIZE; i++) {
        int position = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (copie[i][j].value != 0) {
                board[i][position].value = copie[i][j].value;
                if (j != position) {
                    deplace = 1;
                }
                position++;
            }
        }
        // Remplissage des cases restantes de la ligne avec des zéros
        for (int j = position; j < BOARD_SIZE; j++) {
            board[i][j].value = 0;
        }
    }
    // Renvoie vrai si au moins un entier a bougé, faux sinon.
    return deplace;
}

// Fait les additions si les valeurs sont égaux et met à jour le score.
int update_board() {
    int deplace = shift_board();

    // On parcours le plateau pour faire les additions.
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE - 1; j++) {
            // On vérifie si deux valeurs consécutives sont égales et non nulles.
            if (board[i][j].value != 0 && board[i][j].value == board[i][j + 1].value) {
                // On additionne les valeurs et on met à jour les valeurs.
                board[i][j].value *= 2;
                board[i][j + 1].value = 0;
                score += board[i][j].value; // Mise à jour du score
                deplace = 1;
            }
        }
    }

    shift_board(); // On compresse les cases vidées par une addition
    // Renvoie vrai si au moins un entier a bougé ou a été additionné, faux sinon.
    return deplace;
}

// Définition du type key pour représenter les différentes directions.
typedef enum {MY_KEY_UP = 259, MY_KEY_DOWN = 258, MY_KEY_LEFT = 260, MY_KEY_RIGHT = 261, MY_KEY_BACKSPACE = 263} key;

// fonction qui lit des touches au clavier
key get_key() {
    int a; // variable utilisé pour stocker le caractère lu par le clavier
    while ((a = getch()) != -1) {
        switch (a) { 
            case KEY_UP:
                return KEY_UP;
            case KEY_DOWN:
                return KEY_DOWN;
            case KEY_LEFT:
                return KEY_LEFT;
            case KEY_RIGHT:
                return KEY_RIGHT;
            case KEY_BACKSPACE:
                return KEY_BACKSPACE;
        }
    }
    return KEY_BACKSPACE;
}


// Échange deux entiers en mémoire.
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Échange le contenu de chaque ligne du plateau avec celle en position miroir (une ligne a b c d devient d c b a).
void mirror_board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE / 2; j++) {
            swap(&board[i][j].value, &board[i][BOARD_SIZE - j - 1].value);
        }
    }
}

// Échange chaque case (i, j) du plateau avec la case (j, i).
void pivot_board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = i + 1; j < BOARD_SIZE; j++) {
            swap(&board[i][j].value, &board[j][i].value);
        }
    }
}


// Fait un pas de jeu en considérant la direction donnée.
int play(key dir) {
    int moved = 0;

    // Appliquer les transformations pour orienter le plateau vers la gauche
    switch (dir) {
        case KEY_LEFT:
            // Aucune transformation nécessaire, le plateau est déjà orienté vers la gauche
            break;
        case KEY_RIGHT:
            mirror_board(); 
            break;
        case KEY_UP:
            pivot_board(); 
            break;
        case KEY_DOWN:
            pivot_board(); 
            mirror_board(); 
            break;
        default:
            return 0; // Quitter si la direction n'est pas valide
    }

    // Mettre à jour le plateau pour effectuer les compressions et les additions vers la gauche
    moved = update_board();

    // Rétablir l'orientation d'origine du plateau après les modifications
    switch (dir) {
        case KEY_RIGHT:
            mirror_board(); 
            break;
        case KEY_UP:
            pivot_board(); 
            break;
        case KEY_DOWN:
            mirror_board(); 
            pivot_board(); 
            break;
        default:
            break; 
    }

    return moved; // Renvoyer vrai si au moins un entier a bougé ou a été additionné, faux sinon
}

// initialisation du terminal
int main (){
    INIT_2048();
    init_board(board);
	
	int x = 1;
	
	while (!game_over(x)) {
        key y = get_key(); // On récupère la touche jouée
        if (play(y)) {
            display_board(); // On n'affiche le plateau que si le joueur a réellement joué
        }
    }
	
    DONE_2048(); // quitte le jeu
    return 0;
}
