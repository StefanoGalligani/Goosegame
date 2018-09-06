#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <iomanip>
#include "iocolor.h"

using namespace std;

#define MAX 40

struct statistiche{
	char nomegiocatore[8];
	bool isBot=false;
	char pedina;
	int pos;
	int tiri=0;
	bool primoTiro;
	int fermo;
	int win=0;
};

bool menuIniziale();
int inputGiocatori(statistiche giocatori[]);
void coutTabellone(statistiche giocatori[]);
int tiroDado(statistiche giocatori[], int g, int &log);
bool calcoloPosizione(int dadi, statistiche giocatori[], int turno, int &log, bool suoni, int i, int totP, int match);
void calcoloVincitori(statistiche giocatori[], int clas[], int n);
void creaQuad(int i, int j);
void smussaAngoli();
void coutValore(statistiche giocatori[]);
void coutPos(statistiche giocatori[], int turno, int totG);
int cinTotP ();
int coutLog(int g, int log, int caso, int vario, statistiche giocatori[]);
int clearLog (int totG);
void disOca();
void disCasa();
void disPozzo();
void disPrigione();
void disScheletro();
void coda(int clas[], statistiche giocatori[], int totG, bool suoni);

int main() {
	system("cls");
	setColor(0,15);
	srand(time(NULL));
	system("mode con: cols=120 lines=50");
	int tab[64], totG, totP, dadi, turno, match, i, log, clas[6];
	statistiche giocatori[6];
	bool continua, suoni, isArrivato=false;
	suoni=menuIniziale();
	totP=cinTotP();
	totG=inputGiocatori(giocatori);
	log=8+totG;
	if(suoni){
    	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
	}
	for (match=0; match<totP; match++) {
    	for (i=0;i<6;i++) {
        	giocatori[i].pos=0;
        	giocatori[i].fermo=0;
        	giocatori[i].primoTiro=true;
    	}
    	do {
        	if (log>30) {
            	log=clearLog(totG);
        	}
        	for (turno=0; turno<totG; turno++) {
            	giocatori[turno].tiri++;
            	if (giocatori[turno].fermo==0) {
                	coutTabellone(giocatori);
                	coutPos(giocatori, turno, totG);
                	dadi=tiroDado(giocatori, turno, log);
                	log=coutLog(turno, log, 1, dadi, giocatori);
                	isArrivato=calcoloPosizione(dadi, giocatori, turno, log, suoni, totG, totP, match);
            	} else {
                	log=coutLog(turno, log, 2, giocatori[turno].fermo, giocatori);
                	giocatori[turno].fermo--;
            	}
            	if (isArrivato==true) {
                	log=coutLog(turno, log, 3, match, giocatori);
                	giocatori[turno].win++;
                	coutPos(giocatori, turno, totG);
                	turno=6;
            	}
        	}
    	} while (isArrivato==false);
    	calcoloVincitori(giocatori, clas, totG);
	}
	coda(clas, giocatori, totG, suoni);
}

bool menuIniziale() {
	int menu;
	bool uscita=false, suoni=true;
	do {
    	system ("Cls");
    	cout << "Che cosa vuoi fare?" << endl;
    	cout << "\nGioca[1]\nRegolamento[2]\nSuoni[3]: ";
    	if (suoni==true) {
        	cout << "ON" << endl;
    	} else {
        	cout << "OFF" << endl;
    	}
    	cin >> menu;
    	switch (menu) {
        	case 3:
            	if (suoni==true) {
                	suoni=false;
            	} else {
                	suoni=true;
            	}
            	break;
        	case 2:
            	cout << "Giocatori: 2-6\nDurata media:<5-20 minuti>" << endl;
            	cout << "\nIl gioco e' formato da un percorso a spirale di 63 caselle.";
            	cout << "\nOgni giocatore e' contraddistinto da una differente pedina." << endl;
            	cout << "\nA turno, ogni giocatore tira due dadi e avanza del numero ottenuto";
            	cout << "\nSe il giocatore finisce in una delle caselle speciali, si applica l'effetto corrispondente:";
            	cout << "\n\t9 e multipli (oca): si ripete l'ultimo movimento";
            	cout << "\n\t6 (ponte): si ripete l'ultimo movimento come sull'oca\n\t19 (casa): si rimane fermi tre turni";
            	cout << "\n\t31 (pozzo) e 52 (prigione): si rimane fermi fino a quando non arriva\n\tun altro giocatore nella casella, che viene a sua volta imprigionato";
            	cout << "\n\t42 (labirinto): si torna alla 39\n\t58 (scheletro) si torna alla casella 1";
            	cout << "\n\tLa 63 (arrivo): deve essere raggiunta con un lancio di dadi esatto,\n\taltrimenti si retrocede dei punti in eccesso" << endl;
            	cout << "Se si ottiene 9 al primo tiro, invece di raggiungere subito la fine,\nsi applica uno dei seguenti effetti:";
            	cout << "\n\tSe si ottengono un 3 e un 6, si va alla casella 26";
            	cout << "\n\tSe si ottengono un 4 e un 5, si va alla casella 53\n" << endl;
            	system("Pause");
            	break;
        	case 1:
            	uscita=true;
    	}
	} while (uscita==false);
	return suoni;
}

