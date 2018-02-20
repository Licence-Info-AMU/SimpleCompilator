#!/bin/bash
##############################################################################
#
# Script d'évaluation automatique du compilateur.
# Compile le programme source et compare la sortie avec la référence.
# Pour le code machine, compare la SORTIE de l'exécution
# Vérifie aussi que MIPS et INTEL donnent le même résultat (optionnel)
# Comprend 
#   * analyse lexicale, 
#   * analyse syntaxique, 
#   * arbre abstrait, 
#   * table des symboles et
#   * code machine (Intel).
#
# ATTENTION : pour que le script marche, votre compilateur doit returner 0 en
# cas de succès, autre valeur en cas d'erreur (programme ne compile pas 
# correctement) et doit permettre d'afficher les sorties sur stdout, sauf 
# d'éventuels messages d'erreur, sur stderr. On doit pouvoir, à tout moment,
# changer le type de sortie (lex, synt, asynt, tab, nasm) avec des options.
#
##############################################################################
###     LLL      IIIIIII    RRRRRRR     EEEEEEEEE     ||||
###     LLL        III      RRR  RRR    EEEEEEEEE     ||||
###     LLL        III      RRR    RR   EEE           ||||
###     LLL        III      RRR  RRR    EEEEEEEE      ||||
###     LLL        III      RRRRRRR     EEEEEEEE    \\\\//// 
###     LLL        III      RRR RRR     EEE          \\\/// 
###     LLLLLLLL   III      RRR   RRR   EEEEEEEE      \\// 
###     LLLLLLLL IIIIIII    RRR   RRR   EEEEEEEEE      \/  
##############################################################################

# 1) MODIFIEZ LA VARIABLE CI-DESSOUS AVEC LE CHEMIN VERS VOTRE COMPILATEUR

MYCOMPILO="../Compilateur/Compilateur"

# 2) DÉCOMMENTEZ + MODIFIEZ LES COMMANDES POUR GÉNÉRER LES DIFFÉRENTES SORTIES

EXITONFAIL=1                     # mettre à zéro pour continuer après erreurs
MYCOMPILODEFAULT="${MYCOMPILO}"  # utilisé pour test reconnaissance et erreur
MYCOMPILOLEX="${MYCOMPILO} -l"   # exécuter l'analyseur lexical
MYCOMPILOSYNT="${MYCOMPILO} -s"  # exécuter l'analyseur syntaxique
#MYCOMPILOASYNT="${MYCOMPILO} -a" # afficher l'arbre abstrait
#MYCOMPILOTAB="${MYCOMPILO} -t"   # afficher les tables des symboles
#MYCOMPILOMIPS="${MYCOMPILO} -m"  # générer code MIPS
#MYCOMPILONASM="${MYCOMPILO} -n"  # générer code Intel
#MARS="./Mars4_5.jar"             # utilisez autre version de Mars si besoin
#NASM="nasm"                      # utilisez autre version de nasm si besoin
#NASMOPTS="-f elf -g -F dwarf"
#LD="ld"                          # utilisez autre version de ld si besoin
#LDOPTS="-m elf_i386"
##############################################################################
# NE PLUS LIRE À PARTIR D'ICI ;-)
##############################################################################

declare -A testname
testname["lex"]="Analyse lexicale"
testname["synt"]="Analyse syntaxique"
testname["asynt"]="Arbre abstrait"
testname["tab"]="Table des symboles"
testname["mips"]="Code machine MIPS"
testname["nasm"]="Code machine Intel"

##############################################################################
# Fonction pour faire le diff textuel régulier (lex et tab).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier 1
# @param $2 Fichier 2
# @return 0 si pas de différence, autre sinon

function REGDIFF() {
  diff -q -w $1 $2
}

##############################################################################
# Fonction pour faire le diff entre deux fichiers XML (syn et asyn).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier XML 1
# @param $2 Fichier XML 2
# @return 0 si pas de différence, autre sinon

function XMLDIFF() {
  ./compare_arbres_xml $1 $2
}

