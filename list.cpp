#include "list.h"

using namespace list;

void dealloca(List& l){
	unsigned int new_size = l->maxsize - blockSize
	Elem* tmp = new Elem[new_size];
	for(unsigned int i=0; i<new_size; i++) tmp[i] = l->lista[i];
	delete l->lista;
	l=tmp;
}

void alloca(List& l){
	unsigned int new_size = l->maxsize + blockSize
	Elem* tmp = new Elem[new_size];
	for(unsigned int i=0; i<new_size; i++) tmp[i] = l->lista[i];
	delete l->lista;
	l=tmp;	
}

/****************************************************************/
void list::clear(List& l){
   delete l->lista;
   size = 0;
   maxsize = blockSize;
}


/****************************************************************/
void list::set(int pos, Elem e, List& l){ // "set" nel senso di "assegna un valore", non nel senso di "insieme"! E' una terminologia standard, non fate confusione con gli insiemi
   if(pos>=l->size) return;
   l->lista[pos] = e;
}


/****************************************************************/
void list::add(int pos, Elem e, List& l){ // aggiunge e nella posizione pos

}

/****************************************************************/
void list::addBack(Elem e, List& l){ // aggiunge e in coda
	if(l->size+1>= maxsize) alloca(l->lista);
	l->lista[l->size] = e;
	l->size++;
}


/****************************************************************/
void list::addFront(Elem e, List& l){ // aggiunge e in testa
   if(l->size+1>= maxsize) alloca(l->lista);
   for(unsigned int i=l->size; i>1; i-- ) l->lista[i] = l->lista[i-1];
   l->lista[0] = e;
}

/****************************************************************/
void list::removePos(int pos, List& l){
	if(n>=size) return;
	for(int i = n; i<size-1; i++) l->lista[n] == l->lista[n+1];
	l->size--;
	if(maxsize-size==blockSize) dealloca(l->lista);
}


List list::createEmpty(){
	List l;
	l->lista = emptyElem;
   	maxsize = 0;
   	size = 0;
   	return l;
}

/****************************************************************/
Elem list::get(int pos, const List& l){
	if(pos>=l->size) return emptyElem;
	return l->lista[pos];
}


/****************************************************************/
bool list::isEmpty(const List& l)
{
   return l->lista==emptyElem;
}

/****************************************************************/
int list::size(const List& l){
   return l->size;
}


/****************************************************************/
void printList(const List& l){
	for(int i=0; l->lista[i]; i++) cout<<l->lista[i]<<endl;
}