int cinTotP() {
	int totP;
	do {
    	cout << "Quante partite vuoi giocare?" << endl;
    	cin >> totP;
	} while (totP<0);
	return totP;
}

int inputGiocatori(statistiche giocatori[]){
	int n, i, j, t=3, scelta, nBot;
	bool isScelta[6]={false, false, false, false, false, false};
	system("cls");
	do {
    	gotoXY(23,1);
    	cout << "   ";
    	gotoXY(12,1);
    	cout << "Giocatori: ";
    	cin >> n;
	} while(n<2||n>6);
	do {
    	gotoXY(24,2);
    	cout << "   ";
    	gotoXY(12,2);
    	cout << "Di cui bot: ";
    	cin >> nBot;
	} while (nBot<0||nBot>=n);
	for (i=0;i<=nBot;i++) {
    	giocatori[n-i].isBot=true;
    	switch (n-i){
        	case 1: strcpy(giocatori[n-i].nomegiocatore,"Bot2");
        	break;
        	case 2: strcpy(giocatori[n-i].nomegiocatore,"Bot3");
        	break;
        	case 3: strcpy(giocatori[n-i].nomegiocatore,"Bot4");
        	break;
        	case 4: strcpy(giocatori[n-i].nomegiocatore,"Bot5");
        	break;
        	case 5: strcpy(giocatori[n-i].nomegiocatore,"Bot6");
        	break;
    	}
	}
	for (i=0;i<n;i++) {
    	if (giocatori[i].isBot==false) {
        	t++;
        	gotoXY(12,i+t);
        	cout << "Nome G" << i+1 << ": ";
        	fflush(stdin);
        	cin.getline(giocatori[i].nomegiocatore,8);
            	do {
            	t++;
            	gotoXY(12,i+t);
            	cout << "Pedina G" << i+1 << "(";
            	cout << "1=";
            	putchar(35);
            	cout << " 2=";
            	putchar(36);
            	cout << " 3=";
            	putchar(37);
            	cout << " 4=";
            	putchar(38);
            	cout << " 5=";
            	putchar(190);
            	cout << " 6=";
            	putchar(245);
            	cout << "): ";
            	cin >> scelta;
            	if (isScelta[scelta-1]==false) {
                	switch (scelta){
                    	case 1:
                        	giocatori[i].pedina=35;
                        	break;
                    	case 2:
                        	giocatori[i].pedina=36;
                        	break;
                    	case 3:
                        	giocatori[i].pedina=37;
                        	break;
                    	case 4:
                        	giocatori[i].pedina=38;
                        	break;
                    	case 5:
                        	giocatori[i].pedina=190;
                        	break;
                    	case 6:
                        	giocatori[i].pedina=245;
                        	break;
                    	default:
                        	isScelta[scelta-1]=true;
                	}
            	}
            	if (isScelta[scelta-1]==true) {
                	t++;
                	gotoXY(12,i+t);
                	cout << "La scelta non e' valida" << endl;
                	t++;
                	gotoXY(12,i+t);
            	}
        	} while (isScelta[scelta-1]==true);
        	isScelta[scelta-1]=true;
    	}
	}
	system ("cls");
	for (i=0;i<6;i++) {
    	if (giocatori[i].isBot==true) {
        	for (j=0;j<6;j++) {
            	if (isScelta[j]==false) {
                	switch (j+1){
                    	case 1:
                        	giocatori[i].pedina=35;
                        	break;
                    	case 2:
                        	giocatori[i].pedina=36;
                        	break;
                    	case 3:
                        	giocatori[i].pedina=37;
                        	break;
                    	case 4:
                        	giocatori[i].pedina=38;
                        	break;
                    	case 5:
                        	giocatori[i].pedina=190;
                        	break;
                    	case 6:
                        	giocatori[i].pedina=245;
                        	break;
                	}
            	isScelta[j]=true;
            	}
        	}
    	}
	}
	return n;
}

