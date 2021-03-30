##
# Questions:
# 1. What should happend if modify -r is provided with path to the single file - not a directory.
# 2. <dir/file names> implies that we can provide modify with more than one path? How should it look like? Should the paths be separated by an empty space?
# 3. Files extensions. Should anything after dot stay the same?
# 4. What should happen when we do -u -l ?


# TO DO:
# - checking if path exists
# - if modified file already exists - error

####
# $1 - 1st parameter - mode selection
# $2 - 2nd parameter - file path
###


# Recursion flag.
REC=0
modify_mode=$not_set

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
    echo $file_name
    echo $modified_file_name
    echo $2
}


# If there are no parameters.
if [ $# -eq 0 ]; then
    echo "No parameters provided."
    help
    exit
fi

# Checking recursion flag.
case "$1" in
    -r | -R) REC=1 shift ;;
esac

case "$1" in
    -h | -H) help exit ;;
    -u | -U) modify_mode=up ;;
    -l | -L) modify_mode=low ;;
    *) modify_mode=sed sed_pattern=$1 ;;
esac


while [ -n "$2" ]; do
    if [ "$REC" -eq 1 ]; then
        echo "-r $modify_mode"
    else
        modify "$2" "$modify_mode"
    fi
    shift
done

