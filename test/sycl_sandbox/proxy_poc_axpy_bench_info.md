# Explications du contexte SYCL de Kiwaku, écrit le 2023-08-17 par Joël

On a pris pour exemple l'algo axpy. On a appliqué la bonne pratique consistant à faire du bottom-up, c'est à dire à commencer par l'implémentation de l'algo en l'ayant rendu fonctionnel, puis abstraire les méthodes et rendre le code générique petit à petit.

On a choisi de partir sur les buffers/accesseurs de SYCL en pensant que ce serait plus optimisé et plus ergonomique qu'USM, donnant à SYCL la possibilité d'optimiser les choses sans réinventer l'eau tiède en utilisant USM device (USM shared étant moins performant, selon notre papier ACAT).


## Description de l'algo SYCL de base

L'algorithme axpy en SYCL se décompose en plusieurs parties. (cf fichier `buffers/buffers-axpy.cpp`)

- 1) Création de la queue SYCL, choix du device, handler d'exception
- 2) Allocation de la mémoire hôte (un tableau en output, deux en input)
- 3) Création des buffers à partir des pointeurs hôte (un output, deux input)
- 4) Submit à la queue SYCL:
  - 4.1) Création des accesseurs sur les buffers (out, in1, in2)
  - 4.2) Kernel (parallel for) et wait sur la queue SYCL
- 5) Récupération des données sur l'hôte via la destruction du buffer out, ou buffer.get_access(...).
- 6) Vérification des résultats via calcul sur CPU
- 7) Libération de la mémoire hôte


## Objectif Kiwaku

D'une manière générique (peu importe l'algo), on veut pouvoir :
- A) Créer des tables Kiwaku (out, in1, in2) et initialiser les input
- B) Appeler une fonction transform sur les tableaux kiwaku choisissant le contexte d'exécution qu'on veut :
  - `transform(contexte_execution, lambda, tab_out, tabs_in...)` avec 0, 1 ou n input(s).


## Description de notre choix de design et lien avec notre implémentation SYCL

On veut que notre fonction transform puisse prendre n'importe quel contexte d'exécution (CPU, SYCL, CUDA, OpenMP, MPI, ...). Tout contexte d'exécution devra hériter de `kwk::base_context` et réimplémenter ses méthodes (je vais y revenir plus loin). A un moment, on fera des contextes imbriqués, mais pour l'instant on ne gère pas le cas où on voudrait faire du SYCL sur les noeuds MPI.


## Contextes d'exécution

Tout contexte d'exécution doit offrir les méthodes suivantes :
- `static auto const&  in   (kwk::concepts::container auto const& c)` qui retourne quelque chose qui permet l'accès en lecture seule (input) aux données du conteneur kiwaku `c`
- `static auto&        out  (kwk::concepts::container auto& c)` idem mais en écriture seule, donc sans transfert initial des données vers le device, et avec récupération des données depuis le device (automatiquement) lors de la destruction du buffer dans le cas de SYCL
- `static auto&        inout(kwk::concepts::container auto& c)` idem, avec donc envoi des données vers le device avant le kernel et récupération des données vers l'hôte lors de la destruction des buffers (dans le cas de SYCL).

(à vérif) Ces méthodes sont utilisées à l'intérieur du contexte en lui-même, elles n'ont pas nécessairement de sens hors du contexte. Dans le cas du `kwk::sycl::context`, ces méthodes retournent un objet proxy déclaré dans le `kwk::sycl::context` permettant la création d'un accesseur (via `kwk::sycl::in_proxy{un conteneur kiwaku}.access(::sycl::handler& h)`) relatif à la queue du `kwk::sycl::context`, ces accesseurs n'ont aucun sens hors de cette queue, ie hors de cette instance de contexte.

On a créé un contexte d'exécution de base : `kwk::base_context` dont doivent hériter publiquement tous les autres contextes d'exécution. Ce contexte de base doit offrir des fonctions de base, comme le `for_each`, `in`, `out`, `inout`... Lorsqu'on implémente un contexte d'exécution Kiwaku, on doit le faire hériter publiquement de `kwk::base_context` et en redéfinir les méthodes lorsque c'est pertinent. Les méthodes de `kwk::base_context` ne sont pas virtuelles, elles seront écrasées par les méthodes des autres types enfants, sans appel virtuel (programmation à base de templates sans appels virtuels, donc en conservant les performances). Exemple d'appel de méthode avec une utilisation maligne de templates :