##############################################################################
# Fonction qui exécute un programme mips passé en paramètre
# La sortie est affichée sur stdout
# @param $1 Fichier mips à exécuter
# @param $2 Entrée à donner à l'exécution du programme (optionnel)

function mips_exec(){
  mipsfile="$1"
  input="$2"
  echo -e "${input}" | 
  java -jar $MARS "${mipsfile}" | 
  tail -n +3 | head -n -1 | # Output has author and version info, remove
  sed ':a;N;$!ba;s/[ \n]/_/g' # Homogenize whitespace
}

##############################################################################
# Fonction pour faire le diff entre deux sorties d'exécution Mars (mips).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier mips 1
# @param $2 Fichier mips 2
# @param $3 Entrée à donner à l'exécution du programme (optionnel)
# @return 0 si pas de différence, autre sinon

function MARSOUTDIFF() {
  diff <(mips_exec "${1}" "${3}") <(mips_exec "${2}" "${3}")
}

##############################################################################
# Fonction qui exécute un programme nasm passé en paramètre
# La sortie est affichée sur stdout
# @param $1 Fichier nasm à exécuter
# @param $2 Entrée à donner à l'exécution du programme (optionnel)

function nasm_exec(){
  nasmfile="$1"
  input="$2"
  ${NASM} ${NASMOPTS} "${nasmfile}" -o "${nasmfile}.o"
  ${LD} ${LDOPTS} -o "${nasmfile}.exe" "${nasmfile}.o"
  echo -e "${input}" | 
  ./"${nasmfile}".exe |
  sed ':a;N;$!ba;s/[ \n]/_/g' # Homogenize whitespace
  # Comment the line below out if you want to keep .o and executable
  rm "${nasmfile}.o" "${nasmfile}.exe" 
}

##############################################################################
# Fonction pour faire le diff entre deux sorties d'exécution nasm (Intel).
# Cette fonction peut être passée en paramètre à la fonction diff_prog.
# @param $1 Fichier nasm 1
# @param $2 Fichier nasm 2
# @param $3 Entrée à donner à l'exécution du programme (optionnel)
# @return 0 si pas de différence, autre sinon

function NASMOUTDIFF() {
  diff <(nasm_exec "$1" "$3") <(nasm_exec "$2" "$3")
}

##############################################################################
# Fonction pour comparer les références mips et nasm et s'assurer que les
# références sont identiques. Cette fonction est interne aux développeurs
# du compilateur de référence (enseignants) et n'est pas à utiliser par les
# étudiants.
# @param $1 Fichier mips
# @param $2 Fichier nasm
# @param $3 Entrée à donner à l'exécution du programme (optionnel)
# @return 0 si pas de différence, autre sinon

function compare_mips_nasm(){
  diff <(mips_exec "$1" "$3") <(nasm_exec "$2" "$3")
}

##############################################################################
# Fonction pour faire le diff entre deux fichiers, vérifier et afficher le 
# résultat avec de belles couleurs.
# @param $1 Nom de la fonction utilisée pour faire le diff (parmi 3 ci-dessus)
# @param $2 Nom du fichier d'entrée à tester sans extension (p. ex. affect)
# @param $3 Extension du fichier à tester (p. ex. synt)
# @param $4 Entrée à donner à l'exécution du programme (optionnel)

function diff_prog() {
  diffprog=$1
  input=$2
  suffix=$3
  values=$4
  refoutput="ref-${suffix}/${input}.${suffix}"
  sysoutput="output/${input}.${suffix}"
  echo -e "\033[35m > ${testname[${suffix}]} (.${suffix})\033[0m"
  echo -e "`wc -l output/${input}.${suffix} | awk '{print $1}'` lignes"
  if [ -f ref-${suffix}/$input.${suffix} ]; then
    ${diffprog} output/${input}.${suffix} ref-${suffix}/${input}.${suffix} ${values} 2>&1 2> /dev/null # TODO: UNCOMMENT
    if [ ! $? -eq 0 ]; then 
      echo -e "\033[31mTEST ${testname[${suffix}]} ÉCHOUÉ\033[0m"
      echo -e "Différences entre output/${input}.${suffix} ref-${suffix}/${input}.${suffix} en utilisant $diffprog:"
      ${diffprog} output/${input}.${suffix} ref-${suffix}/${input}.${suffix} ${values}
      if [ $EXITONFAIL -eq 1 ]; then exit 1; fi
    else
      echo -e "\033[32mTEST ${testname[${suffix}]} OK\033[0m"
    fi
  else
    echo -e "\033[34mRéférence ref-${suffix}/${input}.${suffix} absente\033[0m"
  fi
}

