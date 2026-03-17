#!/bin/bash

nfiles_per_list=500
basedir="$(pwd)"
echo "---------------------------------------------------------"
echo "Current working dir: $basedir"
echo "---------------------------------------------------------"

mkdir -p "list"
rm -f list/*.list

for species in Electron Positron; do
    dirs=( $basedir/out_"$species"_* )
    if [ "${dirs[0]}" = "out_${species}_*" ]; then
        echo "No directories found for $species"
        continue
    fi

    for dir in "${dirs[@]}"; do
        num="$(basename "$dir" | sed "s/out_${species}_//" | sed "s#/##")"

        mapfile -t allfiles < <(find "$dir" -type f -name "*.root" | sort)
        count=${#allfiles[@]}
        if [ "$count" -eq 0 ]; then
            echo "No ROOT files found in $dir"
            continue
        fi
        echo "---------------------------------------------------------"
        echo "Processing directory: $dir with $count files"

        idx=0
        part=1
        while [ $idx -lt $count ]; do
            outfile="list/list_${species}_${num}_part${part}.list"
            rm -f "$outfile"

            end_idx=$(( idx + nfiles_per_list ))
            if [ $end_idx -gt $count ]; then
                end_idx=$count
            fi

            for ((i=idx; i<end_idx; i++)); do
                echo "${allfiles[i]}" >> "$outfile"
            done

            idx=$end_idx
            part=$((part+1))
        done

        if [ $part -eq 2 ]; then
            echo "1 Files Created!"
        else
            echo "Not 1 Files Created!"
        fi
    done
done