void coutTabellone(statistiche giocatori[]) {
	int i, j;
	for (i=0; i<10; i++) {
    	for (j=0;j<11;j++) {
        	creaQuad(i,j);
    	}
	}
	smussaAngoli();
	coutValore(giocatori);
}

void coutPos(statistiche giocatori[], int turno, int totG) {
	int i;
	gotoXY(80,4);
	cout << "POSIZIONI";
	for(i=0;i<totG;i++){
    	gotoXY(75,i+6);
    	cout<< giocatori[i].nomegiocatore << "= " << giocatori[i].pos << " ";
	}
}

void creaQuad(int i, int j) {
	if (j==0||j==10||(i==0&&j!=9)||i==9||(j==8&&i!=8)||(i==7&&(j>1&&j<9))||(j==2&&(i>1&&i<8))||i==2&&(j>2&&j<7)||j==6&&(i>2&&i<6)||j==5&&i==5) {
    	gotoXY((i+2)*5,(j+2)*2);
    	putchar(194);
    	putchar(196);
    	putchar(196);
    	putchar(196);
    	putchar(196);
    	putchar(194);
    	gotoXY(i*5+10,j*2+5);
    	putchar(179);
    	cout << "    ";
    	putchar (179);
    	gotoXY(i*5+10,j*2+6);
    	putchar(193);
    	putchar(196);
    	putchar(196);
    	putchar(196);
    	putchar(196);
    	putchar(193);
	}
}

void smussaAngoli() {
	int c;
	gotoXY(10,4);
	putchar(218);
	gotoXY(20,8);
	putchar(218);
	gotoXY(10,24);
	putchar(218);
	gotoXY(35,14);
	putchar(218);
	for (c=6;c<22;c=c+2) {
    	gotoXY(10,c);
    	putchar(195);
    	gotoXY(15,c);
    	putchar(180);
    	gotoXY(55,c);
    	putchar(195);
    	gotoXY(60,c);
    	putchar(180);
	}
	gotoXY(60,4);
	putchar(191);
	gotoXY(50,8);
	putchar(191);
	gotoXY(40,4);
	putchar(191);
	gotoXY(55,20);
	putchar(195);
	gotoXY(55,24);
	putchar(195);
	gotoXY(60,20);
	putchar(180);
	gotoXY(60,24);
	putchar(180);
	gotoXY(55,22);
	putchar(195);
	gotoXY(60,22);
	putchar(180);
	gotoXY(10,22);
	putchar(192);
	gotoXY(10,26);
	putchar(192);
	gotoXY(60,26);
	putchar(217);
	gotoXY(50,22);
	putchar(217);
	for (c=10;c<18;c=c+2) {
    	gotoXY(20,c);
    	putchar(195);
    	gotoXY(25,c);
    	putchar(180);
	}
	for (c=10;c<22;c=c+2) {
    	gotoXY(45,c);
    	putchar(195);
    	gotoXY(50,c);
    	putchar(180);
	}
	gotoXY(20,18);
	putchar(192);
	gotoXY(40,14);
	putchar(191);
	gotoXY(40,16);
	putchar(180);
	gotoXY(40,18);
	putchar(217);
}

