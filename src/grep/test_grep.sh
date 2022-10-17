#!/bin/bash

COUNT_SUCCESS=0
COUNT_FAIL=0
TEST_FILE1="grep.c grep.h s21_grep.c Makefile"                           # -ivclnho
TEST_FILE2="-s grep 56 7834 s21_grep.c rew grep.h tbd 67qb"             # -s
TEST_FILE3="-f patterns.txt grep.c grep.h s21_grep.c"                   # -f
TEST_FILE4="-e grep grep.c grep.h -e puts s21_grep.c -e )$"          # -e
echo "" > log.txt
echo ""
echo "Testing..."


for var in -i -v -c -l -n -h -o
do
    for key in grep null all arg s
    do
        TEST1="$key $var $TEST_FILE1"
        ./s21_grep $TEST1 > s21_grep.txt
        grep $TEST1 > grep.txt
        if [ "$(diff -s s21_grep.txt grep.txt)" == "Files s21_grep.txt and grep.txt are identical" ]
            then
                echo "$TEST1 - SUCCESS"
                (( COUNT_SUCCESS++ ))
            else
                echo "$TEST1" >> log.txt
                echo "$TEST1 - FAIL"
                (( COUNT_FAIL++ ))
        fi
        rm s21_grep.txt grep.txt
    done
done

./s21_grep $TEST_FILE2 > s21_grep.txt
grep $TEST_FILE2 > grep.txt
if [ "$(diff -s s21_grep.txt grep.txt)" == "Files s21_grep.txt and grep.txt are identical" ]
    then
        echo "$TEST_FILE2 - SUCCESS"
        (( COUNT_SUCCESS++ ))
    else
        echo "$TEST_FILE2" >> log.txt
        echo "$TEST_FILE2 - FAIL"
        (( COUNT_FAIL++ ))
fi
rm s21_grep.txt grep.txt

echo "[2-8]" > patterns.txt
echo "\"%[[:alnum:]_]" >> patterns.txt
echo "llo" >> patterns.txt
echo ")$" >> patterns.txt
echo "patterns" >> patterns.txt
echo "\w\d" >> patterns.txt
echo "[1-3]" >> patterns.txt
echo "aga[a-g]" >> patterns.txt


./s21_grep $TEST_FILE3 > s21_grep.txt
grep $TEST_FILE3 > grep.txt
if [ "$(diff -s s21_grep.txt grep.txt)" == "Files s21_grep.txt and grep.txt are identical" ]
    then
        echo "$TEST_FILE3 - SUCCESS"
        (( COUNT_SUCCESS++ ))
    else
        echo "$TEST_FILE3" >> log.txt
        echo "$TEST_FILE3 - FAIL"
        (( COUNT_FAIL++ ))
fi
rm s21_grep.txt grep.txt


./s21_grep $TEST_FILE4 > s21_grep.txt
grep $TEST_FILE4 > grep.txt
if [ "$(diff -s s21_grep.txt grep.txt)" == "Files s21_grep.txt and grep.txt are identical" ]
    then
        echo "$TEST_FILE4 - SUCCESS"
        (( COUNT_SUCCESS++ ))
    else
        echo "$TEST_FILE4" >> log.txt
        echo "$TEST_FILE4 - FAIL"
        (( COUNT_FAIL++ ))
fi
rm s21_grep.txt grep.txt

#TEST_FILE4="s -o grep.c grep.h s21_grep.c Makefile"
#./s21_grep $TEST_FILE4 > s21_grep.txt
#grep $TEST_FILE4 > grep.txt
#if [ "$(diff -s s21_grep.txt grep.txt)" == "Files s21_grep.txt and grep.txt are identical" ]
#    then
#        echo "$TEST_FILE4 - SUCCESS"
#        (( COUNT_SUCCESS++ ))
#    else
#        echo "$TEST_FILE4" >> log.txt
#        echo "$TEST_FILE4 - FAIL"
#        (( COUNT_FAIL++ ))
#fi

echo "SUCCESS: $COUNT_SUCCESS"
echo "FAILURES: $COUNT_FAIL"
echo "Testing completed"
echo ""
