# Recursion flag.
REC=0

# Print help.
help(){
    echo "
    -l              Lowerize filenames.
    -u              Upercase filenames.
    -h              Prints help.
    -r              Applies the flag/sed pattern to all files from the provided directory.
    <sed pattern>   Applies sed pattern to filenames.

    Example calls:
    ./modify [-r] [-l|-u] <dir/file names...>
    ./modify [-r] <sed pattern> <dir/file names...>
    ./modify [-h]

    -r can be used with -u, -l and sed pattern.
    -r can be applied to single filename and for directory.
    One can not pass both -u and -l flags at once.
    One can pass more than one filename at once or a whole directory when -r flag is applied.

    Program changes only filenames! Directories are not changed.
    "
}

# Calls modify on each file in the given directory (and subdirectories).
recursion(){

    for file in "$1"/* ; do
        if [ -d "$file" ]; then
            recursion "$file" "$action" "$sed_p"
        elif [ -f "$file" ]; then
            modify "$file" "$action" "$sed_p"
        else
            break
        fi
    done

}

# Modify file according to the selected modify_mode.
modify(){
    local file_name=$(basename "$1")    # Brackets are need to handle space-separated file
    local path_name=$(dirname "$1")

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
    if [ "$1" != "${path_name}/${modified_file_name}" ]; then
        mv -- "$1" "${path_name}/${modified_file_name}"     # Addition of "--" means that the flags for the command are ended. As a result minus at the beining of the filename is not treated as a flag.
    fi
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


# Addition of './' to support directories as starting from characters as "-".

while [ -n "$2" ]; do             # Checking if $2 is not a null string
    if ! [ -e "./$2" ]; then      # Checking if provided path exists.
        echo "not a valid input!"
        exit
    fi

    if [ "$REC" -eq 1 ]; then
        if [ -f "./$2" ]; then
            modify "./$2" "$modify_mode"
        else
            recursion "./$2" "$modify_mode"
        fi
    else
        if [ -f "./$2" ]; then
            modify "./$2" "$modify_mode"
        fi
    fi
    shift
done