int tiroDado(statistiche giocatori[], int g, int &log) {
	int dado1, dado2, dado, i, n=540, somma;
	char ris;
	gotoXY(41,28);
	cout << " ";
	gotoXY(13,28);
	cout << "G" << g+1;
	if (giocatori[g].isBot==true) {
    	cout << "(bot)";
	}
	cout << " lancia i dadi\t\t\t\t" << endl;
	if (giocatori[g].isBot==false) {
    	cout << "\t";
    	system("Pause");
	}
	gotoXY(0,29);
	cout << "\t\t\t\t\t\t";
	dado1=rand()%6+1;
	dado2=rand()%6+1;
	gotoXY(10,30);
	putchar(218);
	for(i=10;i<17;i++){
    	putchar(196);
	}
	putchar(191);
	for(i=0;i<3;i++){
    	gotoXY(10,i+31);
    	putchar(179);
	}
	gotoXY(10,34);
	putchar(192);
	for(i=0;i<3;i++){
    	gotoXY(18,i+31);
    	putchar(179);
	}
	gotoXY(18,34);
	putchar(217);
	for(i=0;i<7;i++){
    	gotoXY(i+11,34);
    	putchar(196);
	}
	gotoXY(20,30);
	putchar(218);
	for(i=20;i<27;i++){
    	putchar(196);
	}
	putchar(191);
	for(i=0;i<3;i++){
    	gotoXY(20,i+31);
    	putchar(179);
	}
	gotoXY(20,34);
	putchar(192);
	for(i=0;i<3;i++){
    	gotoXY(28,i+31);
    	putchar(179);
	}
	gotoXY(28,34);
	putchar(217);
	for(i=0;i<7;i++){
    	gotoXY(i+21,34);
    	putchar(196);
	}
	dado=rand()%6+1;
	for(i=0;n<581;i++){
    	if(i>0){
        	gotoXY(14,i+30);
        	cout << " ";
        	gotoXY(24,i+30);
        	cout << " ";
    	}
    	if(i==0){
        	gotoXY(14,i+33);
        	cout << " ";
        	gotoXY(24,i+33);
        	cout << " ";
    	}
    	gotoXY(14,i+31);
    	cout << dado;
    	dado=rand()%6+1;
    	gotoXY(24,i+31);
    	cout << dado;
    	if(i>=2){
        	i=-1;
    	}
    	n++;
    	Sleep(n-500);
	}
	gotoXY(14,32);
	cout << dado1 << endl << endl;
	gotoXY(24,32);
	cout << dado2 << endl << endl;
	if (giocatori[g].primoTiro==true) {
    	giocatori[g].primoTiro=false;
    	if (dado1+dado2==9) {
        	if (dado1==4||dado1==5) {
            	log=coutLog(g, log, 5, 1, giocatori);
            	giocatori[g].pos=53;
        	} else {
            	log=coutLog(g, log, 5, 2, giocatori);
            	giocatori[g].pos=26;
        	}
        	somma=0;
    	} else {
        	somma=dado1+dado2;
    	}
	} else {
    	somma=dado1+dado2;
	}
	return somma;
}

bool calcoloPosizione(int dadi, statistiche giocatori[], int turno, int &log, bool suoni, int i, int totP, int match) {
	bool isArrivato=false, uscita;
	int temp, j;
	do {
    	giocatori[turno].pos=giocatori[turno].pos+dadi;
    	temp=giocatori[turno].pos;
    	if (giocatori[turno].pos>63) {
        	giocatori[turno].pos=126-giocatori[turno].pos;
    	}
    	if (!uscita) {
        	coutPos(giocatori, turno, turno);
    	}
    	coutTabellone;
    	uscita=true;
    	switch (giocatori[turno].pos) {
        	case 6:
            	if(suoni){
                	sndPlaySound("Ponte.wav",SND_ASYNC);
                	Sleep(4000);
            	}
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 9:
            	if(suoni){
                	sndPlaySound("Oca",SND_ASYNC);
            	}
            	clearLog(i);
            	disOca();
            	log=clearLog(i);
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 18:
            	if(suoni){
                	sndPlaySound("Oca",SND_ASYNC);
            	}
            	clearLog(i);
            	disOca();
            	log=clearLog(i);
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 19:
            	if(suoni){
                	sndPlaySound("casa (online-audio-converter.com).wav",SND_ASYNC);
            	}
            	clearLog(i);
            	disCasa();
            	log=clearLog(i);
            	giocatori[turno].fermo=3;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 27:
            	if(suoni){
                	sndPlaySound("Oca",SND_ASYNC);
            	}
            	clearLog(i);
            	disOca();
            	log=clearLog(i);
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 31:
            	if(suoni){
                	sndPlaySound("pozzo (online-audio-converter.com).wav",SND_ASYNC);
            	}
            	clearLog(i);
            	disPozzo();
            	log=clearLog(i);
            	for (j=0;j<6;j++) {
                	if (giocatori[j].pos==31||giocatori[j].pos==52) {
                    	giocatori[j].fermo=0;
                    	j=6;
                	}
            	}
            	giocatori[turno].fermo=999;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 36:
            	if(suoni){
                	sndPlaySound("Oca",SND_ASYNC);
            	}
            	clearLog(i);
            	disOca();
            	log=clearLog(i);
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 42:
            	if(suoni){
                	sndPlaySound("labirinto (online-audio-converter.com).wav",SND_ASYNC);
                	Sleep(3000);
            	}
            	giocatori[turno].pos=39;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 45:
            	if(suoni){
                	sndPlaySound("Oca",SND_ASYNC);
            	}
            	clearLog(i);
            	disOca();
            	log=clearLog(i);
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 52:
            	if(suoni){
                	sndPlaySound("Prigione.wav",SND_ASYNC);
            	}
            	clearLog(i);
            	disPrigione();
            	log=clearLog(i);
            	for (j=0;j<6;j++) {
                	if (giocatori[j].pos==31||giocatori[j].pos==52) {
                    	giocatori[j].fermo=0;
                    	j=6;
                	}
            	}
            	giocatori[turno].fermo=999;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 54:
            	if(suoni){
                	sndPlaySound("Oca",SND_ASYNC);
            	}
            	clearLog(i);
            	disOca();
            	log=clearLog(i);
            	uscita=false;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 58:
            	if(suoni){
                	sndPlaySound("Scheletro.wav",SND_ASYNC);
            	}
            	clearLog(i);
            	disScheletro();
            	log=clearLog(i);
            	giocatori[turno].pos=1;
            	if(suoni){
                	sndPlaySound("sottofondo.wav",SND_LOOP | SND_ASYNC);
            	}
            	break;
        	case 63:
            	if(suoni&&match<totP-1){
                	sndPlaySound("Vittoria.wav",SND_ASYNC);
                	Sleep(10000);
            	}
            	isArrivato=true;
            	uscita=true;
    	}
    	if (dadi!=0){
        	log=coutLog(turno, log, 4 , temp, giocatori);
    	}
	} while (!uscita);
	return isArrivato;
}


