# Présentation reprises du 7 et 8 avril 2022 à Jussieu

## Lancement local de godbolt

- `cd ~/programs/compiler-explorer && make EXTRA_ARGS='--language c++'`

## Présentation de ce que je pense dire (en français)

Questions à poser à Joël :
- slide 2 : se mettre d'accord avec Joël
- slide 4 : ça veut dire quoi "paramétrique" ?
- slide 4 : est-ce que code c'est la même chose que "programme" ?
- slide 10 : may or may not be of type T => on force une évaluation là ?
- slide 13 : ça veut dire quoi exactement "in resting position" ?
- slide 14 : le nom est à donner à tableau ou à n'importe quel objet ?
- regarder slide 15
- regarder slide 19

0. Présentation : 
    - bonjour, je m'appelle... sous la direction de Joël, D., H.
    - Ma présentation a pour sujet une bibliothèque nommée Kiwaku pour le calcul haute performance qui repose sur l'utilisation de la programmation générative.

```
- Kiwaku = calcul hp repose util prog générative
```

1. Un peu de contexte maintenant. Les tableau n-dimensionnels sont très courants dans le domaine du calcul haute performance. C'est par exemple la structure principale de nombreuses simulations numériques. Ces tableaux doivent être performants, faciles d'utilisation et expressifs. C'est un domaine d'étude compliqué où il n'existe pas de solution unique et où les besoins sont multiples.

```
- tab nD très courants ds dom chp
- struct principale simu
- doivent ê performants, faciles d'u & expr
- Dom d'étude compliqué pas sol unique où besoins multiples 
```

2. Voici quelques exemples de bibliothèques. **La question que nous nous sommes posée est de savoir s'il serait possible de contourner les limitations actuelles en traitant ce problème comme une question de design logiciel plutôt que simplement de fonctionnalités offertes. ???**

```

```

3. L'espace de conception est très vaste, il y a : la possession ou non de la mémoire, les informations connues au runtime, le storage order, l'index du départ des tableaux, les différents types d'allocations...
On pourrait alors penser à faire une seule grande implémentation monolithique, mais l'histoire a montré que ça mène à une grande masse de code impossible à maintenir proprement. Réduire de manière arbitraire l'API n'est pas non plus une solution.
Face à ça, on propose une stratégie de conception basée sur la programmation générative.

```

```

4. On a d'une part la description du programme dans le langage du domaine d'étude ici à gauche. Au milieu il y a les composants permettant la génération du code : la description du programme est lue par un traducteur qui construit l'implémentation du programme via des sous-composants **paramétriques ???**, créant ainsi un **code / programme ???** exécutable.

```

```

5. La programmation générative présente un certain nombre d'avantages. Les descripteurs et composants spécifiques au domaine sont individuellement simples à modifier et à étendre. Le fait que les composants soient indépendants les uns des autres diminue énormément le nombre d'implémentations nécessaires. La programmation générative en C++ est basée sur la métaprogrammation via les templates et la génération de code.

```

```

6. Je vais maintenant parler des solutions déjà existantes.

7. Globalement, les utilisateurs veulent des interfaces de taille raisonnables, non ambiguës et qu'il soit possible de comprendre un intuitivement, ainsi qu'une documentation claire et concise.
- Actuellement, certaines API souffrent d'une beaucoup trop grande surcharge fonctionnelle (ou ballonnement logiciel) qui rendent difficiles la compréhension de chaque fonction. Des API nécessitent parfois une bonne maîtrise du langage et de certains concepts de programmation. Et la plupart des API sont verbeuses et ne sont pas intuitives.
- Il serait intéressant d'offrir une API raisonnablement grande, facile d'utilisation et adaptée aux utilisateurs non experts en programmation.

```

```

8. Voici un exemple de l'allocation d'un vecteur en 5 dimensions via std::vector. On observe que c'est très verbeux et peu expressif. En plus, on est très mauvais niveau performances : la mémoire n'est pas contiguë, il y a besoin d'un très grand nombre d'allocations et le pattern d'accès aux données induit une très mauvaise utilisation des caches.

```

```

9. Le problème principal de std::mdspan est que l'API est très verbeuse, peu expressive et simplement franchement pas claire.

```

```

