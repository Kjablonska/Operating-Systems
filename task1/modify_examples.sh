# ##
# EOPSY 21L Task1
# Laboratory group E
# Karolina Jabłońska, 295814
# ##

# Script for testing modify.sh
# Script is divided into 4 sections:
# 1. No recursion - checks only simple cases of -u, -l and sed pattern applied to one and many filenames.
# 2. Recursion - checks -r and -u, -l and sed pattern.
# 3. Errors handling - provide not existing paths, apply not correct flags etc.
# 4. Problematic file names - provide filenames staring with a minus, space-speoarated, containg numbers and signs, etc.


echo "===========================   1. NO RECURSION    =========================================
"
mkdir test1
touch test1/test1_f1.txt
ls


echo    "
        ---------------------------   TEST 1.1 UPERCASE   -----------------------------------

        ./modify.sh -u test1/test1_f1.txt

        "

echo "Before: "
ls test1/

./modify.sh -u test1/test1_f1.txt

echo "
After: "
ls test1/

echo    "
        ---------------------------   TEST 1.2 LOWERCASE   -----------------------------------

        ./modify.sh -l test1/TEST1_F1.TXT

        "
echo "Before: "
ls test1/

./modify.sh -l test1/TEST1_F1.TXT

echo "
After: "
ls test1/

echo    "
        ---------------------------   TEST 1.3 SED PATTERN   ----------------------------------

        ./modify.sh 's/test/sed_success/g' test1/test1_f1.txt

        "

echo "Before: "
ls test1/

./modify.sh 's/test/sed_success/g' test1/test1_f1.txt

echo "
After: "
ls test1


echo    "
        ---------------------------   TEST 1.4 PASS MORE THAN ONE FILENAME   ------------------"
echo    "Tested directory:"
touch test1/TeSt1.c
touch test1/test_F2.cpp

echo    "
        ./modify.sh -u test1/sed_success1_f1.txt test1/TeSt1.c test1/test_F2.cpp

        "
echo "Before: "
ls test1/

./modify.sh -u test1/sed_success1_f1.txt test1/TeSt1.c test1/test_F2.cpp

echo "
After: "
ls test1

rm -R test1/


echo "==============================   2. RECURSION    ===============================================
"
mkdir test2
touch test2/test2_f1.txt
mkdir test2/sub_test2
touch test2/sub_test2/test2_f1.txt

echo    "
        ---------------------------   TEST 2.1 DIR UPERCASE   -----------------------------------------

        ./modify.sh -r -u test2/

        "
echo "Before: "
ls -R test2/

./modify.sh -r -u test2/

echo "
After: "
ls -R test2/

echo    "
        ---------------------------   TEST 2.2 DIR LOWERCASE   ---------------------------------------

        ./modify.sh -r -l test2/

        "

echo "Before: "
ls -R test2/

./modify.sh -r -l test2/

echo "
After: "
ls -R test2/


echo    "
        ---------------------------  TEST 2.3 DIR SED PATTERN   --------------------------------------

        ./modify.sh -r 's/test2/t2/g' test2/

        "
echo "Before: "
ls -R test2/

./modify.sh -r 's/test2/t2/g' test2/

echo "
After: "
ls -R test2/


echo    "
        ---------------------------   TEST 2.4 FILENAME   ---------------------------------------------

        ./modify.sh -r -u test2/t2_f1.txt

        "
echo "Before: "
ls -R test2/

./modify.sh -r -u test2/t2_f1.txt

echo "
After: "
ls -R test2/

rm -R test2


echo "==============================   3. ERRORS HANDLING    =============================================
"

mkdir test3

echo    "
        ---------------------------   TEST 3.1 NON EXISTING PATH   ------------------------------------

        ./modify.sh -u test3/

        "

echo "Before: "
ls test3/
./modify.sh -u test3/no_such_file.txt
echo "
After: "
ls test3/

echo    "
        ---------------------------   TEST 3.1 EMPTY DIRECTORY RECURSION   -----------------------------

        ./modify.sh -r -u test3/

        "

echo "Before: "
ls test3/

./modify.sh -r -u test3/
echo "
After: "
ls test3/


touch test3/file3.txt
echo    "
        ---------------------------   TEST 3.2 -l FLAG FOR FILE ALREADY IN LOWERCASE   -----------------

        ./modify.sh -l test3/file3.txt

        "
echo "Before: "
ls test3/
./modify.sh -l test3/file3.txt
echo "
After: "
ls test3/


echo    "
        ---------------------------   TEST 3.2 -l -u FLAG   -----------------------------------------------

        ./modify.sh -l -u test3/file3.txt

        "

echo "Before: "
ls test3/
./modify.sh -l -u test3/file3.txt
echo "
After: "
ls test3/

rm -R test3

echo "==============================   4. PROBLEMATIC FILENAMES    =======================================
"
mkdir test4
touch test4/-minus.txt
touch test4/--double_minus.c
touch test4/'space separated.txt'
touch test4/'-space minus.py'
touch test4/1234.txt
touch test4/'<|\:),&.txt'
touch test4/'AbCd<|\:),&.txt'

echo    "
        ---------------------------   TEST 4.1 - MINUS AT THE BEGINNING  --------------------------

        ./modify.sh -u test4/-minus.txt
        ./modify.sh -u test4/--double_minus.c

        "
echo "Before: "
ls test4/
./modify.sh -u test4/-minus.txt
./modify.sh -u test4/--double_minus.c
echo "
After: "
ls test4/


echo    "
        ---------------------------   TEST 4.2 - SPACE SEPARATED NAME  ---------------------------

        ./modify.sh -u test4/'space separated.txt'
        ./modify.sh -u test4/'-space minus.py'

        "
echo "Before: "
ls test4/
./modify.sh -u test4/'space separated.txt'
./modify.sh -u test4/'-space minus.py'
echo "
After: "
ls test4/

echo    "
        ---------------------------   TEST 4.3 - CONTAINING NUMBERS -------------------------------

        ./modify.sh -l test4/1234.txt
        ./modify.sh -l test4/AbCd<|\:),&.txt

        "
echo "Before: "
ls test4/
./modify.sh -l test4/1234.txt
./modify.sh -l test4/'AbCd<|\:),&.txt'
echo "
After: "
ls test4/


echo    "
        ---------------------------   TEST 4.4 - SYMBOLS ONLY --------------------------------------

        ./modify.sh -l test4/<|\:),&.txt

        "
echo "Before: "
ls test4/
./modify.sh -l test4/'<|\:),&.txt'
echo "
After: "
ls test4/

echo    "
        ---------------------------   TEST 4.4 - SED PATTERN APPLIED FOR ONE FILE ------------------

        ./modify.sh -r 's/MINUS/m/g' test4/-SPACE MINUS.PY

        "
echo "Before: "
ls test4/
./modify.sh -r 's/MINUS/m/g' test4/'-SPACE MINUS.PY'
echo "
After: "
ls test4/


echo    "
        ---------------------------   TEST 4.5 - APPLY SED PATTERN TO THE DIRECTORY   ------------------

        ./modify.sh -r 's/MINUS/m/g' test4/

        "

echo "Before: "
ls test4/
./modify.sh -r 's/MINUS/m/g' test4
echo "
After: "
ls test4/

rm -R test4

mkdir -- -test4
touch -- ./-test4/-file4.txt
echo    "
        ---------------------------   TEST 4.6 - PROBLEMATIC DIRECTORY NAME   ------------------

        "

echo "
./modify.sh -r -u -test4
"
echo "Before:"
ls -R ./-test4

./modify.sh -r -u -test4

echo "
After: "
ls -R ./-test4

rm -R ./-test4