void calcoloVincitori(statistiche giocatori[], int clas[], int n) {
	int vet[6], i, j, max, temp;
	bool sup[6]={true,true,true,true,true,true};
	for (i=0;i<6;i++) {
    	vet[i]=giocatori[i].win;
	}
	for(i=0;i<n-1;i++) {
    	max=i;
    	for(j=i+1;j<n;j++){
        	if(vet[j]>vet[max]){
            	max=j;
        	}
    	}
    	temp=vet[max];
    	vet[max]=vet[i];
    	vet[i]=temp;
	}
	for (i=0;i<n;i++){
    	for (j=0;j<n;j++){
        	if (vet[j]==giocatori[i].win&&sup[i]){
            	clas[j]=i;
            	sup[i]=false;
            	vet[j]=-1;
        	}
    	}
	}
	for (i=0;i<n;i++) {
    	for (j=i+1;j<n;j++) {
        	if (giocatori[clas[j]].win==giocatori[clas[j-1]].win) {
            	if (giocatori[clas[j]].tiri<giocatori[clas[j-1]].tiri) {
                	temp=clas[j];
                	clas[j]=clas[j-1];
                	clas[j-1]=temp;
            	}
        	}
    	}
	}
}

void coutValore(statistiche giocatori[]) {
	int cas,g;
	bool isThere;
	int coord [64][2] {{7,25},{12,25},{17,25},{22,25},{27,25},{32,25},{37,25},{42,25},{47,25},{52,25},{57,25},
	{57,23},{57,21},{57,19},{57,17},{57,15},{57,13},{57,11},{57,9},{57,7},{57,5},{52,5},{47,5},{42,5},{37,5},
	{32,5},{27,5},{22,5},{17,5},{12,5},{12,7},{12,9},{12,11},{12,13},{12,15},{12,17},{12,19},{12,21},{17,21},
	{22,21},{27,21},{32,21},{37,21},{42,21},{47,21},{47,19},{47,17},{47,15},{47,13},{47,11},{47,9},{42,9},
	{37,9},{32,9},{27,9},{22,9},{22,11},{22,13},{22,15},{22,17},{27,17},{32,17},{37,17},{37,15}};
	for (cas=1; cas<64; cas++) {
    	isThere=false;
    	for (g=0;g<6&&isThere==false;g++) {
            gotoXY(coord[cas][0],coord[cas][1]);
            switch (cas) {
                case 6:
                    setColor(22,0);
                    break;
                case 9:
                    setColor(11,4);
                    break;
                case 18:
                    setColor(11,4);
                    break;
                case 19:
                    setColor(5,15);
                    break;
                case 27:
                    setColor(11,4);
                    break;
                case 31:
                    setColor(4,15);
                    break;
                case 36:
                    setColor(11,4);
                    break;
                case 42:
                    setColor(1,15);
                    break;
                case 45:
                    setColor(11,4);
                    break;
                case 52:
                    setColor(4,15);
                    break;
                case 54:
                    setColor(11,4);
                    break;
                case 58:
                    setColor(7,0);
                    break;
                case 63:
                    setColor(14,0);
                    break;
            }
            if (giocatori[g].pos==cas) {
                setColor(10,0);
                cout << giocatori[g].pedina << " ";
                isThere=true;
            } else {
                cout << cas;
                isThere=false;
            }
            setColor(0,15);
    	}
	}
}

