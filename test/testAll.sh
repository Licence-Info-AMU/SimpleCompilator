#!/bin/bash

################################################################################
# Script d'évaluation automatique des compilateurs du projet.
# Compile le programme source et compare avec la référence.
# Comprend analyse lexicale, syntaxique, arbre abstrait, table des symboles et
# code MIPS.
################################################################################

# MODIFIEZ LES VARIABLES CI-DESSOUS AVEC LE CHEMIN/OPTIONS DE VOTRE COMPILATEUR

MYCOMPILO="../src/compilo"
MYCOMPILOLEX="${MYCOMPILO} -l"
MYCOMPILOSYNT="${MYCOMPILO} -s"
MYCOMPILOASYNT="${MYCOMPILO} -a"
MYCOMPILOTAB="${MYCOMPILO} -t"
MYCOMPILOMIPS="${MYCOMPILO} -m"

################################################################################

XMLDIFF="./compare_arbres_xml"
REGDIFF="diff -q -w"
EXITONFAIL=1
declare -A testname
testname["lex"]="Analyse lexicale"
testname["synt"]="Analyse syntaxique"
testname["asynt"]="Arbre abstrait"
testname["tab"]="Table des symboles"
testname["mips"]="Code machine MIPS"

################################################################################

function diff_prog() {
    diffprog=$1
    input=$2
    suffix=$3
    echo -e "\033[35m > ${testname[${suffix}]} (.${suffix})\033[0m"
    if [ -f ref-${suffix}/$input.${suffix} ]; then
        ${diffprog} output/${input}.${suffix} ref-${suffix}/${input}.${suffix} 2> /dev/null
        if [ $? != 0 ]; then 
            echo -e "\033[31mTEST ${testname[${suffix}]} ÉCHOUÉ\033[0m"
            echo -e "Pour connaître les différences, exécutez :"
            echo -e "  ${diffprog} output/${input}.${suffix} ref-${suffix}/${input}.${suffix}"
            if [ $EXITONFAIL = 1 ]; then exit 1; fi
        else
            echo -e "\033[32mTEST ${testname[${suffix}]} OK\033[0m"
        fi
    else
        echo -e "\033[34mRéférence ref-${suffix}/${input}.${suffix} absente\033[0m"
    fi
}

################################################################################

function test_fichier_ok() {
    input=$1
    echo -e "\n\033[4m ---- Test input/$input.l ----\033[0m"
    if [ -f input/$input.l ]; then
        echo -e "\033[35m > Reconnaissance (accepte l'entrée)\033[0m"        
        ${MYCOMPILOSYNT} input/$input.l > output/$input.synt
        if [ $? != 0 ]; then 
            echo -e "\033[31mTEST Reconnaissance ÉCHOUÉ\033[0m"
            echo -e "Le programme $input.l n'a pas été compilé correctement"	
            if [ $EXITONFAIL = 1 ]; then exit 1; fi
        else
            echo -e "\033[32mTEST Reconnaissance OK\033[0m"
        fi 
        ${MYCOMPILOLEX} input/$input.l > output/$input.lex
        diff_prog "${REGDIFF}" $input lex        
        diff_prog ${XMLDIFF} $input synt
        ${MYCOMPILOASYNT} input/$input.l > output/$input.asynt
        diff_prog ${XMLDIFF} $input asynt
        ${MYCOMPILOTAB} input/$input.l > output/$input.tab
        diff_prog "${REGDIFF}" $input tab
        ${MYCOMPILOMIPS} input/$input.l > output/$input.mips
        diff_prog "${REGDIFF}" $input mips
    else
        echo -e "\033[31minput/$input.l non trouvé\033[0m"
        echo -e "\033[31mTest impossible\033[0m"        
    fi
}

################################################################################

function test_fichier_fail() {
	input=$1
    echo -e "\n\033[4m ---- Test input/$input.l ----\033[0m"
    ${MYCOMPILO} input/$input.l > output/$input.synt.xml
    if [ $? = 0 ]; then 
    echo -e "\033[31mTEST REJET ÉCHOUÉ\033[0m"
        echo -e "Le programme $input.l a été accepté alors qu'il aurait dû être rejeté"
        if [ $EXITONFAIL = 1 ]; then exit 1; fi
    else
        echo -e "\033[32mTEST REJET OK\033[0m"
    fi
}

################################################################################

mkdir -p output
make

echo -e "Votre compilateur : ${MYCOMPILO}"
if [ ! -f  ${MYCOMPILO} ]; then
    echo -e "\033[31mCompilateur introuvable"
    echo -e "Modifiez la variable MYCOMPILO avant de lancer l'éval\033[0m"
    exit 1
fi

echo -e "\033[1m\n>> 1) Tests connus OK\033[0m"

test_fichier_ok affect
test_fichier_ok boucle
test_fichier_ok expression
test_fichier_ok max
test_fichier_ok tri

echo -e "\033[1m\n>> 1) Tests connus FAIL\033[0m"

test_fichier_fail affect-err