##############################################################################
# Exécute tous les tests (reco, lex, synt, asynt, tab, mips) pour un exemple
# donné qui doit être correctement compilé à tous les niveaux.
# @param $1 Nom du fichier d'entrée à tester sans extension (p. ex. affect)
# @param $2 Entrée à donner à l'exécution du programme (optionnel)

function test_fichier_ok() {
    input=$1
    echo -e "\n\033[4m ---- Test input/$input.l ----\033[0m"
    if [ -n "$2" ]; then
      echo "Input : $2"
    fi
    if [ -f input/$input.l ]; then
      # Reconnaissance : programme correct doit être accepté
      if [ -n "${MYCOMPILODEFAULT}" ]; then
        echo -e "\033[35m > Reconnaissance (accepte l'entrée)\033[0m"        
        ${MYCOMPILODEFAULT} input/$input.l > output/$input.synt
        if [ ! $? -eq 0 ]; then 
          echo -e "\033[31mTEST Reconnaissance ÉCHOUÉ\033[0m"
          echo -e "Le programme $input.l n'a pas été compilé correctement"	
          if [ $EXITONFAIL -eq 1 ]; then exit 1; fi
        else
          echo -e "\033[32mTEST Reconnaissance OK\033[0m"
        fi
      fi     
      # Teste analyseur lexical
      if [ -n "${MYCOMPILOLEX}" ]; then
        ${MYCOMPILOLEX} input/$input.l > output/$input.lex
        diff_prog REGDIFF $input lex
      fi 
      # Teste analyseur syntaxique
      if [ -n "${MYCOMPILOSYNT}" ]; then
        ${MYCOMPILOSYNT} input/$input.l > output/$input.synt          
        diff_prog XMLDIFF $input synt
      fi
      # Teste création de l'arbre abstrait
      if [ -n "${MYCOMPILOASYNT}" ]; then
        ${MYCOMPILOASYNT} input/$input.l > output/$input.asynt
        diff_prog XMLDIFF $input asynt
      fi
      # Teste replissage de la table des symboles
      if [ -n "${MYCOMPILOTAB}" ]; then
        ${MYCOMPILOTAB} input/$input.l > output/$input.tab
        diff_prog REGDIFF $input tab
      fi
      # Teste génération de code MIPS
      if [ -n "${MYCOMPILOMIPS}" ]; then
        ${MYCOMPILOMIPS} input/$input.l > output/$input.mips
        diff_prog MARSOUTDIFF $input mips "$2"
      fi
      # Teste génération de code Intel
      if [ -n "${MYCOMPILONASM}" ]; then
        ${MYCOMPILONASM} input/$input.l > output/$input.nasm
        diff_prog NASMOUTDIFF $input nasm "$2"
      fi
      # Sanity check: MIPS et Intel génèrent la même sortie
      if [ -n "${MYCOMPILOMIPS}" -a -n "${MYCOMPILONASM}" ]; then
        compare_mips_nasm ref-mips/$input.mips ref-nasm/$input.nasm "$2"
	if [ ! $? -eq 0 ]; then 
           echo -e "\033[34mATTENTION: MIPS et Intel différents\033[0m"
           exit -1
	fi
      fi
    else
      echo -e "\033[31minput/$input.l non trouvé\033[0m"
      echo -e "\033[31mTest impossible\033[0m"     
    fi
}

##############################################################################
# Vérifie qu'un programme contenant des erreurs n'est pas reconnu/compilé.
# @param $1 Nom du fichier d'entrée à tester sans extension (p. ex. affect)