int coutLog(int g, int log, int caso, int vario, statistiche giocatori[]) {
	gotoXY(70,log);
	 switch (caso) {
    	case 1:
        	cout << giocatori[g].nomegiocatore << " ha totalizzato un punteggio di " << vario << "\t";
        	log++;
        	break;
    	case 2:
        	if (vario>10) {
            	cout << giocatori[g].nomegiocatore << " resta fermo fino all'arrivo di";
            	log++;
            	gotoXY(70,log);
            	cout << "un altro giocatore";
            	log++;
        	} else {
            	cout << giocatori[g].nomegiocatore << " resta fermo per " << vario << " turni";
            	log++;
        	}
        	break;
    	case 3:
        	setColor(6,0);
        	cout << giocatori[g].nomegiocatore << " ha vinto la partita " << vario+1 << "!";
        	log++;
        	break;
    	case 4:
        	switch (vario) {
            	case 6:
                	setColor(22,0);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 9:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 18:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 19:
                	setColor(5,15);
                	cout << giocatori[g].nomegiocatore << " e' bloccato per 3 turni!";
                	log++;
                	break;
            	case 27:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 31:
                	setColor(4,15);
                	cout << giocatori[g].nomegiocatore << " e' bloccato fino all'arrivo di";
                	setColor(0,15);
                	cout << "\t\t\t";
                	log++;
                	gotoXY(70,log);
                	setColor(4,15);
                	cout <<"un altro giocatore!";
                	log++;
                	break;
            	case 36:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 42:
                	setColor(1,15);
                	cout << giocatori[g].nomegiocatore << " torna alla casella 39!";
                	log++;
                	break;
            	case 45:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 52:
                	setColor(4,15);
                	cout << giocatori[g].nomegiocatore << " e' bloccato fino all'arrivo di";
                	setColor(0,15);
                	cout << "\t\t\t";
                	log++;
                	gotoXY(70,log);
                	setColor(4,15);
                	cout <<"un altro giocatore!";
                	log++;
                	break;
            	case 54:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " raddoppia il punteggio!";
                	log++;
                	break;
            	case 58:
                	setColor(7,0);
                	cout << giocatori[g].nomegiocatore << " torna alla casella 1!";
                	log++;
                	break;
        	}
        	break;
    	case 5:
        	switch (vario) {
            	case 1:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " ha ottenuto 9 al primo tiro";
                	setColor(0,15);
                	cout << "\t\t\t";
                	log++;
                	gotoXY(70,log);
                	setColor(11,4);
                	cout <<"e si sposta alla casella 53!";
                	log++;
                	break;
            	case 2:
                	setColor(11,4);
                	cout << giocatori[g].nomegiocatore << " ha ottenuto 9 al primo tiro";
                	setColor(0,15);
                	cout << "\t\t\t";
                	log++;
                	gotoXY(70,log);
                	setColor(11,4);
                	cout <<"e si sposta alla casella 26!";
                	log++;
                	break;
        	}
	}
	setColor(0,15);
	return log;
}

int clearLog (int totG) {
	int log;
	for (log=8+totG;log<40;log++) {
    	gotoXY(70,log);
    	cout << "                                               	";
	}
	log=8+totG;
	return log;
}

