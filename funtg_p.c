/*  Konputagailuen Arkitektura - Informatika Ingeniaritza (IF - EHU)
    OpenMP laborategia - PROIEKTUA

    funtg_s.c
    taldegen_s.c programan erabiltzen diren errutinak

    OSATZEKO
******************************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>		// DBL_MAX
#include <float.h>
#include <stdbool.h>
#include <omp.h>

#include "definetg.h"		// konstante eta datu-egituren definizioak



/* 1 - Bi elementuren arteko distantzia genetikoa kalkulatzeko funtzioa 
       (distantzia euklidearra)

       Sarrera:  ALDAKOP aldagaiko bi elementu (erreferentziaz)
       Irteera:  distantzia (double)
******************************************************************************************/

double distantzia_genetikoa (float *elem1, float *elem2)
{
  // Kalkulatu bi elementuren arteko distantzia (euklidearra)
  double sum = 0;
  //Elementu guztien distantziaren karratua gehitu
  for ( int i=0; i<ALDAKOP; i++ )
  {
    sum += pow( elem1[i] - elem2[i] , 2);
  }
  //distantziaren erro karratua atera.
  return sqrt(sum);
}




/* 2 - Talde gertuena kalkulatzeko funtzioa (zentroide gertuena)

       Sarrera:  elekop   elementu kopurua, int
                 elem     EMAX x ALDAKOP tamainako matrizea
                 zent     taldekop x ALDAKOP tamainako matrizea
       Irteera:  sailka   EMAX tamainako bektorea, elementu bakoitzari dagokion taldea
******************************************************************************************/

void talde_gertuena (int elekop, float elem[][ALDAKOP], float zent[][ALDAKOP], int *sailka)
{
  int talde_gertuena;
  double distantzia, distantziaMin;
  // EGITEKO
  // sailka: elementu bakoitzaren zentroide hurbilena, haren "taldea"
  // i eta j ez dute privatean egon behar hari barruan deklaratzerakoan pribatuak direlako definizioz
  // gainera, ez dira oraindik existitzen
  #pragma omp parallel for default(none) \
    private(distantziaMin,distantzia, talde_gertuena) \
    shared(taldekop, elekop, elem, sailka, zent)
  for (int i=0; i<elekop; i++)
  {
    talde_gertuena = -1;
    distantziaMin = DBL_MAX;
    //talde bakoitzaren distantzia minimoa eskuratu.
    for (int j=0; j<taldekop; j++)
    {  
      distantzia = distantzia_genetikoa(elem[i], zent[j]); 
      if (distantzia < distantziaMin) {
        distantziaMin = distantzia;
        talde_gertuena = j;
      }
    }
    //distantzia minimioa gorde
    sailka[i] = talde_gertuena;
  }
}




/* 3 - Sailkapenaren balidazioa: taldeen trinkotasuna eta zentroideen trinkotasuna
       cvi indizea kalkulatzen da

       Sarrera:  elem     fitxategiko elementuak (EMAX x ALDAKOP tamainako matrizea)
                 kideak   taldekideen zerrenda (taldekop tamainako struct-bektore bat: elem eta kop)
                 zent     taldeen zentroideak (taldekop x ALDAKOP)
       Irteera:  cvi indizea
                 talde_trinko    taldeen trinkotasuna
******************************************************************************************/

double balidazioa (float elem[][ALDAKOP], struct taldeinfo *kideak, float zent[][ALDAKOP], float *talde_trinko)
{
  // Kalkulatu taldeen trinkotasuna: kideen arteko distantzien batezbestekoa
  double a_bb, b_bb, talde_bereizketa[taldekop], max, cvi; // Zentroide bakoitzaren batez batezbesteko distantzia besteekiko
  int count;

  for (int i=0; i<taldekop; i++) 
  {
    a_bb = 0; count = 0;
    //taldeak elementurik ez badu edo elementu bat bakarrik izatekotan, ez da zenbatzen batezbestekoarentzako
    if (kideak[i].kop>1)
    {
      #pragma omp parallel for default(none) \
        shared(kideak, elem, i) \
        reduction(+:a_bb) \
        reduction(+:count) \
        schedule(runtime)
      //elementu guztien distantzia konparatzen dugu
      for (int j=0; j<kideak[i].kop-1; j++)
      {
         for (int k=j+1; k<kideak[i].kop; k++)
        {
          count++;
          a_bb += distantzia_genetikoa( elem[ kideak[i].osagaiak[j] ] , elem[ kideak[i].osagaiak[k] ] );
        }
      }
      //distantzien batura zenbatutako elementuen kopuruarengatik zatitzen da, batazbestekoa lortuz.
      talde_trinko[i] = a_bb / count;
    }

    // Kalkulatu zentroideen trinkotasuna: zentroide bakoitzeko, besteekiko b.b.-ko distantzia
    b_bb = 0.0;
    #pragma omp parallel for default(none) \
      shared(taldekop, zent, i) \
      reduction(+:b_bb) \
      schedule(runtime)
    for (int j=0; j<taldekop; j++)
    {
      if (i != j) b_bb += distantzia_genetikoa(zent[i], zent[j]);
    }

    talde_bereizketa[i] = b_bb/(taldekop-1);
  }

  // Kalkulatu cvi indizea
  #pragma omp parallel for default(none) \
    shared(taldekop, talde_bereizketa, talde_trinko) \
    reduction(+:cvi) \
    schedule(runtime)
  for (int i=0; i<taldekop; i++)
  {
    cvi += (talde_bereizketa[i] - talde_trinko[i]) /
      (talde_bereizketa[i] > talde_trinko[i] ? talde_bereizketa[i] : talde_trinko[i]);
  }

  return cvi / taldekop;
}