function test_fichier_fail() {
	input=$1
    echo -e "\n\033[4m ---- Test input/$input.l ----\033[0m"
    ${MYCOMPILODEFAULT} input/$input.l > /dev/null
    if [ $? -eq 0 ]; then 
    echo -e "\033[31mTEST REJET ÉCHOUÉ\033[0m"
      echo -e "Le programme $input.l est accepté mais il devrait être rejeté"
      if [ $EXITONFAIL -eq 1 ]; then exit 1; fi
    else
      echo -e "\033[32mTEST REJET OK\033[0m"
    fi
}

##############################################################################
#######  #####  ######   ######    ###  #####  ###############################
#######   ###   #####  #  ######  ####    ###  ###############################
#######    #    ####  ###  #####  ####  ##  #  ###############################
#######  #   #  ###         ####  ####  ###    ###############################
#######  ## ##  ##  #######  ##    ###  ####   ###############################
##############################################################################

make clean
make
mkdir -p output

################################################################################
# Vérifications initiales : MYCOMPILO et MARS bien configurés?

echo -e "Votre compilateur : ${MYCOMPILO}"
if [ ! -f  ${MYCOMPILO} ]; then
  echo -e "\033[31mCompilateur introuvable"
  echo -e "Modifiez la variable MYCOMPILO avant de lancer l'éval\033[0m"
  exit 1
fi
if [ -n "${MYCOMPILOMIPS}" ]; then
  [ -n "${MARS}" ] && java -jar ${MARS} -v > /dev/null
  if [ ! $? -eq 0 ]; then
    echo -e "\033[31mMARS introuvable"
    echo -e "Modifiez la variable MARS avant de lancer l'éval\033[0m"
    exit 1
  fi
fi
if [ -n "${MYCOMPILONASM}" ]; then
  [ -n "${NASM}" ] && ${NASM} -v > /dev/null
  if [ ! $? -eq 0 ]; then
    echo -e "\033[31mNASM introuvable"
    echo -e "Modifiez la variable NASM avant de lancer l'éval\033[0m"
    exit 1
  fi
  if [ ! -f "io.asm" ]; then
    echo -e "\033[31mFichier io.asm introuvable"
    echo -e "Copiez-le dans le répertoire courant\033[0m"
    exit 1
  fi
fi

################################################################################

echo -e "\033[1m\n>> 1.1) Tests connus OK\033[0m"

test_fichier_ok affect
test_fichier_ok boucle
test_fichier_ok expression
test_fichier_ok max
test_fichier_ok tri

################################################################################

echo -e "\033[1m\n>> 1.2) Tests connus FAIL\033[0m"

test_fichier_fail affect-err

################################################################################

echo -e "\033[1m\n>> 2.1) Tests nouveaux OK\033[0m"

test_fichier_ok appel
test_fichier_ok assoc-prec
test_fichier_ok procedure_arg
test_fichier_ok procedure
test_fichier_ok procedure_retour
test_fichier_ok procedure_varloc
test_fichier_ok si
test_fichier_ok sinon
test_fichier_ok tableau
test_fichier_ok tantque
test_fichier_ok lexunits
test_fichier_ok factorielle
test_fichier_ok fibo
test_fichier_ok pgcd

################################################################################

echo -e "\033[1m\n>> 2.2) Tests nouveaux FAIL\033[0m"

test_fichier_fail lex-err     # fails at lex
test_fichier_fail synt-err    # fails at synt
test_fichier_fail extra       # fails at synt
test_fichier_fail 33a         # fails at lex
test_fichier_fail ordre       # fails at synt

################################################################################

echo -e "\033[1m\n>> 3.1) Tests nouvelle fonctionnalité OK\033[0m"

# 3 fichiers OK nouvelle fonctionnalité
test_fichier_ok nouveau1
test_fichier_ok nouveau2
test_fichier_ok nouveau3

################################################################################
echo -e "\033[1m\n>> 3.2) Tests nouvelle fonctionnalité FAIL\033[0m"

# 1 fichier fail nouvelle fonctionnalité
test_fichier_fail nouveau-erreur

