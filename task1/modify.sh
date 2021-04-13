####
# EOPSY 21L Task1
# Laboratory group E
# Karolina Jabłońska, 295814
####

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

    In order to use script with a space-separated files there is a need to pass them in brackets. The syntax is as follows:
        ./modify [-r] [-l|-u] <dir>/'<file name>'
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
    else
        echo "${modified_file_name} File not modified."
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
while [ -n "$2" ]; do
    if ! [ -e "./$2" ]; then      # Checking if provided path exists.
        echo "not a valid path!"
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

