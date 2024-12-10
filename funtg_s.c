/*  Konputagailuen Arkitektura - Informatika Ingeniaritza (IF - EHU)
    OpenMP laborategia - PROIEKTUA

    funtg_s.c
    taldegen_s.c programan erabiltzen diren errutinak

    OSATZEKO
******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>		// DBL_MAX
#include <float.h>

#include "definetg.h"		// konstante eta datu-egituren definizioak



/* 1 - Bi elementuren arteko distantzia genetikoa kalkulatzeko funtzioa 
       (distantzia euklidearra)

       Sarrera:  ALDAKOP aldagaiko bi elementu (erreferentziaz)
       Irteera:  distantzia (double)
******************************************************************************************/

double distantzia_genetikoa (float *elem1, float *elem2)
{
   
  // EGITEKO
  // Kalkulatu bi elementuren arteko distantzia (euklidearra)

}




/* 2 - Talde gertuena kalkulatzeko funtzioa (zentroide gertuena)

       Sarrera:  elekop   elementu kopurua, int
                 elem     EMAX x ALDAKOP tamainako matrizea
                 zent     taldekop x ALDAKOP tamainako matrizea
       Irteera:  sailka   EMAX tamainako bektorea, elementu bakoitzari dagokion taldea
******************************************************************************************/

void talde_gertuena (int elekop, float elem[][ALDAKOP], float zent[][ALDAKOP], int *sailka)
{

  // EGITEKO
  // sailka: elementu bakoitzaren zentroide hurbilena, haren "taldea"

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

  // EGITEKO


  // Kalkulatu taldeen trinkotasuna: kideen arteko distantzien batezbestekoa

  // Kalkulatu zentroideen trinkotasuna: zentroide bakoitzeko, besteekiko b.b.-ko distantzia 

  // Kalkulatu cvi indizea
  
}



void mergeOrdenazioa(float arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    float leftArr[n1], rightArr[n2];

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}
void eritasunen_analisia (struct taldeinfo *kideak, float eri[][ERIMOTA], struct analisia *eripro)
{

  // EGITEKO
  // Prozesatu eritasunei buruzko informazioa talde bakoitzeko kideen artean:
// eritasunak agertzeko probabilitateen mediana.
  // Eritasun bakoitzerako, medianen maximoa eta minimoa eta zein taldetan.
  //
  // Pseudokodea:
  // I begizta erimota arte, honek eritasun bakoitzeko iteratuko du.
  //      Mediana float array bat taldekop tamainakoa taldeen medianak gordetzeko
  //      J begizta taldekop arte, honek eritasun bakoitzean taldeak iteratuko ditu
  //            arr[] float array bat (malloc) kideak[j].kop tamainakoa honek elementuen informazioa izango du.
  //            K begizta kideak[j].kop tamainakoa, honek eritasun bakoitzeko talde bakoitzeko elementuak iteratuko ditu.
  //                  kideak[j].osagaiak[k] begiratu eri[i]n eta arr[] en jarri.
  //            ordenatu arr eta i/2 balioa hartu medianak arraian sartu
  //      medianen maximoa eta minimoa atera, eripron-sartu
  //
  for (int i = 0; i < ERIMOTA; i++) {
      float medianak[taldekop];
      for (int j = 0; j < taldekop; j++) {
          int kop = kideak[j].kop;
          float *arr = (float *)malloc(kop * sizeof(float));

          for (int k = 0; k < kop; k++) {
              arr[k] = eri[kideak[j].osagaiak[k]][i];
          }
          mergeOrdenazioa(arr, 0, kop/2, kop - 1);
          medianak[j] = arr[kop / 2];
      }
      float mmax = -1;
      float mmin = 101;
      int tmax = -1;
      int tmin = -1;
  
      for (int j = 1; j < taldekop; j++) {
          if (medianak[j] > mmax) {
              mmax = medianak[j];
              tmax = j;
          }
          if (medianak[j] < mmin) {
              mmin = medianak[j];
              tmin = j;
          }
      }
  
      eripro[i].mmax = mmax;
      eripro[i].mmin = mmin;
      eripro[i].taldemax = tmax;
      eripro[i].taldemin = tmin;
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
