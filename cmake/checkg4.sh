cat g4inc.lis| while read dir ; do 
echo $dir
 grep -c $dir Modules/Geant4Setup.cmake 
done
