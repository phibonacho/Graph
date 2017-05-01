#include "tree.h"

using namespace tree;

/* Nota: nei commenti usiamo "albero" e "puntatore a un nodo" come sinonimi, scegliendo di volta in volta
la formulazione più adatta a dare la specifica di una funzione. 
Dal punto di vista tecnico, usiamo sempre il tipo Tree sia quando operiamo su un albero, sia quando operiamo, 
a livello concettuale, su un puntatore a un nodo dell'albero. 
Questo approccio e' corretto in quanto un albero (o sottoalbero) è un puntatore a una struct di tipo treeNode:
un albero è identificato univocamente dal puntatore alla sua radice */ 

/*******************************************************************************************************/
// il nodo di un albero è caratterizzato dall'etichetta, il puntatore al primo figlio e il puntatore al prossimo fratello
// NON manteniamo il puntatore al padre, anche se sarebbe comodo per alcune funzioni (ma del tutto inutile per altre)
struct tree::treeNode {
    Label label;    
    treeNode *firstChild;
    treeNode *nextSibling;
};

Tree create_node(const Label& to_add){
  Tree tmp = new treeNode;
  tmp->label = to_add;
  tmp->nextSibling = emptyTree;
  tmp->firstChild = emptyTree;
  return tmp;
}

Tree getnode(const Label& labelOfNodeInTree, const Tree& t){
  if(t==emptyTree||t->label==labelOfNodeInTree) return t;
  Tree child = getnode(labelOfNodeInTree, t->firstChild);
  Tree sibl = getnode(labelOfNodeInTree, t->nextSibling);
  if(sibl==emptyTree && child==emptyTree) return emptyTree;
  if(sibl!=emptyTree) return sibl;
  return child;
}

Label pointed_by(const Label l, const Tree& t){
  //caso base:
  if(t==emptyTree) return emptyLabel;
  if(t->nextSibling==emptyTree&&t->firstChild==emptyTree) return emptyLabel;
  if(t->nextSibling!=emptyTree) if(t->nextSibling->label==l) return t->label;
  if(t->firstChild!=emptyTree) if(t->firstChild->label==l) return t->label;
  Label sibl = father(l, t->nextSibling);
  Label child = father(l, t->firstChild);
  if(sibl==emptyLabel&&child==emptyLabel) return emptyLabel;
  if(sibl!=emptyLabel) return sibl;
  return child;
}

/*******************************************************************************************************/
// isEmpty restituisce true se l'albero t e' vuoto, false altrimenti
bool tree::isEmpty(const Tree& t)
{ 
   return t==emptyTree;
}

/*******************************************************************************************************/
// addElem aggiunge il nodo etichettato con labelOfNodeToAdd come
// figlio del nodo etichettato con labelOfNodeInTree.
// Caso particolare: aggiunta della radice, che si ottiene
// con labelOfNodeInTree uguale a emptyLabel (= nessun padre), e ha successo
// solo se l'albero e' vuoto (in caso contrario restituisce FAIL 
// e non aggiunge nulla).
// Restituisce FAIL se non esiste un nodo nell'albero etichettato 
// con labelOfNodeInTree oppure se nell'albero esiste gia' un nodo 
// etichettato con labelOfNodeToAdd; aggiunge e restituisce OK altrimenti
Error tree::addElem(const Label labelOfNodeInTree, const Label labelOfNodeToAdd, Tree& t)
{
  if(getnode(labelOfNodeToAdd, t)!=emptyTree) return FAIL;
  Tree aux, tmp;
  //caso particolare:
  if(labelOfNodeInTree==emptyLabel){
    if(!isEmpty(t)) return FAIL;
    t = create_node(labelOfNodeToAdd);
    return OK;
  }
  aux = getnode(labelOfNodeInTree, t);
  if(aux==emptyTree) return FAIL;
  //se non ha figli aggiungo
  if(aux->firstChild==emptyTree) aux->firstChild = create_node(labelOfNodeToAdd);
  //se ha figli scorro fino la lista dei fratelli del primo figlio ed aggiungo;
  else{
    tmp = create_node(labelOfNodeToAdd);
    tmp->nextSibling = aux->firstChild;
    aux->firstChild = tmp;
  }
  return OK;
}     



