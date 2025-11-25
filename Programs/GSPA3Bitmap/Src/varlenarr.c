
/*
 ********************************************************************
 * @file    varlenarr.c
 * @author  Franz Korf 
 *          HAW-Hamburg
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version V1.0
 * @date    Nov. 2021
 * @brief   Beispiele zum Umgang mit variable length arrays (VLA) 
 *          VLA werden seit C99 unterstuetzt.
 *******************************************************************
 */

/*
 * Genereller Hinweis:
 * ===================
 * - Fuer VLAs muss der Compiler den C99 Standard verwenden. In der Regel wird 
     der Compiler über einen Parameter auf den C99 Standard eingestellt. 
 *   Zum Beispiel: clang -std=c99 varlenarr.c 
 *   
 * Vorbemerkung:
 * =============
 * In der Regel muss in C die Groesse der Variablen - also wieviel Speicher-
 * platz man für ein Objekt von einem gewissen Typ braucht - zur Compilezeit
 * bekannt sein. Auf dieser Basis wird zum Beispiel die Indexberechnung für 
 * den Zugriff auf ein Feldelement realisiert.
 *
 * Somit muss die Dimension eines Arrays zur Compilezeit bekannt sein.
 * Will man dies umgehen, legt man das Feld mit malloc auf dem Heap ab.
 * Ohne die Verwendung von VLAs muss in diesem Fall die Positionsberechnung 
 * beim Zugriff auf ein Array Element explizit programmiert werden. Dies ist
 * für 1-dimensionale Felder  problemlos, weil der Compiler die Größe eines 
 * Feldelements zur Compilezeit kennt. Bei mehrdimensionalen Arrays ist dies 
 * komplexer und fehleranfaelliger. Die ersten beiden Beispiele stellen 
 * diesen Aspekt dar.
 */

#include <stdlib.h>  
// #include <malloc.h> 
#include <stdio.h>

/* 
 * Beispiel 1: Ein eindimensionales Array, dessen Laenge erst zur 
 *             Laufzeit bekannt ist. VLAs werden nicht verwendet.
 */

void test1(unsigned int dim_1) {
   printf("T E S T 1\n");
   // Lege Speicher auf dem Heap an
   int *a = (int *) malloc(dim_1 * sizeof(int)); 

   // Fehlerbehandlung
   if (NULL == a) { return; }

   // initialisiere das Array
   for (int i = 0; i < dim_1; ++i) { a[i] = i; }

   // drucke das Array
   printf("a[0 .. %d] = [ ", dim_1 -1); 
   for (int i = 0; i < dim_1; ++i) { printf("%d ", a[i]); }
   printf("]\n"); 

   // Gebe Speicher auf dem Heap wieder frei
   free(a);
}

/* Bei mehrdimensionalen Arrays ist die Indexberechnung aufwendiger
 * und fehleranfaelliger - eigentlich will man das so nicht programmieren.
 *
 * Beispiel 2: Ein zweidimensionales Feld, dessen Feldlaenge erst zur 
 *             Laufzeit bekannt ist. VLAs werden nicht verwendet.
 */
 
void test2(unsigned int dim_1, unsigned int dim_2) {
   printf("T E S T 2\n");
   // Lege Speicher auf dem Heap an
   int *a = (int *) malloc(dim_1 * dim_2 * sizeof(int)); 

   // Fehlerbehandlung
   if (NULL == a) { return; }

   // initialisiere das Array
   for (int i = 0; i < dim_1; ++i) {
       for (int j = 0; j < dim_2; ++j) {
           a[i * dim_2  + j] = i * 10 + j;
       }
   }

   // drucke das Array
   for (int i = 0; i < dim_1; ++i) {
       printf("a[%d][0 .. %d] = [ ", i, dim_2 -1); 
       for (int j = 0; j < dim_2; ++j) { 
           printf("%d ", a[i * dim_2 + j]); 
       }
       printf("]\n"); 
   }

   // Gebe Speicher auf dem Heap wieder frei
   free(a);
}

/* 
 * Im Beispiel 2 ist die explizite Indexberechnung kompliziert - a[i * dim_2  + j] 
 * man muss den ersten Index mit der zweiten (!) Dimension multiplizieren 
 * und dann den zweiten * Index addieren. Probieren Sie dies mal für ein 
 * dreidimensionales Feld aus und die Fehleranfaelligkeit dieser Vorgehensweise 
 * wird deutlich.
 *
 * Verwendung von variable length arrays (VLA):
 * ============================================
 *
 * Seit dem C99 Standard duerfen die Array Grenzen bei automatic Variablen
 * (lokale Variablen einer Funktion oder Funktionsparameter) und Felder, die
 * auf dem Heap anglegt werden, dynamisch sein - erst beim Aufruf festgelegt 
 * werden. Dieses Konzept nennt man "variable length array" (VLA).
 *
 * VLAs erlauben leichter verständlichen Code und man erspart sich die aufwendige 
 * Zeigerarithmetik für die Positionierung in Feldern mit dynamischer Länge.
 *
 * Beispiel 3: Ein dreidimensionales Array, dessen Laenge erst zur Laufzeit 
 *             bekannt ist. Ein VLA wird verwendet. Das VLA ist hier eine lokale 
 *             (automatische) Variable der Funktion.
 *
 *             Über eine Funktion, der das VLA und die Felddimensionen übergeben werden, 
 *             wird das Feld gedruckt.
 *             Bitte beachten Sie, dass C Arrays als Funktionsparameter stets ueber 
 *             Zeiger auf den aktuellen Parameter realisiert werden (also call by 
 *             reference - es wird nur die Adresse des aktuellen Parameters übergeben).
 */

