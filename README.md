# Advent of Code C++ High Performance

Structure optimisée pour la performance utilisant `mmap` et des flags de compilation agressifs, supportant plusieurs années.

## Structure
- `src/<YEAR>/day<XX>/` : Code source pour chaque jour.
- `include/utils.hpp` : Fonctions utilitaires (mmap, parsing rapide, benchmark).
- `data/<YEAR>/` : Vos fichiers d'input (`dayXX.txt`) et de test (`testXX.txt`).
- `template/` : Le template utilisé pour les nouveaux jours.

## Utilisation

### 1. Créer un nouveau jour
```bash
./new_day.sh 2025 01
```
Ceci créera `src/2025/day01/main.cpp`, `data/2025/day01.txt` et `data/2025/test01.txt`.

### 2. Compiler et Exécuter
Le script `run.sh` s'occupe de tout (cmake, build, run) :
```bash
./run.sh 2025 01
```
Pour tester avec le fichier d'exemple :
```bash
./run.sh 2025 01 -test
```
Pour désactiver le benchmark (exécution unique) :
```bash
./run.sh 2025 01 -nobench
```
On peut combiner les deux : `./run.sh 2025 01 -test -nobench`.

## Fonctionnalités "High Performance"
- **Accès Fichier** : `aoc::MappedFile` utilise `mmap` pour un accès mémoire direct sans copie (Zero-copy).
- **Parsing** : `aoc::fast_atoi` et `aoc::skip_non_digits` pour éviter le surpoids de la STL.
- **Benchmark** : Chaque solution est lancée 1000 fois (après warmup) pour obtenir le temps minimal précis en microsecondes.
- **Optimisations** : Flags `-O3`, `-march=native` et `-flto` activés par défaut en mode Release.

## Astuces
- Pour tester avec l'exemple, décommentez la ligne correspondante au début de la fonction `solve()` dans votre fichier `main.cpp`.
- Utilisez `std::string_view` pour manipuler les segments de texte sans allocation.