#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>
#include <stack>
using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

struct nod {
	int info, grad;
	nod *p; // parinte
	nod *f; // frate
	nod *c; // copil
};

class HeapBinomial{
private:
    nod *radacina1;
    static void nodcopy(nod *to, nod *from)
    {
        to->info = from->info;
		to->grad = from->grad;
		to->p = from->p;
		to->f = from->f;
		to->c = from->c;
    }
    static void initnod(nod *to_init, int info, int grad) {
		to_init->info = info;
		to_init->grad = grad;
		to_init->p = nullptr;
		to_init->f = nullptr;
		to_init->c = nullptr;
	}

	static void merge_trees (nod *r1, nod *r2)
	{
	    if (r1->grad == r2->grad)
        {
            if (r1->info > r2->info)
            {
                r1->p = r2;
                r1->f = r2->c;
                r2->c = r1;
                r2->grad += 1;
            }
            else
            {
                r2->p = r1;
                r2->f = r1->c;
                r1->c = r2;
                r1->grad += 1;
            }
        }
	}
public:
    HeapBinomial(){radacina1 = nullptr;}
    HeapBinomial(const HeapBinomial &other){radacina1 = other.radacina1;}
    ~HeapBinomial()
    {
        nod* curent = radacina1;
		while (curent != nullptr)
        {
			queue<nod*> q;
			q.push(curent);
			nod* next = new nod;
            next = curent->f;
			while (!q.empty())
            {
				nod* p = q.front();
				q.pop();

				if (p->c != nullptr)
                {
					nod* temp = p->c;
					while (temp != nullptr)
					{
						q.push(temp);
						temp = temp->f;
					}
				}
				cout<<"\nAm sters "<<p->info<<".\n";
				//delete p;
				//pot sterge din pointeri fara griji doar daca nu am merge
				//altfel cand sterg pointerii la call-ul unui destructor
				//se sterg si din reuniune
			}
			curent = next;
		}
		cout<<"\n~HB\n";
    }
    nod* get_rad(){return radacina1;}
    void set_rad(nod* other){radacina1 = other;}
    void binomify()
    {
        nod* curent = radacina1;
		nod* prev = nullptr;
		nod* next = curent->f;

		while (next != nullptr)
        {
			if ((curent->grad != next->grad )|| (next->f != nullptr && curent->grad == next->f->grad))
            {
				prev = curent;
				curent = next;
			}
			else
            {
				if (curent->info <= next->info)
				{
					curent->f = next->f;
					HeapBinomial::merge_trees(curent, next);
				}
                else
                {
					if (prev == nullptr)
					{
						radacina1 = next;
					}
                    else
                    {
						prev->f = next;
					}

					HeapBinomial::merge_trees(next, curent);
					curent = next;
				}
			}

			next = curent->f;
		}
    }
    void insereaza (int info)
    {
        nod *to_insert = new nod;
        initnod(to_insert, info, 0);

        if (radacina1!=nullptr && radacina1->f!=nullptr)
        {
            to_insert->f = radacina1;
            nod *temp = radacina1;
            radacina1 = to_insert;
            to_insert = temp;
        }
        else if(radacina1!=nullptr)
        {
            to_insert->f = radacina1;
            nod *temp = radacina1;
            radacina1 = to_insert;
            to_insert = temp;
        }
        else
        {
            set_rad(to_insert);
        }
        binomify();
    }
    void const print_heap()
    {
		nod* curent = radacina1;
		while (curent != nullptr)
        {
			cout<<"Grad: "<<curent->grad<<"\n";
			cout<<"Noduri: "<<pow(2, curent->grad)<<"\n";
			cout<<"Traversarea: "<<"\n";
			queue<nod*> coada;
			coada.push(curent);
			while (!coada.empty())
            {
				nod* temp = coada.front();
				coada.pop();

				cout<<temp->info<<" ";

				if (temp->c != nullptr)
                {
					nod* tempp = temp->c;
					while (tempp != nullptr)
                    {
						coada.push(tempp);
						tempp = tempp->f;
					}
				}
			}
			curent = curent->f;
			cout<<"\n\n";
		}
	}
    void merge_heaps(HeapBinomial &h1)
    {
		nod* curent1 = get_rad();
		nod* curent2 = h1.get_rad();
		nod* curent3 = new nod;
		nod* temp = nullptr;

		if (curent1->grad <= curent2->grad)
        {
			nodcopy(curent3, curent1);
			curent1 = curent1->f;
		}
		else
        {
			nodcopy(curent3, curent2);
			curent2 = curent2->f;
		}

		temp = curent3;

		while(curent1 != nullptr && curent2 != nullptr) //uneste intr-o singura structura toate heap-urile in ordinea gradului
        {
			if (curent1->grad <= curent2->grad)
			{
			    nod *nou = new nod;
			    nodcopy(nou, curent1);
				curent3->f = nou;
				curent1 = curent1->f;
			}
            else
            {
				nod *nou = new nod;
			    nodcopy(nou, curent2);
				curent3->f = nou;
				curent2 = curent2->f;
			}

			curent3 = curent3->f;
		}

		if (curent1 != nullptr) //adauga restul de heapuri din prima structura
        {
			while(curent1 != nullptr)
			{
				nod *nou = new nod;
			    nodcopy(nou, curent1);
				curent3->f = nou;

				curent1 = curent1->f;
				curent3 = curent3->f;
			}
		}

		if (curent2 != nullptr)
        {
			while(curent2 != nullptr) //adauga restul de heapuri din a doua structura
			{
				nod *nou = new nod;
			    nodcopy(nou, curent2);
				curent3->f = nou;

				curent2 = curent2->f;
				curent3 = curent3->f;
			}
		}
		set_rad(temp);
		// reface structura de heap binomial
		binomify();
	}
    void build()
    {
        unsigned n;
        cout<<"\nIntroduceti numarul de elemente pe care doriti sa le contina heap-ul: ";
        cin>>n;
        for (unsigned i=0; i<n; i++)
        {
            cout<<"\nIntroduceti un element: ";
            int temp;
            cin>>temp;
            insereaza(temp);
        }
        cout<<"\nHeap-ul a fost construit cu succes.\n";
        print_heap();
    }
    void delete_min()
    {
		nod* curent = radacina1;
		nod* prevp = nullptr;
		nod* prevmin = nullptr;
		nod* minp = nullptr;
		int mini = 999999;

		while (curent != nullptr) //cauta radacina cu cea mai mica valoare
        {
			if (curent->info <= mini)
            {
				prevmin = prevp;
				minp = curent;
				mini = curent->info;
			}
			prevp = curent;
			curent = curent->f;
		}

		if (prevmin==nullptr && minp != nullptr) //cand minimul este radacina1
        {
            nod* temp;
            temp = radacina1;
            radacina1 = temp->f;
        }
		else if (prevmin != nullptr && minp->f != nullptr) //cand minimul este in interiorul listei de radacini
			{prevmin->f = minp->f;}
		else if (prevmin != nullptr && minp->f == nullptr) //cand minimul este la finalul listei de radacini
			{prevmin->f = nullptr;}

        if (minp->c != nullptr) //daca minimul sters avea copii
        {
            nod* copil = minp->c;
            while (copil != nullptr) //sterge legaturile dintre minim si copiii lui
            {
                copil->p = nullptr;
                copil = copil->f;
            }

            stack<nod*> s;
            copil = minp->c;
            while (copil != nullptr) //introduce copiii minimului intr un stack
            {
                s.push(copil);
                copil = copil->f;
            }

            curent = s.top(); //construieste un heap binomial cu radacinile = copiii minimului
            nod* temp = curent;
            s.pop();

            while (!s.empty())
            {
                curent->f = s.top();
                s.pop();
                curent = curent->f;
            }

            curent->f = nullptr;
            HeapBinomial h;
            h.set_rad(temp);

            merge_heaps(h);//uneste heap ul original cu cel nou creat

            h.set_rad(nullptr); // seteaza radacina1 a noului heap creat la nullptr pentru a evita stergerea pointerilor (din cauza apelarii destructorului pe obiectul local) la iesirea din scope
        }
        delete minp;
    }
    void delete_val(int to_delete)
    {
        nod* curent = radacina1;
        nod* what_delete = nullptr;
        bool am_val = false;

        while(curent != nullptr && am_val == false)
        {
            if (curent->info == to_delete)
            {
                what_delete = curent;
                am_val = true;
                break;
            }
            else if(curent->info < to_delete && curent->c != nullptr)
            {
                queue<nod*> coada;
                coada.push(curent);
                while (!coada.empty())
                {
                    nod* temp = coada.front();
                    coada.pop();

                    if (temp->info == to_delete)
                    {
                        what_delete = temp;
                        am_val = true;
                        break;
                    }

                    if (temp->c != nullptr)
                    {
                        nod* tempp = temp->c;
                        while (tempp != nullptr)
                        {
                            coada.push(tempp);
                            tempp = tempp->f;
                        }
                    }
                }
            }
            curent = curent->f;
        }

        if(am_val)
        {
            cout<<"\nAm gasit: "<<what_delete->info<<".\n";
            cout<<"\nCu parinte: "<<what_delete->p->info<<".\n";
            what_delete->info = -999999;
            while(what_delete->p != nullptr)
            {
                int temp = what_delete->info;
                what_delete->info = what_delete->p->info;
                what_delete->p->info = temp;
                what_delete = what_delete->p;
            }
            delete_min();
        }
        else
        {
            cout<<"\nNu s-a gasit valoarea ceruta in Heap!\n";
        }
    }
    nod* find_min()
    {
        nod* curent = radacina1;
		nod* prevp = nullptr;
		nod* prevmin = nullptr;
		nod* minp = nullptr;
		int mini = 999999;

		while (curent != nullptr) //cauta radacina cu cea mai mica valoare
        {
			if (curent->info <= mini)
            {
				prevmin = prevp;
				minp = curent;
				mini = curent->info;
			}
			prevp = curent;
			curent = curent->f;
		}
		return minp;
    }
};