void printArr(unsigned int dim_1, unsigned int dim_2, unsigned int dim_3, int a[dim_1][dim_2][dim_3]) {
   // drucke das Array
   for (int i = 0; i < dim_1; ++i) {
       for (int j = 0; j < dim_2; ++j) {
           printf("a[%d][%d][0 .. %d] = [ ", i, j, dim_3 -1); 
           for (int k = 0; k < dim_3; ++k) { 
               printf("%d ", a[i][j][k]); 
           }
           printf("]\n"); 
       }
   }
}

void test3(unsigned int dim_1, unsigned int dim_2, unsigned int dim_3) {
   printf("T E S T 3\n");
   int a[dim_1][dim_2][dim_3]; // VLA - die Dimensionen werden erste durch die 
                               //       Parameter dim_1, dim_2 und dim_3 zur Laufzeit festgelegt.
   // Der Speicher fuer a wird auf dem Stack angelegt und nach dem Funktionsaufruf 
   // wieder automatisch geloescht - wie fuer jede Variable der Speicherklasse auto.
   // Damit entfallen die Aufrufe von malloc und free.
                       
   // initialisiere das Array
   for (int i = 0; i < dim_1; ++i) {
       for (int j = 0; j < dim_2; ++j) {
           for (int k = 0; k < dim_3; ++k) {
               a[i][j][k] = i * 100 + j * 10 + k;
           }
       }
   }

   // drucke das Array
   printArr(dim_1, dim_2, dim_3, a);
}

/*
 * Die einfache Indexberechnung für Arrays ist offentsichtlich - a[i][j][k].
 *
 * ACHTUNG Gefahrenstelle:
 * =======================
 * In test3 wird das VLA a auf dem Stack abgelegt. Damit spart man sich 
 * malloc und free. Auf den ersten Blick ist das sehr gut und einfach.
 * Jedoch ist bei grossen VLAs, die lokale Variablen von Funktionen
 * sind, aus folgendem Grund Vorsicht geboten: Bei eingebetteten Systemen 
 * ist der Bereich für den Stack oftmals viel kleiner als der Heap.
 * Wird bei einem Funktionsaufruf der Stackbereich überschritten 
 * (aufgrund des VLA), wird der Fehler i.d.R. nicht gemeldet und seine Auswirkungen
 * in der Regel erst viel spaeter erkannt. Kann hingegen malloc nicht genug 
 * Speicher liefern, wird malloc keine Speicherbereiche ueberschreiben sondern
 * einen Fehler zurueckgeben. Auf diesen kann das Programm dann reagieren.
 * Trotzdem sind kleine VLAs als lokale Funktionsvariablen und (kleine und große)
 * VLAs als Funktionsparameter gut geeignet.
 * Weiterhin kann man in diesem Fall das Feld nur innerhalb der Funktion, in der
 * die automatische Variable angelegt wurde, nutzen.
 *
 * Beispiel 4 liegt das VLA mit malloc auf dem Heap an.
 *
 */

void test4(unsigned int dim_1, unsigned int dim_2, unsigned int dim_3) {
   printf("T E S T 4\n");
   // f ist ein Zeiger, der auf ein zweidimensionales Feld vom Typ int zeigt.
   // Gemäß den C Typen entspricht dies einem dreidimensionalen int Feld.
   int (*f)[dim_2][dim_3] = (int(*)[dim_2][dim_3]) malloc(dim_1*dim_2*dim_3*sizeof(int)); 

   // Fehlerbehandlung
   if (NULL == f) { return; }

   // initialisiere das Array
   for (int i = 0; i < dim_1; ++i) {
       for (int j = 0; j < dim_2; ++j) {
           for (int k = 0; k < dim_3; ++k) {
               f[i][j][k] = i * 100 + j * 10 + k;
           }
       }
   }

   // drucke das Array
   printArr(dim_1, dim_2, dim_3, f);

   // Gebe dem Speicher auf dem Heap wieder frei
   free(f);
}

/* 
 * Der Weg des letzten Beispiels ist gut für Praktikumsaufgabe 3 geeignet.
 */

#define DIM_1    5
#define DIM_2    7
#define DIM_3    4
/*
int main(void) {
   test1(DIM_1);
   test2(DIM_1, DIM_2);
   test3(DIM_1, DIM_2, DIM_3);
   test4(DIM_1, DIM_2, DIM_3);
}
*/
// EOF

