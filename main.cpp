#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

#define inf 4e18

struct punct
{
    float x;
    float y;
};

struct dreapta
{
    punct stg,dr;
    float panta;
};

struct punct_minim
{
    punct coord;
    int poz;
};
void Citire(vector<punct> &P, int &n, punct &A)
{
    fin >> A.x >> A.y; //pe prima linie punctul A
    fin >> n; //pe a 2-a linie numarul de varfuri ale poligonului

    /*pe urmatoarele n linii punctele ce definesc poligonul P, in ordine
    trigonometrica*/
    for (int i = 0; i < n; i++)
    {
        punct p;
        fin >> p.x >> p.y;
        P.push_back(p);
    }

    fin.close();
}

//verificam daca punctul coincide cu unul din vf patrulaterului
int Verificare_varf(vector<punct> &P, int n, punct A)
{
    for (int i = 0; i < n; i++)
        if(A.x == P[i].x && A.y == P[i].y)
        {
            cout<<"A coincide cu un varf" << endl;
            return 1;
        }
    return 0;
}

//calculeaza distanta euclidiana dintre 2 puncte
float distanta_intre_2_puncte(punct a, punct b)
{
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

//verificam daca punctul se gaseste pe latura
int Verificare_latura(vector<punct> &P, int n, punct A)
{
    float d1,d2;

    for (int i = 0; i < n-1; i++)
    {
        d1 = distanta_intre_2_puncte(A,P[i]) + distanta_intre_2_puncte(A,P[i+1]);
        d2 = distanta_intre_2_puncte(P[i],P[i+1]);

        if (d1 == d2)
        {
            cout << "A este pe latura" << endl;
            return 1;
        }
    }

    //cand A este pe latura care inchide poligonul
    d1 = distanta_intre_2_puncte(A,P[n-1]) + distanta_intre_2_puncte(A,P[0]);
    d2 = distanta_intre_2_puncte(P[n-1],P[0]);

    if (d1 == d2)
    {
        cout << "A este pe latura" << endl;
        return 1;
    }
    return 0;
}

//comparator pentru ordonarea in functie de panta
bool comp(const dreapta &a1, const dreapta &a2)
{
    return(a1.panta < a2.panta);
}

punct_minim Gasire_punct_stg(vector<punct> &P, int n)
{
    /*gasim cel mai din stanga punct al poligonului pentru a putea detremina
    un punct exterior*/
    punct_minim minim;
    minim.coord = P[0];
    minim.poz = 0;

    for (int i = 1; i < n; i++)
        if (P[i].x < minim.coord.x)
        {
            minim.coord = P[i];
            minim.poz = i;
        }
        else if (P[i].x == minim.coord.x)
            if (P[i].y < minim.coord.y)
            {
                minim.coord = P[i];
                minim.poz = i;
            }
    return minim;
}

//efectuam o cautare dupa panta in lista de laturi
int cautare(vector<dreapta> &D, float panta, int s, int d, punct A)
{
    if (s > d)
        return d;
    else
    {
        int m = (s + d) / 2;
        if (panta == D[m].panta) //punctul se afla pe o diagoala a poligonului
        {
            if (abs((int)A.x) < abs((int)D[m].dr.x)) //punctul e in interior
            {
                cout << "A este in interior" << endl;
                cout << "A este pe diagonala " << "(" << D[m].stg.x << ",";
                cout << D[m].stg.y << ");(" << D[m].dr.x << "," << D[m].dr.y;
                cout << ")" << endl;
            }
            else
            {
                cout << "A este in exterior";
            }

            exit(0);
        }
        if (panta < D[m].panta)
            return cautare(D,panta,s,m-1,A);
        else
            return cautare(D,panta,m+1,d,A);
    }
}

//calculeaza determinantul
float Determinant(punct A, punct B, punct C)
{
    return A.x*B.y + B.x*C.y + A.y*C.x - B.y*C.x - A.x*C.y - A.y*B.x;
}

void Verificare_interior_exterior(vector <dreapta> &D, int n, punct A, int poz)
{
    float d1, d2, d3;

    d1 = Determinant(D[0].stg, D[poz].dr, A);
    d2 = Determinant(D[poz+1].dr, D[0].stg, A);
    d3 = Determinant(D[poz].dr, D[poz+1].dr, A);

    if ((d1 < 0 && d2 < 0 && d3 < 0) || (d1 >= 0 && d2 >= 0 && d3 >= 0))
    {
        cout << "A este in interior\n";
        cout << "A se afla in triunghiul: " << "(" << D[0].stg.x << ",";
        cout << D[0].stg.y << ");(" << D[poz].dr.x << "," << D[poz].dr.y;
        cout << ");(" << D[poz+1].dr.x << "," << D[poz+1].dr.y << ")" << endl;
    }
    else
        cout << "A este in exterior";
    return;
}

/*verific unde se afla punctul fata de cele 2 laturi extreme ale poligonului
(extrem stg si dr) - folosim testul de orientre*/
void Verificare_exterior(vector<dreapta> &D, int n, punct A)
{
    float ext_dr, ext_stg;

    punct E,B,C;
    E.x = D[0].stg.x;
    E.y = D[0].stg.y;
    B.x = D[0].dr.x;
    B.y = D[0].dr.y;
    C.x = A.x;
    C.y = A.y;

    ext_dr = Determinant(E,B,C);

    E.x = D[0].stg.x;
    E.y = D[0].stg.y;
    B.x = D[n-2].dr.x;
    B.y = D[n-2].dr.y;
    C.x = A.x;
    C.y = A.y;

    ext_stg = Determinant(E,B,C);

    //daca nu e in stg ext_dr si in dr ext_stg inseamna ca se afla in exterior
    if(!(ext_dr > 0 && ext_stg < 0))
    {
        cout << "A este in exterior" << endl;
        exit(0);
    }
}

void Determinare_interior_sau_exterior(vector<punct> &P, int n, punct A)
{
    vector<dreapta> D;
    dreapta d;

    punct_minim extrem_stg_poligon = Gasire_punct_stg(P,n);

    /*ordonam multimea de puncte
    pe prima pozitie in vectorul P va ajunge punctul din extremitatea stanga
    a poligonului urmat de celelalte puncte(in ordine)*/
    for (int i = 0; i < extrem_stg_poligon.poz; i++)
        P.push_back(P[i]);

    P.erase(P.begin(),P.begin()+extrem_stg_poligon.poz);

    extrem_stg_poligon.poz = 0;

    /*luam un varf al poligonului P[0] si retinem in vectorul D toate dreptele
     care au un capat in varful P[0] si celalalt capat in restul varfurilor
     poligonului*/
    for (int i = 1; i < n; i++)
    {
        d.stg = P[extrem_stg_poligon.poz];
        d.dr = P[i];
        if (P[extrem_stg_poligon.poz].x == P[i].x)
            d.panta = inf;
        else
        {
            float a = (P[extrem_stg_poligon.poz].y - P[i].y);
            float b = (P[extrem_stg_poligon.poz].x - P[i].x);
            d.panta = a / b;
        }
        D.push_back(d);
    }

    /*panta dreptei ce contine punctul din extremitatea stanga a poligonului
    si punctul A*/
    float panta_principala;

    float a = (P[extrem_stg_poligon.poz].y - A.y);
    float b = (P[extrem_stg_poligon.poz].x - A.x);
    panta_principala = a / b;

    //verific daca punctul se afla in exteriorul sectoarelor
    Verificare_exterior(D,n,A);

    /*cautare binara dupa panta pentru a afla in ce sector se afla punctul
    nostru*/
    int pozitie = cautare(D,panta_principala,0,n-1,A);

    /*verificam daca punctul A este in interiorul sau in exteriorul triunghiului
    format din 2 diagonale si o latura a poligonului*/
    Verificare_interior_exterior(D, n, A, pozitie);
}

int main()
{
    vector <punct> P;
    punct A;
    int n;

    Citire(P,n,A);

    if(Verificare_varf(P,n,A) == 1) //A coincide cu un varf
        return 0;

    if(Verificare_latura(P,n,A) == 1) //A se afla pe o latura
        return 0;

    Determinare_interior_sau_exterior(P,n,A);

    return 0;
}