/*******************************************************************************************************/
// deleteElem (versione iterativa) rimuove dall'albero il nodo etichettato con la Label l
// e collega al padre di tale nodo tutti i suoi figli
// Restituisce FAIL se si tenta di cancellare la radice e questa ha 
// dei figli (non si saprebbe a che padre attaccarli) oppure se non esiste 
// un nodo nell'albero etichettato con la Label; cancella e restituisce OK altrimenti
Error tree::deleteElemI(const Label l, Tree& t){
  if(getnode(father(l, t), t)==emptyTree && degree(father(l, t), t)>0) return FAIL;
  //ottengo puntatore al nodo da eliminare:
  Tree to_delete = getnode(l, t);
  //ottengono puntatore al nodo da cui viene puntato il nodo da eliminare:
  Tree previous_node = getnode(pointed_by(l, t), t);
  Tree tmp = to_delete;
  //scorro la lista dei fratelli del nodo da eliminare:
  while(tmp->nextSibling!=emptyTree) tmp = tmp->nextSibling;
  //appendo i figli del nodo da eliminare in fondo alla lista dei suoi fratelli:
  tmp->nextSibling = to_delete->firstChild;
  cerr<<"updating pointer\n";
  if(previous_node->firstChild!=emptyTree){
    if(previous_node->firstChild->label==l) previous_node->firstChild = to_delete->nextSibling;
  }
  else previous_node->firstChild = to_delete->nextSibling;
  delete to_delete;
  return OK;
}



/*******************************************************************************************************/
// deleteElem (versione ricorsiva): stesso comportamento della versione iterativa, ma implementata ricorsivamente
// (può anche non essere ricorsiva la deleteElemR, ma deve fare uso di funzioni ausiliarie ricorsive)
Error deleteElemR_aux(const Label& l, Tree& t){
  //se la cella a cui sono arrivato è vuota, allora termino;
  if(t==emptyTree) return FAIL;
  Tree tmp = emptyTree, aux;
  bool is_child = false;
  //controllo se nelle celle figlie c'è qualcosa:
  if(t->firstChild==emptyTree&&t->nextSibling==emptyTree) return FAIL;
  if(t->firstChild!=emptyTree) if(t->firstChild->label==l){
    tmp = t->firstChild;
    is_child = true;
  }
  if(t->nextSibling!=emptyTree) if(t->nextSibling->label==l) tmp = t->nextSibling;
  if(tmp!=emptyTree){
    //se ho trovato la cella giusta:
    aux = tmp;
    //scorro la lista dei fratelli del nodo da eliminare:
    while(aux->nextSibling!=emptyTree) aux=aux->nextSibling;
    //appendo il primo figlio del nodo da eliminare in coda ai suoi fratelli:
    aux->nextSibling = tmp->firstChild;
    //aggiorno puntatori:
    if(is_child) t->firstChild = tmp->nextSibling;
    else t->nextSibling = tmp->nextSibling;
    //elimino il nodo
    delete tmp;
    return OK;
  }
  Error sibl = deleteElemR_aux(l, t->nextSibling);
  Error child = deleteElemR_aux(l, t->firstChild);
  if(sibl != FAIL) return sibl;
  if(child != FAIL) return child;
  return FAIL;
}

Error tree::deleteElemR(const Label l, Tree& t){
  if(father(l, t)==emptyLabel&&degree(l, t)>0) return FAIL;
  return deleteElemR_aux(l, t);
}

 

/*******************************************************************************************************/
// father restituisce l'etichetta del padre del nodo con etichetta l se il nodo esiste nell'albero 
// (o sottoalbero) t e se il padre esiste. 
// Restituisce la costante emptyLabel altrimenti

Label tree::father(const Label l, const Tree& t){
  if(t==emptyTree) return emptyLabel;
  //if(t->firstChild==emptyTree) return emptyLabel;
  for(Tree tmp=t->firstChild; tmp!=emptyTree; tmp=tmp->nextSibling) if(tmp->label==l) return t->label;
  Label sibl = father(l, t->nextSibling);
  Label child = father(l, t->firstChild);
  if(sibl==emptyLabel&&child==emptyLabel) return emptyLabel;
  if(sibl!=emptyLabel) return sibl;
  return child;
}

/*******************************************************************************************************/
// children restituisce una lista di Label, contenente le etichette 
// di tutti i figli nell'albero t del nodo etichettato con l
// La lista può essere implementata usando una qualunque delle strutture dati viste a lezione
// (non un Vector!!!): si riusi una implementazione del TDD list, adattandola 
// a elementi di tipo string (nelle precedenti esercitazioni gli elementi erano int)
void children_aux(const Tree& t, list::List& lst){
  if(t==emptyTree) return;
  list::addFront(lst, t->label);
  //children_aux(t->firstChild, lst);
  children_aux(t->nextSibling, lst);
}

list::List tree::children(const Label l, const Tree& t)
{
  list::List lst = list::createEmpty();
  Tree aux = getnode(l, t);
  aux = aux->firstChild;
  children_aux(aux, lst);  
  return lst;
}