```c++
template<typename C>
struct base1
{
  static auto machin() { return 3; }
  static auto un_truc() { return C::machin(); }; // permet d'éviter l'appel virtuel
};

struct deriv1 : public base1<deriv1>
{
  static auto machin() { return 4; };
};


struct base2
{
  static auto machin() { return 3; }
  static auto un_truc() { return machin(); };
};

struct deriv2 : public base2
{
  static auto machin() { return 4; };
};

int main()
{
  auto n1 = deriv1::un_truc(); // retourne 3
  auto n2 = deriv2::un_truc(); // retourne 4

  std::cout << "n1 = " << n1 << std::endl;
  std::cout << "n2 = " << n2 << std::endl;
}
```


On procède de la même manière avec nos contextes d'exécution, on a `template<typename Context>  struct base_context` et les définitions des enfants par `struct kwk::quelquechose::context : public base_context<kwk::cpu::context>`. Ça permet d'écrire une fois pour toutes des algos de base, sans avoir à les redéfinir dans les classes filles lorsque leur implémentation ne change pas, et que seules changent quelques fonctions. Exemple de la fonction `transform` définie dans `base_context` uniquement mais appelable avec `kwk::cpu::context` et `kwk::sycl::context`, avec le `for_each` redéfini à chaque fois.

*Redéfinition des fonctions de base_context lorsqu'il y a de meilleures fonctions, plus spécifiques, voir la photo que j'avais prise du tableau (substitution à la compilation des paramètres template, écrasementr des fonctions de base_context lorsqu'il y en a une mieux, et appel aux fonctions de base_context sinon).*


## Implémentation SYCL

Le `kwk::sycl::context` hérite d'une manière privée de `::sycl::queue` et en hérite les constructeurs via `using parent = ::sycl::queue; using parent::parent;`. Il hérite également publiquement de `kwk::base_context<kwk::sycl::context>`, illustration de l'astuce décrite précédemment.

Il est nécessaire de redéfinir les fonctions `in`, `out` et `inout` de `base_context` pour notre contexte sycl. On veut qu'un appel à `kwk::sycl::context::in/out/inout` retourne quelque chose qui nous permette d'accéder aux données SYCL. Un `sycl::buffer` convient (presque) parfaitement, créé à partir d'un conteneur Kiwaku (pointeur + taille) : il gère les données et les transferts aux bons moments, et sera détruit et les données remontées sur l'hôte (cas `out` ou `inout`) lors de la fin de vie de sa variable, donc à la fin de l'algo.

Lors du kernel sycl, les accesseurs doivent être créés à partir des buffers, avec les bons paramètres (in/out/inout...). On a donc décidé de créer des objets proxy s'occupant de la création du buffer et des accesseurs. On a trois types de proxy : `in_proxy`, `out_proxy` et `inout_proxy`. Le buffer a la même durée de vie que l'objet proxy (lui-même créé lors de l'appel à context::in/out/inout), les données seront donc bien gérées par sycl via le cycle de vie des buffers. Un objet proxy est une surcouche aux buffers : il contient une instance de buffer initialisée lors de la création de l'objet proxy, et offre la méthode `.access` retournant un accesseur vers le buffer, avec les bons attributs (lecture, écriture ou les deux). Dans la dernière version du `kwk::sycl::proxy::in/out/inout`, j'ai créé une instance mère `base` qui contient le code commun à tous les proxy sycl, et les classes filles spécialisées pour l'utilisation des accesseurs.

La méthode transform peut désormais appartenir à la struct `kwk::base_context` étant donné qu'elle a en paramètre template la vraie struct qui gère les méthodes (in/out/inout etc.) selon ce que j'ai décrit au paragraphe `Contextes d'exécution`.


## Implémentation CPU

L'implémentation CPU en découle naturellement, avec seulement la fonction `for_each` à redéfinir (à voir s'il ne serait pas mieux de directement avoir la fonction for_each dans `base_context`).