void disOca (){
	gotoXY(107,14);
	cout << "___" ;
	gotoXY(103,15);
	cout << ",- 	`." ;
	gotoXY(105,15);
	putch(34);
	gotoXY(106,15);
	putch(34);
	gotoXY(101,16);
	cout << ",'  _   e )`-._" ;
	gotoXY(100,17);
	cout << "/  ,' `-._<.===-'" ;
	gotoXY(99,18);
	cout << "/  /" ;
	gotoXY(98,19);
	cout << "/  ;";
	gotoXY(86,20);
	cout << "_      	/   ;";
	gotoXY(74,21);
	cout << "(`._	_.-	--..__,'	|";
	gotoXY(84,21);
	putch(34);
	gotoXY(85,21);
	putch(34);
	gotoXY(87,21);
	putch(34);
	gotoXY(88,21);
	putch(34);
	gotoXY(74,22);
	cout << "<_  `-""                  	";
	gotoXY(80,22);
	putch(34);
	gotoXY(81,22);
	putch(34);
	gotoXY(102,22);
	putch(92);
	gotoXY(75,23);
	cout << "<`-                     	:";
	gotoXY(76,24);
	cout << "(__   <__.              	;";
	gotoXY(78,25);
	cout << "`-.   '-.__.  	_.'	/";
	gotoXY(82,26);
	cout << "  	`-.__,-'	_,'";
	gotoXY(81,26);
	putch(92);
	gotoXY(81,27);
	cout << "`._	,	/__,-'";
	gotoXY(87,28);
	cout << "._ __,'< <____";
	gotoXY(84,28);
	putch(34);
	gotoXY(85,28);
	putch(34);
	gotoXY(89,28);
	putch(92);
	gotoXY(90,29);
	cout << "| |  `----.`.";
	gotoXY(90,30);
	cout << "| |      	`.";
	gotoXY(101,30);
	putch(92);
	gotoXY(90,31);
	cout << "; |___   	-``";
	gotoXY(102,31);
	putch(92);
	gotoXY(91,32);
	cout << "   --<";
	gotoXY(90,32);
	putch(92);
	gotoXY(91,33);
	cout << "`.`.<";
	gotoXY(93,34);
	cout << "`-'";
	Sleep(3000);
}

void disCasa(){
	gotoXY(92,14);
	cout << ") )" ;
	gotoXY(91,15);
	cout << "/" ;
	gotoXY(92,15);
	putch(92);
	gotoXY(79,16);
	cout << "=====  	/  " ;
	gotoXY(93,16);
	putch(92);
	gotoXY(78,17);
	cout << "_|___|_____/ __  ____________" ;
	gotoXY(94,17);
	putch(92);
	gotoXY(77,18);
	cout << "|::::::::::/ |  |  :::::::::::|" ;
	gotoXY(95,18);
	putch(92);
	gotoXY(77,19);
	cout << "|:::::::::/  ====   ::::::::::|";
	gotoXY(96,19);
	putch(92);
	gotoXY(77,20);
	cout << "|::::::::/__________ :::::::::|";
	gotoXY(97,20);
	putch(92);
	gotoXY(77,21);
	cout << "|_________|  ____  |__________|";
	gotoXY(78,22);
	cout << "| ______ | / ||   | _______ |";
	gotoXY(94,22);
	putch(92);
	gotoXY(78,23);
	cout << "||  |   || ====== ||   |   ||";
	gotoXY(78,24);
	cout << "||--+---|| |	| ||---+---||";
	gotoXY(78,25);
	cout << "||__|___|| |   o| ||___|___||";
	gotoXY(78,26);
	cout << "|========| |____| |=========|";
	gotoXY(77,27);
	cout << "(^^-^^^^^-|________|-^^^--^^^)";
	gotoXY(77,28);
	cout << "(,, , ,, ,/________\,,,, ,, ,)";
	gotoXY(76,29);
	cout << "','',,,,' /__________\,,,',',;;";
	Sleep(5000);
}

void disPozzo(){
	gotoXY(80,14);
	cout << "_" ;
	gotoXY(79,15);
	cout << "/" ;
	gotoXY(81,15);
	putch(92);
	gotoXY(78,16);
	cout << "/ '.";
	gotoXY(82,16);
	putch(92);
	gotoXY(77,17);
	cout << "/   '.";
	gotoXY(83,17);
	putch(92);
	gotoXY(76,18);
	cout << "/ 	'.";
	gotoXY(84,18);
	putch(92);
	gotoXY(76,19);
	cout << "~|~~~~~|~";
	gotoXY(76,20);
	cout << "+|-===-|;-.";
	gotoXY(77,21);
	cout << "|  |  |  `-";
	gotoXY(76,22);
	cout << "_|--|--|_";
	gotoXY(75,23);
	cout << "|--.....--|";
	gotoXY(75,24);
	cout << "|--.....--|";
	gotoXY(75,25);
	cout << "|--.....--| .''.";
	gotoXY(75,26);
	cout << "|--.....--| |~~|";
	gotoXY(76,27);
	cout << "~~-----~~   ~~";
	Sleep(5000);
}

