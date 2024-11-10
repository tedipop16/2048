/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
// #include <ctype.h>
#include <ctype.h>
#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define FOREVER 1
/* Am definit aici capetele de functie pentru a le putea folosi una in
 * interiorul celelilalte*/
void print_step(int Game[][4], int *points);
void run_menu(int Game[][4], int *points, bool *inGame, bool *inMenu);
void run_game(int Game[][4], int *points, bool *inGame, bool *inMenu);
int game_status(int Game[][4], int *points);
void start_new_game(int Game[][4], int *points);
bool move_game_up(int Game[][4], int *points);
bool move_game_down(int Game[][4], int *points);
bool move_game_right(int Game[][4], int *points);
bool move_game_left(int Game[][4], int *points);
void generate_random(int Game[][4]);
bool check_game_up(int testGame[][4], int *points);
bool check_game_down(int testGame[][4], int *points);
bool check_game_right(int testGame[][4], int *points);
bool check_game_left(int testGame[][4], int *points);
bool check_if_sure();
void matrix_copy(int destination[][4], int source[][4]);
void create_colors();

int main(void) {
  // int row = INIT_ROW, col = INIT_COL, new_row, new_col;
  // int nrows, ncols;
  int Game[4][4], points = 0;
  bool inMenu = 1, inGame = 0;
  // setez radacina pt random
  srand(time(0));

  /* Se inițializează ecranul;*/
  initscr();
  use_default_colors();
  curs_set(0);
  cbreak();
  noecho();
  create_colors();
  keypad(stdscr, TRUE);
  run_menu(Game, &points, &inGame, &inMenu);
  /*While - ul acesta va fi rulat pana la iesirea din aplicatie, gasita in
    run_menu
    Acest while face trecerea de la joc meniu cand acestea termina rularea
   */
  while (FOREVER) {
    if (inMenu) run_menu(Game, &points, &inGame, &inMenu);

    if (inGame) run_game(Game, &points, &inGame, &inMenu);
  }

  /* Se închide fereastra ncurses */
  endwin();

  return 0;
}
// Functia care genereaza perechile de culori
void create_colors() {
  start_color();
  // use_default_colors();
  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_BLUE);
  init_pair(4, COLOR_WHITE, COLOR_CYAN);
  init_pair(5, COLOR_BLACK, COLOR_YELLOW);
}
// Functia de auto-mutare, ce verifica si produce cea mai buna miscare
void auto_move(int Game[][4], int *points) {
  int cpy[4][4];
  int localPoints = *points, maxpoints = -1, poz = 0;

  matrix_copy(cpy, Game);
  if (check_game_up(cpy, &localPoints))
    if (localPoints > maxpoints) {
      maxpoints = localPoints;
      poz = 1;
    }
  localPoints = *points;

  matrix_copy(cpy, Game);
  if (check_game_down(cpy, &localPoints))
    if (localPoints > maxpoints) {
      maxpoints = localPoints;
      poz = 2;
    }
  localPoints = *points;

  matrix_copy(cpy, Game);
  if (check_game_left(cpy, &localPoints))
    if (localPoints > maxpoints) {
      maxpoints = localPoints;
      poz = 3;
    }
  localPoints = *points;

  matrix_copy(cpy, Game);
  if (check_game_right(cpy, &localPoints))
    if (localPoints > maxpoints) {
      maxpoints = localPoints;
      poz = 4;
    }

  switch (poz) {
    case 1:
      move_game_up(Game, points);
      break;
    case 2:
      move_game_down(Game, points);
      break;
    case 3:
      move_game_left(Game, points);
      break;
    case 4:
      move_game_right(Game, points);
      break;

    default:
      break;
  }
}
// Printeaza ecranul pt cazul in care este castigat meciul
void print_win_menu(int points) {
  clear();
  attron(A_BOLD);
  mvaddstr(1, 3, "Felicitari!");
  attroff(A_BOLD);
  attron(A_REVERSE);
  mvprintw(2, 3, "Ai castigat jocul cu %d puncte", points);
  attroff(A_REVERSE);
  mvaddstr(10, 1, "Apasa orice tasta pentru a te intoarce la meniu!");
  refresh();
  getch();
}
// Printeaza ecranul pt cazul in care este pierdut meciul