//bi float motatako datuak trukatzen du.
void swap(float* xp, float* yp){
  float temp = *xp;
  *xp = *yp;
  *yp = temp;
}

//buble sort ordenazio algoritmoa erabiliz float motatako datuen arraya ordenatzen du.
void bubbleSort(float arr[], int n){
  int i, j;
  bool swapped;
  for (i = 0; i < n - 1; i++) {
    swapped = false;
    for (j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
          swap(&arr[j], &arr[j + 1]);
          swapped = true;
      }
    }

    if (swapped == false)
        break;
  }
}


/* 4 - Eritasunak analizatzeko funtzioa
       Sarrera:  kideak  taldekideen zerrenda (taldekop tamainako struct-bektore bat: elem eta kop)
                 eri     eritasunei buruzko informazioa (EMAX x ERIMOTA)
       Irteera:  eripro  eritasunen analisia: medianen maximoa/minimoa, eta taldeak
******************************************************************************************/
void eritasunen_analisia (struct taldeinfo *kideak, float eri[][ERIMOTA], struct analisia *eripro)
{
  float mediana, *arr;
  int kop;

  for (int i = 0; i < taldekop; i++)
    #pragma parallel for default(none) \
      shared(i, eri, eripro, kideak) \
      private(arr, mediana, kop) \
      schedule(runtime)
    for (int j = 0; j < ERIMOTA; j++)
    {
      kop = kideak[i].kop;
      //Talde batek ez badu elementurik hurrengo taldeekin jarraitzen da.
      if (kop == 0) continue;
      //array dinamikoa erabiltzen da (malloc) talde bakoitzak elementu kopuru desberdina duelako.
      arr = (float *)malloc(kop * sizeof(float));
      // array batean talde eta eritasun bateko elementu guztiak sartzen ditu
      for (int k = 0; k < kop; k++)
      {
          arr[k] = eri[kideak[i].osagaiak[k]][j];
      }
      //arraya ordenatzen da mediana arrayaren erdiko posizioaren balioa izan dadin.
      bubbleSort(arr, kop);
      mediana = arr[kop / 2];
      // malloc egiturak eskatzen duen memoria askatzeko funtzioa
      free(arr);

      // Mediana maximoa eta minimoa eskuratzen ditu honek taldearen barruan hartzen duen posizioarekin.
      // Lehenengo elementuak beti esleitzen dira.  
      if (i==0 || mediana > eripro[j].mmax) {
          eripro[j].mmax = mediana;
          eripro[j].taldemax = i;
      }
      if (i==0 || mediana < eripro[j].mmin) {
        eripro[j].mmin = mediana;
        eripro[j].taldemin = i;
      }
    }
}


// PROGRAMA NAGUSIAREN BESTE BI FUNTZIO
// ====================================


/* 5 - Zentroideen hasierako balioak
**************************************************************/

void hasierako_zentroideak (float zent[][ALDAKOP])
{
  int  i, j; 


  srand (147);

  for (i=0; i<taldekop; i++)
  for (j=0; j<ALDAKOP/2; j++)
  {
    zent[i][j] = (rand() % 10000) / 100.0;
    zent[i][j+ALDAKOP/2] = zent[i][j];
  }
}


/* 6 - Zentroide berriak; erabaki bukatu behar den edo ez
**************************************************************/

int zentroide_berriak (float elem[][ALDAKOP], float zent[][ALDAKOP], int *sailka, int elekop)
{
  int     i, j, bukatu;
  float   zentberri[taldekop][ALDAKOP];
  double  diszent;
  double  baturak[taldekop][ALDAKOP+1];


  bukatu = 1;	// 1: simulazioa bukatu da; 0: jarraitu behar da

  // Zentroide berriak: kideen dimentsio bakoitzeko aldagaien batezbestekoak
  // sailka: elementu bakoitzaren taldea
  // baturak: taldekideen aldagaien balioak batzeko; azkena kopurua da

  for (i=0; i<taldekop; i++)
  for (j=0; j<ALDAKOP+1; j++)
    baturak[i][j] = 0.0;

  for (i=0; i<elekop; i++)
  {
    for (j=0; j<ALDAKOP; j++)
      baturak[sailka[i]][j] += elem[i][j];	// batu taldekideen balioak aldagai bakoitzeko

    baturak[sailka[i]][ALDAKOP] ++;		// azken osagaia (ALDAKOP) kopuruak (zenbat) gordetzeko
  }

  // Kalkulatu taldeetako zentroide berriak, eta erabaki bukatu behar den (DELTA1)
  
  for (i=0; i<taldekop; i++)
  {
    if (baturak[i][ALDAKOP] > 0)		// taldea ez dago hutsik
    {
      // Zentroide berrien "koordenatuak": kideen batezbestekoak
      for (j=0; j<ALDAKOP; j++)
        zentberri[i][j] = baturak[i][j] / baturak[i][ALDAKOP];

      // Erabaki bukatu behar den
      diszent = distantzia_genetikoa (&zentberri[i][0], &zent[i][0]);
      if (diszent > DELTA1)  bukatu = 0;	// taldeko zentroidea mugitu da DELTA1 baino gehiago; segi simulazioarekin
 
      // Kopiatu zentroide berria
      for (j=0; j<ALDAKOP; j++)
        zent[i][j] = zentberri[i][j];
    }
  }

  return (bukatu);
}