void disPrigione(){
	gotoXY(79,14);
	cout << "_________________________" ;
	gotoXY(82,15);
	cout << "||   ||     ||   ||" ;
	gotoXY(82,16);
	cout << "||   ||, , ,||   ||" ;
	gotoXY(82,17);
	cout << "||  (||/|/( ||/  ||" ;
	gotoXY(93,17);
	putch(92);
	gotoXY(82,18);
	cout << "||  ||| _'_`|||  ||" ;
	gotoXY(82,19);
	cout << "||   || o o ||   ||";
	gotoXY(82,20);
	cout << "||  (||  - `||)  ||";
	gotoXY(82,21);
	cout << "||   ||  =  ||   ||";
	gotoXY(82,22);
	cout << "||   || ___/||   ||";
	gotoXY(89,22);
	putch(92);
	gotoXY(82,23);
	cout << "||___||) , (||___||";
	gotoXY(81,24);
	cout << "/||---||- _/-||---||";
	gotoXY(90,24);
	putch(92);
	gotoXY(101,24);
	putch(92);
	gotoXY(80,25);
	cout << "/ ||--_||_____||_--|| ";
	gotoXY(102,25);
	putch(92);
	gotoXY(79,26);
	cout << "(_(||)-| S123-45 |-(||)_)";
	gotoXY(79,27);
	cout << "_________________________";
	Sleep(4000);
}

void disScheletro(){
	gotoXY(79,14);
	cout << ".-." ;
	gotoXY(78,15);
	cout << "(o.o)" ;
	gotoXY(79,16);
	cout << "|=|";
	gotoXY(78,17);
	cout << "__|__";
	gotoXY(76,18);
	cout << "//.=|=.";
	gotoXY(83,18);
	putch(92);
	gotoXY(84,18);
	putch(92);
	gotoXY(75,19);
	cout << "// .=|=. ";
	gotoXY(84,19);
	putch(92);
	gotoXY(85,19);
	putch(92);
	gotoXY(77,20);
	cout << " .=|=. //";
	gotoXY(75,20);
	putch(92);
	gotoXY(76,20);
	putch(92);
	gotoXY(78,21);
	cout << "(_=_)//";
	gotoXY(76,21);
	putch(92);
	gotoXY(77,21);
	putch(92);
	gotoXY(77,22);
	cout << "(:| |:)";
	gotoXY(78,23);
	cout << "|| ||";
	gotoXY(78,24);
	cout << "() ()";
	gotoXY(78,25);
	cout << "|| ||";
	gotoXY(78,26);
	cout << "|| '==";
	gotoXY(77,27);
	cout << "=='";
	Sleep(3000);
}

void coda(int clas[], statistiche giocatori[], int totG, bool suoni) {
    char titoli[MAX][MAX], pos[6][10]{"1 posto:", "2 posto:", "3 posto:", "4 posto:", "5 posto:", "6 posto:"} ,
    sup[MAX][MAX]{"","","","","","","","","","","","","","","","Menu iniziale: ", "    Stefano Galligani", "Input giocatori:",
    "    Marcel Bujor", "Calcolo classifica:", "    Stefano Galligani", "Animazione dadi:", "    Marcel Bujor","Lista eventi:",
    "    Stefano Galligani", "Lista posizioni:", "    Marcel Bujor", "Grafica tabellone:", "    Stefano Galligani",
    "Scelta e preparazione disegni e musica:", "    Stefano Galligani", "Implementazione disegni e musica:", "    Marcel Bujor",
    "Titoli di coda:", "    Stefano Galligani", "Gioco offerto da", "    Marcel Bujor & Stefano Galligani"};
	int i, j, z;
	system("Cls");
	if(suoni){
        sndPlaySound("Classifica.wav",SND_ASYNC);
    }
	for (i=0;i<totG;i++) {
        strcpy(sup[12-(i*2)],pos[totG-i-1]);
        strcpy(sup[12-(i*2)+1],giocatori[clas[totG-i-1]].nomegiocatore);
	}
	for(i=0;i<MAX;i++) {
        for (j=0;j<MAX-1;j++){
            strcpy(titoli[j],titoli[j+1]);
        }
        strcpy(titoli[MAX-1],sup[i]);
        for (z=6-totG;z<MAX;z++){
            if (z>MAX-i) {
                if (z>10&&z<40) {
                    gotoXY(45,z);
                    cout << titoli[z] << "                                    ";
                }
            }
        }
        Sleep(500);
    }
}
