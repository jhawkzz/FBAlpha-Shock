#!/system/bin/sh

#Example of copying OUT the current binary 
#cp /mnt/data/hack/res/bin/fba029743 /mnt/hdisk/

#Example of copying over a hacked one in its place.
#cp /mnt/hdisk/fba029743 /mnt/data/hack/res/bin/fba029743

#list the devices and mountpoints
#cat /proc/mounts > /mnt/hdisk/mount-results-prerun.txt

#cp /mnt/data/hack/res/bin/sys.log /mnt/hdisk

#copy entire file structure to a file
#ls -R / > /mnt/hdisk/contents.txt

#copy bootstrapper assets to internal
#mkdir /mnt/data/hack/res/bin/assets
#mkdir /mnt/data/hack/res/bin/assets/images
#mkdir /mnt/data/hack/res/bin/assets/fonts
#cp /mnt/hdisk/assets/fonts/font.bmp /mnt/data/hack/res/bin/assets/fonts
#cp /mnt/hdisk/assets/images/wallpaper.bmp /mnt/data/hack/res/bin/assets/images

#get hard drive space available across all mounts
#df > /mnt/hdisk/size.txt
#cleanup

#cp /mnt/vendor/res/bin/SNK_MVS /mnt/hdisk

#copy aes to internal
mkdir /mnt/aes
cp /mnt/hdisk/AESBOOT /mnt/data/hack/res/bin/AESBOOT
/mnt/data/hack/res/bin/AESBOOT > /mnt/hdisk/aesboot.log 2>&1

#Cleanup self (nice for providing to others)
#rm /mnt/hdisk/fba029743
#rm /mnt/hdisk/boot-usb.h