void print_lose_menu(int points) {
  clear();
  refresh();
  attron(A_BOLD);
  mvaddstr(1, 3, "Mai baga o fisa!");
  attroff(A_BOLD);
  attron(A_REVERSE);
  mvprintw(2, 3, "Ai pierdut jocul la %d puncte, mai era un pic si castigai",
           points);
  attroff(A_REVERSE);
  mvaddstr(10, 1, "Apasa orice tasta pentru a te intoarce la meniu!");
  refresh();
  getch();
}
// Printeaza un ecran care verfica daca utilizatorul este sigur, nu a apasat din
// greseala, si returneaza 1 daca este sigur

bool check_if_sure() {
  clear();
  mvaddstr(5, 5, "Are you sure you want to do this?");
  mvaddstr(6, 8, "Press y if you are 100\% sure");
  refresh();
  char c = getch();
  return (tolower(c) == 'y');
}

// Functia care afiseaza si ruleaza jocul , cu toate mutarile acestuia

void run_game(int Game[][4], int *points, bool *inGame, bool *inMenu) {
  int c;
  if (!*points)
    start_new_game(Game, points);
  else
    print_step(Game, points);

  while (*inGame) {
    switch (game_status(Game, points)) {
      case 1:
        print_win_menu(*points);
        *inGame = false;
        *inMenu = true;
        *points = 0;
        return;
        break;
      case 2:
        print_lose_menu(*points);
        *inGame = false;
        *inMenu = true;
        *points = 0;
        return;
        break;
    }
    c = getch();
    switch (c) {
      case 'r':
        if (check_if_sure()) {
          start_new_game(Game, points);
          break;
        } else
          print_step(Game, points);
        break;
      case 'q':
        *inGame = false;
        *inMenu = true;
        clear();
        refresh();
        run_menu(Game, points, inGame, inMenu);
        return;
        break;
      case 'g':
        auto_move(Game, points);
        break;
      case 'a':
        move_game_left(Game, points);
        break;
      case KEY_LEFT:
        move_game_left(Game, points);
        break;
      case KEY_RIGHT:
        move_game_right(Game, points);
        break;
      case 'd':
        move_game_right(Game, points);
        break;
      case KEY_UP:
        move_game_up(Game, points);
        break;
      case 'w':
        move_game_up(Game, points);
        break;
      case 's':
        move_game_down(Game, points);
        break;
      case KEY_DOWN:
        move_game_down(Game, points);
        break;
    }
    refresh();
  }
}
// Functie ce copiaza intr o tabla de joc alta tabla
void matrix_copy(int destination[][4], int source[][4]) {
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) destination[i][j] = source[i][j];
}
// Functie care arata daca jocul este in derulare, pierdut(returneaza 2) sau
// castigat(returneaza 1)
int game_status(int Game[][4], int *points) {
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if (Game[i][j] == 2048) return 1;
  int cpy[4][4];
  bool result = false;
  int localPoints = *points;

  matrix_copy(cpy, Game);
  result = (result || check_game_up(cpy, &localPoints));
  matrix_copy(cpy, Game);
  result = (check_game_down(cpy, &localPoints) || result);
  matrix_copy(cpy, Game);
  result = (result || check_game_left(cpy, &localPoints));
  matrix_copy(cpy, Game);
  result = (result || check_game_right(cpy, &localPoints));
  refresh();
  if (!result) return 2;
  return 0;
}
// functia de afisare a pasului curent in joc, cu puncte, culori si tabla de joc
void print_step(int Game[][4], int *points) {
  char a[4][40] = {""};
  char str[5] = "";
  int i, j, n, l;

  // aici se formateaza afisarea tablii de joc cu | intre si cu dimensiuni egale
  // pe celule

  for (i = 0; i < 4; i++) {
    a[i][0] = 0;
    // strcat(a[i], "| ");
    for (j = 0; j < 4; j++) {
      if (Game[i][j]) {
        n = Game[i][j];
        l = 3;
        while (n) {
          str[l--] = n % 10 + '0';
          n /= 10;
        }
        while (l >= 0) str[l--] = ' ';
      } else
        strcpy(str, "    ");
      str[4] = 0;
      strcat(a[i], str);
      strcat(a[i], " | ");
    }
    a[i][strlen(a[i]) - 3] = 0;
  }
  time_t timer;
  char buffer[26];
  struct tm *tm_info;
  timer = time(NULL);
  tm_info = localtime(&timer);
  strftime(buffer, 26, "%Y-%m-%d  %H:%M", tm_info);

  clear();
  attron(COLOR_PAIR(2));
  mvprintw(0, 12, "Puncte : %d          %s", *points, buffer);
  attroff(COLOR_PAIR(2));
  attron(COLOR_PAIR(3));
  mvaddstr(7, 5, "Options");
  mvaddstr(8, 1,
           "W, up arrow - make a move upward       S, down arrow  - make a "
           "move downward ");
  mvaddstr(9, 1,
           "A, left arrow - make a move to left    D, right arrow  - make a "
           "move to right");
  mvaddstr(10, 1, "G - auto move                          Q - go to menu");
  attroff(COLOR_PAIR(3));
  mvaddstr(0, 55, "  /$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$ ");
  mvaddstr(1, 55, " /$$__  $$ /$$$_  $$| $$  | $$ /$$__  $$");
  mvaddstr(2, 55, "|__/  \\ $$| $$$$\\ $$| $$  | $$| $$  \\ $$");
  mvaddstr(3, 55, "  /$$$$$$/| $$ $$ $$| $$$$$$$$|  $$$$$$/");
  mvaddstr(4, 55, " /$$____/ | $$\\ $$$$|_____  $$ >$$__  $$");
  mvaddstr(5, 55, "| $$      | $$ \\ $$$      | $$| $$  \\ $$");
  mvaddstr(6, 55, "| $$$$$$$$|  $$$$$$/      | $$|  $$$$$$/");
  mvaddstr(7, 55, "|________/ \\______/       |__/ \\______/ ");
  refresh();
  WINDOW *wnd;
  wnd = newwin(6, 28, 1, 1);
  box(wnd, 0, 0);
  //
  /* wmvaddstr(1, 1, "_____________________________");
   mvaddstr(6, 1, "_____________________________");*/
  //  wattron(wnd, COLOR_PAIR(1));
  mvwaddstr(wnd, 1, 1, a[0]);
  mvwaddstr(wnd, 2, 1, a[1]);
  mvwaddstr(wnd, 3, 1, a[2]);
  mvwaddstr(wnd, 4, 1, a[3]);
  // wattroff(wnd, COLOR_PAIR(1));

  wrefresh(wnd);
}

