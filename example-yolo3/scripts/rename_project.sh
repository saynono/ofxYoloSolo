#!/bin/bash
# files that need to be edited to change the project name

# get the current project name from the buffer file
SCRIPT_PATH="$(pwd)/${0}";
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
FILENAME="${SCRIPT_PATH##*/}"
if [ -z ${1} ]; then 
	echo "Usage: ${0} <new_project_name>"; 
	exit 0;
fi
# echo $DIR
cd $DIR
cd ..;
# echo $(pwd);
FILES_TO_RENAME=( CMakeLists.txt .idea/runConfigurations/of_debug.xml )
PROJECT_NAME=${1};
echo "Setting Project Name to: $PROJECT_NAME";

REPLACE_STRING="__nono_clion__";
# loop over all the files and rename them using ex
echo "renaming $project_name to $1..."
for f in "${FILES_TO_RENAME[@]}"
do
    echo modifying "$f"...
    ex -s +%s/$REPLACE_STRING/$PROJECT_NAME/g +x "$f"
done
exit 0;

