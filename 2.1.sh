#!/bin/bash

# This solution is horrible. Do not use it please.

# Run script in empty directory:
# example:
# mkdir w && cd w
# ../2.1.sh

rm * 2>/dev/null
rm nums/* 2>/dev/null

sed s/,/$(echo -n "\n")/g ../2.input |
  nl -v 0 |
  sed -E 's_\s+([0-9]+)\t([0-9]+)_echo \2 > nums/\2; ln -s nums/\2 \1_g' |
  xargs -0 bash -c

pc=0
while true; do
  case $(cat ./$pc) in
    1)
      a1=$(cat ./$(cat ./$[$pc + 1]))
      a2=$(cat ./$(cat ./$[$pc + 2]))
      a3=$(cat ./$[$pc + 3])
      ans=$[$a1 + $a2]
      # echo "*$a3 = $a1 + $a2 = $ans"
      echo $ans > nums/$ans
      ln -fs nums/$ans $a3
      ;;
    
    2)
      a1=$(cat ./$(cat ./$[$pc + 1]))
      a2=$(cat ./$(cat ./$[$pc + 2]))
      a3=$(cat ./$[$pc + 3])
      ans=$[$a1 * $a2]
      # echo "*$a3 = $a1 * $a2 = $ans"
      echo $ans > nums/$ans
      ln -fs nums/$ans $a3
      ;;

    99)
      echo $(ls | grep -v nums | sort -n | xargs cat) | sed -e s/' '/,/g
      exit 0
      ;;

    *)
      echo Invalid opcode.
      exit 0
      ;;
  esac
  pc=$[$pc + 4]
done