// Initializarea unei noi table de joc, resetarea punctelor si afisarea noului
// joc
void start_new_game(int Game[][4], int *points) {
  int i, j, val1, val2;
  *points = 0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) Game[i][j] = 0;
  i = rand() % 16;
  j = rand() % 16;
  while (i == j) j = rand() % 16;
  val1 = rand() % 2;
  val2 = rand() % 2;
  Game[i / 4][i % 4] = 2 + val1 * 2;
  Game[j / 4][j % 4] = 2 + val2 * 2;
  print_step(Game, points);
}
// rularea meniului, cu comenzile acestuia
void run_menu(int Game[][4], int *points, bool *inGame, bool *inMenu) {
  int i;
  int ch = 0;
  clear();
  mvaddstr(11, 18, "..--- ----- ....- ---..");
  mvaddstr(2, 15, "  /$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$ ");
  mvaddstr(3, 15, " /$$__  $$ /$$$_  $$| $$  | $$ /$$__  $$");
  mvaddstr(4, 15, "|__/  \\ $$| $$$$\\ $$| $$  | $$| $$  \\ $$");
  mvaddstr(5, 15, "  /$$$$$$/| $$ $$ $$| $$$$$$$$|  $$$$$$/");
  mvaddstr(6, 15, " /$$____/ | $$\\ $$$$|_____  $$ >$$__  $$");
  mvaddstr(7, 15, "| $$      | $$ \\ $$$      | $$| $$  \\ $$");
  mvaddstr(8, 15, "| $$$$$$$$|  $$$$$$/      | $$|  $$$$$$/");
  mvaddstr(9, 15, "|________/ \\______/       |__/ \\______/ ");
  refresh();
  MENU *menu;
  ITEM **items;
  WINDOW *wnd;
  // Am folosit menu.h pentru claritatea codului si aproprierea cat mai mare de
  // o aplicatie serioasa dezvoltata
  char options[][20] = {"NEW GAME", "RESUME", "QUIT"};

  /* Crearea Itemilor din meniu */
  items = (ITEM **)calloc(4, sizeof(ITEM *));
  for (i = 0; i < 3; i++) items[i] = new_item(options[i], "");

  /* Crearea Meniului */
  menu = new_menu(items);
  /* Create Window for the menu */
  wnd = newwin(5, 13, 1, 1);
  box(wnd, 0, 0);
  keypad(wnd, TRUE);
  set_menu_win(menu, wnd);
  set_menu_sub(menu, derwin(wnd, 4, 12, 1, 1));
  set_menu_mark(menu, "->");
  /* set menu format - aspectul liniar al aplicatiei*/
  set_menu_format(menu, 3, 1);
  /*afisarea meniului */
  post_menu(menu);
  wrefresh(wnd);

  // executarea comenzilor date
  while ((ch = wgetch(wnd))) {
    switch (ch) {
      case KEY_DOWN:
        menu_driver(menu, REQ_DOWN_ITEM);
        if (!*points) menu_driver(menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(menu, REQ_UP_ITEM);
        if (!*points) menu_driver(menu, REQ_UP_ITEM);
        break;
      case 10:
        if (!strcmp(item_name(current_item(menu)), "NEW GAME")) {
          *inGame = true;
          *inMenu = false;
          unpost_menu(menu);
          free_menu(menu);
          for (i = 0; i < 3; i++) free_item(items[i]);
          start_new_game(Game, points);
          return;
        }
        if (!strcmp(item_name(current_item(menu)), "RESUME")) {
          *inGame = true;
          *inMenu = false;
          unpost_menu(menu);
          free_menu(menu);
          for (i = 0; i < 3; i++) free_item(items[i]);

          print_step(Game, points);
          return;
        }
        if (!strcmp(item_name(current_item(menu)), "QUIT")) {
          endwin();
          exit(0);
        }
        // refresh();
        break;
    }
    wrefresh(wnd);
  }
  unpost_menu(menu);
  free_menu(menu);
  for (i = 0; i < 3; i++) free_item(items[i]);
  endwin();
}

// generarea aleatoare a unei casute 2 sau 4
void generate_random(int Game[][4]) {
  int rdm = rand() % 16, i, j;
  for (i = rdm / 4; i < 4; i++)
    for (j = rdm % 4; j < 4; j++)
      if (!Game[i][j]) {
        Game[i][j] = 2 + (rand() % 2) * 2;
        return;
      }
  for (i = rdm / 4; i >= 0; i--)
    for (j = rdm % 4; j >= 0; j--)
      if (!Game[i][j]) {
        Game[i][j] = 2 + (rand() % 2) * 2;
        return;
      }
}

// mutarea spre in sus, ce intoarce validitatea acesteia
bool move_game_up(int Game[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (col = 0; col < 4; col++) {
    lpoz = 0;
    for (lin = 1; lin < 4; lin++)
      if (Game[lin][col]) {
        lpoz = lin;
        while (lin - 1 >= 0 && !Game[lin - 1][col]) lin--;
        if (lin == lpoz && Game[lin - 1][col] != Game[lin][col]) continue;
        valid = true;
        if (!lin) {
          Game[lin][col] = Game[lpoz][col];
          Game[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        if (Game[lin - 1][col] != Game[lpoz][col]) {
          Game[lin][col] = Game[lpoz][col];
          Game[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        Game[lin - 1][col] *= 2;
        *points += Game[lin - 1][col];
        Game[lpoz][col] = 0;
        lin = lpoz;
        continue;
      }
  }
  if (valid) {
    generate_random(Game);
    print_step(Game, points);
  } else {
    attron(COLOR_PAIR(5));
    mvaddstr(3, 37, "Mutare invalida!");
    attroff(COLOR_PAIR(5));
    refresh();
  }
  return valid;
}

// verificarea mutarii in sus, ce intoarce validitatea, tottodata manipuland is
// punctele
bool check_game_up(int testGame[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (col = 0; col < 4; col++) {
    lpoz = 0;
    for (lin = 1; lin < 4; lin++)
      if (testGame[lin][col]) {
        lpoz = lin;
        while (lin - 1 >= 0 && !testGame[lin - 1][col]) lin--;
        if (lin == lpoz && testGame[lin - 1][col] != testGame[lin][col])
          continue;
        valid = true;
        if (!lin) {
          testGame[lin][col] = testGame[lpoz][col];
          testGame[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        if (testGame[lin - 1][col] != testGame[lpoz][col]) {
          testGame[lin][col] = testGame[lpoz][col];
          testGame[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        testGame[lin - 1][col] *= 2;
        *points += testGame[lin - 1][col];
        testGame[lpoz][col] = 0;
        lin = lpoz;
        continue;
      }
  }
  return valid;
}

// mutarea in jos
bool move_game_down(int Game[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (col = 0; col < 4; col++) {
    lpoz = 0;
    for (lin = 2; lin >= 0; lin--)
      if (Game[lin][col]) {
        lpoz = lin;
        while (lin + 1 < 4 && !Game[lin + 1][col]) lin++;
        if (lin == lpoz && Game[lin + 1][col] != Game[lin][col]) continue;
        valid = true;
        if (lin == 3) {
          Game[lin][col] = Game[lpoz][col];
          Game[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        if (Game[lin + 1][col] != Game[lpoz][col]) {
          Game[lin][col] = Game[lpoz][col];
          Game[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        Game[lin + 1][col] *= 2;
        *points += Game[lin + 1][col];
        Game[lpoz][col] = 0;
        lin = lpoz;
        continue;
      }
  }
  if (valid) {
    generate_random(Game);
    print_step(Game, points);
  } else {
    attron(COLOR_PAIR(5));
    mvaddstr(3, 37, "Mutare invalida!");
    attroff(COLOR_PAIR(5));
    refresh();
  }
  return valid;
}

// verificarea posibilitatii mutarii in jos
bool check_game_down(int testGame[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (col = 0; col < 4; col++) {
    lpoz = 0;
    for (lin = 2; lin >= 0; lin--)
      if (testGame[lin][col]) {
        lpoz = lin;
        while (lin + 1 < 4 && !testGame[lin + 1][col]) lin++;
        if (lin == lpoz && testGame[lin + 1][col] != testGame[lin][col])
          continue;
        valid = true;
        if (lin == 3) {
          testGame[lin][col] = testGame[lpoz][col];
          testGame[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        if (testGame[lin + 1][col] != testGame[lpoz][col]) {
          testGame[lin][col] = testGame[lpoz][col];
          testGame[lpoz][col] = 0;
          lin = lpoz;
          continue;
        }
        testGame[lin + 1][col] *= 2;
        *points += testGame[lin + 1][col];
        testGame[lpoz][col] = 0;
        lin = lpoz;
        continue;
      }
  }
  return valid;
}

// mutarea la dreapta
bool move_game_right(int Game[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (lin = 0; lin < 4; lin++) {
    lpoz = 0;
    for (col = 2; col >= 0; col--)
      if (Game[lin][col]) {
        lpoz = col;
        while (col + 1 < 4 && !Game[lin][col + 1]) col++;
        if (col == lpoz && Game[lin][col + 1] != Game[lin][col]) continue;
        valid = true;
        if (col == 3) {
          Game[lin][col] = Game[lin][lpoz];
          Game[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        if (Game[lin][col + 1] != Game[lin][lpoz]) {
          Game[lin][col] = Game[lin][lpoz];
          Game[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        Game[lin][col + 1] *= 2;
        *points += Game[lin][col + 1];
        Game[lin][lpoz] = 0;
        col = lpoz;
        continue;
      }
  }
  if (valid) {
    generate_random(Game);
    print_step(Game, points);
  } else {
    attron(COLOR_PAIR(5));
    mvaddstr(3, 37, "Mutare invalida!");
    attroff(COLOR_PAIR(5));
    refresh();
  }
  return valid;
}

// verificarea posibilitatii mutarii la dreapta
bool check_game_right(int testGame[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (lin = 0; lin < 4; lin++) {
    lpoz = 0;
    for (col = 2; col >= 0; col--)
      if (testGame[lin][col]) {
        lpoz = col;
        while (col + 1 < 4 && !testGame[lin][col + 1]) col++;
        if (col == lpoz && testGame[lin][col + 1] != testGame[lin][col])
          continue;
        valid = true;
        if (col == 3) {
          testGame[lin][col] = testGame[lin][lpoz];
          testGame[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        if (testGame[lin][col + 1] != testGame[lin][lpoz]) {
          testGame[lin][col] = testGame[lin][lpoz];
          testGame[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        testGame[lin][col + 1] *= 2;
        *points += testGame[lin][col + 1];
        testGame[lin][lpoz] = 0;
        col = lpoz;

        continue;
      }
  }

  return valid;
}

// mutare la stanga
bool move_game_left(int Game[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (lin = 0; lin < 4; lin++) {
    lpoz = 0;
    for (col = 1; col < 4; col++)
      if (Game[lin][col]) {
        lpoz = col;
        while (col - 1 >= 0 && !Game[lin][col - 1]) col--;
        if (col == lpoz && Game[lin][col - 1] != Game[lin][col]) continue;
        valid = true;
        if (!col) {
          Game[lin][col] = Game[lin][lpoz];
          Game[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        if (Game[lin][col - 1] != Game[lin][lpoz]) {
          Game[lin][col] = Game[lin][lpoz];
          Game[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        Game[lin][col - 1] *= 2;
        *points += Game[lin][col - 1];
        Game[lin][lpoz] = 0;
        col = lpoz;
        continue;
      }
  }
  if (valid) {
    generate_random(Game);
    print_step(Game, points);
  } else {
    attron(COLOR_PAIR(5));
    mvaddstr(3, 37, "Mutare invalida!");
    attroff(COLOR_PAIR(5));
    refresh();
  }
  return valid;
}

// verificarea poisibilitatii mutarii la stanga
bool check_game_left(int testGame[][4], int *points) {
  int lin, col;
  int lpoz;
  bool valid = false;
  for (lin = 0; lin < 4; lin++) {
    lpoz = 0;
    for (col = 1; col < 4; col++)
      if (testGame[lin][col]) {
        lpoz = col;
        while (col - 1 >= 0 && !testGame[lin][col - 1]) col--;
        if (col == lpoz && testGame[lin][col - 1] != testGame[lin][col])
          continue;
        valid = true;
        if (!col) {
          testGame[lin][col] = testGame[lin][lpoz];
          testGame[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        if (testGame[lin][col - 1] != testGame[lin][lpoz]) {
          testGame[lin][col] = testGame[lin][lpoz];
          testGame[lin][lpoz] = 0;
          col = lpoz;
          continue;
        }
        testGame[lin][col - 1] *= 2;
        *points += testGame[lin][col - 1];
        testGame[lin][lpoz] = 0;
        col = lpoz;
        continue;
      }
  }
  return valid;
}