#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//printeaza 0 daca litera nu apartine cuvantului, 1 daca apartine dar nu e pe poz corecta si 2 daca e pe poz
int verLitera(char *cuvAles, char lit, int pozLit)
{
    int i, eInCuvant = 0;
    for(i=0;i<5;i++)
    {
        //daca si pozitia si litera se potrivesc atunci putem sa oprim si returnam 2
        if(cuvAles[i] == lit && pozLit == i)
        {
            return 2;
        }
        //daca am gasit o litera care se potriveste marcam contorul si asteptam sa vedem daca se potriveste si pozitia la un mom dat
        if(cuvAles[i] == lit)
        {
            eInCuvant = 1;
        }
    }
    return eInCuvant;
}


//afiseaza feedbackul pentru raspuns
void verCuvant(char *cuvAles, char *raspuns, int crtLinie)
{
    crtLinie--;
    int i, cntX = 14;
    for(i=0;i<5;i++)
    {
        //daca nu se potriveste afisam cu negru
        if(verLitera(cuvAles, raspuns[i], i)==0)
        {
            attron(COLOR_PAIR(3));
            mvprintw(crtLinie, cntX, "%c", raspuns[i]);
            attroff(COLOR_PAIR(3));
        }
        //daca doar se regaseste in cuvant litera afisam cu galben
        if(verLitera(cuvAles, raspuns[i], i)==1)
        {
            attron(COLOR_PAIR(7));
            mvprintw(crtLinie, cntX, "%c", raspuns[i]);
            attroff(COLOR_PAIR(7));
        }
        //daca se potriveste si pe pozitie litera afisam cu verde
        if(verLitera(cuvAles, raspuns[i], i)==2)
        {
            attron(COLOR_PAIR(1));
            mvprintw(crtLinie, cntX, "%c", raspuns[i]);
            attroff(COLOR_PAIR(1));
        }
        cntX++;
    }
}

//Incepe joc de wordle de pe crtLine
void jocWorlde(char *solutie, int *crtLine)
{
    char raspuns[200];
    int gasit = 0, cnt = 0;
    //pentru debuging, printam cuvantul
    //attron(COLOR_PAIR(3));
    //mvprintw((*crtLine)++, 14, "%s\n", solutie);
    //attroff(COLOR_PAIR(3));

    //miscam cursorul si asteptam cuvantul
    mvprintw((*crtLine), 0, "%d", cnt+1);
    move((*crtLine)++, 14);
    //pornim jocul
    while(cnt<=5)
    {
        //citim raspunsul utilizatorului
        getstr(raspuns);
        //verificam daca cuvantul are lungimea corecta
        if(strlen(raspuns)>5)
        {
            attron(COLOR_PAIR(5));
            mvprintw((*crtLine)++, 0, "Cuvant prea lung, mai incearca:");
            attroff(COLOR_PAIR(5));
            
        }
        if(strlen(raspuns)<5)
        {
            attron(COLOR_PAIR(5));
            mvprintw((*crtLine)++, 0, "Cuvant prea scurt, mai incearca:");
            attroff(COLOR_PAIR(5));
           
        }
        if(strlen(raspuns)==5)
        {
            verCuvant(solutie, raspuns, (*crtLine));
            //daca utilizatorul castiga oprim whileul
            if(strcmp(raspuns, solutie)==0)
            {
                cnt = 10;
                gasit = 1;
            }
            cnt++;
        }
        mvprintw((*crtLine), 0, "%d", cnt+1);
        move((*crtLine)++, 14);
    }
    (*crtLine)--;
    //afisam mesajul potrivit la sfarsitul jocului
    if(gasit == 1)
    {
        attron(COLOR_PAIR(5));
        mvprintw((*crtLine), 0, "Esti tare ai gasit cuvantul!");
        attroff(COLOR_PAIR(5));
    }
    else
    {
        attron(COLOR_PAIR(5));
        mvprintw((*crtLine), 0, "Nu ai gasit cuvantul la timp! Acesta era");
        attroff(COLOR_PAIR(5));
        //afisam si solutia daca utilizatorul nu a gasit-o
        attron(COLOR_PAIR(1));
        mvprintw((*crtLine), 42, "%s", solutie);
        attroff(COLOR_PAIR(1));
    }
}

int main()
{
    //initializam ecranul
    initscr();
    raw();
    //initializam culorile
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_YELLOW);
    //lista de cuvinte
    char listaCuv[28][6]={
        {"arici"},{"atent"},{"baiat"},{"ceata"},{"debut"},{"peste"},{"fixat"},{"hamac"},{"harta"},{"jalon"},
        {"jucam"},{"lacat"},{"magie"},{"nufar"},{"oaste"},{"perus"},{"rigle"},{"roman"},{"sanie"},{"scris"},
        {"sonda"},{"texte"},{"tipar"},{"titan"},{"zebra"},{"vapor"},{"vatra"}
    };
    //printam logoul sa fie frumos
    attron(COLOR_PAIR(5));
    printw("======================================\n");
    attroff(COLOR_PAIR(5));
    attron(COLOR_PAIR(4));
    printw(" _      __ ____   _____ ____/ // /___ \n| | /| / // __ \\ / ___// __  // // _ \\\n| |/ |/ // /_/ // /   / /_/ // //  __/\n|__/|__/ \\____//_/    \\__,_//_/ \\___/ \n");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(5));
    printw("======================================\n");
    attroff(COLOR_PAIR(5));

    int crtLine = 7, r;
    char vreaJoc = 'n';
    //intreaba jucatorul daca vrea sa joace un joc de worlde
    attron(COLOR_PAIR(5));
    mvprintw(crtLine, 0, "Vrei sa joci un joc de wordle? (y/n)");
    move(crtLine, strlen("Vrei sa joci un joc de wordle? (y/n)")+1);
    scanw("%c", &vreaJoc);
    attroff(COLOR_PAIR(5));
    crtLine++;
    //daca vrea joc incepem joc, daca nu inchide programul cand apasa o tasta
    while(vreaJoc == 'y')
    {
        //alegem un numar la nimereala
        srand(time(NULL));
        r = rand()%28;
        //incepe jocul de wordle
        jocWorlde(listaCuv[r], &crtLine);
        crtLine++;
        //intreaba daca mai vrea sa se joace
        attron(COLOR_PAIR(5));
        mvprintw(crtLine, 0, "Vrei sa incepi alt joc de wordle? (y/n)");
        move(crtLine++, strlen("Vrei sa incepi alt joc de wordle? (y/n)")+1);
        scanw("%c", &vreaJoc);
        attroff(COLOR_PAIR(5));
    }

    mvprintw(crtLine++, 0, "Apasa o tasta sa inchizi programul.");
    
    getch();
    endwin();
    return 0;
}