10. L'utilisation des expression template est dangereuse et réservée à des développeurs avertis, notamment l'utilisation du mot clef auto qui est très dangereuse. (explications du code en direct) :
- The type of C is not matrix => la lazy evaluation peut construire un arbre pour une évaluation future, lorsque le résultat sera demandé et les variables toutes connues.
- returns a reference to a temporary => m est déclaré dans le scope de la fonction et si m est stocké dans la valeur de retour (un arbre d'évaluation par exemple) ça pose problème. Globalement, là encore, c'est le auto qui est dangereux car m/t n'est pas forcément de type matrix.
- silent_performance_bug : that may or may not be of type T => on force une évaluation en obligeant le résultat that à être de type T, sans laisser place à l'évaluation paresseuse et aux simplifications futures.

```

```

11. Je vais parler de Kiwaku désormais, la bibliothèque sur laquelle on travaille.

12. Pour que ce projet ait un sens on lui assigne l'espace de conception suivant : ce n'est pas une solution monolithique, et Kiwaku ne gère que les tableaux numériques multidimensionnels.
TODO : à finir vu que la slide n'est elle-même pas finie.

```

```

13. - Les views Kiwaku ne possèdent jamais de mémoire, elles sont une interface similaire aux tableaux n-dimensionnels, permettant d'accéder à la mémoire vers laquelle elles ont une réference. Elles sont pensées pour prendre le moins d'espace mémoire possible. **in resting position ???**
- Les tables Kiwaku possèdent leur mémoire allouée via un allocateur spécifique ou directement sur la pile. Leur interface doit être semblable aux vues Kiwaku, et elles sont aussi pensées pour prendre le moins d'espace mémoire possible.
- Au final, une table kiwaku n'est qu'une vue possédant sa mémoire. Des vues performantes mènent à des tables performantes.

```

```

14. Les options supportées par Kiwaku sont perpendiculaires les unes aux autres, c'est à dire qu'elles sont chacunes spécifiques et qu'elle ne se recoupent pas. Il suffit de leur associer une valeur dans les constructeurs pour les définir, il n'est pas nécessaire de respecter un ordre de déclaration. Elles sont aussi utilisées pour optimiser les performances à la fois lors de la compilation et de l'exécution. je n'ai pas le temps de rentrer dans les détails, mais globalement il y a la source des données, la taille du tableau, le ou les index de départ dans ce tableau, un nom à éventuellement **donner au tableau ???** et éventuellement aussi un allocateur.

```

```

15. Là on a un exemple de code montrant la construction d'une vue à partir d'un tableau C classique. La taille du tableau est déduite automatiquement du tableau C passé en option via kwk::source. On a bien un tableau de taille 6 à une dimension. On voit également qu'on peut itérer sur la vue de la même manière que sur un vecteur classique (**à vérifier avec Joël ???**) et que la mémoire est bien gérée correctement. 

**TODO question ???** : pourquoi on a une shape de 3 et une dimension de 1 là ?
```c++
std::cout << "\n";

double data2[3][2] = {{1.2,2.3},  {3.4,4.5}, {5.6,6.7}};

auto vd2 = view{ source = data2 };
std::cout << vd2.shape() << "\n";
std::cout << vd2.rank() << "\n";
```

```

```

16. Je présente ici un exemple plus complet de création d'une vue 3D à partir d'un std:vector basique. On voit qu'on a bien les dimensions attendues pour la vue, et qu'on a un bel affichage bien formaté avec le nom qu'on a assigné à cette vue. On peut aussi observer que les valeurs sont bien dans l'ordre attendu.

```

```

17. Là c'est un petit exemple pour montrer que l'utilisation d'une vue Kiwaku ne coute pas plus cher que l'utilisation d'un tableau C classique, et que l'assembleur généré est exactement le même.

```

```

18. En conclusion, à l'heure actuelle on travaille sur les allocateur et les slicers. Enfin, des sujets d'étude intéressants dans le cadre de mon doctorat sont :
- le support des tuiles et les parcours non triviaux des **vues / de la mémoire ???**
- le support de la localité des données dans des scénarios de calcul distribué
- le support pour des plateformes de calcul hétérogènes telles que les CPU, GPU et FPGA
- l'intégration fluide avec SYCL.

```

```

19. Merci bisous !