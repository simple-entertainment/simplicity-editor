lightBlue='\033[1;34m'
noColor='\033[0m'

echo -e "${lightBlue}Executing '"$@"' in all repos!${noColor}"
echo -e "${lightBlue}simplicity${noColor}"
cd lib/simplicity
"$@"
echo -e "${lightBlue}simplicity-cef${noColor}"
cd ../simplicity-cef
"$@"
echo -e "${lightBlue}simplicity-editor${noColor}"
cd ../..
"$@"

