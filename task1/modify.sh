##
# Questions:
# 1. What should happend if modify -r is provided with path to the single file - not a directory. SHOULD CHANGE ONLY THIS FILE.
# 3. Files extensions. Should anything after dot stay the same? SHOULD BE MODIFIED
# 4. What should happen when we do -u -l ? ERROR
# 5. Should we modify directory names?

# Recursion flag.
REC=0

# Print help.
help(){
    echo "
    -l          Lowerize file names
    -u          Upercase file names
    -h          Prints help

    Example calls:
    ./modify [-r] [-l|-u] <dir/file names...>
    ./modify [-r] <sed pattern> <dir/file names...>
    ./modify [-h]
    "
}

# Calls modify on each file in the given directory (and subdirectories).
recursion(){
    declare -a files=($(find $1/* -depth))

    for file in "${files[@]}"; do
        echo "$file"
        if [ -d "$file" ]; then
            # echo "dir"
            recursion "$file" "$modify_mode"
        elif [ -f "$file" ]; then
            # echo "file"
            modify "$file" "$modify_mode"
        else
            return
        fi
    done
}

# Modify file according to the selected modify_mode.
modify(){
    local file_name=$(basename $1)
    local path_name=$(dirname $1)

    case "$modify_mode" in
        low)
            local modified_file_name="$(echo "$file_name" | tr [:upper:] [:lower:])"
            ;;
        up)
            local modified_file_name="$(echo "$file_name" | tr [:lower:] [:upper:])"
            ;;
        sed)
            local modified_file_name="$(echo "$file_name" | sed -s "$sed_pattern")"
            ;;
    esac

    # Change file_name to modified_file_name.
    if [ "$1" != "${path_name}/$modified_file_name" ]; then
        mv -- "$1" "${path_name}/$modified_file_name"
    fi

    # echo $file_name
    # echo $modified_file_name
    # echo $2
}


# If there are no parameters.
if [ $# -eq 0 ]; then
    echo "No parameters provided."
    help
    exit
fi

# Checking recursion flag.
case "$1" in
    -r | -R)
        REC=1
        shift ;;
esac

case "$1" in
    -h | -H) help exit ;;
    -u | -U) modify_mode=up ;;
    -l | -L) modify_mode=low ;;
    *) modify_mode=sed sed_pattern=$1 ;;
esac


while [ -n "$2" ]; do
    if ! [ -e "$2" ]; then      # Checking if provided path exists.
        echo "not a valid path!"
        exit
    fi

    if [ "$REC" -eq 1 ]; then
        echo "-r $modify_mode"
        recursion "$2" "$modify_mode"
    else
        echo "no recursion"
        modify "$2" "$modify_mode"
    fi
    shift
done