/*******************************************************************************************************/
// degree restituisce il numero di archi uscenti dal nodo etichettato con l; restituisce -1 se non esiste 
// nessuna etichetta l nell'albero
int tree::degree(const Label l, const Tree& t)
{
  Tree tmp = getnode(l ,t);
  if(tmp==emptyTree) return -1;
  int deg = 0;
  if(tmp->nextSibling!=emptyTree) deg++;
  if(tmp->firstChild!=emptyTree) deg++;
  return deg;
}



/*******************************************************************************************************/
// ancestors (versione ricorsiva) restituisce una lista di Label, contenente le etichette 
// di tutti gli antenati del nodo l ESCLUSA l'etichetta del nodo stesso
// La lista può essere implementata usando una delle strutture dati viste a lezione
// (non un Vector)
// (può anche non essere ricorsiva la ancestorsR, ma deve fare uso di funzioni ausiliarie ricorsive)
void ancestorsR_aux(const Label& l, const Tree& t, list::List& lst){
  if(l==emptyLabel) return;
  list::addFront(lst, l);
  return ancestorsR_aux(father(l, t), t, lst);
}

list::List tree::ancestorsR(const Label l, const Tree& t)
{
  list::List lst = list::createEmpty();
  if(getnode(l, t)==emptyTree)return lst;
  ancestorsR_aux(father(l, t), t, lst);
  return lst;
}



/*******************************************************************************************************/
// ancestors (versione iterativa) restituisce una lista di Label, contenente le etichette 
// di tutti gli antenati del nodo l ESCLUSA l'etichetta del nodo stesso
// La lista può essere implementata usando una delle strutture dati viste a lezione
// (non un Vector)
list::List tree::ancestorsI(const Label l, const Tree& t)
{
  list::List lst = list::createEmpty();
  if(getnode(l, t)==emptyTree) return lst;
  Label tmp = father(l, t);  
  do{
    list::addFront(lst, tmp);
    tmp = father(tmp, t);
  }while(tmp!=emptyLabel);
  return lst;
}



/*******************************************************************************************************/
// leastCommonAncestor restituisce l'etichetta del minimo antenato comune 
// ai nodi etichettati con label1 e label2
Label tree::leastCommonAncestor(const Label label1, const Label label2, const Tree& t)
{
  list::List lst1 = ancestorsR(label1, t);
  list::List lst2 = ancestorsR(label2, t);
  for(int i=size(lst1)-1; i>0; i--)
    for(int j=size(lst2)-1; j>0; j--)
      if(get(i, lst1)==get(j, lst2)) return get(i, lst1);
  return emptyLabel;
}



/*******************************************************************************************************/
// member restituisce true se il nodo etichettato con la label l appartiene all'albero t e false altrimenti
bool tree::member(const Label l, const Tree& t)
{ 
   return getnode(l, t)!=emptyTree;  
}



/*******************************************************************************************************/
// numNodes restituisce il numero di nodi nell'albero t mediante una visita ricorsiva in depthfirst
int tree::numNodes(const Tree& t){
  int n_nodi = 0;
  //se sono in fondo a un ramo, restituisco zero:
  if (t == emptyTree) return 0;
  n_nodi++;
  //richiamo ricorsivamente sul ramo sinistro, questa chiamata termina quando arrivo in fondo al ramo, quindi il primo elem stampato è il più piccolo del ramo:
  n_nodi += numNodes(t->firstChild);
  //richiamo ricorsivamente sul ramo destro, in questo caso l'elemento stampato è quello corrente, a meno che il nodo non abbia rami sinistri, in quel caso si legga sopra:
  n_nodi += numNodes(t->nextSibling);
  return n_nodi;
}



/*******************************************************************************************************/
// createEmptyTree restituisce l'albero vuoto
Tree tree::createEmpty()
{
   return emptyTree;
}
//`
void print_aux(const Tree& son, string aux, int level){
  if(son==emptyTree) return;
  //aggiungo livello
  aux+=" |";
  //stampo elemento
  cout<<aux<<(son->nextSibling!=emptyTree?"-":(level==10)?"\b":"\b `")<<"\e[38;5;"<<level<<"m"<<son->label<<"\e[0m\n";
  if(son->nextSibling==emptyTree) if(aux[aux.size()-1]=='|') aux[aux.size()-1]=' ';
  print_aux(son->firstChild, aux, level+10);
  aux.erase(aux.end()-2, aux.end());
  print_aux(son->nextSibling, aux, level);
}
/*******************************************************************************************************/
// Visualizzazione possibilmente strutturata come visto a lezione
void printTree(const Tree& t) {
    if(t==emptyTree) return;
    print_aux(t, "", 10);
    return;
}