int main(){
    /*HeapBinomial h;
    h.build();
    h.delete_min();
    HeapBinomial h1;
    h1.build();
    h1.delete_min();
    h.merge_heaps(h1);
    h.print_heap();
    h.print_heap();*/

    HeapBinomial h;
    unsigned N,i;
    int x,y;
    /*cout<<"Introduceti numarul de cerinte: ";
    cin>>N;
    for(i=0;i<N;i++)
    {
        cout<<"\nIntroduceti cerinta: ";
        cin>>x;
        if(x==1)
        {
            cin>>y;
            h.insereaza(y);
        }

        if(x==2)
        {
            cin>>y;
            h.delete_val(y);
        }

        if(x==3)
        {
            cout<<"\nMinimul este: "<<h.find_min()->info<<"\n";
        }

        if(x==4)
        {
            h.delete_min();
        }
    }*/
    fin>>N;
    for(i=0;i<N;i++)
    {
        fin>>x;
        if(x==1)
        {
            fin>>y;
            h.insereaza(y);
        }

        if(x==2)
        {
            fin>>y;
            h.delete_val(y);
        }

        if(x==3)
        {
            fout<<"\nMinimul este: "<<h.find_min()->info<<"\n";
        }

        if(x==4)
        {
            h.delete_min();
        }
    }
    return 0;
}
