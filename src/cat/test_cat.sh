#!/bin/bash

COUNT_SUCCESS=0
COUNT_FAIL=0
TEST_FILE="cat.c cat.h s21_cat.c bytes.txt"  # "bytes.txt" #
echo "" > log.txt
LC_ALL=C perl -e 'for( my $i=0; $i < 128; $i++) { print ( sprintf("%c is %d %x\t\t\n", $i,$i,$i))}' > bytes.txt
echo ""
echo "Testing..."

for files in cat.c cat.h s21_cat.c bytes.txt
do    
    for var in -b -e -n -s -t -v
    do
        TEST1="$var $files"
        ./s21_cat $TEST1 > s21_cat.txt  # valgrind -s --leak-check=full 
        cat $TEST1 > cat.txt
        if [ "$(diff -s s21_cat.txt cat.txt)" == "Files s21_cat.txt and cat.txt are identical" ]
            then
                echo "$TEST1 - SUCCESS"
                (( COUNT_SUCCESS++ ))
            else
                echo "$TEST1" >> log.txt
                echo "$TEST1 - FAIL"
                (( COUNT_FAIL++ ))
        fi
        rm s21_cat.txt cat.txt
    done
done

for var in -b -e -n -s -t -v
do
    TEST1="$var $TEST_FILE"
    ./s21_cat $TEST1 > s21_cat.txt  # valgrind -s --leak-check=full 
    cat $TEST1 > cat.txt
    if [ "$(diff -s s21_cat.txt cat.txt)" == "Files s21_cat.txt and cat.txt are identical" ]
        then
            echo "$TEST1 - SUCCESS"
            (( COUNT_SUCCESS++ ))
        else
            echo "$TEST1" >> log.txt
            echo "$TEST1 - FAIL"
            (( COUNT_FAIL++ ))
    fi
    rm s21_cat.txt cat.txt
done

TEST1="-s bytes.txt"
TEST2="--squeeze-blank bytes.txt"
./s21_cat $TEST2 > s21_cat.txt
cat $TEST1 > cat.txt
if [ "$(diff -s s21_cat.txt cat.txt)" == "Files s21_cat.txt and cat.txt are identical" ]
    then
        echo "--squeeze-blank $TEST_FILE - SUCCESS"
        (( COUNT_SUCCESS++ ))
    else
        echo "--squeeze-blank $TEST_FILE" >> log.txt
        echo "--squeeze-blank $TEST_FILE - FAIL"
        (( COUNT_FAIL++ ))
fi
rm s21_cat.txt cat.txt


#for var1 in -E -T --number-nonblank --number --squeeze-blank
#do
#    for var2 in -E -T --number-nonblank --number --squeeze-blank
#    do
#        if [ $var1 != $var2 ]
#        then
#            TEST1="$var1 $var2 $TEST_FILE"
#            ./s21_cat $TEST1 > s21_cat.txt
#            cat $TEST1 > cat.txt
#            if [ "$(diff -s s21_cat.txt cat.txt)" == "Files s21_cat.txt and cat.txt are identical" ]
#                then
#                    echo "$TEST1 - SUCCESS"
#                    (( COUNT_SUCCESS++ ))
#                else
#                    echo "$TEST1" >> log.txt
#                    echo "$TEST1 - FAIL"
#                    (( COUNT_FAIL++ ))
#            fi
#            rm s21_cat.txt cat.txt
#        fi
#    done
#done

#TEST1="$TEST_FILE"
#    ./s21_cat $TEST1 > s21_cat.txt
#    cat $TEST1 > cat.txt
#    if [ "$(diff -s s21_cat.txt cat.txt)" == "Files s21_cat.txt and cat.txt are identical" ]
#        then
#            echo "$TEST1 - SUCCESS"
#            (( COUNT_SUCCESS++ ))
#        else
#            echo "$TEST1" >> log.txt
#            echo "$TEST1 - FAIL"
#            (( COUNT_FAIL++ ))
#    fi

#rm bytes.txt
echo "SUCCESS: $COUNT_SUCCESS"
echo "FAILURES: $COUNT_FAIL"
echo "Testing completed"
echo ""
